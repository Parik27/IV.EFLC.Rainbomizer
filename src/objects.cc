#include "Utils.hh"
#include "CDrawCommands.hh"
#include "CEntity.hh"
#include "CStreaming.hh"
#include <utility>
#include <CCrypto.hh>
#include <CModelInfoStore.hh>
#include <CMaths.hh>
#include <CPed.hh>
#include <unordered_map>
#include "common.hh"
#include "logger.hh"
#include "config.hh"

class CVehicle;

CBaseDC *(__thiscall *CDynamicEntity__m8C) (CEntity *, int, int, uint32_t *,
                                            uint32_t);
struct VehicleObject
{
    Vector3_SinCos rotation    = {0, 0, 0};
    Vector3        translation = {0, 0, 0};
    Vector3        scale       = {1, 1, 1};
};

class ObjectRandomizer
{
    static int                                                mOdds;
    static std::unordered_map<int, VehicleObject>             mObjects;
    static std::unordered_map<CEntity *, std::pair<int, int>> mEntities;

    /*******************************************************/
    static void
    AssignNewModelToEntity (CEntity *vehicle)
    {
        // Free the previous resource if necessary
        if (mEntities.count (vehicle))
            CStreaming::MarkResourceAsNoLongerNeeded (
                mEntities[vehicle].second, CStreaming::g_pFileTypeWdrIndex ());

        if (RandomInt (mOdds - 1) == 0 && mObjects.size ())
            {
                // Get random map element
                auto it = mObjects.begin ();
                std::advance (it, RandomInt (mObjects.size () - 1));
                int newModel = it->first;

                mEntities[vehicle]
                    = std::make_pair (vehicle->m_wModelIndex, newModel);

                // Request new model
                CStreaming::RequestResource (newModel,
                                             CStreaming::g_pFileTypeWdrIndex (),
                                             0);
            }
        else
            mEntities[vehicle] = std::make_pair (vehicle->m_wModelIndex, -1);
    }

    /*******************************************************/
    static int
    GetNewModelIndexForEntity (CEntity *vehicle)
    {
        if (!mEntities.count (vehicle)
            || mEntities.at (vehicle).first != vehicle->m_wModelIndex)
            AssignNewModelToEntity (vehicle);

        return mEntities.at (vehicle).second;
    }

    /*******************************************************/
    static void
    ProcessEntityForNewDrawModel (CEntity *param_1, uint32_t &outModelIndex,
                                  Matrix34 *matrix)
    {
        int model = GetNewModelIndexForEntity (param_1);
        if (model == -1)
            return;

        // Request resource if it hasn't been loaded
        if (!CStreaming::HasResourceLoaded (model,
                                            CStreaming::g_pFileTypeWdrIndex ()))
            return CStreaming::RequestResource (
                model, CStreaming::g_pFileTypeWdrIndex (), 0);

        param_1->field_24 &= ~67108864; // required to prevent crashes
        outModelIndex = model;

        const auto &objInfo = mObjects.at (model);

        // Apply transformations
        matrix->pos += objInfo.translation;
        matrix->scale (objInfo.scale, true);
        matrix->rotate (objInfo.rotation);
    }

    /*******************************************************/
    static CBaseDC *__fastcall ChangeDrawnModelForVehicle (CEntity *param_1,
                                                           void *, int param_2,
                                                           int       param_3,
                                                           uint32_t *param_4,
                                                           uint32_t  param_5)
    {
        Matrix34 *matrix = ((CPed *) param_1)->GetBoneMatrix (0);

        uint32_t originalFlags  = param_1->field_24;
        Matrix34 originalMatrix = *matrix;

        ProcessEntityForNewDrawModel (param_1, param_5, matrix);

        CBaseDC *ret
            = CDynamicEntity__m8C (param_1, param_2, param_3, param_4, param_5);

        param_1->field_24 = originalFlags;
        *matrix           = originalMatrix;
        return ret;
    }

    /*******************************************************/
    static void
    ReadObjectsList (int episodeId)
    {
        FILE *modelFile
            = Rainbomizer::Common::GetRainbomizerDataFile ("Objects.txt", "r",
                                                           false);

        mObjects.clear ();
        mEntities.clear ();

        if (!modelFile)
            return;
        
        fscanf (modelFile, " %d \n", &mOdds);

        char line[1024] = {0};
        while (fgets (line, 1024, modelFile))
            {
                char          modelName[64] = {0};
                VehicleObject obj;
                Vector3       rotation = {0, 0, 0};

                sscanf (line, "%s %f %f %f %f %f %f %f %f %f", modelName,
                        &obj.translation.x, &obj.translation.y,
                        &obj.translation.z, &rotation.x, &rotation.y,
                        &rotation.z, &obj.scale.x, &obj.scale.y, &obj.scale.z);

                obj.rotation = rotation;
                
                int index;
                if (CStreaming::GetModelAndIndexFromHash (
                        CCrypto::atStringHash (modelName), &index))
                        mObjects[index] = obj;
                else
                    Rainbomizer::Logger::LogMessage ("Failed to find model: %s",
                                                     modelName);
            }
    }

public:
    ObjectRandomizer ()
    {

        if (!ConfigManager::GetConfigs ().objects.enabled)
            return;

        // For non-player vehicles
        RegisterHook (ByVersion ("51 8b ce e8 ? ? ? ? 89 44 ? ? 8b e8",
                                 "? 55 53 e8 ? ? ? ? 6a 00 6a 0c"),
                      3, CDynamicEntity__m8C, ChangeDrawnModelForVehicle);

        // For player vehicle
        RegisterHook (ByVersion ("53 51 8b ce e8 ? ? ? ? 8b e8",
                                 "8b cf 55 53 e8 ? ? ? ? 8b d8"),
                      4, CDynamicEntity__m8C, ChangeDrawnModelForVehicle);

        Rainbomizer::Common::AddEpisodeChangeCallback (ReadObjectsList);
    }
} _obj;

int                                    ObjectRandomizer::mOdds = 0;
std::unordered_map<int, VehicleObject> ObjectRandomizer::mObjects;
std::unordered_map<CEntity *, std::pair<int, int>> ObjectRandomizer::mEntities;

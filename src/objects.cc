#include "Utils.hh"
#include "CDrawCommands.hh"
#include "CEntity.hh"
#include "CStreaming.hh"
#include <utility>
#include <CCrypto.hh>
#include <CModelInfoStore.hh>
#include <CMaths.hh>
#include <windows.h>
#include <CPed.hh>
#include <unordered_map>
#include "common.hh"
#include "logger.hh"
#include "config.hh"

//#define EDIT_MODE

class CVehicle;

CBaseDC *(__thiscall *CDynamicEntity__m8C) (CEntity *, int, int, uint32_t *,
                                            uint32_t);

/*******************************************************/
template<int key, typename Func>
inline void OnKeyUp(Func f)
{
    static bool held = false;
    if (held && !GetAsyncKeyState(key))
        {
            f();
            held = false;
        }

    if (GetAsyncKeyState (key))
        held = true;
}

/*******************************************************/
template<int key, typename Func>
inline void OnKeyDown(Func f)
{
    if (GetAsyncKeyState (key))
        f();
}


struct VehicleObject
{
    std::string name;
    Vector3 rotation    = {0, 0, 0};
    Vector3 translation = {0, 0, 0};
    Vector3 scale       = {1, 1, 1};
};

class ObjectRandomizer
{
    static int mOdds;
    static std::vector<int> mObjectList;
    static std::unordered_map<int, VehicleObject> mObjects;
    static std::unordered_map<CEntity*, std::pair<int, int>> mEntities;

    /*******************************************************/
    static void
    AssignNewModelToEntity (CEntity* vehicle)
    {

        // Free the previous resource if necessary
        if (mEntities.count (vehicle))
            CStreaming::MarkResourceAsNoLongerNeeded (
                                                      mEntities[vehicle].second, CStreaming::g_pFileTypeWdrIndex ());

        if (RandomInt(mOdds - 1) == 0)
            {
                // Get random map element
                auto it = mObjects.begin();
                std::advance(it, RandomInt (mObjects.size() - 1));
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
#ifdef EDIT_MODE

        static FILE* f = fopen("log.txt", "a+");
        static int currentModel = -1;
        static enum
            {
                MODE_TRANSLATION,
                MODE_ROTATION,
                MODE_SCALE
            } currentMode;

        // Next Object - M
        // Previous Object - N
        OnKeyUp<'M'>([&] {

            static int currentIndex = -1;
            ++currentIndex;
            currentIndex %= mObjects.size();
            currentModel = mObjectList[currentIndex];
            Rainbomizer::Logger::LogMessage("%s %d", mObjects[currentModel].name.c_str(), currentModel);
        });
        
        OnKeyUp<'N'>([&] {

            static int currentIndex = -1;
            --currentIndex;
            currentIndex = (currentIndex < 0) ? mObjectList.size() - 1 : currentIndex;
            currentModel = mObjectList[currentIndex];            
        });

        //Activate Translation - T
        //Activate Rotation - Y
        //Activate Scaling - U
        OnKeyUp<'T'>([&] {
            currentMode = MODE_TRANSLATION;
        });
        OnKeyUp<'Y'>([&] {
            currentMode = MODE_SCALE;
        });
        OnKeyUp<'U'>([&] {
            currentMode = MODE_ROTATION;
        });

        /*
          Translation X+ - D
          Translation X- - A
          Translation Y+ - W
          Translation Y- - S
          Translation Z+ - Left Shift
          Translation Z- - Space
          Rotation X+ - D
          Rotation X- - A
          Rotation Y+ - W
          Rotation Y- - S
          Rotation Z+ - Left Shift
          Rotation Z- - Space
        */

        if(currentModel == -1)
            return currentModel;
        
        auto GetVector = [&] {
            switch (currentMode)
                {
                case MODE_TRANSLATION:
                    return &mObjects[currentModel].translation;
                case MODE_ROTATION:
                    return &mObjects[currentModel].rotation;
                case MODE_SCALE:
                    return &mObjects[currentModel].scale;
                }
        };
        
        OnKeyDown<'D'>([&] {
            GetVector()->x += 0.01;
        });
        OnKeyDown<'A'>([&] {
            GetVector()->x -= 0.01;
        });
        OnKeyDown<'W'>([&] {
            GetVector()->y += 0.01;
        });
        OnKeyDown<'S'>([&] {
            GetVector()->y -= 0.01;
        });
        OnKeyDown<VK_LSHIFT>([&] {
            auto vector = GetVector();
            vector->z += 0.01;
            if (currentMode == MODE_SCALE) {
                vector->x += 0.01;
                vector->y += 0.01;
            }
            
        });
        OnKeyDown<VK_SPACE>([&] {
            auto vector = GetVector();
            vector->z -= 0.01;
            if (currentMode == MODE_SCALE) {
                vector->x -= 0.01;
                vector->y -= 0.01;
            }
        });
        OnKeyUp<'P'>([&] {
            auto &obj = mObjects[currentModel];
            fprintf(f, "%s %f %f %f %f %f %f %f %f %f\n",
                    obj.name.c_str(), obj.translation.x, obj.translation.y,
                    obj.translation.z, obj.rotation.x, obj.rotation.y,
                    obj.rotation.z, obj.scale.x, obj.scale.y,
                    obj.scale.z);
            fflush(f);
        });
        
        return currentModel;
#endif
        
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
    static CBaseDC * __fastcall
    ChangeDrawnModelForVehicle (CEntity *param_1, void *, int param_2,
                                int param_3, uint32_t *param_4,
                                uint32_t param_5)
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
        fscanf (modelFile, " %d \n", &mOdds);

        char line[1024] = {0};
        while (fgets (line, 1024, modelFile))
            {
                char          modelName[64] = {0};
                VehicleObject obj;

                sscanf (line, "%s %f %f %f %f %f %f %f %f %f", modelName,
                        &obj.translation.x, &obj.translation.y,
                        &obj.translation.z, &obj.rotation.x, &obj.rotation.y,
                        &obj.rotation.z, &obj.scale.x, &obj.scale.y,
                        &obj.scale.z);

                obj.name = modelName;
                int index;
                if (CStreaming::GetModelAndIndexFromHash (
                                                          CCrypto::atStringHash (modelName), &index)) {
                    mObjects[index] = obj;
                    mObjectList.push_back(index);
                }
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
        
        RegisterHook ("e8 d0 5b 02 00 ", 0, CDynamicEntity__m8C,
                      ChangeDrawnModelForVehicle);
        RegisterHook ("e8 39 5c 02 00  ", 0, CDynamicEntity__m8C,
                      ChangeDrawnModelForVehicle);
        
        Rainbomizer::Common::AddEpisodeChangeCallback(ReadObjectsList);
    }
} _obj;

int ObjectRandomizer::mOdds = 0;
std::unordered_map<int, VehicleObject> ObjectRandomizer::mObjects;
std::vector<int> ObjectRandomizer::mObjectList;
std::unordered_map<CEntity*, std::pair<int, int>> ObjectRandomizer::mEntities;

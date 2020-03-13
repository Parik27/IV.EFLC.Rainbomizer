#include <string>

#pragma once

class CBaseModelInfo;

class CLoadedGroup
{
public:
    int m_aMembers[64];
    template<class UnaryFunction>
    void for_each(UnaryFunction f)
    {
        for (auto i : m_aMembers)
            {
                if (i < 0)
                    break;
                f (i);
            }
    }
};

class CStreaming
{
public:
    CLoadedGroup m_aAppropriateCars;
    CLoadedGroup m_aInappropriateCars;
    CLoadedGroup m_aSpecialCars;
    CLoadedGroup m_aBoats;

    bool m_bPlayerNearWater;

private:
    uint8_t pad__401[3];

public:
    CLoadedGroup  m_aAppropriatePeds;
    CLoadedGroup  m_aInappropriatePeds;
    CLoadedGroup  m_aSpecialPeds;
    int           m_nUnk704;
    int           m_nTotalPedBytesLoaded;
    int           m_nTotalVehicleBytesLoaded;

    static CStreaming* ms_instance;
    
    static void RequestResource (int index, int fileType, int flags);
    static int GetStreamingFlags (int index, int fileType);
    static void LoadAllObjects (int bPriorityOnly);
    static bool HasResourceLoaded (int index, int fileType);
    static void MarkResourceAsNoLongerNeeded (int index, int fileType);

    static CBaseModelInfo* GetModelAndIndexFromHash(unsigned int hash, int* indexOut);
    static CBaseModelInfo* GetModelFromHashHash(unsigned int hash);

    static int& g_pFileTypeWdrIndex;
    
    static void InitialisePatterns();

    ////////////////////////////////////////////////////
    // Utility functions (not game functions)
    ////////////////////////////////////////////////////
    
    /// Loads a model (using g_pFileTypeWdrIndex) with a model name
    /// Returns true if the model was successfully loaded
    static bool AttemptToLoadModel(const std::string &modelName, int numTries = 1);

    /// Loads a model (using g_pFileTypeWdrIndex) with a hash
    /// Returns true if the model was successfully loaded
    static bool AttemptToLoadModel(uint32_t hash, int numTries = 1);
};

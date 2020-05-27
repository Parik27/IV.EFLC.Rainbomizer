#pragma once

struct scrThread;

struct scrThreadContext
{
    unsigned int nThreadId;
    unsigned int dwScriptHash;
    unsigned int eThreadState;
    unsigned int m_nIP;
    unsigned int m_nFrameSP;
    unsigned int m_nSP;
    unsigned int nTimerA;
    unsigned int nTimerB;
    unsigned int nTimerC;
    unsigned int fWaitTime;
    unsigned int field_28;
    unsigned int field_2C;
    unsigned int field_30;
    unsigned int field_34;
    unsigned int field_38;
    unsigned int field_3C;
    unsigned int field_40;
    unsigned int m_nExceptCatchPointer;
    unsigned int field_48;
    unsigned int field_4C;
    unsigned int unk_data_len;
};

struct scrThread__vftable
{
    int (__thiscall *deconstructor) (scrThread *scr, bool free);
    void (__thiscall *Reset) (scrThread *scr, unsigned int hash, int newStack,
                              void *stackSize);
    int (__thiscall *Run) (scrThread *scr, unsigned int param_2);
    void (__thiscall *Update) (scrThread *scr);
    void (__thiscall *Kill) (scrThread *scr);
};

struct scrThread
{ /* size = 0x70 */
    scrThread__vftable *__vmt;
    scrThreadContext    m_context;
    int *               m_pScriptStack;
    unsigned int        field_5C;
    unsigned int        m_protectedStorage;
    unsigned int        xlive_buffer_index;
    unsigned int        unk_data;
    char *              m_pszExitMessage;
};
static_assert (sizeof (scrThread) == 0x70);

class GtaThread : scrThread
{
public:
    char         m_szProgramName[24];
    unsigned int field_88;
    unsigned int field_8C;
    unsigned int m_nSavedDeatharrestStackOff;
    char         field_94;
    bool         m_bScriptSafeForNetworkGame;
    bool         m_bThisScriptShouldBeSaved;
    bool         m_bPlayerControlOnInMissionCleanup;
    bool         m_bClearHelpInMissionCleanup;
    bool         m_bMinigameScript;
    bool         m_bAllowNonMinigameTextMessages;
    bool         m_bAllowOneTimeOnlyCommandToRun;
    bool         m_bPaused;
    bool         m_bCanBePaused;
    char         field_9E;
    char         field_9F;
    unsigned int field_A0;
    char         m_bCanRemoveBlipsCreatedByAnyScript;
    char         field_0xa5;
    char         field_0xa6;
    char         field_0xa7;
    unsigned int field_A8;
    unsigned int field_AC;
};
static_assert (sizeof (GtaThread) == 176);

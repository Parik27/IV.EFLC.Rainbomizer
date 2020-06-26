#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <any>
#include <cstdarg>
#include <gtaThread.hh>
#include "atPrimitives.hh"

union CScriptParams
{
    int      int_param;
    uint32_t uint_param;
    void *   ptr_param;
    float    float_param;
    char *   cstr_param;
};

struct scrProgram
{
    unsigned int   m_pszName;
    unsigned int   m_dwHash;
    unsigned char *m_pCodeBlock;
    unsigned int   m_pStaticSeg;
    unsigned int   m_nCodeSize;
    unsigned short m_wStaticSize;
    unsigned short m_wArgCount;
    unsigned short m_wUsageCount;
    unsigned short field_1A;
};

/*******************************************************/
struct NativeData
{
    void *         m_ret;
    uint8_t        __unk00[3];
    CScriptParams *Params;

    template <typename T>
    T&
    GetParam (int arg)
    {
        return *(T *) &Params[arg];
    }
};

typedef atHashPair<void (*) (NativeData *)> NativeHashPair;

/*******************************************************/
class CTheScripts
{
public:
    static bool m_bNeedTranslation;

    static GtaThread *&     m_pRunningThread ();
    static NativeHashPair **m_aNatives;
    static unsigned int *   m_nMaximumNatives;

    static atHashPair<scrProgram *> *&m_aScriptPrograms ();
    static unsigned int &             m_nNumScriptPrograms ();
    static unsigned int &             m_nMaxScriptPrograms ();

    static int *&m_pGlobals ();

    template <typename T = int>
    static T &
    GlobalVar (int index)
    {
        return *reinterpret_cast<T*>(&m_pGlobals ()[index]);
    }

    static void InitialisePatternsCE ();
    static void InitialisePatterns ();

    static scrProgram *GetScrProgram (const std::string &name);
    static scrProgram *GetScrProgram (unsigned int hash);

    static int GetNativeIndex (const std::string &name);
    static int GetNativeIndexTranslated (const std::string &name);
    static int GetNativeIndexTranslated (uint32_t hash);
    static int GetNativeIndex (uint32_t hash);
};

/*******************************************************/
class CNativeManager
{
    static std::vector<std::pair<int, void (*) (NativeData *)>> *m_aNewNatives;
    static std::unordered_map<int, void (*) (NativeData *)> m_aOriginalNatives;

    static void NativeHook ();

    template <typename... Args>
    static void
    StoreParams (CScriptParams *params, Args... args)
    {
        ((memcpy (params, &args, 4), params++), ...);
    }

public:
    static void OverwriteNative (const std::string &native,
                                 void (*func) (NativeData *));

    static void CallOriginalNative (const std::string &name, NativeData *data);
    static void CallNativeRaw (const std::string &name, NativeData *data);

    // not-type-safe method of calling natives
    template <typename... Args>
    static void
    CallNativeRet (void *ret, const std::string &name, Args... args)
    {
        NativeData data;

        data.m_ret  = ret;
        data.Params = new CScriptParams[sizeof...(args)];
        StoreParams (data.Params, args...);

        CallNativeRaw (name, &data);

        delete[] data.Params;
    }

    template <typename T, typename... Args>
    static T
    CallNativeRet (const std::string &name, Args... args)
    {
        int ret;
        CallNativeRet (&ret, name, args...);
        return (T) ret;
    }

    template <typename... Args>
    static void
    CallNative (const std::string &name, Args... args)
    {
        CallNativeRet (nullptr, name, args...);
    }

    static void InitialiseCE ();
    static void Initialise ();
};

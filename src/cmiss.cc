// c7 05 ?? ?? ?? ?? ?? ?? ?? ?? 89 1d ?? ?? ?? ?? c7 05 ?? ?? ?? ?? 64 00 00 00
#include "Utils.hh"
#include "injector/utility.hpp"
#include <cstring>
#include <CScrVM.hh>

class MissionRandomizer
{

    // Macro so I don't have to repeat similar code multiple times
#define ADJUST_SCRIPT_NAME(name, native)                        \
    static void name(NativeData* data)                          \
    {                                                           \
        if(std::string(data->Params[0].cstr_param) == "Roman2") \
            data->Params[0].cstr_param = (char*) "jacob1";      \
                                                                \
        CNativeManager::CallOriginalNative(native, data);       \
    }

    ADJUST_SCRIPT_NAME (HasScriptLoaded, "HAS_SCRIPT_LOADED")
    ADJUST_SCRIPT_NAME (RequestScript, "REQUEST_SCRIPT")
    ADJUST_SCRIPT_NAME (MarkScriptAsNoLongerNeeded, "MARK_SCRIPT_AS_NO_LONGER_NEEDED")
    ADJUST_SCRIPT_NAME (StartNewScript, "START_NEW_SCRIPT")

#undef ADJUST_SCRIPT_NAME

public:
    
    /*******************************************************/
    MissionRandomizer ()
    {
        InitialiseAllComponents();
        CNativeManager::OverwriteNative("HAS_SCRIPT_LOADED", HasScriptLoaded);
        CNativeManager::OverwriteNative("REQUEST_SCRIPT", RequestScript);
        CNativeManager::OverwriteNative ("MARK_SCRIPT_AS_NO_LONGER_NEEDED",
                                         MarkScriptAsNoLongerNeeded);
        CNativeManager::OverwriteNative ("START_NEW_SCRIPT", StartNewScript);
    }

} _missions;

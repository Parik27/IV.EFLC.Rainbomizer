#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <injector/calling.hpp>

#include <windows.h>
#include <cassert>

/* Hooking Utils for slightly more complicated (and very unsafe) hooking.
 * Potentially too large to merge into the actual Utils.hh, so I just split it
 * into this file. */

enum CallbackOrder
    {
        CALLBACK_ORDER_BEFORE,
        CALLBACK_ORDER_AFTER
    };

template <uint32_t Addr, typename Ret, typename... Args>
class ReplaceJmpHook
{
protected:
    static std::vector<std::function<void (Args&...)>>
        mAfterFunctions;

    static bool mInitialised;
    static uint64_t mStoredValue;
    static uint64_t mOriginalValue;

    static uint64_t* mHookedAddress;
    static uint32_t mThreadId;

    inline static std::vector<std::function<void (Args&...)>>&
    GetBeforeFunctions ()
    {
        static std::vector<std::function<void (Args&...)>> mBeforeFunctions;
        return mBeforeFunctions;
    }

    inline static std::vector<std::function<void (Args&...)>>&
    GetAfterFunctions ()
    {
        static std::vector<std::function<void (Args&...)>> mAfterFunctions;
        return mAfterFunctions;
    }
    
public:

    virtual void Activate() = 0;

    static void
    SwapValues ()
    {
        std::swap (mStoredValue, *mHookedAddress);
    }

    static void Deactivate ()
    {
        std::swap (mOriginalValue, *mHookedAddress);
    }
    
    ReplaceJmpHook (void* addr,
                    std::function<void (Args&...)> callback, CallbackOrder order)
    {
        mHookedAddress = (uint64_t*) addr;

        switch (order)
            {
            case CALLBACK_ORDER_BEFORE:
                GetBeforeFunctions().push_back (callback);
                break;

            case CALLBACK_ORDER_AFTER:
                GetAfterFunctions().push_back (callback);
                break;
            }
    }
};

template <uint32_t Addr, typename Ret, typename Class, typename... Args>
class ReplaceJmpHook__thiscall : public ReplaceJmpHook<Addr, Ret, Class*, Args...>
{
    using base = ReplaceJmpHook<Addr, Ret, Class*, Args...>;
    
public:
    static Ret __fastcall
    HookedFunction (Class* th, void*, Args... args)
    {
        assert (base::mThreadId == 0
                || base::mThreadId == GetCurrentThreadId ());
        base::mThreadId = GetCurrentThreadId ();

        for (const auto &i : base::GetBeforeFunctions())
            i (th, args...);

        base::SwapValues ();
        injector::thiscall<Ret (Class *, Args...)>::call (base::mHookedAddress,
                                                          th, args...);
        base::SwapValues ();

        for (const auto &i : base::GetAfterFunctions())
            i (th, args...);
    }

    virtual void
    Activate () override
    {
        if (base::mInitialised)
            return;

        base::mOriginalValue
            = *base::mHookedAddress;
        base::mStoredValue   = base::mOriginalValue;

        injector::MakeJMP (base::mHookedAddress, HookedFunction);

        base::mInitialised = true;
    }

    using base::base;
};

#define INIT_VARIABLE(type, name, def)                                      \
    template <uint32_t Addr, typename Ret, typename... Args>                   \
    type ReplaceJmpHook<Addr, Ret, Args...>::name = def;

INIT_VARIABLE(std::vector<std::function<void (Args&...)>>, mAfterFunctions, 0)
INIT_VARIABLE(bool, mInitialised, false)
INIT_VARIABLE(uint64_t, mStoredValue, 0)
INIT_VARIABLE(uint64_t, mOriginalValue, 0)
INIT_VARIABLE(uint64_t*, mHookedAddress, 0)
INIT_VARIABLE(uint32_t, mThreadId, 0)

#undef INIT_VARIABLE

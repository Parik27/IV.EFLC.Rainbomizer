#pragma once

#include "Patterns/Patterns.hh"
#include "injector/injector.hpp"

template <typename Func, typename Addr>
void
ReadCall (Addr address, Func &func)
{
    func = (Func) injector::GetBranchDestination (address).as_int ();
}

template <typename Func, typename Addr>
void
ConvertCall (Addr address, Func &func)
{
    func = Func (address);
}

int  RandomInt (int max);
int  RandomInt (int min, int max);
void InitialiseAllComponents ();

bool IsCompleteEdition ();

template <typename T>
inline auto
VersionedData (T preCE, T CE)
{
    return IsCompleteEdition () ? CE : preCE;
}

/*******************************************************/
template <typename F, typename O>
void
RegisterHook (const std::string &pattern, int offset, O &originalFunc,
              F hookedFunc)
{
    void *addr = hook::get_pattern (pattern, offset);
    ReadCall (addr, originalFunc);
    injector::MakeCALL (addr, hookedFunc);
}

/*******************************************************/
template <typename F>
void
RegisterHook (const std::string &pattern, int offset, F hookedFunc)
{
    void *addr = hook::get_pattern (pattern, offset);
    injector::MakeCALL (addr, hookedFunc);
}

/*******************************************************/
template <typename T = void>
void *
SearchBack (const std::string &pattern, const std::string &pattern2,
            int max_offset, int offset = 0)
{
    puts (pattern.c_str ());
    injector::memory_pointer_raw addr = hook::get_pattern (pattern);

    return hook::make_range_pattern ((addr - max_offset).as_int (),
                                     addr.as_int (), pattern2)
        .get_one ()
        .get<T> (offset);
}

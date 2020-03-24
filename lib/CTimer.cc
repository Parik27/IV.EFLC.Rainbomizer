#include <CTimer.hh>
#include "Patterns/Patterns.hh"

float *_ms_fFrameTime;

/*******************************************************/
void
CTimer::InitialisePatterns ()
{
    _ms_fFrameTime = *hook::get_pattern<float *> (
        "f3 0f 10 46 24 f3 0f 5c 05 ? ? ? ? 0f 57 c9 0f 2f c8 ", 9);
}

float &
CTimer::ms_fFrameTime ()
{
    return *_ms_fFrameTime;
}

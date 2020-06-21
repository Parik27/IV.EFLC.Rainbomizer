#pragma once

#include <cstdint>

/*******************************************************/
/* Also controls other draw command patterns           */
/*******************************************************/
class CBaseDC
{
public:

    void* operator new(size_t size, uint32_t param2 = 0);
    void operator delete(void*, size_t, uint32_t) {};
  
    static void InitialisePatterns ();
    static void InitialisePatternsCE ();

    void Render ();
};

class CDrawSLODPedDC : public CBaseDC
{
    uint8_t data[28];
public:
    CDrawSLODPedDC (void *frag, void *param_3, uint8_t alpha, uint8_t param_5,
                    void *param_6);
};

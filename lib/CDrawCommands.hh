#pragma once

#include <cstdint>

/*******************************************************/
/* Also controls other draw command patterns           */
/*******************************************************/
class CBaseDC
{
public:

    static CBaseDC* operator_new(uint32_t size, uint32_t param2);
  
    static void InitialisePatterns ();
    static void InitialisePatternsCE ();

    void Render ();
};

class CDrawSLODPedDC : public CBaseDC
{
public:
    CDrawSLODPedDC (void *frag, void *param_3, uint8_t alpha, uint8_t param_5,
                    void *param_6);
};

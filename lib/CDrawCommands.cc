#include "CDrawCommands.hh"
#include "Utils.hh"

void *(*CBaseDC__operatorNew) (uint32_t size, uint32_t param2);
CDrawSLODPedDC *(__thiscall *CDrawSLODPedDC_ctor) (CDrawSLODPedDC *, void *,
                                                   void *, uint8_t, uint8_t,
                                                   void *);
void (*CBaseDC__Render) (CBaseDC*);

/*******************************************************/
void*
CBaseDC::operator new (size_t size, uint32_t param2)
{
    return CBaseDC__operatorNew (size, param2);
}

/*******************************************************/
CDrawSLODPedDC::CDrawSLODPedDC (void *frag, void *param_3, uint8_t param_4,
                                uint8_t param_5, void *param_6)
{
    CDrawSLODPedDC_ctor (this, frag, param_3, param_4, param_5, param_6);
}

/*******************************************************/
void
CBaseDC::Render()
{
    CBaseDC__Render (this);
}

/*******************************************************/
void
CBaseDC::InitialisePatterns ()
{
    ConvertCall (hook::get_pattern ("d9 ee 8a 54 ? ? 56 8b f1 8b 46 04 8a 4c"),
                 CDrawSLODPedDC_ctor);

    ConvertCall (hook::get_pattern ("8b 44 ? ? 56 57 8b 7c ? ? 8b f7"),
                 CBaseDC__operatorNew);

    ConvertCall (hook::get_pattern (
                     "56 8b f1 8b 06 8b 50 08 57 ff d2 25 0f 00 00 80"),
                 CBaseDC__Render);
}

/*******************************************************/
void
CBaseDC::InitialisePatternsCE ()
{
    ConvertCall (hook::get_pattern ("56 8b f1 51 8b 46 04 c7 06"),
                 CDrawSLODPedDC_ctor);

    ConvertCall (hook::get_pattern ("53 56 57 8b 7c ? ? ff 74 ? ? 8b f7 "),
                 CBaseDC__operatorNew);

    ReadCall (hook::get_pattern (
                  "50 56 8b cb e8 ? ? ? ? 50 e8 ? ? ? ? 83 c4 04 5f ", 10),
              CBaseDC__Render);
}

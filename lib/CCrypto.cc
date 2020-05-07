#include "CCrypto.hh"
#include "Utils.hh"
#include "Patterns/Patterns.hh"

/*******************************************************/
uint32_t __cdecl CCrypto::HashStringLowercase (const char *param_1)

{
    char     cVar1;
    char     cVar2;
    uint32_t uVar3;

    cVar1 = *param_1;
    uVar3 = 0;
    if (cVar1 == '\"')
        {
            param_1 = param_1 + 1;
        }
    cVar2 = *param_1;
    while ((cVar2 != '\0' && ((cVar1 != '\"' || (cVar2 != '\"')))))
        {
            param_1 = param_1 + 1;
            if ((byte) (cVar2 + 0xbfU) < 0x1a)
                {
                    cVar2 = cVar2 + ' ';
                }
            else
                {
                    if (cVar2 == '\\')
                        {
                            cVar2 = '/';
                        }
                }
            uVar3 = ((int) cVar2 + uVar3) * 0x401;
            uVar3 = uVar3 ^ uVar3 >> 6;
            cVar2 = *param_1;
        }
    uVar3 = (uVar3 * 9 >> 0xb ^ uVar3 * 9) * 0x8001;
    if (uVar3 < 2)
        {
            uVar3 = uVar3 + 2;
        }
    return uVar3;
}

#include "CCrypto.hh"
#include "Utils.hh"
#include "Patterns/Patterns.hh"

uint32_t (*CCrypto__HashString) (const char *, int);

/*******************************************************/
uint32_t
CCrypto::HashString (const char *string, int seed)
{
    return CCrypto__HashString (string, seed);
}

/*******************************************************/
void
CCrypto::InitialisePatternsCE()
{
    ConvertCall (hook::get_pattern ("32 23 6e ed 60 4b e6 38 36 1e"),
                 CCrypto__HashString);
}

/*******************************************************/
void
CCrypto::InitialisePatterns ()
{
    CCrypto__HashString = hook::get_pattern<uint32_t (const char *, int)> (
        "8b 4c 24 08 56 8b 74 24 08 80 3e 22 0f 94 c2", 0);
}

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

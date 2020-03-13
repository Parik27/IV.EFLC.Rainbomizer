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

uint32_t
LowercaseHash (char *a1)
{
    uint32_t hash = 0;
    size_t   len  = strlen (a1);

    for (size_t i = 0; i < len; i++)
        {
            hash += tolower (a1[i]);
            hash += (hash << 10);
            hash ^= (hash >> 6);
        }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
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

#include "CHud.hh"
#include "Patterns/Patterns.hh"

CARGB* CHud::aColours;
int* CHud::aColourNameHashes;

/*******************************************************/
void
CHud::InitialisePatterns ()
{
    aColours = *hook::get_pattern<CARGB *> (
        "56 8b 74 24 0c 8b 04 b5 ? ? ? ? 8b c8 c1 e9 18", 8);

    aColourNameHashes = *hook::get_pattern<int *> (
        "52 89 04 b5 ? ? ? ? 8b 44 24 38 50 51 56 e8", 4);
}

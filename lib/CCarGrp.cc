#include "CCarGrp.hh"
#include "Utils.hh"

bool (*CCarGrp__IsCarPartOfGroup) (uint32_t, ePopcycleGrps);

/*******************************************************/
void
CCarGrp::InitialisePatterns ()
{
    ReadCall (hook::get_pattern ("6a 18 50 e8 ? ? ? ? 83 c4 08 84 c0 b0 01",
                                 3),
              CCarGrp__IsCarPartOfGroup);
}

/*******************************************************/
void
CCarGrp::InitialisePatternsCE ()
{
    ReadCall (hook::get_pattern ("6a 18 50 e8 ? ? ? ? 0f b6 cb 83 c4 08", 3),
              CCarGrp__IsCarPartOfGroup);
}

/*******************************************************/
bool
CCarGrp::IsCarPartOfGroup (uint32_t modelId, ePopcycleGrps group)
{
    return CCarGrp__IsCarPartOfGroup (modelId, group);
}

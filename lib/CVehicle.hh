#pragma once

#include <cstdint>
#include <CMaths.hh>
#include <CEntity.hh>

enum CVehicleColourIndex
  {
    VEHICLE_COLOUR_PRIMARY,
    VEHICLE_COLOUR_SECONDARY,
    VEHICLE_COLOUR_TERTIARY,
    VEHICLE_COLOUR_QUATERNARY,
    VEHICLE_COLOUR_REFLECTION, // Unsure,
    VEHICLE_COLOUR_MISC, // Basically anything that isn't that
    
    VEHICLE_COLOUR_COUNT
  };

struct CVehicleMaterials
{ /* PlaceHolder Class Structure */
    uint8_t   field_0x0[16];
    grcColour m_aMaterialColours[VEHICLE_COLOUR_COUNT];
    uint8_t   field_0x70[8];
    bool      m_bBurned;
    uint8_t   field_0x79[95];
    int       field_0xd8;
    uint32_t  field_0xdc;
    uint8_t   field_0xe0[24];
    int       field_0xf8;
};

class CVehicle : public CEntity
{
};

#pragma once

class CVehicle;
class CVehicleFactory;

struct CVehicleFactory__vtable
{
    int (__thiscall *deconstructor) (CVehicleFactory *, char);
    CVehicle *(__thiscall *CreateCar) (CVehicleFactory *, int, int, int, int);
    void *m8;
    void *m12;
    void *m16;
    void *GetTypeIdByTypeName;
};

class CVehicleFactory
{
public:
    CVehicleFactory__vtable *vtable;

    CVehicle *CreateCar (int index, int unk0, int unk2, int unk3);

    static void InitialisePatterns ();
    static void InitialisePatternsCE ();

    static CVehicleFactory **g_pVehicleFactory;
};

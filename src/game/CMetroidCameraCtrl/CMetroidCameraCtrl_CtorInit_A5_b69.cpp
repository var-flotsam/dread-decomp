// Unit 1424 of tools/tu_map.py (CMetroidCameraCtrl): fn_7100935f60, the camera controller's constructor
// (as fn_7100934724, src/game/CMetroidCameraCtrl/CMetroidCameraCtrl_Ctor_b52.cpp, with the base fields set in
// another order), followed by a call of its vtable slot 0x18. It runs the shared camera-controller base constructor (fn_7100763ed4), sets
// the vtable, sets its limits (-FLT_MAX/FLT_MAX pairs) and tunables, constructs a member at
// +0x310 (fn_710002f4e4) and sets three base fields. Names are placeholders.
#include <cfloat>
#include <new>
#include "game/CCameraBoundaryCtrl.h"
#include "game/CMetroidCameraCtrl.h"
#include "types.h"

struct UnkStruct_7100763ed4;
void fn_710002f4e4(void*);                      // member constructor

struct UnkMetroidCameraMember_A5_b69 {
    u64 unk_0x0[0xb0 / 8];
    UnkMetroidCameraMember_A5_b69() { fn_710002f4e4(this); }
};

struct UnkMetroidCameraBase_A5_b69 {
    void* vtable;
    u8 unk_0x8[0x20];
    u32 field_0x28;
    u8 unk_0x2c[0x9c - 0x2c];
    u32 field_0x9c;
    u8 unk_0xa0[4];
    u32 field_0xa4;
    u8 unk_0xa8[0x2a0 - 0xa8];
    UnkMetroidCameraBase_A5_b69() { fn_7100763ed4(reinterpret_cast<UnkStruct_7100763ed4*>(this)); }
};

struct UnkMetroidCameraCtrl_A5_b69 : UnkMetroidCameraBase_A5_b69 {
    u64 field_0x2a0;
    u64 field_0x2a8 = 0;
    u64 field_0x2b0 = 0;
    u64 field_0x2b8 = 0;
    u64 field_0x2c0 = 0;
    u64 field_0x2c8 = 0;
    f32 field_0x2d0 = -FLT_MAX;
    f32 field_0x2d4 = FLT_MAX;
    f32 field_0x2d8 = -FLT_MAX;
    f32 field_0x2dc = FLT_MAX;
    f32 field_0x2e0 = 280.0f;
    f32 field_0x2e4 = 3.0f;
    f32 field_0x2e8 = 3.0f;
    f32 field_0x2ec = 20.0f;
    f32 field_0x2f0 = 20.0f;
    f32 field_0x2f4 = 1.0f;
    f32 field_0x2f8 = 1.0f;
    bool field_0x2fc = false;
    bool field_0x2fd = false;
    u64 field_0x300 = 0;
    u32 field_0x308 = 0;
    UnkMetroidCameraMember_A5_b69 field_0x310;
    f32 field_0x3c0 = -1.0f;
    f32 field_0x3c4 = 0.0f;

    UnkMetroidCameraCtrl_A5_b69();
};

// The vtable is stored before the members are set (the comma in the first initialiser).
inline UnkMetroidCameraCtrl_A5_b69::UnkMetroidCameraCtrl_A5_b69()
    : field_0x2a0((vtable = g_unk_7101a83ad0 + 0x10, 0)) {
    field_0x28 = 1;
    field_0x9c = 1;
    field_0xa4 = 0;
}

void fn_7100935f60(UnkMetroidCameraCtrl_A5_b69* self) {
    new (self) UnkMetroidCameraCtrl_A5_b69;
    using Fn = void (*)(UnkMetroidCameraCtrl_A5_b69*);
    (*reinterpret_cast<Fn**>(self))[3](self);
}

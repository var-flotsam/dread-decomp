// CBossCameraCtrl's unit (tools/tu_map.py): a camera controller's constructor on the shared
// camera-controller base (fn_7100763ed4), as fn_7100934724
// (src/game/CMetroidCameraCtrl/CMetroidCameraCtrl_Ctor_b52.cpp): the vtable, a list (allocation flags
// 0xf), cleared words, a scale of 1, three base fields, then the post-construction virtual call
// (slot 3). Names are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "game/CCameraBoundaryCtrl.h"
#include "types.h"

struct UnkStruct_7100763ed4;

extern void* g_unk_7101a6df08[];  // vtable

struct UnkBossCameraBase_b53 {
    void** vtable;
    u8 unk_0x8[0x20];
    u32 field_0x28;
    u8 unk_0x2c[0x9c - 0x2c];
    u32 field_0x9c;
    u8 unk_0xa0[4];
    u32 field_0xa4;
    u8 unk_0xa8[0x2a0 - 0xa8];
    UnkBossCameraBase_b53() { fn_7100763ed4(reinterpret_cast<UnkStruct_7100763ed4*>(this)); }
};

struct UnkBossCameraCtrl_b53 : UnkBossCameraBase_b53 {
    // A list (base::UnkVector, allocation flags 0xf) spelled out field by field.
    u64 list_data;
    u32 list_count = 0;
    u32 list_capacity = 0;
    u32 list_step = 10;
    bool list_field_0x14 = true;
    u32 list_flags = 0xf;
    base::UnkAllocator* list_allocator = fn_7100083990();
    u64 field_0x2c8[15] = {};
    u8 unk_0x340[4];
    f32 field_0x344 = 1.0f;
    u8 unk_0x348[0xc];
    u32 field_0x354 = 0;

    UnkBossCameraCtrl_b53();
};

inline UnkBossCameraCtrl_b53::UnkBossCameraCtrl_b53() : list_data((vtable = reinterpret_cast<void**>(reinterpret_cast<char*>(g_unk_7101a6df08) + 0x10), 0)) {
    field_0x28 = 2;
    field_0xa4 = 0;
    field_0x9c = 1;
}

void fn_7100750b80(UnkBossCameraCtrl_b53* self) {
    new (self) UnkBossCameraCtrl_b53;
    reinterpret_cast<void (*)(UnkBossCameraCtrl_b53*)>(self->vtable[3])(self);
}

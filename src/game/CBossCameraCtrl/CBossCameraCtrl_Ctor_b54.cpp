// CBossCameraCtrl's unit (tools/tu_map.py): the twin of fn_7100750b80
// (src/game/CBossCameraCtrl_Ctor_b53.cpp) without the post-construction virtual call. The vtable
// is stored by the base's constructor and the list is a real base::UnkVector (allocation flags
// 0xf), which gives the address-based bool store. Names are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "game/CBossCameraCtrl.h"
#include "game/CCameraBoundaryCtrl.h"
#include "types.h"

struct UnkStruct_7100763ed4;

struct UnkBossCameraBase_b54 {
    void** vtable;
    u8 unk_0x8[0x20];
    u32 field_0x28;
    u8 unk_0x2c[0x9c - 0x2c];
    u32 field_0x9c;
    u8 unk_0xa0[4];
    u32 field_0xa4;
    u8 unk_0xa8[0x2a0 - 0xa8];
    UnkBossCameraBase_b54() {
        fn_7100763ed4(reinterpret_cast<UnkStruct_7100763ed4*>(this));
        vtable = reinterpret_cast<void**>(reinterpret_cast<char*>(g_unk_7101a6df08) + 0x10);
    }
};

struct UnkBossCameraCtrl_b54 : UnkBossCameraBase_b54 {
    base::UnkVector<u64> list;
    u64 field_0x2c8[15] = {};
    u8 unk_0x340[4];
    f32 field_0x344 = 1.0f;
    u8 unk_0x348[0xc];
    u32 field_0x354 = 0;

    UnkBossCameraCtrl_b54();
};

inline UnkBossCameraCtrl_b54::UnkBossCameraCtrl_b54() : list(0xf) {
    field_0x28 = 2;
    field_0xa4 = 0;
    field_0x9c = 1;
}

void fn_710074ef00(UnkBossCameraCtrl_b54* self) {
    new (self) UnkBossCameraCtrl_b54;
}

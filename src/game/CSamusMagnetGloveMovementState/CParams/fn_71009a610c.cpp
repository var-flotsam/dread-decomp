// In the CSamusMagnetGloveMovementState::CParams unit: the state's constructor (its destructor is fn_71009a61c8):
// the movement-state base (fn_71009b06dc), the vtable (g_unk_7101a87520), two cleared magnet targets, the capsule
// shape at +0x278 (fn_7100d5ff7c) and an empty handle; the base's kind becomes 13. Names and layouts are placeholders.
#include <new>

#include "game/logic/collision/CCapsuleShape2D.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkSamusParams_l9b06dc;
void fn_71009b06dc(UnkSamusParams_l9b06dc* self);
extern void* g_unk_7101a87520[];

struct UnkMagnetSegment_l9a610c {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    f32 field_0x10 = -1.0f;
};

struct UnkMagnetPlainVec3_l9a610c {
    u32 x, y, z;
};

struct UnkMagnetVec3_l9a610c {
    u32 x = 0, y = 0, z = 0;
};

struct UnkMagnetTarget_l9a610c {
    u32 field_0x0;
    UnkMagnetPlainVec3_l9a610c field_0x4;
    UnkMagnetSegment_l9a610c field_0x10;
    u32 field_0x24;
    UnkRefObject* field_0x28 = nullptr;
    UnkRefObject* field_0x30 = nullptr;
    UnkMagnetVec3_l9a610c field_0x38;
};
static_assert(sizeof(UnkMagnetTarget_l9a610c) == 0x48, "layout");

struct UnkMagnetCapsule_l9a610c {
    u8 storage[0xc0];
    UnkMagnetCapsule_l9a610c() { fn_7100d5ff7c(this); }
};

struct UnkMagnetHandle_l9a610c {
    void* target = nullptr;
    s64 serial = -1;
};

struct UnkMovementStateBase_l9a610c {
    void* vtable;
    u8 pad_0x8[0x8];
    u32 field_0x10;
    u8 pad_0x14[0x17c];
    explicit UnkMovementStateBase_l9a610c(void** vt) {
        fn_71009b06dc(reinterpret_cast<UnkSamusParams_l9b06dc*>(this));
        vtable = vt;
    }
};

struct UnkMagnetLink_l9a610c {
    f32 field_0x0 = 0;
    f32 field_0x4 = 0;
    UnkRefObject* field_0x8 = nullptr;
    u32 field_0x10 = 0;
};

struct UnkMagnetState_l9a610c : UnkMovementStateBase_l9a610c {
    u64 field_0x190 = 0;
    u8 pad_0x198[0x8];
    bool field_0x1a0 = true;
    u8 pad_0x1a1[0x7];
    UnkMagnetLink_l9a610c field_0x1a8;
    UnkMagnetTarget_l9a610c field_0x1c0;
    UnkMagnetTarget_l9a610c field_0x208;
    u8 pad_0x250[0x4];
    u32 field_0x254[9] = {};
    UnkMagnetCapsule_l9a610c field_0x278;
    UnkMagnetHandle_l9a610c field_0x338;

    UnkMagnetState_l9a610c() : UnkMovementStateBase_l9a610c(g_unk_7101a87520 + 2), field_0x1a8(), field_0x1c0(), field_0x208() {
        field_0x10 = 13;
    }
};
static_assert(__builtin_offsetof(UnkMagnetState_l9a610c, field_0x1c0) == 0x1c0, "layout");
static_assert(__builtin_offsetof(UnkMagnetState_l9a610c, field_0x278) == 0x278, "layout");
static_assert(__builtin_offsetof(UnkMagnetState_l9a610c, field_0x338) == 0x338, "layout");

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71009a610c(UnkMagnetState_l9a610c* self) {
    new (self) UnkMagnetState_l9a610c;
}

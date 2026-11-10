// In the CSamusMagnetGloveMovementState::CParams unit: the state's destructor (fn_71009a61c8): its vtable
// (g_unk_7101a87520), the capsule shape at +0x278 (fn_7100d6006c), the counted pointers of the two magnet targets
// (+0x1c0, +0x208) and of the link at +0x1a8 released, then the movement-state base destructor (fn_71009b0800).
// Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a87520[];
void fn_71009b0800(void*);
void fn_7100d6006c(void* self);

struct UnkMagnetRef_ovb0082 {
    UnkRefObject* ptr = nullptr;
    ~UnkMagnetRef_ovb0082() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkMagnetSegment_ovb0082 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    f32 field_0x10 = -1.0f;
};

struct UnkMagnetPlainVec3_ovb0082 {
    u32 x, y, z;
};

struct UnkMagnetVec3_ovb0082 {
    u32 x = 0, y = 0, z = 0;
};

struct UnkMagnetTarget_ovb0082 {
    u32 field_0x0;
    UnkMagnetPlainVec3_ovb0082 field_0x4;
    UnkMagnetSegment_ovb0082 field_0x10;
    u32 field_0x24;
    UnkMagnetRef_ovb0082 field_0x28;
    UnkMagnetRef_ovb0082 field_0x30;
    UnkMagnetVec3_ovb0082 field_0x38;
};
static_assert(sizeof(UnkMagnetTarget_ovb0082) == 0x48, "layout");

struct UnkMagnetCapsule_ovb0082 {
    u8 storage[0xc0];
    ~UnkMagnetCapsule_ovb0082() { fn_7100d6006c(this); }
};

struct UnkMagnetHandle_ovb0082 {
    void* target = nullptr;
    s64 serial = -1;
};

struct UnkMovementStateBase_ovb0082 {
    void* vtable;
    u8 pad_0x8[0x8];
    u32 field_0x10;
    u8 pad_0x14[0x17c];
    ~UnkMovementStateBase_ovb0082() { fn_71009b0800(this); }
};

struct UnkMagnetLink_ovb0082 {
    f32 field_0x0 = 0;
    f32 field_0x4 = 0;
    UnkMagnetRef_ovb0082 field_0x8;
    u32 field_0x10 = 0;
};

struct UnkMagnetState_ovb0082 : UnkMovementStateBase_ovb0082 {
    u64 field_0x190 = 0;
    u8 pad_0x198[0x8];
    bool field_0x1a0 = true;
    u8 pad_0x1a1[0x7];
    UnkMagnetLink_ovb0082 field_0x1a8;
    UnkMagnetTarget_ovb0082 field_0x1c0;
    UnkMagnetTarget_ovb0082 field_0x208;
    u8 pad_0x250[0x4];
    u32 field_0x254[9] = {};
    UnkMagnetCapsule_ovb0082 field_0x278;
    UnkMagnetHandle_ovb0082 field_0x338;

    __attribute__((always_inline)) ~UnkMagnetState_ovb0082() { vtable = g_unk_7101a87520 + 2; }
};
static_assert(__builtin_offsetof(UnkMagnetState_ovb0082, field_0x1c0) == 0x1c0, "layout");
static_assert(__builtin_offsetof(UnkMagnetState_ovb0082, field_0x278) == 0x278, "layout");
static_assert(__builtin_offsetof(UnkMagnetState_ovb0082, field_0x338) == 0x338, "layout");

void fn_71009a61c8(UnkMagnetState_ovb0082* self) {
    self->~UnkMagnetState_ovb0082();
}

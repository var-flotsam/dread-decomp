// UnkTU1370 (tools/tu_map.py unit 1370): resets the object fn_71008c0c74 builds: drops its two
// counted references, clears its flags and values and empties its string ids.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU1370.h"
#include "types.h"

using base::global::CStrId;

// A counted object deleted through its virtual destructor when the last reference goes.
struct UnkCounted_l8c0e30 {
    virtual void vf_0x0();
    virtual ~UnkCounted_l8c0e30();
    s32 ref_count;
};

struct UnkResetTarget_l8c0e30 {
    void** vtable;
    UnkCounted_l8c0e30* field_0x8;
    UnkCounted_l8c0e30* field_0x10;
    bool field_0x18;
    u32 field_0x1c;
    bool field_0x20;
    bool field_0x21;
    bool field_0x22;
    bool field_0x23;
    bool field_0x24;
    bool field_0x25;
    bool field_0x26;
    bool field_0x27;
    bool field_0x28;
    bool field_0x29;
    bool field_0x2a;
    u32 field_0x2c;
    bool field_0x30;
    u64 field_0x38;
    u64 field_0x40;
    bool field_0x48;
    bool field_0x49;
    f32 field_0x4c;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    u64 field_0x60;
    u8 pad_0x68[0x2c];
    u32 field_0x94;
    u64 field_0x98;
    CStrId field_0xa0;
    CStrId field_0xa8;
    u64 field_0xb0;
    CStrId field_0xb8;
    u32 field_0xc0;
    u32 field_0xc4;
    u32 field_0xc8;
    bool field_0xcc;
    u32 field_0xd0;
    bool field_0xd4;
    bool field_0xd5;
    u8 pad_0xd6[0x1e];
    bool field_0xf4;
    bool field_0xf5;
    bool field_0xf6;
    u32 field_0xf8;
};
static_assert(__builtin_offsetof(UnkResetTarget_l8c0e30, field_0x94) == 0x94, "layout");
static_assert(__builtin_offsetof(UnkResetTarget_l8c0e30, field_0xf8) == 0xf8, "layout");

// A zero whose constructor is not constexpr, so the static is guarded.
struct UnkStaticZero_l8c0e30 {
    u32 value;
    UnkStaticZero_l8c0e30() : value(0) {}
};

inline const UnkStaticZero_l8c0e30& StaticZero_l8c0e30() {
    static UnkStaticZero_l8c0e30 s_value asm("g_unk_71726b5354");
    return s_value;
}

static inline void Release(UnkCounted_l8c0e30*& ref) {
    if (UnkCounted_l8c0e30* old = ref) {
        if (--old->ref_count == 0)
            delete old;
        ref = nullptr;
    }
}

void fn_71008c0e30(UnkStruct_71008c0c74* object) {
    UnkResetTarget_l8c0e30* self = reinterpret_cast<UnkResetTarget_l8c0e30*>(object);
    self->field_0xf8 = 0;
    Release(self->field_0x8);
    Release(self->field_0x10);
    self->field_0x18 = false;
    self->field_0x21 = false;
    self->field_0x22 = false;
    self->field_0x23 = false;
    self->field_0x24 = false;
    self->field_0x25 = false;
    self->field_0x26 = false;
    self->field_0x27 = true;
    self->field_0x28 = false;
    self->field_0x29 = false;
    self->field_0x1c = 0;
    self->field_0x20 = false;
    self->field_0x38 = 0;
    self->field_0x40 = 0;
    self->field_0x48 = false;
    self->field_0x94 = StaticZero_l8c0e30().value;
    self->field_0xa0 = g_unk_7101d470e0;
    self->field_0xa8 = g_unk_7101d470e0;
    self->field_0xb8 = g_unk_7101d470e0;
    self->field_0x4c = 1.0f;
    self->field_0x50 = 0.0f;
    self->field_0x54 = 1.0f;
    self->field_0x58 = 1.0f;
    self->field_0x2a = true;
    self->field_0xb0 = 0;
    self->field_0x60 = 0;
    self->field_0x98 = 0;
    self->field_0x49 = false;
    self->field_0xc0 = 0;
    self->field_0xc4 = 0;
    self->field_0xc8 = 0;
    self->field_0xcc = false;
    self->field_0xd5 = true;
    self->field_0xd0 = 0;
    self->field_0xd4 = false;
    self->field_0x2c = 0;
    self->field_0xf4 = false;
    self->field_0xf5 = true;
    self->field_0xf6 = false;
    self->field_0x30 = false;
}

// Unit 1728 (tools/tu_map.py, no name hint): a constructor of a larger record than
// src/game/UnkUnit1728Record_b43.cpp's: cleared words with a few defaults (1, 1, 0.12, -1), an empty
// string id, two -1 words, a base::UnkVector (allocator flags 0x58), a second array without the
// vector's flag (also 0x58), the two members built out of line (fn_7100068eb4 at +0xa8,
// fn_710006b6a4 at +0x170) and a base::UnkVector with flags 0xf. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

void fn_7100068eb4(void* self);
void fn_710006b6a4(void* self);

struct UnkArray_b49 {
    void* data = nullptr;
    u64 field_0x8 = 0;
    u64 field_0x10 = 0;
    u32 step = 10;
    u32 flags;
    base::UnkAllocator* allocator = fn_7100083990();

    explicit UnkArray_b49(u32 flags) : flags(flags) {}
};

struct UnkUnit1728Big_b49 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;
    u32 field_0x1c;
    u64 field_0x20;
    u32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    u32 field_0x34;
    CStrId name;  // 0x38
    s64 field_0x40;
    s64 field_0x48;
    u64 field_0x50;
    base::UnkVector<void*> field_0x58;
    UnkArray_b49 field_0x80;
    u8 field_0xa8[0xc8];
    u8 field_0x170[0x28];
    base::UnkVector<void*> field_0x198;
};
static_assert(__builtin_offsetof(UnkUnit1728Big_b49, field_0x198) == 0x198, "layout");

void fn_7100b8970c(UnkUnit1728Big_b49* self) {
    self->field_0x0 = 0;
    self->field_0x8 = 0;
    self->field_0x10 = 0;
    self->field_0x18 = 1;
    self->field_0x1c = 1;
    self->field_0x20 = 0;
    self->field_0x28 = 0;
    self->field_0x2c = 0.12f;
    self->field_0x30 = -1.0f;
    self->field_0x34 = 0;
    self->name.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->name.data = &g_unk_7101d47108->empty;
    self->field_0x40 = -1;
    self->field_0x48 = -1;
    self->field_0x50 = 0;
    new (&self->field_0x58) base::UnkVector<void*>(0x58);
    new (&self->field_0x80) UnkArray_b49(0x58);
    fn_7100068eb4(self->field_0xa8);
    fn_710006b6a4(self->field_0x170);
    new (&self->field_0x198) base::UnkVector<void*>(0xf);
}

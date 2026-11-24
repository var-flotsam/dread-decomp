// UnkTU2732 (tools/tu_map.py unit 2732): the constructor of an object holding an empty pooled
// array (allocator flags 0x13), 60 cleared slots and a few settings. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkTU2732.h"
#include "types.h"

extern char g_unk_7101cb5328[];

struct UnkVec4_l19aee8 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;

    UnkVec4_l19aee8() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
};

struct UnkSlot_l19aee8 {
    u64 field_0x0;
    u32 field_0x8;
    UnkVec4_l19aee8 field_0xc[8];
    u32 field_0x8c;

    UnkSlot_l19aee8() : field_0x0(0), field_0x8(0), field_0x8c(0xfe) {}
};
static_assert(sizeof(UnkSlot_l19aee8) == 0x90, "UnkSlot_l19aee8 layout");

struct UnkArray_l19aee8 {
    void* data;
    u32 count;
    u32 capacity;
    base::IAllocator* allocator;
    u32 flags;

    explicit UnkArray_l19aee8(u32 flags)
        : data(nullptr), count(0), capacity(0), allocator(GetDefaultAllocator()), flags(flags) {}
};

struct UnkObject_l19aee8 {
    void* vtable;
    u32 field_0x8;
    UnkArray_l19aee8 array;
    UnkSlot_l19aee8 slots[60];
    u32 field_0x21f0;
    bool field_0x21f4;
    f32 field_0x21f8;
    f32 field_0x21fc;
    bool field_0x2200;
};
static_assert(__builtin_offsetof(UnkObject_l19aee8, field_0x21f0) == 0x21f0, "UnkObject_l19aee8 layout");

// The object's constructor, written out.
// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_710119aee8(void* self) {
    auto* obj = static_cast<UnkObject_l19aee8*>(self);
    obj->vtable = g_unk_7101cb5328 + 0x10;
    obj->field_0x8 = 0;
    new (&obj->array) UnkArray_l19aee8(0x13);
    for (UnkSlot_l19aee8& slot : obj->slots)
        new (&slot) UnkSlot_l19aee8();
    obj->field_0x21f0 = 0;
    obj->field_0x21f4 = true;
    obj->field_0x21f8 = 0.0f;
    obj->field_0x21fc = 60.0f;
    obj->field_0x2200 = false;
}

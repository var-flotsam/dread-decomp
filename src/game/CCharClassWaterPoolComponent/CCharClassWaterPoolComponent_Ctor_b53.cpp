// CCharClassWaterPoolComponent unit (tools/tu_map.py): a component constructor in the water pool unit:
// the base constructor fn_71009000bc, its vtable (g_unk_7101a80ed8), two arrays (flags 0xf) around a
// string-id list (fn_710090b950, refl_StrIdListCtor), a copy of the empty string id, a few cleared
// members, then the object's own slot 3 (vf_0x18). Names and layout are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassWaterPoolComponent.h"
#include "types.h"

struct UnkWaterPoolComponent_b53;
void fn_71009000bc(UnkWaterPoolComponent_b53* self);
void fn_710090b950(char* self);

struct UnkWaterPoolComponent_b53 {
    void** vtable;
    u8 pad_0x8[0xbd8];
    bool field_0xbe0;
    u8 pad_0xbe1[0xaf];
    void* field_0xc90_data;
    u64 field_0xc90_count;  // count and capacity
    u32 field_0xc90_step;
    bool field_0xc90_grow;
    u32 field_0xc90_flags;
    base::UnkAllocator* field_0xc90_allocator;
    u64 field_0xcb8;
    char field_0xcc0[0x38];  // string-id list (fn_710090b950)
    u64 field_0xcf8;
    void* field_0xd00_data;
    u64 field_0xd00_count;  // count and capacity
    u32 field_0xd00_step;
    bool field_0xd00_grow;
    u32 field_0xd00_flags;
    base::UnkAllocator* field_0xd00_allocator;
    base::global::CStrId field_0xd28;
    bool field_0xd30;
    u64 field_0xd38;
    u8 pad_0xd40[0x8];
    bool field_0xd48;
};

// An array (base/UnkVector.h, flags 0xf) built in place member by member: written as flat members, the
// flag at +0x14 is stored at its own offset rather than through the array's base address.
#define INIT_ARRAY_B53(o)                          \
    self->field_##o##_data = nullptr;              \
    self->field_##o##_count = 0;                   \
    self->field_##o##_step = 10;                   \
    self->field_##o##_grow = true;                 \
    self->field_##o##_flags = 0xf;                 \
    self->field_##o##_allocator = fn_7100083990();

void fn_710090be10(UnkWaterPoolComponent_b53* self) {
    fn_71009000bc(self);
    self->vtable = g_unk_7101a80ed8 + 2;
    INIT_ARRAY_B53(0xc90)
    self->field_0xcb8 = 0;
    fn_710090b950(self->field_0xcc0);
    INIT_ARRAY_B53(0xd00)
    new (&self->field_0xd28) base::global::CStrId(g_unk_7101d470e0);
    self->field_0xcf8 = 0;
    self->field_0xbe0 = true;
    self->field_0xd30 = false;
    self->field_0xd38 = 0;
    self->field_0xd48 = false;
    reinterpret_cast<void (*)(UnkWaterPoolComponent_b53*)>(self->vtable[3])(self);
}

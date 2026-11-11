// CDoorShieldLifeComponent (the name from reflection): the in-place constructor its reflection descriptor points
// to (fn_0x28). After the life component base constructor (fn_71008e04e8) it sets the vtable, builds an empty
// string at +0x388 and a second string at +0x3b8 (fn_710007e6c4), sets its tunables, three engine arrays (flags
// 0xf) and trailing flags, then makes the post-construction virtual call (slot 3). Defined under the `void()`
// symbol src/reflection/CDoorShieldLifeComponent.cpp declares. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "game/CDoorShieldLifeComponent.h"
#include "game/CItemLifeComponent.h"
#include "types.h"

using base::global::CRntString;

struct UnkLifeComponent_A5_b71;
void fn_710007e6c4(void*);

struct UnkDoorShieldLifeCtor_A5_b71 {
    void** vtable;
    u8 pad_0x8[0x388 - 0x8];
    CRntString name;      // 0x388
    u8 field_0x3b8[0x30];
    f32 field_0x3e8;
    u8 pad_0x3ec[0x4];
    u64 field_0x3f0;
    s64 field_0x3f8;
    u32 field_0x400;
    u8 pad_0x404[0x4];
    void* list_a_data;  // 0x408
    u64 list_a_count;  // count and capacity
    u32 list_a_step;
    u8 list_a_grow;
    u32 list_a_flags;
    base::UnkAllocator* list_a_allocator;
    void* list_b_data;  // 0x430
    u64 list_b_count;
    u32 list_b_step;
    u8 list_b_grow;
    u32 list_b_flags;
    base::UnkAllocator* list_b_allocator;
    void* list_c_data;  // 0x458
    u64 list_c_count;
    u32 list_c_step;
    u8 list_c_grow;
    u32 list_c_flags;
    base::UnkAllocator* list_c_allocator;
    bool field_0x480;
    u8 pad_0x481[0x7];
    u64 field_0x488;
    bool field_0x490;
};
static_assert(__builtin_offsetof(UnkDoorShieldLifeCtor_A5_b71, field_0x490) == 0x490, "layout");

// An engine array (base/UnkVector.h, flags 0xf) built member by member: written as flat members, the flag at
// +0x14 is stored at its own offset rather than through the array's base address (as in
// CCharClassWaterPoolComponent_Ctor_b53.cpp).
#define INIT_LIST_A5_B71(o)               \
    self->o##_data = nullptr;             \
    self->o##_count = 0;                  \
    self->o##_step = 10;                  \
    self->o##_grow = 1;                   \
    self->o##_flags = 0xf;                \
    self->o##_allocator = fn_7100083990()

void fn_71008df4c4_A5_b71(UnkDoorShieldLifeCtor_A5_b71* self) asm("_Z13fn_71008df4c4v");
void fn_71008df4c4_A5_b71(UnkDoorShieldLifeCtor_A5_b71* self) {
    fn_71008e04e8(reinterpret_cast<UnkLifeComponent_A5_b71*>(self));
    self->vtable = &g_unk_7101a7ebf8[2];
    new (&self->name) CRntString("");
    fn_710007e6c4(self->field_0x3b8);
    self->field_0x3e8 = 0.15f;
    self->field_0x3f0 = 0;
    self->field_0x3f8 = -1;
    self->field_0x400 = 0;
    INIT_LIST_A5_B71(list_a);
    INIT_LIST_A5_B71(list_b);
    INIT_LIST_A5_B71(list_c);
    self->field_0x480 = false;
    self->field_0x488 = 0;
    self->field_0x490 = true;
    reinterpret_cast<void (*)(UnkDoorShieldLifeCtor_A5_b71*)>(self->vtable[3])(self);
}

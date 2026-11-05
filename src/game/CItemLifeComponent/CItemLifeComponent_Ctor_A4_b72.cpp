// CItemLifeComponent (the name from reflection): the in-place constructor its reflection descriptor points to
// (fn_0x28). After the life component base constructor (fn_710115f7bc) it sets the vtable, clears the fields at
// +0x240, builds the string-id list at +0x258 (fn_71008e266c), three engine arrays (flags 0xf), a flag, and the
// object at +0x318 (vtable g_unk_7101a22d48, a count of 1 at +0x340), then makes the post-construction virtual
// call (slot 3). Defined under the `void()` symbol src/reflection/CItemLifeComponent.cpp declares. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

void fn_710115f7bc(void* self);  // as CPowerBombBlockLifeComponent.cpp declares it
// base::refl_StrIdListCtor<..>, instantiated in src/clones (UnkClone_fn_71008e266c).
void fn_71008e266c(char* self);
extern void* g_unk_7101a7f028[];
extern void* g_unk_7101a22d48[];

struct UnkItemLifeCtor_A4_b72 {
    void** vtable;
    u8 pad_0x8[0x238];
    u32 field_0x240;
    u8 pad_0x244[0x4];
    u64 field_0x248;
    u64 field_0x250;
    char list[0x38];  // 0x258
    void* list_a_data;  // 0x290
    u64 list_a_count;  // count and capacity
    u32 list_a_step;
    u8 list_a_grow;
    u32 list_a_flags;
    base::UnkAllocator* list_a_allocator;
    void* list_b_data;  // 0x2b8
    u64 list_b_count;
    u32 list_b_step;
    u8 list_b_grow;
    u32 list_b_flags;
    base::UnkAllocator* list_b_allocator;
    void* list_c_data;  // 0x2e0
    u64 list_c_count;
    u32 list_c_step;
    u8 list_c_grow;
    u32 list_c_flags;
    base::UnkAllocator* list_c_allocator;
    bool field_0x308;
    u8 pad_0x309[0xf];
    void* inner_vtable;  // 0x318
    u64 field_0x320;
    u64 field_0x328;
    u64 field_0x330;
    u64 field_0x338;
    u32 field_0x340;
    u8 pad_0x344[0x4];
    u64 field_0x348;
    u64 field_0x350;
    u64 field_0x358;
    u64 field_0x360;
    u64 field_0x368;
    u64 field_0x370;
    u64 field_0x378;
    u64 field_0x380;
};
static_assert(__builtin_offsetof(UnkItemLifeCtor_A4_b72, field_0x380) == 0x380, "layout");

// An engine array (base/UnkVector.h, flags 0xf) written as flat members, so the flag at +0x14 is stored at its
// own offset (as CDoorShieldLifeComponent_Ctor_A5_b71.cpp).
#define INIT_LIST_A4_B72(o)               \
    self->o##_data = nullptr;             \
    self->o##_count = 0;                  \
    self->o##_step = 10;                  \
    self->o##_grow = 1;                   \
    self->o##_flags = 0xf;                \
    self->o##_allocator = fn_7100083990()

void fn_71008e28c0_A4_b72(UnkItemLifeCtor_A4_b72* self) asm("_Z13fn_71008e28c0v");
void fn_71008e28c0_A4_b72(UnkItemLifeCtor_A4_b72* self) {
    fn_710115f7bc(self);
    self->vtable = &g_unk_7101a7f028[2];
    self->field_0x240 = 0;
    self->field_0x248 = 0;
    self->field_0x250 = 0;
    fn_71008e266c(self->list);
    INIT_LIST_A4_B72(list_a);
    INIT_LIST_A4_B72(list_b);
    INIT_LIST_A4_B72(list_c);
    self->field_0x308 = false;
    self->inner_vtable = reinterpret_cast<char*>(g_unk_7101a22d48) + 0x10;
    self->field_0x320 = 0;
    self->field_0x328 = 0;
    self->field_0x330 = 0;
    self->field_0x338 = 0;
    self->field_0x340 = 1;
    self->field_0x348 = 0;
    self->field_0x350 = 0;
    self->field_0x358 = 0;
    self->field_0x360 = 0;
    self->field_0x368 = 0;
    self->field_0x370 = 0;
    self->field_0x378 = 0;
    self->field_0x380 = 0;
    reinterpret_cast<void (*)(UnkItemLifeCtor_A4_b72*)>(self->vtable[3])(self);
}

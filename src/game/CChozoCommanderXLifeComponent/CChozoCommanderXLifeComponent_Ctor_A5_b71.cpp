// CChozoCommanderXLifeComponent (the name from reflection): the in-place constructor its reflection descriptor
// points to (fn_0x28). After the life component base constructor (fn_710115f7bc) it sets the vtable, builds the
// object at +0x248 (vtable g_unk_7101a22d48), the member at +0x278 (fn_71008bfca4) and two callback lists
// (base vtable g_unk_7101a20998, then g_unk_7101a33fa0) at +0x2b0 and +0x2f0, then makes the post-construction
// virtual call (slot 3). Defined under the `void()` symbol src/reflection/CChozoCommanderXLifeComponent.cpp
// declares. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CLifeComponent.h"
#include "types.h"

struct UnkLifeBase_A5_b71;
void fn_71008bfca4(void* self);
extern void* g_unk_7101a7df38[];
extern void* g_unk_7101a22d48[];
extern void* g_unk_7101a20998[];  // the callback lists' base vtable
extern void* g_unk_7101a33fa0[];  // their vtable

struct UnkXLifeCtor_A5_b71 {
    void** vtable;
    u8 pad_0x8[0x240];
    void* inner_vtable;  // 0x248
    u64 field_0x250;
    u64 field_0x258;
    u64 field_0x260;
    u16 field_0x268;
    u8 pad_0x26a[0x2];
    u32 field_0x26c;
    u8 field_0x270;
    u8 pad_0x271[0x3];
    u32 field_0x274;
    u8 member[0x38];  // 0x278
    void* list_a_vtable;  // 0x2b0
    void* list_a_data;
    u64 list_a_count;  // count and capacity
    u32 list_a_step;
    bool list_a_grow;
    u32 list_a_flags;
    base::UnkAllocator* list_a_allocator;
    u32 list_a_field_0x30;
    void* list_a_field_0x38;
    void* list_b_vtable;  // 0x2f0
    void* list_b_data;
    u64 list_b_count;  // count and capacity
    u32 list_b_step;
    bool list_b_grow;
    u32 list_b_flags;
    base::UnkAllocator* list_b_allocator;
    u32 list_b_field_0x30;
    void* list_b_field_0x38;
};
static_assert(__builtin_offsetof(UnkXLifeCtor_A5_b71, list_b_vtable) == 0x2f0, "layout");

// A callback list built member by member (written as flat members, so the flag is stored at its own offset):
// the base sets its vtable and an empty array (step 10, flags 0xf), the list clears its fields and stores its own
// vtable (as src/game/CCharClassShootActivatorComponent/CCharClassShootActivatorComponent.cpp).
#define INIT_LIST_A5_B71(o)                                                \
    self->o##_vtable = reinterpret_cast<char*>(g_unk_7101a20998) + 0x10;   \
    self->o##_data = nullptr;                                              \
    self->o##_count = 0;                                                   \
    self->o##_step = 10;                                                   \
    self->o##_grow = true;                                                 \
    self->o##_flags = 0xf;                                                 \
    self->o##_allocator = fn_7100083990();                                 \
    self->o##_field_0x30 = 0;                                              \
    self->o##_vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;   \
    self->o##_field_0x38 = nullptr

void fn_71008bfef8_A5_b71(UnkXLifeCtor_A5_b71* self) asm("_Z13fn_71008bfef8v");
void fn_71008bfef8_A5_b71(UnkXLifeCtor_A5_b71* self) {
    fn_710115f7bc(self);
    self->vtable = &g_unk_7101a7df38[2];
    self->inner_vtable = reinterpret_cast<char*>(g_unk_7101a22d48) + 0x10;
    self->field_0x250 = 0;
    self->field_0x258 = 0;
    self->field_0x260 = 0;
    self->field_0x268 = 0;
    self->field_0x26c = 0;
    self->field_0x270 = 0;
    self->field_0x274 = 0;
    fn_71008bfca4(self->member);
    INIT_LIST_A5_B71(list_a);
    INIT_LIST_A5_B71(list_b);
    reinterpret_cast<void (*)(UnkXLifeCtor_A5_b71*)>(self->vtable[3])(self);
}

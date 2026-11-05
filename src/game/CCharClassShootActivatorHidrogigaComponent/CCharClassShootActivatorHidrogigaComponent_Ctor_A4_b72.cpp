// CCharClassShootActivatorHidrogigaComponent (the name from reflection): the in-place constructor its reflection
// descriptor points to (fn_0x28). After the base constructor (fn_710088fce4) it sets the vtable, builds two callback
// lists (base vtable g_unk_7101a20998, then g_unk_7101a33fa0) at +0x580 and +0x5c0, clears the fields after them
// (three 1.0f scales at +0x604), then makes the post-construction virtual call (slot 3). Defined under the `void()`
// symbol src/reflection/CCharClassShootActivatorHidrogigaComponent.cpp declares. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

void fn_710088fce4(void* self);
extern void* g_unk_7101a7c850[];
extern void* g_unk_7101a20998[];  // the callback lists' base vtable
extern void* g_unk_7101a33fa0[];  // their vtable

struct UnkHidrogigaCtor_A4_b72 {
    void** vtable;
    u8 pad_0x8[0x570];
    u64 field_0x578;
    void* list_a_vtable;  // 0x580
    void* list_a_data;
    u64 list_a_count;  // count and capacity
    u32 list_a_step;
    bool list_a_grow;
    u32 list_a_flags;
    base::UnkAllocator* list_a_allocator;
    u32 list_a_field_0x30;
    void* list_a_field_0x38;
    void* list_b_vtable;  // 0x5c0
    void* list_b_data;
    u64 list_b_count;  // count and capacity
    u32 list_b_step;
    bool list_b_grow;
    u32 list_b_flags;
    base::UnkAllocator* list_b_allocator;
    u32 list_b_field_0x30;
    void* list_b_field_0x38;
    bool field_0x600;
    u8 pad_0x601;
    bool field_0x602;
    u8 field_0x603;
    float scale_x;  // 0x604
    float scale_y;
    float scale_z;
    u64 field_0x610;
};
static_assert(__builtin_offsetof(UnkHidrogigaCtor_A4_b72, field_0x600) == 0x600, "layout");

#define INIT_LIST_A4_B72(o)                                                \
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

void fn_71008986d8_A4_b72(UnkHidrogigaCtor_A4_b72* self) asm("_Z13fn_71008986d8v");
void fn_71008986d8_A4_b72(UnkHidrogigaCtor_A4_b72* self) {
    fn_710088fce4(self);
    self->vtable = &g_unk_7101a7c850[2];
    self->field_0x578 = 0;
    INIT_LIST_A4_B72(list_a);
    INIT_LIST_A4_B72(list_b);
    self->field_0x600 = false;
    self->field_0x602 = true;
    self->field_0x603 = 0;
    self->scale_x = 0.0f;
    self->scale_y = 0.0f;
    self->scale_z = 0.0f;
    self->field_0x610 = 0;
    self->scale_z = 1.0f;
    self->scale_y = 1.0f;
    self->scale_x = 1.0f;
    reinterpret_cast<void (*)(UnkHidrogigaCtor_A4_b72*)>(self->vtable[3])(self);
}

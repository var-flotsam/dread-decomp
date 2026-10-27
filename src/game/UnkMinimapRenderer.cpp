// From unit 263 of tools/tu_map.py (0x71001e12dc-0x71001e5284, no reflected class; its strings name
// the minimap's render passes, "MINIMAP_PASS", "MINIMAP_2D_PASS", "MINIMAP_MASK_PASS" and so on, and
// shader constants such as "g_uBackgroundClr"): the minimap's renderer, a 0xfe0-byte polymorphic
// object that fn_710020ffbc allocates and constructs with fn_71001e12dc. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a1e9c8[];  // the renderer's vtable (GOT 0x7101cc7568)

// One of four records at +0x170 (0x98 bytes; only the words the constructor clears are named).
struct UnkMinimapPassState {
    u32 field_0x0;
    u8 pad_0x4[0x84];
    u64 field_0x88;
    u8 pad_0x90[0x8];
};
static_assert(sizeof(UnkMinimapPassState) == 0x98, "UnkMinimapPassState placeholder layout");

// The polymorphic member at +0x3d0 (its constructor is fn_710014ea24).
struct UnkMinimapPassObject {
    u8 pad_0x0[0x8];
};
void fn_710014ea24(UnkMinimapPassObject* self);

struct UnkMinimapRenderer {
    void** vtable;
    base::UnkVector<void*> field_0x8;
    base::UnkVector<void*> field_0x30;
    base::UnkVector<void*> field_0x58;
    base::UnkVector<void*> field_0x80;
    u64 field_0xa8;
    u64 field_0xb0;
    u64 field_0xb8;
    u64 field_0xc0;
    u32 field_0xc8;
    f32 field_0xcc;
    u32 field_0xd0;
    u8 pad_0xd4[0x84];
    u64 field_0x158;
    u8 pad_0x160[0x10];
    UnkMinimapPassState field_0x170[4];
    UnkMinimapPassObject field_0x3d0;
};
static_assert(__builtin_offsetof(UnkMinimapRenderer, field_0x158) == 0x158, "UnkMinimapRenderer placeholder layout");
static_assert(__builtin_offsetof(UnkMinimapRenderer, field_0x3d0) == 0x3d0, "UnkMinimapRenderer placeholder layout");

// The constructor: four empty arrays, cleared state with a scale of 1, then the member at +0x3d0.
void fn_71001e12dc(UnkMinimapRenderer* self) {
    self->vtable = &g_unk_7101a1e9c8[2];
    new (&self->field_0x8) base::UnkVector<void*>(0xf);
    new (&self->field_0x30) base::UnkVector<void*>(0xf);
    new (&self->field_0x58) base::UnkVector<void*>(0xf);
    new (&self->field_0x80) base::UnkVector<void*>(0xf);
    self->field_0xa8 = 0;
    self->field_0xb0 = 0;
    self->field_0xb8 = 0;
    self->field_0xc0 = 0;
    self->field_0xc8 = 0;
    self->field_0xcc = 1.0f;
    self->field_0xd0 = 0;
    self->field_0x158 = 0;
    for (UnkMinimapPassState& state : self->field_0x170) {
        state.field_0x0 = 0;
        state.field_0x88 = 0;
    }
    fn_710014ea24(&self->field_0x3d0);
}

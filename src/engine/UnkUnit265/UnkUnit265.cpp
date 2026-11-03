// Unit 265 of tools/tu_map.py (0x71001e659c-0x71001e74bc): setting up an object with two cull views
// (src/base/spatial/UnkFrustumCull.cpp) at +0xd0 and +0xd8, each with its weight at +0x94 set to 1,
// and the graphics system (g_unk_710b372a98) at +0x180. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkGlobal_710b372a98.h"
#include "types.h"

struct UnkCullView;
void fn_71001b76ec(UnkCullView* self);
void fn_71001b7a60(UnkCullView* self);

struct UnkCullViewWeight265 {
    u8 pad_0x0[0x94];
    f32 weight;
};

struct UnkViewPair265 {
    u8 pad_0x0[0x10];
    void* field_0x10;
    u8 pad_0x18[0xb8];
    UnkCullView* view_a;  // 0xd0
    UnkCullView* view_b;  // 0xd8
    u8 pad_0xe0[0xa0];
    UnkGlobal_710b372a98* gfx;  // 0x180
};
static_assert(__builtin_offsetof(UnkViewPair265, gfx) == 0x180, "UnkViewPair265 layout");

static inline __attribute__((always_inline)) UnkCullView* NewView() {
    UnkCullView* view = static_cast<UnkCullView*>(fn_71000839a0(0x4e8, 8));
    fn_71001b76ec(view);
    return view;
}

void fn_71001e6814(UnkViewPair265* self) {
    self->gfx = g_unk_710b372a98;
    self->field_0x10 = nullptr;
    UnkCullView* a = NewView();
    self->view_a = a;
    fn_71001b7a60(a);
    reinterpret_cast<UnkCullViewWeight265*>(self->view_a)->weight = 1.0f;
    UnkCullView* b = NewView();
    self->view_b = b;
    fn_71001b7a60(b);
    reinterpret_cast<UnkCullViewWeight265*>(self->view_b)->weight = 1.0f;
}

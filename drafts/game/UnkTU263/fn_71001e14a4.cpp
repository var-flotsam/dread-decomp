// Unit 263 (tools/tu_map.py): sets up a renderer's targets. Five 1280x720 textures and a 1024x512 one are
// created (fn_710015c1e0) with a shared depth buffer; each target set draws into its own texture with that
// depth, the viewport covers 1280x720, then the part at +0x3d0 is set up. Names and layouts are placeholders.
#include "game/UnkGlobal_710b372a98.h"
#include "game/UnkTU179.h"
#include "types.h"

struct UnkRenderPart_l1e14a4;
void fn_710014ea54(UnkRenderPart_l1e14a4* self);

struct UnkViewport_l1e14a4 {
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    f32 near;
    f32 far;
};

// A set of render targets: its colour targets (only the first used here), its depth, and the texture it owns.
struct UnkTargetSet_l1e14a4 {
    u32 count;
    UnkHandle* colors[16] __attribute__((packed));  // 0x4
    u8 pad_0x84[0x4];
    UnkHandle* depth;    // 0x88
    UnkHandle* texture;  // 0x90

    // Draws into its own texture with the given depth.
    void Bind(UnkHandle* depth_target) {
        colors[0] = texture;
        count = 1;
        depth = depth_target;
    }
};
static_assert(sizeof(UnkTargetSet_l1e14a4) == 0x98, "target set size");

struct UnkRenderer_l1e14a4 {
    u8 pad_0x0[0xa8];
    UnkGlobal_710b372a98* global;  // 0xa8
    u8 pad_0xb0[0x8];
    UnkViewport_l1e14a4 viewport;  // 0xb8
    UnkTargetSet_l1e14a4 main;     // 0xd0
    UnkHandle* depth;              // 0x168
    UnkTargetSet_l1e14a4 sets[4];  // 0x170
};
static_assert(__builtin_offsetof(UnkRenderer_l1e14a4, sets) == 0x170, "layout");
static_assert(sizeof(UnkRenderer_l1e14a4) == 0x3d0, "layout");

void fn_71001e14a4(UnkRenderer_l1e14a4* self) {
    self->global = g_unk_710b372a98;
    // Each texture's format, one 8-byte slot each in the original.
    alignas(8) s32 format_3 = 0x12;
    fn_710015c1e0(&self->sets[3].texture, 1280, 720, 3, format_3);
    alignas(8) s32 format_main = 0x12;
    fn_710015c1e0(&self->main.texture, 1280, 720, 3, format_main);
    alignas(8) s32 format_1 = 0x12;
    fn_710015c1e0(&self->sets[1].texture, 1280, 720, 3, format_1);
    alignas(8) s32 format_2 = 0x12;
    fn_710015c1e0(&self->sets[2].texture, 1280, 720, 3, format_2);
    alignas(8) s32 format_depth = 0x12;
    fn_710015c1e0(&self->depth, 1280, 720, 0xe, format_depth);
    alignas(8) s32 format_0 = 0x1a;
    fn_710015c1e0(&self->sets[0].texture, 1024, 512, 3, format_0);
    self->sets[0].Bind(self->depth);
    self->sets[1].Bind(self->depth);
    self->main.Bind(self->depth);
    self->sets[2].Bind(self->depth);
    self->sets[3].Bind(self->depth);
    self->viewport = {0, 0, 1280, 720, 0.0f, 1.0f};
    fn_710014ea54(reinterpret_cast<UnkRenderPart_l1e14a4*>(self + 1));
}

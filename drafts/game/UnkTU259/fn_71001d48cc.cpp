// Unit 259 (the graphics system's unit): sets up the overlay pass: the camera's view and projection,
// the program (or the default one), the draw state and the tint uniforms. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU174.h"
#include "game/UnkTU177.h"
#include "game/UnkTU178.h"
#include "game/UnkTU180.h"
#include "game/UnkTU181.h"
#include "game/UnkTU182.h"
#include "game/UnkTU223.h"
#include "game/UnkTU230.h"
#include "game/UnkTU259.h"
#include "types.h"

struct UnkGraphicsSystem;

extern f32 g_unk_7101a1e720[] __attribute__((visibility("hidden")));

s32 fn_710015c460();
f32 fn_7100132fb0();  // the display's aspect correction

struct UnkGfxCamera {
    u8 unk_0x0[0x20];
    UnkMatrix44 projection;  // 0x20
    u8 unk_0x60[0x8];
    void* field_0x68[2];  // indexed by field_0x78
    u32 field_0x78;
    u8 unk_0x7c[0x4];
    void* field_0x80;
};

// The renderer's draw context inside the graphics system.
struct UnkGfxContext_l1d48cc {
    UnkGfxCamera* camera;  // 0x0
    u8 pad_0x8[0x3f8 - 0x8];
    u64 projection_location;  // 0x3f8
    u64 field_0x400;
    u8 pad_0x408[0x548 - 0x408];
    void* state;    // 0x548
    void* program;  // 0x550
};

struct UnkGfx_l1d48cc {
    u8 pad_0x0[0x317aa20];
    UnkGfxContext_l1d48cc context;  // 0x317aa20
    u8 pad_0x317af78[0x3183650 - 0x317af78];
    s32 field_0x3183650;
};

static_assert(__builtin_offsetof(UnkGfx_l1d48cc, field_0x3183650) == 0x3183650, "");

// Scales the view vertically by the display's aspect correction.
inline void ApplyAspectCorrection(UnkMatrix44* view) {
    *view = fn_71000127c8(fn_7100079138(1.0f, fn_7100132fb0(), 1.0f), *view);
}

bool fn_71001d48cc(UnkRenderTargetA1* target, void* program, f32 alpha) {
    UnkGfx_l1d48cc* self = reinterpret_cast<UnkGfx_l1d48cc*>(target);
    self->field_0x3183650 = -1;
    UnkGfxContext_l1d48cc* context = &self->context;
    UnkGfxCamera* camera = context->camera;
    UnkMatrix44 view;
    for (u32 i = 0; i < 16; ++i)
        view.m[i] = 0.0f;
    view.m[0] = 1.0f;
    view.m[5] = 1.0f;
    view.m[10] = 1.0f;
    view.m[15] = 1.0f;
    ApplyAspectCorrection(&view);
    if (fn_710015c460() == 1)
        view.m[5] = -view.m[5];
    fn_710019c474(context->camera, &view);
    if (program)
        fn_71001cc608(reinterpret_cast<UnkGraphicsSystem*>(self), program);
    else
        fn_71001cafa0(reinterpret_cast<UnkStruct_71001cdd74*>(self), 1, 1, 0);
    fn_710015a860(0);
    fn_710015aa20(0);
    fn_710015a8c0(0);
    fn_710015a3e0(0);
    fn_710015ab00(4);
    fn_71001606f0(0, 1, true);
    fn_7100161800(camera->field_0x68[camera->field_0x78]);
    fn_7100154420(camera->field_0x80);
    fn_7100160ab0(0, 1, 1);
    fn_7100160990(0, 1, 1);
    if (context->projection_location) {
        UnkMatrix44 projection;
        UnkCopyMatrix(&projection, camera->projection);
        // The original adjusts the view here, not the projection.
        if (fn_710015c460() == 1)
            view.m[13] = 1.0f - view.m[13];
        fn_710015bf40(&projection, fn_7100132fa0());
        fn_710015e4b0(context->state, 0, context->projection_location, 4, 1, &projection, 1);
    }
    if (context->field_0x400)
        fn_710015e1f0(context->state, 0, context->field_0x400, 4, g_unk_7101a1e720);
    u64 location;
    if (fn_71001aa0b4(context->program, 1, 0x59, &location)) {
        f32 values[4];
        values[0] = 1.0f;
        values[1] = 1.0f;
        values[2] = 1.0f;
        values[3] = alpha;
        fn_710015e1f0(context->state, 1, location, 4, values);
    }
    if (fn_71001aa0b4(context->program, 1, 0x5a, &location)) {
        f32 values[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        fn_710015e1f0(context->state, 1, location, 4, values);
    }
    return true;
}

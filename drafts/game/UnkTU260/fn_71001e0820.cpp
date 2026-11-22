// Unit 260 (render passes): its static initialiser, which sets the unit's header ids, the render profile and
// pass names as string ids (each destructor registered with __cxa_atexit), and the default screen settings.
// Names and layouts are placeholders.
#include "../../../src/game_5821e4/UnkStaticInit.h"
#include "types.h"

using namespace unk_static_init;

struct UnkPair_71001e0820 {
    u32 field_0x0;
    u32 field_0x4;
};

struct UnkCountScale_71001e0820 {
    u32 field_0x0;
    f32 field_0x4;
};

// The unit's statics, merged by the compiler into one block that starts 4 bytes past an 8-byte boundary.
struct __attribute__((packed, aligned(4))) UnkStatics_71001e0820 {
    u32 field_0x0;
    u8 pad_0x4[0x18];
    UnkStrIdBase field_0x1c;
    UnkStrIdBase field_0x24;
    UnkStrIdBase markers[26];  // the profiler marker names
    UnkStrIdBase passes[28];  // the render pass names
    u8 pad_0x1dc[0x328];
    u64 field_0x504;
    UnkPair_71001e0820 field_0x50c;
    UnkCountScale_71001e0820 field_0x514;
};
static_assert(__builtin_offsetof(UnkStatics_71001e0820, field_0x504) == 0x504, "layout");
extern UnkStatics_71001e0820 g_unk_716e59603c __attribute__((visibility("hidden")));

void fn_71001e0820() {
    g_unk_716e59603c.field_0x0 = 6;
    InitId(&g_unk_716e59603c.field_0x1c, "binaries");
    InitId(&g_unk_716e59603c.field_0x24, "sources");
    InitId(&g_unk_716e59603c.markers[0], "g_vMaxScreenSize");
    InitId(&g_unk_716e59603c.markers[1], "g_vScreenSize");
    InitId(&g_unk_716e59603c.markers[2], "GEOMETRY");
    InitId(&g_unk_716e59603c.markers[3], "GEOMETRY_GBUFFER");
    InitId(&g_unk_716e59603c.markers[4], "GEOMETRY_AMBIENT");
    InitId(&g_unk_716e59603c.markers[5], "GEOMETRY_LIGHTING");
    InitId(&g_unk_716e59603c.markers[6], "GEOMETRY_FORWARD_OPAQUE");
    InitId(&g_unk_716e59603c.markers[7], "GEOMETRY_FORWARD_TRANSL");
    InitId(&g_unk_716e59603c.markers[8], "GEOMETRY_ZFILL");
    InitId(&g_unk_716e59603c.markers[9], "GEOMETRY_ZMASK");
    InitId(&g_unk_716e59603c.markers[10], "GEOMETRY_ZOCCLUDED");
    InitId(&g_unk_716e59603c.markers[11], "GEOMETRY_GBUFFER_DECAL");
    InitId(&g_unk_716e59603c.markers[12], "GEOMETRY_IBL_PASS");
    InitId(&g_unk_716e59603c.markers[13], "POSTPROCESS");
    InitId(&g_unk_716e59603c.markers[14], "BLOOM");
    InitId(&g_unk_716e59603c.markers[15], "ANAMORPHIC_BLOOM");
    InitId(&g_unk_716e59603c.markers[16], "SSAO_COMPUTE");
    InitId(&g_unk_716e59603c.markers[17], "TONEMAP");
    InitId(&g_unk_716e59603c.markers[18], "DOWNSCALE_SCENE");
    InitId(&g_unk_716e59603c.markers[19], "FXAA");
    InitId(&g_unk_716e59603c.markers[20], "RT_COPY");
    InitId(&g_unk_716e59603c.markers[21], "DOF");
    InitId(&g_unk_716e59603c.markers[22], "MOTIONBLUR");
    InitId(&g_unk_716e59603c.markers[23], "FADE_COPY");
    InitId(&g_unk_716e59603c.markers[24], "FADE_MERGE");
    InitId(&g_unk_716e59603c.markers[25], "DEBUG");
    InitId(&g_unk_716e59603c.passes[0], "ZFILL_PASS");
    InitId(&g_unk_716e59603c.passes[1], "ZMASK_PASS");
    InitId(&g_unk_716e59603c.passes[2], "OPAQUE_DEFERRED_PASS");
    InitId(&g_unk_716e59603c.passes[3], "OPAQUE_FORWARD_PASS");
    InitId(&g_unk_716e59603c.passes[4], "CLEAR_PASS");
    InitId(&g_unk_716e59603c.passes[5], "TRANSLUCENT_FORWARD_PASS");
    InitId(&g_unk_716e59603c.passes[6], "ZOCCLUDED_PASS");
    InitId(&g_unk_716e59603c.passes[7], "GBUFFER_PASS");
    InitId(&g_unk_716e59603c.passes[8], "AMBIENT_PASS");
    InitId(&g_unk_716e59603c.passes[9], "LIGHTING_PASS");
    InitId(&g_unk_716e59603c.passes[10], "POSTPROCESS_PASS");
    InitId(&g_unk_716e59603c.passes[11], "BLOOM");
    InitId(&g_unk_716e59603c.passes[12], "ANAMORPHIC_BLOOM");
    InitId(&g_unk_716e59603c.passes[13], "COLOR_PASS_OPAQUE");
    InitId(&g_unk_716e59603c.passes[14], "COLOR_PASS_TRANSLUCENT");
    InitId(&g_unk_716e59603c.passes[15], "GBUFFER_PASS_DECAL");
    InitId(&g_unk_716e59603c.passes[16], "IBL_PASS_DECAL");
    InitId(&g_unk_716e59603c.passes[17], "PASS_BLEND_DECAL");
    InitId(&g_unk_716e59603c.passes[18], "DEBUG_PASS");
    InitId(&g_unk_716e59603c.passes[19], "SSAO_COMPUTE");
    InitId(&g_unk_716e59603c.passes[20], "SSAO_DRAW");
    InitId(&g_unk_716e59603c.passes[21], "TONE_DRAW");
    InitId(&g_unk_716e59603c.passes[22], "DOWNSCALE_SCENE");
    InitId(&g_unk_716e59603c.passes[23], "HDR_FROM_CUBEMAP");
    InitId(&g_unk_716e59603c.passes[24], "DRAW_LINEARZ");
    InitId(&g_unk_716e59603c.passes[25], "SSR");
    InitId(&g_unk_716e59603c.passes[26], "FXAA");
    InitId(&g_unk_716e59603c.passes[27], "SCENE COPY");
    g_unk_716e59603c.field_0x504 = 0;
    g_unk_716e59603c.field_0x50c = UnkPair_71001e0820{10, 10};
    g_unk_716e59603c.field_0x514 = UnkCountScale_71001e0820{0, 1.0f};
}

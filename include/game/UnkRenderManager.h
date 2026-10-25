#pragma once

// The render manager singleton (reached through the GOT; its pointer sits in base::global's .bss
// sixteen bytes below the game manager's, g_unk_7101d47310). The unit that owns it
// (tools/tu_map.py: unit 315, 0x71002182c0-0x710021aca4, no reflected class) names the render
// passes in its static initialiser (GBUFFER_PASS, LIGHTING_PASS, SSAO_DRAW, BLOOM, TONEMAP,
// POSTPROCESS ...), and its functions take this object first: fn_7100219ae0 moves a scene node to
// a layer, fn_7100219588 projects a position through a view, fn_7100219294 sets a pass parameter
// by hash, fn_7100219d4c, fn_7100219cb0 and fn_7100219da4 switch a layer's flags, fn_71002199d8
// hands an object to the pool and fn_7100219bb4 one to the pass list. Only the members the
// hand-written files touch are spelled out; the names and the layout are placeholders, and the
// object's size is unknown (0x268 is the largest offset read so far).

#include "types.h"

// The pool at +0x218 that creates the scene objects: effects (fn_7100149930), models
// (fn_71006cae2c) and scene nodes (fn_7100e13928); fn_710014239c hands it an object.
struct UnkRenderPool;

struct UnkRenderManager {
    u8 pad_0x0[0x218];
    UnkRenderPool* field_0x218;
    void* field_0x220;  // the pass list the accessors forward to: fn_71001fc69c says whether it knows a layer, fn_71001fd7b4 and fn_71001fce38 give a view's transform and camera
    u8 pad_0x228[0x30];
    void* field_0x258;  // an object whose scale (fn_71001d08f4) sizes the screen area fn_7100219588 tests
    u8 pad_0x260[0x8];
    bool field_0x268;  // fn_7100219588 projects only while it is set
};
static_assert(__builtin_offsetof(UnkRenderManager, field_0x258) == 0x258, "UnkRenderManager placeholder layout");
static_assert(__builtin_offsetof(UnkRenderManager, field_0x268) == 0x268, "UnkRenderManager placeholder layout");

extern UnkRenderManager* g_unk_7101d47300;

#pragma once

// The manager that fn_71011a870c builds (0x1e8 bytes, created lazily through g_unk_71726b0cd8 by
// fn_710081348c's unit): seven debug-overlay models it loads in its constructor (the paths are
// system/engine/models/{damage,damagehard,selected_hi,selected_lo,sedisolve,sedisolver}.bcmdl and
// system/fx/custommaterials/immune.bcmdl), two chained maps and two pooled containers. Names and
// the layout of the parts are placeholders inferred from the code.

#include "base/UnkBlockFreeList.h"
#include "base/UnkChainedMap.h"
#include "base/UnkCloneHelpers.h"
#include "game/UnkRenderManager.h"
#include "types.h"

// A loaded model; the scene node the render manager takes sits at +8 (after the vtable).
struct UnkOverlayModel {
    void* vtable;
    UnkSceneNode node;
};

// A pooled container fn_71011a8d30 and fn_71011a8e48 construct.
struct UnkModelManagerPool_0x70 {
    u8 unk_0x0[0x70];
};

struct UnkModelManager {
    UnkOverlayModel* models[7];  // damage, damagehard, selected_hi, selected_lo, immune, sedisolve, sedisolver
    UnkChainedMap map_a;  // 0x38
    base::UnkBlockPool pool;  // 0x80
    UnkModelManagerPool_0x70 field_0xc0;
    UnkChainedMap map_b;  // 0x130
    UnkModelManagerPool_0x70 field_0x178;
};
static_assert(sizeof(UnkModelManager) == 0x1e8, "UnkModelManager layout");

void fn_71011a870c(UnkModelManager* self);

extern UnkModelManager* g_unk_71726b0cd8;  // the manager instance (reached through the GOT)

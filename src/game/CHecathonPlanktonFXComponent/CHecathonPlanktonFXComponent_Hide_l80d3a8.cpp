// CHecathonPlanktonFXComponent (tools/tu_map.py unit 1271): hides the effect. Every emitter's model is
// taken off the render manager and its points cleared, the model at +0x12c0 has its slots 0 and 1
// turned off, then the base component's handler runs. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "game/CComponent.h"
#include "game/UnkFixedVector_l808234.h"
#include "game/UnkPlanktonEmitter_l808234.h"
#include "game/UnkPlanktonFX_l808234.h"
#include "game/UnkPlanktonPoint_l808234.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSceneObject.h"
#include "game/UnkTU315.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkModel;

static_assert(sizeof(UnkPlanktonEmitter_l808234) == 0x1d0, "emitter size");

static_assert(__builtin_offsetof(UnkPlanktonFX_l808234, intensity) == 0x1294, "layout");
static_assert(__builtin_offsetof(UnkPlanktonFX_l808234, model) == 0x12c0, "layout");

void fn_710080d3a8(UnkPlanktonFX_l808234* self) {
    for (u32 i = 0; i < self->emitters.count; ++i) {
        fn_7100219e18(g_unk_7101d47300, self->emitters.At(i).model, false);
        self->emitters.At(i).points.Clear();
    }
    if (self->model) {
        fn_71001be430(reinterpret_cast<UnkSceneObject*>(self->model)->field_0x130, 0, false);
        fn_71001be430(reinterpret_cast<UnkSceneObject*>(self->model)->field_0x130, 1, false);
    }
    fn_71010c422c(reinterpret_cast<UnkMorphBallLauncher_b57*>(self));
}

// CHecathonPlanktonFXComponent (tools/tu_map.py unit 1271): the destructor. It drops the model at
// +0x12c0, takes every emitter's model off the render manager, clears the emitters, frees the list
// at +0x1298 and releases the path id before the base destructor. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "game/CHecathonPlanktonFXComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkFixedVector_l808234.h"
#include "game/UnkPlanktonEmitter_l808234.h"
#include "game/UnkPlanktonFX_l808234.h"
#include "game/UnkPlanktonPoint_l808234.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU315.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkModel;

static_assert(sizeof(UnkPlanktonEmitter_l808234) == 0x1d0, "emitter size");

static_assert(__builtin_offsetof(UnkPlanktonFX_l808234, intensity) == 0x1294, "layout");
static_assert(__builtin_offsetof(UnkPlanktonFX_l808234, model) == 0x12c0, "layout");

static inline UnkItemBase_71011a91c4* Item_l808234(UnkModel* model) {
    return model ? reinterpret_cast<UnkItemBase_71011a91c4*>(reinterpret_cast<char*>(model) + 8) : nullptr;
}

void fn_7100808234(UnkPlanktonFX_l808234* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101a74d08 + 0x10);
    if (self->model) {
        fn_71002199d8(g_unk_7101d47300, Item_l808234(self->model), 0);
        self->model = nullptr;
    }
    for (u32 i = 0; i < self->emitters.count; ++i) {
        fn_7100219e18(g_unk_7101d47300, self->emitters.At(i).model, false);
        fn_71002199d8(g_unk_7101d47300, Item_l808234(self->emitters.At(i).model), 0);
        self->emitters.At(i).points.Clear();
    }
    self->emitters.Clear();
    self->list_0x1298.Free();
    self->emitters.~UnkFixedVector_l808234();
    self->path_0x48.~CFilePathStrId();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}

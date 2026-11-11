// CHecathonPlanktonFXComponent's constructor (tools/tu_map.py; the name from reflection): the base
// constructor (fn_71009f1910), the vtable, a copy of the empty path id at +0x48, a cleared word, the
// intensity at +0x1294, a list (allocation flags 0xf) and no model (+0x12c0), then the
// post-construction virtual call (slot 3). Names and layout are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "game/CSceneComponent.h"
#include "types.h"

using base::global::CFilePathStrId;

extern char g_unk_7101a74d08[];

struct UnkBreakableTileGroup_b46a2;

struct UnkPlanktonFX_b53 {
    void** vtable;
    u8 pad_0x8[0x40];
    CFilePathStrId path_0x48;
    u32 field_0x50;
    u8 pad_0x54[0x1240];
    f32 intensity;  // 0x1294
    base::UnkVector<u64> list_0x1298{0xf};
    void* model;  // 0x12c0
};

void fn_710080ed3c(UnkPlanktonFX_b53* self) {
    fn_71009f1910(reinterpret_cast<UnkBreakableTileGroup_b46a2*>(self));
    self->vtable = reinterpret_cast<void**>(g_unk_7101a74d08 + 0x10);
    new (&self->path_0x48) CFilePathStrId(g_unk_7101d470e8);
    self->field_0x50 = 0;
    self->intensity = 0.0f;
    new (&self->list_0x1298) base::UnkVector<u64>(0xf);
    self->model = nullptr;
    reinterpret_cast<void (*)(UnkPlanktonFX_b53*)>(self->vtable[3])(self);
}

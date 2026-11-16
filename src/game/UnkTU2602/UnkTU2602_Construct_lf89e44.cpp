// Unit 2602 of tools/tu_map.py: the constructor of an object holding an empty string id and the
// slots layout of game/UnkRenderPassSlots.h. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkRenderPassSlots.h"
#include "types.h"

using base::global::CStrId;

struct UnkNamedSlots_lf89e44 {
    CStrId name;
    UnkRenderPassSlots slots;  // 0x8
};

void fn_7100f89e44(UnkNamedSlots_lf89e44* self) {
    self->name.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->name.data = &g_unk_7101d47108->empty;
    new (&self->slots) UnkRenderPassSlots();
}

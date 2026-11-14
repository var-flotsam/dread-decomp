// From CSpawnPointComponent's unit: the id (+0x30) of the actor the link at +0x60 points at, or the empty id.
// The link is resolved first (as fn_71003b0160, src/game/CBossAIComponent/fn_71003b0160.cpp). Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CSpawnPointComponent.h"

using base::global::CStrId;

// The linked actor, as far as this function reads it: its id.
struct UnkSpawnTarget {
    u8 pad_0x0[0x30];
    CStrId id;
};

const CStrId* fn_710112bf20(CSpawnPointComponent* self) {
    self->ResolveField0x60();
    // The linked actor, through this function's view of it.
    auto* handle = static_cast<CGameLinkHandle<UnkSpawnTarget>*>(self->field_0x60.handle.ptr);
    if (handle && handle->target)
        return &handle->target->id;
    return &g_unk_7101d470e0;
}

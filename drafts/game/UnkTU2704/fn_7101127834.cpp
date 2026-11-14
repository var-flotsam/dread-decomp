// CSpawnPointComponent's unit: copies a link into the component's link at +0x60 (its key, its state and its
// resolved handle), resolving both first when the actor is active. Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "game/CGameLink.h"
#include "game/CSpawnPointComponent.h"
#include "game_s2/UnkWeakHandle.h"

struct UnkLinkOwner_l127834 {
    u8 pad_0x0[0x1b2];
    bool field_0x1b2;
};

static bool IsGone(const CGameLink<CActor>& link) {
    return link.state == 2 && !link.Target();
}

static void Resolve(CGameLink<CActor>& link) {
    {
        UnkRefPtr ref = fn_710023b580(link.key);
        fn_7100234f78(&link.handle, reinterpret_cast<const base::UnkRefHandle*>(&ref));
    }
    link.state = 2;
}

void fn_7101127834(CSpawnPointComponent* self, CGameLink<CActor>* other) {
    CGameLink<CActor>& mine = self->field_0x60;
    if (IsGone(mine) || IsGone(*other))
        return;
    auto* actor = *reinterpret_cast<UnkLinkOwner_l127834**>(reinterpret_cast<u8*>(self) + 0x18);
    u32 mineState = mine.state;
    u32 otherState = other->state;
    if (mineState != 0 && actor->field_0x1b2 && otherState != 0) {
        if (mineState == 1)
            Resolve(mine);
        if (other->state == 1)
            Resolve(*other);
    }
    fn_7100234f78(&mine.handle, &other->handle);
    mine.state = other->state;
    fn_710023b29c(reinterpret_cast<UnkErasedVector*>(mine.key), reinterpret_cast<const UnkErasedVector*>(other->key));
}

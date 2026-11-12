// CMorphBallMovement's unit (tools/tu_map.py), fn_71009432e4: when the owner (+0x30) has its sub-object at +0x220,
// passes the id it holds under the usual track kind (fn_7101177ae8 returns a pointer to it) to the game manager's
// fn_7100df31b8 (true, false, 0, id, false). Names are placeholders.
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;

struct UnkMorphBallOwner_A4_b76 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkMorphBallMovement_A4_b76 {
    u8 pad_0x0[0x30];
    UnkMorphBallOwner_A4_b76* owner;
};

void fn_71009432e4(UnkMorphBallMovement_A4_b76* self) {
    if (UnkAnimationComponent* target = self->owner->field_0x220) {
        const CStrId& id = *fn_7101177ae8(target, 0xf4061ff25a2163d6);
        fn_7100df31b8(g_unk_7101d47310, true, false, 0, id, false);
    }
}

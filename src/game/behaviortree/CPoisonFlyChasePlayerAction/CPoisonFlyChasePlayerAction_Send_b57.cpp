// behaviortree::CPoisonFlyChasePlayerAction's unit (tools/tu_map.py). fn_7100633ffc sends the owner's
// (+0x18) sub-object at +0x220 the hash of "chase" under the usual track kind with mode 0 (UnkSpitSend,
// src/game_s2/UnkSpitSend.h); when that sub-object is then in a track of the class of fn_7100cfd938, the
// owner's AI component (+0x240), cast to the class of fn_7100638be0, gets fn_710111094c with 1 and the
// action runs fn_7100634174. It returns 2. Names are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkPoisonFlyChaseOwner_b57a4 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x18];
    UnkReflObject* field_0x240;
};

struct UnkPoisonFlyChase_b57a4 {
    u8 pad_0x0[0x18];
    UnkPoisonFlyChaseOwner_b57a4* owner;
};

void fn_7100634174(UnkPoisonFlyChase_b57a4* self);

u32 fn_7100633ffc(UnkPoisonFlyChase_b57a4* self) {
    UnkAnimationComponent* target = self->owner->field_0x220;
    UnkSpitSend(target, fn_7100001570("chase", 5), 0xf4061ff25a2163d6);
    if (fn_710117b348(target, fn_7100cfd938())) {
        UnkReflObject* ai = self->owner->field_0x240;
        UnkReflObject* cast = ai && fn_710009d50c(ai->vf_0x0(), fn_7100638be0()) ? ai : nullptr;
        fn_710111094c(cast, 1);
        fn_7100634174(self);
    }
    return 2;
}

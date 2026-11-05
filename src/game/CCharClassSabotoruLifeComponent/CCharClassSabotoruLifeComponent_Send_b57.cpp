// The CCharClassSabotoruLifeComponent unit. fn_7100677b20 picks the death id for `actor`: when its AI
// component (+0x240) is a CSabotoruAIComponent (the class of fn_7100677dd4) in state 1 (+0x14778) and the
// actor has its sub-object at +0x220, that sub-object is sent the hash of "door_death" under the usual
// track kind with mode 0 (UnkSpitSend, src/game_s2/UnkSpitSend.h) and the empty id is returned;
// otherwise the component's own id at +0x70. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;

struct UnkSabotoruAI_b57a4 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x14778 - 0x8];
    u32 field_0x14778;
};

struct UnkSabotoruActor_b57a4 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x18];
    UnkSabotoruAI_b57a4* field_0x240;
};

struct UnkSabotoruLife_b57a4 {
    u8 pad_0x0[0x70];
    CStrId field_0x70;
};

const CStrId& fn_7100677b20(UnkSabotoruLife_b57a4* self, UnkSabotoruActor_b57a4* actor) {
    UnkSabotoruAI_b57a4* ai = actor->field_0x240;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_7100677dd4()) && ai->field_0x14778 == 1) {
        if (void* target = actor->field_0x220) {
            UnkSpitSend(target, fn_7100001570("door_death", 10), 0xf4061ff25a2163d6);
            return g_unk_7101d470e0;
        }
    }
    return self->field_0x70;
}

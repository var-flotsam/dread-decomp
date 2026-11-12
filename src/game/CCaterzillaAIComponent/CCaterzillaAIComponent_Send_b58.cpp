// CCaterzillaAIComponent's unit (tools/tu_map.py): after the component's update steps (fn_71003695b8),
// stops layers 12 and 6 of the owner's (+0x30) sub-object at +0x220 and sends it a one under the
// "relax_spawn90" id (refl_EmitU32). Names are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkBehaviorTree.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkActorTarget;
void fn_710117662c(UnkActorTarget*, u32 value, u32 extra, bool apply, bool flag, f32 time);

struct UnkCaterzillaSendOwner_b58 {
    u8 pad_0x0[0x220];
    UnkActorTarget* field_0x220;
};

struct UnkCaterzillaSend_b58 {
    u8 pad_0x0[0x30];
    UnkCaterzillaSendOwner_b58* owner;
};

void fn_71003f775c(UnkCaterzillaSend_b58* self) {
    fn_71003695b8(reinterpret_cast<UnkBehaviorTreeAI*>(self));
    UnkActorTarget* target = self->owner->field_0x220;
    if (target) {
        fn_710117662c(target, 12, 0, false, false, -1.0f);
        fn_710117662c(target, 6, 0, false, false, -1.0f);
        base::reflection::refl_EmitU32(target, fn_7100001570("relax_spawn90", 13), 0xf4061ff25a2163d6, 1,
                                       &g_unk_710164b958, &g_unk_7172769740);
    }
}

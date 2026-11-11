// CCooldownXBossFireBallMovementComponent's unit (tools/tu_map.py): an update. After the base update
// fn_71010d1a90, while the ball's owner (+0x30) has its translation x (+0x44) below 1200, notifies the cooldown
// X boss AI component of the owner at +0x230 (when it is one, as fn_71004d771c) and passes the ball's owner
// to fn_7100232c68. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkTU341.h"
#include "types.h"
#include "game/UnkMovementComponent.h"

using base::reflection::CClass;

struct UnkAIComponent;
void fn_71004d43dc(UnkAIComponent*, void*);
struct UnkShardsSphere_b60a2;

struct UnkFireBallComponent_A2_b70 {
    virtual const CClass* vf_0x0();
};
struct UnkFireBallOwnerObject_A2_b70 {
    u8 pad_0x0[0x240];
    UnkFireBallComponent_A2_b70* component;
};
struct UnkFireBallOwner_A2_b70 {
    u8 pad_0x0[0x10];
    UnkFireBallOwnerObject_A2_b70* object;
};
struct UnkFireBallMovement_A2_b70 {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0x1f8];
    UnkFireBallOwner_A2_b70* owner;  // 0x230
};

// `dt` (s0) reaches the base update untouched in the original (tools/forwarded_args.py).
void fn_71004d767c(UnkFireBallMovement_A2_b70* self, f32 dt) {
    fn_71010d1a90(reinterpret_cast<UnkMovementComponent*>(self), dt);
    if (!(self->actor->field_0x44 < 1200.0f))
        return;
    if (self->owner && self->owner->object) {
        UnkFireBallComponent_A2_b70* component = self->owner->object->component;
        if (component && fn_710009d50c(component->vf_0x0(), fn_71004cf318()))
            fn_71004d43dc(reinterpret_cast<UnkAIComponent*>(component), self->actor);
    }
    fn_7100232c68(self->actor);
}

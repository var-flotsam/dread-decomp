// CDropterAIComponent's unit (tools/tu_map.py): after the AI component's fn_7101105c94, when `actor` is the owner's
// (+0x30) actor (fn_7100d7c990) and the owner's animation component (+0x220) plays none of four track
// classes (fn_710117b348), runs fn_71004ee590 and sends the animation component a fixed hash with mode 0 (UnkSpitSend).
// Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkWarLotusAI;
void fn_7101105c94(UnkWarLotusAI* self);
struct UnkDropterHit_b58;
void fn_71004ee590(UnkDropterHit_b58* self);

struct UnkDropterHitOwner_b58 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkDropterHit_b58 {
    u8 pad_0x0[0x30];
    UnkDropterHitOwner_b58* owner;
};

void fn_71004eeed0(UnkDropterHit_b58* self, UnkActor* actor) {
    fn_7101105c94(reinterpret_cast<UnkWarLotusAI*>(self));
    UnkAnimationComponent* animation = self->owner ? self->owner->animation : nullptr;
    if (!actor)
        return;
    if (fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner)) != actor)
        return;
    if (animation) {
        if (fn_710117b348(animation, fn_7100d2b3a8()))
            return;
        if (fn_710117b348(animation, fn_7100cee4b8()))
            return;
        if (fn_710117b348(animation, fn_7100ceda9c()))
            return;
        if (fn_710117b348(animation, fn_7100d253f8()))
            return;
    }
    fn_71004ee590(self);
    UnkSpitSend(animation, 0x48a94369af012f6e, 0xf4061ff25a2163d6);
}

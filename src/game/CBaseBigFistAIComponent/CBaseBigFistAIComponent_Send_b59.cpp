// CBaseBigFistAIComponent's unit (tools/tu_map.py): when the owner's (+0x30) life component (+0x10 of the
// object at +0x1b8) is a CCharClassLifeComponent and the owner has a character class (+0x220): stops its
// layer 0xb unless it is in a CCharClassCombatTrack, then sends it the hash of the id the life component's
// vf_0x40 returns for the owner (descriptors as UnkSpitSend). Then runs the base handler. Names are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkActorTarget;
void fn_7101178e08(UnkActorTarget*, u32, u32, u32, f32);
struct UnkComponent;
void fn_71003aea40(UnkComponent* self);

struct UnkBigFistIdData_b59 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkBigFistLife_b59 : base::reflection::UnkReflObject {
    UnkBigFistIdData_b59** vf_0x40(void* owner) {
        return reinterpret_cast<UnkBigFistIdData_b59** (*)(UnkBigFistLife_b59*, void*)>(
            (*reinterpret_cast<void***>(this))[0x40 / 8])(this, owner);
    }
};

struct UnkBigFistLifeSlot_b59 {
    u8 pad_0x0[0x10];
    UnkBigFistLife_b59* field_0x10;
};

struct UnkBigFistOwner_b59 {
    u8 pad_0x0[0x1b8];
    UnkBigFistLifeSlot_b59* field_0x1b8;
    u8 pad_0x1c0[0x60];
    UnkAnimationComponent* field_0x220;
};

struct UnkBigFistAI_b59 {
    u8 pad_0x0[0x30];
    UnkBigFistOwner_b59* owner;
};

void fn_71003e5ef8(UnkBigFistAI_b59* self, void* arg) {
    UnkBigFistOwner_b59* owner = self->owner;
    if (UnkBigFistLifeSlot_b59* slot = owner->field_0x1b8) {
        if (UnkBigFistLife_b59* life = slot->field_0x10) {
            UnkAnimationComponent* anim = owner->field_0x220;
            bool isa = fn_710009d50c(life->vf_0x0(), fn_71008bbf8c());
            UnkBigFistLife_b59* cast = isa ? life : nullptr;
            if (anim && isa) {
                if (!fn_710117b348(anim, fn_7100c1181c()))
                    fn_7101178e08(reinterpret_cast<UnkActorTarget*>(anim), 0xb, 0, 0, -1.0f);
                UnkBigFistOwner_b59* o = self->owner;
                UnkAnimationComponent* target = o->field_0x220;
                UnkSpitSend(target, (*cast->vf_0x40(o))->hash, 0xf4061ff25a2163d6);
            }
        }
    }
    reinterpret_cast<void (*)(UnkBigFistAI_b59*, void*)>(fn_71003aea40)(self, arg);
}

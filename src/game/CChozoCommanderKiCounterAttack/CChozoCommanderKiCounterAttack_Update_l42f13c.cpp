// CChozoCommanderKiCounterAttack's unit (tools/tu_map.py): the update. After the base update, while the
// owner is a Chozo Commander with a target, runs down the cooldown at +0x5c, sends the owner's class one of
// two counter requests once the target is behind it or the time runs out, and while the flag at +0x5b is
// set sends a closing request when the target is within reach. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CMeleeComponent.h"
#include "game/UnkActor.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkKiCounterTunables_l42f13c {
    u8 pad_0x0[0xfc];
    f32 field_0xfc;  // how long the attack waits for the target to come round
    u8 pad_0x100[0x4];
    f32 field_0x104;  // the reach of the closing request
};

struct UnkKiCounter_l42f13c {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    f32 elapsed;  // 0x10
    u8 pad_0x14[0x3c];
    UnkKiCounterTunables_l42f13c* field_0x50;
    bool field_0x58;  // a counter request went out
    bool field_0x59;
    bool field_0x5a;
    bool field_0x5b;
    f32 cooldown;  // 0x5c
};

// The actor slots this unit reads beyond UnkActor's.
struct UnkActorSlots_l42f13c {
    u8 pad_0x0[0x2a0];
    void* field_0x2a0;
    u8 pad_0x2a8[0x50];
    void* field_0x2f8;  // the melee component (fn_710092b6f8)
};

// The base update (a thunk to fn_71003609a8).
void fn_710042bb44(UnkKiCounter_l42f13c* self, f32 dt);

static inline UnkActorSlots_l42f13c* Slots_l42f13c(UnkActor* actor) {
    return reinterpret_cast<UnkActorSlots_l42f13c*>(actor);
}

// Sends a request with mode 0 and the all-bits mask; the caller owns the two arguments.
static inline bool Send_l42f13c(void* cls, u64 hash, base::reflection::UnkReflTypeIdArg& mode,
                                base::reflection::UnkReflTypeIdArg& mask) {
    mode.id = 0;
    mask.id = UnkAll_SpitSend().value;
    return fn_7101176690(cls, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

void CChozoCommanderKiCounterAttack_UpdateAndSendLaunchRequests(UnkKiCounter_l42f13c* self, f32 dt) {
    // Declared past a goto (no lifetime markers), so each keeps its own slot.
    goto start;
    base::reflection::UnkReflTypeIdArg modeA, maskA, modeB, maskB, closeMode, closeMask;
start:
    fn_710042bb44(self, dt);
    auto* component = static_cast<base::reflection::UnkReflObject*>(self->owner->field_0x240);
    auto* commander = component && fn_710009d50c(component->vf_0x0(), fn_710044e708()) ? component : nullptr;
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!commander || !target)
        return;
    UnkActor* owner = self->owner;
    if (!Slots_l42f13c(owner)->field_0x2a0)
        return;
    void* cls = owner->field_0x220;
    if (!cls)
        return;

    if (!self->field_0x58) {
        bool expired = false;
        if (self->cooldown >= 0.0f) {
            self->cooldown -= dt;
            if (self->cooldown <= 0.0f) {
                expired = true;
                self->cooldown = 0.0f;
            }
        }
        if (self->field_0x59) {
            f32 dz = target->field_0x64 - owner->field_0x64;
            f32 dy = target->field_0x54 - owner->field_0x54;
            f32 dx = target->field_0x44 - owner->field_0x44;
            const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
            f32 fx = m.m[2];
            f32 fy = m.m[6];
            f32 fz = m.m[10];
            bool behind = dx * fx + dy * fy + dz * fz < 0.0f;
            if (behind || self->elapsed >= self->field_0x50->field_0xfc || expired) {
                bool sent;
                if (self->field_0x5a)
                    sent = Send_l42f13c(cls, 0x97f6fea0def7e5b8, modeA, maskA);
                else
                    sent = Send_l42f13c(cls, 0xf8f92abf24464cb2, modeB, maskB);
                self->field_0x58 = sent;
            }
        }
    }

    if (self->field_0x5b) {
        void* melee = Slots_l42f13c(target)->field_0x2f8;
        if (!melee || !fn_710092b6f8(melee)) {
            self->field_0x58 = false;
            self->field_0x5b = false;
            self->cooldown = 0.0f;
            return;
        }
        if (__builtin_fabsf(self->owner->field_0x44 - target->field_0x44) <= self->field_0x50->field_0x104) {
            Send_l42f13c(cls, 0x089dc946f5ecf470, closeMode, closeMask);
            self->field_0x5b = false;
        }
    }
}

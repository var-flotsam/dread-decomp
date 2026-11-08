// CCooldownXBossLaserBiteAttack's unit (tools/tu_map.py): after the base update, with the owner's (+0x8)
// character class (+0x220) in a track of the class fn_7100bf9a78 names: vf_0xe8 and fn_71004c1f18;
// otherwise vf_0xf0 and fn_71004c1e08, then, once the flag at +0x74 is set (it is cleared), sends the class
// a request (descriptors as UnkSpitSend) unless the actor the owner acts against has its +0x200 object's
// flag at +0x3a8 set and the owner's +0x228 answers a fixed id with a height within the limit at +0x104 of
// the tunables at +0x68 of the actor's. Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkWhipSend_b58;
void fn_71004c1968(UnkWhipSend_b58* self, f32 dt);  // the base update
struct UnkGrappleAnswer_b50;
void fn_710117364c(void* target, const u64* id, UnkGrappleAnswer_b50* out);

struct UnkLaserBite_b59;
void fn_71004c1f18(UnkLaserBite_b59* self);
void fn_71004c1e08(UnkLaserBite_b59* self);

struct UnkLaserBiteAnswer_b59 {
    f32 field_0x0;
    u8 pad_0x4[4];
    u32 field_0x8;
};

struct UnkLaserBiteOwner_b59 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
    void* field_0x228;
};

struct UnkLaserBiteTarget_b59 {
    u8 pad_0x0[0x3a8];
    bool field_0x3a8;
};

struct UnkLaserBiteTunables_b59 {
    u8 pad_0x0[0x104];
    f32 field_0x104;
};

struct UnkLaserBite_b59 {
    void** vtable;
    UnkLaserBiteOwner_b59* owner;
    u8 pad_0x10[0x58];
    UnkLaserBiteTunables_b59* field_0x68;
    u8 pad_0x70[4];
    bool field_0x74;

    void vf_0xe8() { reinterpret_cast<void (*)(UnkLaserBite_b59*)>(vtable[0xe8 / 8])(this); }
    void vf_0xf0() { reinterpret_cast<void (*)(UnkLaserBite_b59*)>(vtable[0xf0 / 8])(this); }
};

// `dt` (s0) reaches the base update untouched in the original (tools/forwarded_args.py).
void fn_71004c2814(UnkLaserBite_b59* self, f32 dt) {
    fn_71004c1968(reinterpret_cast<UnkWhipSend_b58*>(self), dt);
    UnkAnimationComponent* anim = self->owner->field_0x220;
    if (!anim)
        return;
    if (fn_710117b348(anim, fn_7100bf9a78())) {
        self->vf_0xe8();
        fn_71004c1f18(self);
        return;
    }
    self->vf_0xf0();
    fn_71004c1e08(self);
    if (!self->field_0x74)
        return;
    self->field_0x74 = false;
    UnkActor* actor = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
    UnkLaserBiteTarget_b59* t;
    if (!actor || !(t = *reinterpret_cast<UnkLaserBiteTarget_b59**>(reinterpret_cast<char*>(actor) + 0x200)) ||
        !t->field_0x3a8)
        goto send;
    // Jumped over: no lifetime markers, so these keep their own slots.
    u64 id;
    UnkLaserBiteAnswer_b59 answer;
    id = 0xd17dcd858381cddf;
    *reinterpret_cast<u64*>(&answer) = 0;
    answer.field_0x8 = 0;
    if (reinterpret_cast<bool (*)(void*, const u64*, UnkLaserBiteAnswer_b59*)>(fn_710117364c)(
            self->owner->field_0x228, &id, &answer) &&
        __builtin_fabsf(answer.field_0x0 - actor->field_0x44) <= self->field_0x68->field_0x104)
        return;
send:
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(anim, 0xc838cd6c37cf6606, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

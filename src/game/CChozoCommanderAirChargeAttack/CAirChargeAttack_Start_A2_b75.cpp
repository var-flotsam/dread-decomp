// CChozoCommanderAirChargeAttack's unit (tools/tu_map.py unit 643): the attack's start. After the base start
// (fn_7100424b5c), mode 1 (+0x58) hands over to fn_7100425268; mode 2, while the owner's (+0x8) animation (+0x220)
// is in a CCharClassAttackCanBeAbortedTrack and the owner's x (+0x44) is inside the CChozoCommanderAIComponent's
// range (+0x15d1c / +0x15d24) widened by the tunable's margin (+0x2c of g_unk_71726c5150; which bound depends on
// fn_7101178e6c(anim, 4)), sends the animation a mode-0 request (as src/game_s2/UnkSpitSend.h) and hooks the
// callback fn_71004263c4 (link at +0x68, handle at +0x80) up to it, as
// src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTUProperty.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkTakumakuComponent;
bool fn_7101178e6c(UnkTakumakuComponent*, u32);
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);

extern UnkTunable g_unk_71726c5150;

struct UnkAirChargeAttack_A2_b75;
struct UnkChozoCommanderAttack;  // game_s2/UnkChozoCommanderAttack.h
void fn_7100424b5c(UnkChozoCommanderAttack* self);
void fn_7100425268(UnkAirChargeAttack_A2_b75* self);
void fn_71004263c4();

struct UnkAirChargeLink_A2_b75 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkAirChargeAttack_A2_b75 {
    u8 pad_0x0[0x8];
    char* owner;
    u8 pad_0x10[0x48];
    s32 mode;  // 0x58
    u8 pad_0x5c[0xc];
    UnkAirChargeLink_A2_b75 link;  // 0x68
    UnkWeakHandle* handle;         // 0x80
};

// `dt` (s0) reaches the base update untouched in the original (tools/forwarded_args.py).
void fn_7100425b98(UnkAirChargeAttack_A2_b75* self, f32 dt) {
    reinterpret_cast<void (*)(UnkChozoCommanderAttack*, f32)>(fn_7100424b5c)(reinterpret_cast<UnkChozoCommanderAttack*>(self), dt);
    switch (self->mode) {
    case 1:
        fn_7100425268(self);
        return;
    case 2:
        break;
    default:
        return;
    }
    void* anim = *reinterpret_cast<void**>(self->owner + 0x220);
    if (!anim)
        return;
    if (!fn_710117b348(static_cast<UnkAnimationComponent*>(anim), fn_7100c0ccc8()))
        return;
    UnkReflObject* ai = *reinterpret_cast<UnkReflObject**>(self->owner + 0x240);
    UnkReflObject* commander = nullptr;
    if (ai)
        commander = fn_710009d50c(ai->vf_0x0(), fn_710044e708()) ? ai : nullptr;
    const f32* range = reinterpret_cast<const f32*>(reinterpret_cast<char*>(commander) + 0x15d1c);
    f32 margin = *reinterpret_cast<const f32*>(reinterpret_cast<char*>(&g_unk_71726c5150) + 0x2c);
    bool high = fn_7101178e6c(static_cast<UnkTakumakuComponent*>(anim), 4);
    f32 x = *reinterpret_cast<const f32*>(self->owner + 0x44);
    if (high) {
        if (!(x <= margin + range[0]))
            return;
    } else {
        if (!(x >= range[2] - margin))
            return;
    }
    UnkSpitSend(anim, 0x407fa97ab1c276fb, 0xf4061ff25a2163d6);
    UnkAirChargeLink_A2_b75* link = &self->link;
    link->context = self;
    link->callback = fn_71004263c4;
    UnkWeakTarget* track = *reinterpret_cast<UnkWeakTarget**>(self->owner + 0x220);
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    if (!track)
        return;
    asm("" : "+r"(self));
    fn_7100316018(track, &self->handle);
    fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
}

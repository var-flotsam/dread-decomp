// CCharClassGobblerAIComponent's unit (tools/tu_map.py unit 831): fn_71005816d4 picks a result
// for an actor: 2 when the actor's +0x220 component has a CCharClassAllowCrazyTrack
// (fn_710117b348); otherwise 5 when the actor's AI (+0x240) is a CGobblerAIComponent that accepts
// the third argument's +0x60 (fn_71005824cc) and the second argument's +0x2f8 passes
// fn_710092b6f8; else 4. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CMeleeComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::reflection::UnkReflObject;

bool fn_71005824cc(UnkReflObject* ai, void* arg);

struct UnkGobblerActor_A3_b66 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* track;  // +0x220
    u8 pad_0x228[0x18];
    UnkReflObject* ai;  // +0x240
};

struct UnkGobblerOther_A3_b66 {
    u8 pad_0x0[0x2f8];
    void* field_0x2f8;
};

struct UnkGobblerArg_A3_b66 {
    u8 pad_0x0[0x60];
    void* field_0x60;
};

u32 fn_71005816d4(void* self, UnkGobblerActor_A3_b66* actor, UnkGobblerOther_A3_b66* other,
                  UnkGobblerArg_A3_b66* arg) {
    UnkAnimationComponent* track = actor->track;
    void* target = other ? other->field_0x2f8 : nullptr;
    UnkReflObject* ai = actor->ai;
    UnkReflObject* gobbler = ai && fn_710009d50c(ai->vf_0x0(), fn_7100581a4c()) ? ai : nullptr;
    if (track && fn_710117b348(track, fn_7100ce9338()))
        return 2;
    if (!arg || !gobbler)
        return 4;
    if (!fn_71005824cc(gobbler, arg->field_0x60) || !target)
        return 4;
    return fn_710092b6f8(target) ? 5 : 4;
}

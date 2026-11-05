// CSamusFloorSlideMovementState unit (tools/tu_map.py): unless `skip`, checks the owner's (+0x30)
// animation component (+0x220) for the tracks it holds (fn_710117b348 with the track classes' getters).
// In state 3 without the enlarge-collision track, virtual slot 38 decides between leaving through
// fn_710099db00 and going on; otherwise a tweak-fall track ends it. Then sends the component a hash by
// the slide track it has (45, 26 or neither) with mode 0 (UnkSpitSend, src/game_s2/UnkSpitSend.h) and
// moves to state 5. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkFloorSlideTrackOwner_b57 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* anim;
};

struct UnkFloorSlideTrackState_b57 {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    virtual void vf_3();
    virtual void vf_4();
    virtual void vf_5();
    virtual void vf_6();
    virtual void vf_7();
    virtual void vf_8();
    virtual void vf_9();
    virtual void vf_10();
    virtual void vf_11();
    virtual void vf_12();
    virtual void vf_13();
    virtual void vf_14();
    virtual void vf_15();
    virtual void vf_16();
    virtual void vf_17();
    virtual void vf_18();
    virtual void vf_19();
    virtual void vf_20();
    virtual void vf_21();
    virtual void vf_22();
    virtual void vf_23();
    virtual void vf_24();
    virtual void vf_25();
    virtual void vf_26();
    virtual void vf_27();
    virtual void vf_28();
    virtual void vf_29();
    virtual void vf_30();
    virtual void vf_31();
    virtual void vf_32();
    virtual void vf_33();
    virtual void vf_34();
    virtual void vf_35();
    virtual void vf_36();
    virtual void vf_37();
    virtual bool vf_38(bool a, bool b, void* c);
    u8 pad_0x8[0x28];
    UnkFloorSlideTrackOwner_b57* owner;
    u8 pad_0x38[0x198 - 0x38];
    s32 state;
};

void fn_710099db00(UnkFloorSlideTrackState_b57* self, bool flag);

void fn_71009947e0(UnkFloorSlideTrackState_b57* self, bool skip) {
    if (skip)
        return;
    UnkAnimationComponent* anim = self->owner->anim;
    bool falling = fn_710117b348(anim, fn_7100d2fa08());
    if (self->state == 3 && self->owner->anim && !fn_710117b348(self->owner->anim, fn_7100cf8778())) {
        if (self->vf_38(false, false, nullptr)) {
            fn_710099db00(self, false);
            return;
        }
    } else if (falling) {
        return;
    }
    u64 hash;
    if (fn_710117b348(anim, fn_7100c1a968()))
        hash = 0xf85faf4733567310;
    else if (fn_710117b348(anim, fn_7100c1a458()))
        hash = 0x2e63b85dc34d3423;
    else
        hash = 0x005732a6bc3abead;
    UnkSpitSend(anim, hash, 0xf4061ff25a2163d6);
    self->state = 5;
}

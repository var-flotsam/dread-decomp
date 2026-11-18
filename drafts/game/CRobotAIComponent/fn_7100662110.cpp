// CRobotAIComponent unit (tools/tu_map.py): after the base step, when the owner's (+0x30) life
// component (+0x10 of the object at +0x1b8) is a CCharClassLifeComponent whose id for the owner is
// empty: the character class (+0x220) gets fn_710117ab64 with 1 and g_unk_71726fd320, and the owner's
// timeline (+0x2e8) gets the id the object at +0x146c8 names (vf_0xd8) set to the all-flags mask.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU2702.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_71726fd320[] __attribute__((visibility("hidden")));

struct UnkRobotLife_l662110 : base::reflection::UnkReflObject {
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual const CStrId* vf_0x40(UnkActor* owner);
};

struct UnkRobotLifeSlot_l662110 {
    u8 pad_0x0[0x10];
    UnkRobotLife_l662110* life;
};

struct UnkRobotIdSource_l662110 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual u64 vf_0xd8();
};

struct UnkRobotAI_l662110 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x146c8 - 0x38];
    UnkRobotIdSource_l662110* field_0x146c8;
};

// `arg` (x1) reaches the base step untouched in the original (tools/forwarded_args.py).
void fn_7100662110(UnkRobotAI_l662110* self, void* arg) {
    fn_710110cca4(self, arg);
    auto* slot = static_cast<UnkRobotLifeSlot_l662110*>(self->owner->field_0x1b8);
    if (!slot)
        return;
    UnkRobotLife_l662110* life = slot->life;
    if (!life || !fn_710009d50c(life->vf_0x0(), fn_71008bbf8c()))
        return;
    if (life->vf_0x40(self->owner)->data != &g_unk_7101d47108->empty)
        return;
    if (auto* animation = static_cast<UnkAnimationComponent*>(self->owner->field_0x220))
        fn_710117ab64(animation, 1, g_unk_71726fd320);
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        u64 id = self->field_0x146c8->vf_0xd8();
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &id, &mask.id, false, -1.0f);
    }
}

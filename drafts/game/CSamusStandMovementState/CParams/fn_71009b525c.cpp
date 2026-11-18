// CSamusStandMovementState::CParams unit (tools/tu_map.py): entering the stand state. After the base
// handler, cancels the player's speed boost, keeps where the owner stands and clears the turn rate.
// While a stand-reposition track plays, asks the animation (vtable slot 0x2e8) for its two tracks:
// unless both are set and under 120, a set first track under 60 gives the rate (60 less its value,
// against the owner's facing). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CSamusMovementState/CParams.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU363.h"
#include "types.h"

struct UnkSlideState_ovb0004;
void fn_710033e494(UnkAbility* ability);  // stops the speed boost

// A track the animation reports: which (-1: none), its layer (0xff: none) and how far it has run.
struct UnkStandTrack_l9b525c {
    s16 id;
    u8 layer;
    u8 pad_0x3[9];
    f32 time;  // 0xc

    UnkStandTrack_l9b525c() { fn_71002878dc(reinterpret_cast<UnkMsg_b152*>(this)); }
    ~UnkStandTrack_l9b525c() { fn_7100287954(reinterpret_cast<UnkMsg_b152*>(this)); }
    bool IsSet() const { return id != -1 && layer != 0xff; }
};

struct UnkStandAnimation_l9b525c {
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
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual void vf_0x1c0();
    virtual void vf_0x1c8();
    virtual void vf_0x1d0();
    virtual void vf_0x1d8();
    virtual void vf_0x1e0();
    virtual void vf_0x1e8();
    virtual void vf_0x1f0();
    virtual void vf_0x1f8();
    virtual void vf_0x200();
    virtual void vf_0x208();
    virtual void vf_0x210();
    virtual void vf_0x218();
    virtual void vf_0x220();
    virtual void vf_0x228();
    virtual void vf_0x230();
    virtual void vf_0x238();
    virtual void vf_0x240();
    virtual void vf_0x248();
    virtual void vf_0x250();
    virtual void vf_0x258();
    virtual void vf_0x260();
    virtual void vf_0x268();
    virtual void vf_0x270();
    virtual void vf_0x278();
    virtual void vf_0x280();
    virtual void vf_0x288();
    virtual void vf_0x290();
    virtual void vf_0x298();
    virtual void vf_0x2a0();
    virtual void vf_0x2a8();
    virtual void vf_0x2b0();
    virtual void vf_0x2b8();
    virtual void vf_0x2c0();
    virtual void vf_0x2c8();
    virtual void vf_0x2d0();
    virtual void vf_0x2d8();
    virtual void vf_0x2e0();
    virtual void vf_0x2e8(UnkStandTrack_l9b525c* first, bool* first_flag, UnkStandTrack_l9b525c* second,
                          bool* second_flag);
};

struct UnkStandState_l9b525c {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    UnkStandAnimation_l9b525c* animation;  // 0x38
    u8 pad_0x40[0x154];
    base::math::CVector3D position;  // 0x194
    f32 turn;  // 0x1a0
};
static_assert(__builtin_offsetof(UnkStandState_l9b525c, turn) == 0x1a0, "layout");

void fn_71009b525c(UnkStandState_l9b525c* self, const u64* event) {
    fn_71009b1ae8(reinterpret_cast<UnkSlideState_ovb0004*>(self), event);
    if (UnkAbility* boost = fn_7100df5928(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310)))
        fn_710033e494(boost);
    UnkActor* owner = self->owner;
    self->position = {owner->field_0x44, owner->field_0x54, owner->field_0x64};
    self->turn = 0.0f;
    auto* animation = static_cast<UnkAnimationComponent*>(owner->field_0x220);
    if (!animation || !fn_710117b348(animation, fn_7100c467d8()))
        return;
    UnkStandTrack_l9b525c first;
    UnkStandTrack_l9b525c second;
    bool first_flag = false;
    bool second_flag = false;
    self->animation->vf_0x2e8(&first, &first_flag, &second, &second_flag);
    if (second.IsSet() && first.IsSet() && second.time < 120.0f && first.time < 120.0f)
        return;
    if (first.IsSet() && first.time < 60.0f) {
        f32 left = 60.0f - first.time;
        f32 facing = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38)).m[2];
        s32 sign = (facing > 0.0f) - (facing < 0.0f);
        self->turn = -(left * sign);
    }
}

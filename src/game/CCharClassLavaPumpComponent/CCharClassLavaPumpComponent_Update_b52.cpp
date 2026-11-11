// CCharClassLavaPumpComponent unit (unit 1333 of tools/tu_map.py): the update, fn_7100879d60. After
// the base update (fn_71010c424c), while active (+0x91) the remaining time (+0x94) runs down by the
// frame time, clamped to the tunable's duration (g_unk_717272a728, +0x28) and to zero, and its
// share of the duration is passed to the floating prop found for the actor (+0x30, fn_71007e4924;
// cached at +0x88) through fn_710117ad18. Once the share is within 0.001 of zero both flags (+0x90,
// +0x91) are cleared. Names and layouts are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAnimationComponent.h"
#include "game/CCharClassEventScenarioComponent.h"
#include "game/CComponent.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkParent;
struct UnkWeakTarget;
struct UnkActionSwitcherAnimation_b48;

// As src/reflection/CCharClassLavaPumpComponent.cpp declares it.
extern base::reflection::UnkTunable g_unk_717272a728;

struct UnkLavaPumpTunable_b52 {
    u8 unk_0x0[0x28];
    f32 duration;
};

struct UnkLavaPump_b52 {
    u8 unk_0x0[0x30];
    UnkActor* actor;  // 0x30
    u8 unk_0x38[0x50];
    UnkWeakTarget* prop;  // 0x88
    bool field_0x90;
    bool active;  // 0x91
    u8 unk_0x92[2];
    f32 time;  // 0x94
};

void fn_7100879d60(UnkLavaPump_b52* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    if (!self->active)
        return;
    UnkWeakTarget* prop = self->prop;
    if (!prop) {
        prop = self->actor ? fn_71007e4924(self->actor) : nullptr;
        self->prop = prop;
        if (!prop)
            return;
    }
    f32 time = self->time - dt;
    f32 duration = reinterpret_cast<UnkLavaPumpTunable_b52*>(&g_unk_717272a728)->duration;
    f32 clamped = time <= 0.0f ? 0.0f : (time >= duration ? duration : time);
    f32 share = clamped / duration;
    self->time = clamped;
    f32 level = !(share <= 0.0f) ? (!(share >= 1.0f) ? share : 1.0f) : 0.0f;
    fn_710117ad18(reinterpret_cast<UnkActionSwitcherAnimation_b48*>(prop), false, level);
    if (level + 0.001f >= 0.0f && level - 0.001f <= 0.0f) {
        self->field_0x90 = false;
        self->active = false;
    }
}

// Unit 1501 (CSamusMovement): the movement's per-frame update. It runs the base update, counts its
// timers, follows the ignore-jump track, eases a value towards its target, moves the body (slowed
// with the game's slow motion for the player) and copies its transform out. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CActorComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkSlomoFlags.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2689.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkMorphBallMovement_01d8;
struct UnkGroundPlayer_b52;
struct UnkContact_ovb0048;

// What the ignore-jump track's record holds: whether the track says to keep ignoring the jump.
struct UnkIgnoreJumpRecord_l9e13b4 {
    u8 pad_0x0[0x18];
    bool ignore;
};

struct UnkOwner_l9e13b4 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkReaction_l9e13b4;
struct UnkReactionVtable_l9e13b4 {
    void* slots[0x98 / 8];
    void (*vf_0x98)(UnkReaction_l9e13b4* self);
};
struct UnkReaction_l9e13b4 {
    UnkReactionVtable_l9e13b4* vtable;
};

// The moving body: a word, then its transform.
struct UnkBody_l9e13b4 {
    u32 field_0x0;
    UnkXform xform;
};

// What the eased value's target comes from.
struct UnkTargetSource_l9e13b4 {
    u8 pad_0x0[0xc4];
    f32 target;
};

// The jump state (+0x2300): whether the ignore-jump track holds, what reacts when it ends, and a
// timer counting up to its limit.
struct UnkJumpState_l9e13b4 {
    bool ignore_jump;
    u8 pad_0x1[0xa08 - 0x1];
    UnkReaction_l9e13b4* reaction;  // 0xa08
    u8 pad_0xa10[0x2e90 - 0xa10];
    f32 field_0x2e90;
    f32 field_0x2e94;
    u8 pad_0x2e98[4];
    f32 limit;  // 0x2e9c
    f32 time;   // 0x2ea0
};

struct UnkSamusMovement_l9e13b4 {
    u8 pad_0x0[0x30];
    UnkOwner_l9e13b4* owner;
    u8 pad_0x38[0x84 - 0x38];
    UnkVec3f position;  // 0x84
    UnkVec3f angle;     // 0x90
    u8 pad_0x9c[0xf9 - 0x9c];
    bool field_0xf9;
    u8 pad_0xfa[0x128 - 0xfa];
    u8 contact[0x280 - 0x128];  // 0x128
    UnkBody_l9e13b4 body;       // 0x280
    u8 pad_0x2e0[0x6f4 - 0x2e0];
    bool field_0x6f4;
    u8 pad_0x6f5[0x2290 - 0x6f5];
    f32 value;        // 0x2290: eased towards its target
    f32 next_value;   // 0x2294
    f32 value_time;   // 0x2298
    f32 value_rate;   // 0x229c
    f32 target_rate;  // 0x22a0
    u8 pad_0x22a4[0x2300 - 0x22a4];
    UnkJumpState_l9e13b4 jump;  // 0x2300
    u8 pad_0x51a8[0x51cc - 0x51a8];  // the jump state is padded to 8
    bool field_0x51cc;
    u8 pad_0x51cd[3];
    f32 field_0x51d0;
    u8 pad_0x51d4[0x5368 - 0x51d4];
    f32 field_0x5368;
    u8 pad_0x536c[0x5458 - 0x536c];
    UnkTargetSource_l9e13b4* target_source;
};
static_assert(__builtin_offsetof(UnkSamusMovement_l9e13b4, body) == 0x280, "layout");
static_assert(__builtin_offsetof(UnkSamusMovement_l9e13b4, value) == 0x2290, "layout");
static_assert(__builtin_offsetof(UnkSamusMovement_l9e13b4, jump) == 0x2300, "layout");
static_assert(__builtin_offsetof(UnkSamusMovement_l9e13b4, field_0x51cc) == 0x51cc, "layout");
static_assert(__builtin_offsetof(UnkSamusMovement_l9e13b4, field_0x51d0) == 0x51d0, "layout");
static_assert(__builtin_offsetof(UnkSamusMovement_l9e13b4, field_0x5368) == 0x5368, "layout");
static_assert(__builtin_offsetof(UnkSamusMovement_l9e13b4, target_source) == 0x5458, "layout");

UnkVec3f CMovementComponent_GetContactTangent(bool flag, const UnkContact_ovb0048* contact);
void fn_71009e1714(UnkSamusMovement_l9e13b4* self, f32 dt);
bool fn_71009e9bb8(UnkSamusMovement_l9e13b4* self);
void fn_71009e244c(UnkSamusMovement_l9e13b4* self, f32 dt);
void fn_7100978aa8(UnkBody_l9e13b4* body, UnkOwner_l9e13b4* owner, f32 dt);
void fn_71010db628(UnkSamusMovement_l9e13b4* self, f32 dt);
void fn_71009e2778(UnkSamusMovement_l9e13b4* self, UnkBody_l9e13b4* body);
void fn_71009e29ec(UnkSamusMovement_l9e13b4* self, const UnkVec3f* side);

void fn_71009e13b4(UnkSamusMovement_l9e13b4* self, f32 dt) {
    fn_71010d7b28(reinterpret_cast<UnkMorphBallMovement_01d8*>(self), dt);
    if (!g_unk_7101d47310->field_0x608)
        return;

    if (self->field_0x5368 >= 0.0f)
        self->field_0x5368 += dt;
    if (self->jump.time < self->jump.limit) {
        self->jump.time += dt;
        if (self->jump.time >= self->jump.limit) {
            self->jump.time = self->jump.limit;
            self->jump.field_0x2e90 = 0.0f;
            self->jump.field_0x2e94 = 0.0f;
        }
    }
    if (self->field_0x51d0 > 0.0f)
        self->field_0x51d0 -= dt;

    fn_71009e1714(self, dt);
    if (!self->owner || !fn_710024fbc8(self))
        return;
    if (self->field_0x51cc)
        fn_71009e1fd4(reinterpret_cast<UnkGroundPlayer_b52*>(self));

    if (self->jump.ignore_jump) {
        UnkTrackQuery query;
        query.cls = fn_7100d0dbe8();
        query.result = nullptr;
        query.value = -1.0f;
        UnkAnimationComponent* animation;
        if (self->owner && (animation = self->owner->animation) &&
            (fn_710117b08c(animation, &query), query.result)) {
            self->jump.ignore_jump = reinterpret_cast<UnkIgnoreJumpRecord_l9e13b4*>(query.result)->ignore;
        } else {
            bool react = fn_71009e9bb8(self);
            self->jump.ignore_jump = false;
            if (react && self->jump.reaction)
                self->jump.reaction->vtable->vf_0x98(self->jump.reaction);
        }
    }

    fn_71009e244c(self, dt);

    if (self->value_time > 0.0f) {
        if (self->value_rate >= 0.0f) {
            f32 step = self->value_rate * dt;
            f32 target = self->next_value;
            f32 value = self->value;
            if (target > value) {
                value += step;
                self->value = target < value ? target : value;
            } else {
                value -= step;
                self->value = target > value ? target : value;
            }
        } else {
            self->value = self->next_value;
        }
        self->value_time -= dt;
    } else {
        f32 target = self->target_source->target;
        f32 value = self->value;
        if (!(value + 0.00001f >= target && value - 0.00001f <= target)) {
            if (self->target_rate >= 0.0f) {
                f32 step = self->target_rate * dt;
                if (target > value) {
                    value += step;
                    self->value = target < value ? target : value;
                } else {
                    value -= step;
                    self->value = target > value ? target : value;
                }
            } else {
                self->value = target;
                self->value_time = -1.0f;
            }
        }
    }

    f32 step = dt;
    UnkGameManager* manager = g_unk_7101d47310;
    if (manager->field_0xc8 != 0 || manager->field_0x10c) {
        UnkSlomoFlags* slomo = reinterpret_cast<UnkSlomoFlags*>(&manager->field_0xc4);
        if (slomo->enabled && !slomo->paused &&
            reinterpret_cast<UnkPlayer*>(self->owner) == fn_7100dec788(manager))
            step = fn_71010d6858() * dt;
    }
    fn_7100978aa8(&self->body, self->owner, step);
    self->position = self->body.xform.GetTranslation();
    self->angle = self->body.xform.GetAngle();
    UnkVec3f side = CMovementComponent_GetContactTangent(self->field_0xf9, reinterpret_cast<const UnkContact_ovb0048*>(self->contact));
    if (self->field_0x6f4)
        fn_71010db628(self, dt);
    fn_71009e2778(self, &self->body);
    fn_71009e29ec(self, &side);
}

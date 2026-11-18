// Unit 1501 (CSamusMovement): one movement step. Runs the base motion, applies the morph-ball displacement,
// lets the attached components see the body, clamps the vertical speed to the tunable range (scaled in slow
// motion for the player), moves the body, drops the held links once their target is gone, runs the
// timed wall push and hands the body's position to the owner. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityLayout.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkMovement3_ovb0048;
struct UnkAimComponent;
struct UnkAimOutput;
struct UnkMovement_l9e307c;
struct UnkMove_b152;

void fn_71010d1e88(UnkMovement3_ovb0048* self, void* a, void* b, f32 t, void* c, void* d, void* e);
void fn_7100352240(UnkAimComponent* self, UnkAimOutput* out);
void fn_71009e307c(UnkMovement_l9e307c* self, const f32* amount);
void fn_71009e3188(UnkMove_b152* self, f32* values);
void fn_71010db084(void* unused, void* arg);
void fn_7100301750(CAbilityComponent* self, void* arg);
extern base::reflection::UnkTunable g_unk_7172a03d80;

// The slow motion bytes of the game manager (+0xc4).
struct UnkSlomoFlags {
    bool enabled;  // 0xc4
    u8 unk_0xc5;
    bool paused;  // 0xc6
};

// The slow motion tunable: its time scale.
struct UnkSlomoTunable_l9e2c34 {
    u8 pad_0x0[0x2c];
    f32 scale;  // 0x2c
};

struct UnkStaticZero_l9e2c34 {
    u32 value;
    UnkStaticZero_l9e2c34() : value(0) {}
};
inline const UnkStaticZero_l9e2c34& StaticZero_l9e2c34() {
    static UnkStaticZero_l9e2c34 s_value asm("g_unk_71726b5354");
    return s_value;
}

// What reacts to the movement (+0x2d08).
struct UnkReaction_l9e2c34;
struct UnkReactionVtable_l9e2c34 {
    void* slots_0x0[0xa0 / 8];
    bool (*vf_0xa0)(UnkReaction_l9e2c34* self);
    void* slots_0xa8[2];
    void (*vf_0xb8)(UnkReaction_l9e2c34* self, void* body);
};
struct UnkReaction_l9e2c34 {
    UnkReactionVtable_l9e2c34* vtable;
};

// The character class component of the owner.
struct UnkCharClass_l9e2c34 {
    u8 pad_0x0[0x144];
    f32 field_0x144;
};

struct UnkOwner_l9e2c34 {
    u8 pad_0x0[0x28];
    UnkReflObject* char_class;  // 0x28
    u8 pad_0x30[0x220 - 0x30];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2b0 - 0x228];
    UnkAimComponent* aim;  // 0x2b0
    u8 pad_0x2b8[0x2e0 - 0x2b8];
    CAbilityComponent* abilities;  // 0x2e0
};

// The moving body (+0x280): its transform, a step along x, and the transform it is placed at.
struct UnkBody_l9e2c34 {
    u32 field_0x0;
    UnkXform xform;   // 0x4
    f32 step_x;       // 0x60
    u8 pad_0x64[8];
    u8 field_0x6c[0xc];
    u8 field_0x78[0x40];
    UnkXform placed;  // 0xb8
};
static_assert(__builtin_offsetof(UnkBody_l9e2c34, step_x) == 0x60, "layout");
static_assert(__builtin_offsetof(UnkBody_l9e2c34, placed) == 0xb8, "layout");

struct UnkTunables_l9e2c34 {
    u8 pad_0x0[0xb4];
    f32 min_speed;  // 0xb4
    f32 max_speed;  // 0xb8
};

struct UnkSamusMovement_l9e2c34;
struct UnkSamusMovementVtable_l9e2c34 {
    void* slots_0x0[0x178 / 8];
    bool (*vf_0x178)(UnkSamusMovement_l9e2c34* self);
    void* slots_0x180[2];
    void (*vf_0x190)(UnkSamusMovement_l9e2c34* self, UnkVec3f* position);
    void* slots_0x198[(0x208 - 0x198) / 8];
    void (*vf_0x208)(UnkSamusMovement_l9e2c34* self, UnkVec3f* motion);
    void* slots_0x210[(0x2f8 - 0x210) / 8];
    void (*vf_0x2f8)(UnkSamusMovement_l9e2c34* self, UnkBody_l9e2c34* body, f32 dt);
};

struct UnkSamusMovement_l9e2c34 {
    UnkSamusMovementVtable_l9e2c34* vtable;
    u8 pad_0x8[0x28];
    UnkOwner_l9e2c34* owner;  // 0x30
    u8 pad_0x38[0x1d8 - 0x38];
    UnkVec3f velocity;  // 0x1d8
    u8 pad_0x1e4[0x208 - 0x1e4];
    u8 field_0x208[0x280 - 0x208];
    UnkBody_l9e2c34 body;  // 0x280
    u8 pad_0x3a8_[0x3a8 - 0x280 - sizeof(UnkBody_l9e2c34)];
    bool on_wall;  // 0x3a8
    u8 pad_0x3a9[0x2390 - 0x3a9];
    UnkWeakHandle* link;        // 0x2390
    UnkWeakHandle* second_link;  // 0x2398
    u8 pad_0x23a0[0x23cc - 0x23a0];
    u32 field_0x23cc;
    u8 pad_0x23d0[0x2d08 - 0x23d0];
    UnkReaction_l9e2c34* reaction;  // 0x2d08
    u8 pad_0x2d10[0x51b0 - 0x2d10];
    bool field_0x51b0;
    u8 pad_0x51b1[0x5390 - 0x51b1];
    f32 displacement;       // 0x5390
    f32 displaced;          // 0x5394
    u8 pad_0x5398[0x5450 - 0x5398];
    f32 push_time;  // 0x5450
    f32 push_dir;   // 0x5454
    UnkTunables_l9e2c34* tunables;  // 0x5458

    // While the push timer runs and she still moves the way she was pushed (and is not on a wall),
    // keeps her moving back from it; otherwise, or once it runs out, the push ends.
    void UpdatePush(f32 dt) {
        if (!(push_time > 0.0f))
            return;
        f32 speed = velocity.x;
        s32 sign = (speed > 0.0f) - (speed < 0.0f);
        if (push_dir != 0.0f && push_dir == (f32)sign && !on_wall) {
            f32 time = push_time - dt;
            velocity.x = push_dir * -100.0f;
            push_time = time;
            if (!(time <= 0.0f))
                return;
        }
        push_time = 0.0f;
        push_dir = 0.0f;
    }
};
static_assert(__builtin_offsetof(UnkSamusMovement_l9e2c34, on_wall) == 0x3a8, "layout");
static_assert(__builtin_offsetof(UnkSamusMovement_l9e2c34, link) == 0x2390, "layout");
static_assert(__builtin_offsetof(UnkSamusMovement_l9e2c34, reaction) == 0x2d08, "layout");
static_assert(__builtin_offsetof(UnkSamusMovement_l9e2c34, displacement) == 0x5390, "layout");
static_assert(__builtin_offsetof(UnkSamusMovement_l9e2c34, tunables) == 0x5458, "layout");

void fn_7100976344(UnkSamusMovement_l9e2c34* self);
void fn_71010daf1c(UnkSamusMovement_l9e2c34* self, UnkBody_l9e2c34* body, bool flag, f32 dt);
void fn_71009e3320(UnkSamusMovement_l9e2c34* self, UnkBody_l9e2c34* body);
void fn_71009e35a4(UnkSamusMovement_l9e2c34* self, UnkBody_l9e2c34* body);
void fn_71010db138(UnkSamusMovement_l9e2c34* self, UnkBody_l9e2c34* body, f32 dt);
void fn_7100978c24(UnkBody_l9e2c34* body, UnkVec3f* velocity, void* field_0x208, f32 value);

// The owner's character class component (null when the component at +0x28 is of another class).
static inline UnkCharClass_l9e2c34* CharClass(UnkOwner_l9e2c34* owner) {
    UnkReflObject* component = owner->char_class;
    if (fn_710009d50c(component->vf_0x0(), fn_71011abbd4()))
        return reinterpret_cast<UnkCharClass_l9e2c34*>(component);
    return nullptr;
}
void fn_71009e4204(UnkSamusMovement_l9e2c34* self);
void fn_71010d8fcc(UnkSamusMovement_l9e2c34* self, UnkBody_l9e2c34* body);

void fn_71009e2c34(UnkSamusMovement_l9e2c34* self, f32 dt) {
    fn_7100976344(self);
    bool allowed = self->reaction ? self->reaction->vtable->vf_0xa0(self->reaction) : true;

    UnkVec3f motion(0.0f, 0.0f, 0.0f);
    fn_71010d1e88(reinterpret_cast<UnkMovement3_ovb0048*>(self), &self->body.placed, &self->body.step_x, dt,
                  self->body.field_0x6c, self->body.field_0x78, &motion);
    self->vtable->vf_0x208(self, &motion);

    if (self->displacement != 0.0f) {
        UnkAnimationComponent* animation = self->owner->animation;
        if (animation && fn_710117b348(animation, fn_7100bf1128())) {
            f32 total = self->displacement;
            f32 step = total * 0.2f;
            self->displaced += step;
            if (__builtin_fabsf(self->displaced) >= __builtin_fabsf(total)) {
                step -= self->displaced - total;
                self->displacement = 0.0f;
                self->displaced = 0.0f;
            }
            self->body.step_x = step + self->body.step_x;
        } else {
            self->displacement = 0.0f;
            self->displaced = 0.0f;
        }
    }
    if (self->reaction)
        self->reaction->vtable->vf_0xb8(self->reaction, &self->body);
    if (CAbilityComponent* abilities = self->owner->abilities)
        fn_7100301750(abilities, &self->body);
    if (UnkAimComponent* aim = self->owner->aim)
        fn_7100352240(aim, reinterpret_cast<UnkAimOutput*>(&self->body));
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation || !fn_710117b348(animation, fn_7100d029c8()))
        fn_71010daf1c(self, &self->body, true, dt);
    fn_71009e307c(reinterpret_cast<UnkMovement_l9e307c*>(self), reinterpret_cast<const f32*>(&self->body));
    fn_71009e3188(reinterpret_cast<UnkMove_b152*>(self), reinterpret_cast<f32*>(&self->body));
    fn_71009e3320(self, &self->body);

    f32 min_speed = self->tunables->min_speed;
    f32 max_speed = self->tunables->max_speed;
    UnkGameManager* manager = g_unk_7101d47310;
    if (manager->field_0xc8 != 0 || manager->field_0x10c) {
        UnkSlomoFlags* slomo = reinterpret_cast<UnkSlomoFlags*>(&manager->field_0xc4);
        if (slomo->enabled && !slomo->paused &&
            reinterpret_cast<UnkPlayer*>(self->owner) == fn_7100dec788(manager)) {
            f32 scale = reinterpret_cast<UnkSlomoTunable_l9e2c34*>(&g_unk_7172a03d80)->scale;
            min_speed *= scale;
            max_speed *= scale;
        }
    }
    UnkVec3f& velocity = self->velocity;
    f32 fall = velocity.y;
    velocity.y = fall <= min_speed ? min_speed : (fall >= max_speed ? max_speed : fall);

    fn_71009e35a4(self, &self->body);
    fn_71010db084(self, &self->body);
    fn_71010db138(self, &self->body, dt);
    fn_7100978c24(&self->body, &velocity, self->field_0x208, CharClass(self->owner)->field_0x144);
    fn_71009e4204(self);

    if (self->link && self->link->target) {
        ResetRef(&self->link);
        ResetRef(&self->second_link);
        self->field_0x23cc = StaticZero_l9e2c34().value;
    }
    self->field_0x51b0 = false;

    if (allowed && !self->vtable->vf_0x178(self)) {
        self->UpdatePush(dt);
        self->vtable->vf_0x2f8(self, &self->body, dt);
    }
    fn_71010d8fcc(self, &self->body);

    UnkVec3f position = self->body.placed.GetTranslation();
    self->vtable->vf_0x190(self, &position);
    self->body.placed.SetTranslation(position);
}

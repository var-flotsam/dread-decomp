// CShakernautPiercingLaserAttack's unit: the attack's update. After the base update it reads the laser's
// aim point; state 3 aims the laser until its delay ends (then plays the next id and moves to 4), state 5
// waits for its delay (then hands over to 6), state 6 fades the laser's blend out over its delay and hands
// over to 7. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharacterMovement.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CCircleShape2D.h"
#include "game_s2/UnkSpitAttack.h"
#include "types.h"
#include "../../game_s7/UnkPropBagS7.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

extern const u64 g_unk_710164a660;
extern __attribute__((visibility("hidden"))) const u8 g_unk_710164a674[];
extern CStrId g_unk_7172705c88 __attribute__((visibility("hidden")));
// The four property ids the laser's blend is written to.
extern CStrId g_unk_7172705cb0[4] __attribute__((visibility("hidden")));

struct UnkLavaDropsShape_A2_b71 {
    u8 pad_0x0[0x8];
    u8 body[0xa8];

    UnkLavaDropsShape_A2_b71(const void* source, f32 size) { fn_7100d6169c(this, source, size); }
    ~UnkLavaDropsShape_A2_b71() { fn_7100d61750(this); }
};

struct UnkShakernautAI_l6a7ee4 {
    u8 pad_0x0[0x1220];
    bool field_0x1220;
    u8 pad_0x1221[0x14810 - 0x1221];
    f32 facing_x;  // 0x14810
    f32 facing_y;  // 0x14814
};
static_assert(__builtin_offsetof(UnkShakernautAI_l6a7ee4, facing_x) == 0x14810, "layout");

struct UnkLaserTiming_l6a7ee4 {
    u8 pad_0x0[0xec];
    f32 aim_time;  // 0xec
    u8 pad_0xf0[0x4];
    f32 aim_window;  // 0xf4
    f32 hold_time;   // 0xf8
    f32 fade_time;   // 0xfc
};

struct UnkLaserEffect_l6a7ee4 {
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
    virtual void vf_0x60(bool on);
};

struct UnkShakernautLaser_l6a7ee4 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x18];
    UnkLaserTiming_l6a7ee4* timing;  // 0x28
    s32 state;                       // 0x30
    u8 pad_0x34[0x2c];
    f32 aim_timer;  // 0x60
    u8 pad_0x64[0x4];
    f32 hold_timer;  // 0x68
    f32 fade_timer;  // 0x6c
    u8 pad_0x70[0x4];
    UnkVec3f aim;  // 0x74
    u8 pad_0x80[0x1c];
    UnkVec3f target;  // 0x9c
    f32 blend;         // 0xa8
    u8 pad_0xac[0x4];
    s32 side;  // 0xb0
    u8 pad_0xb4[0x4];
    UnkLaserEffect_l6a7ee4* effect;  // 0xb8
};
static_assert(__builtin_offsetof(UnkShakernautLaser_l6a7ee4, effect) == 0xb8, "layout");

// What the target's +0x208 component holds at +0x2d08: a state, 3 while it holds the target.
struct UnkTargetHold_l6a7ee4 {
    u8 pad_0x0[0x10];
    s32 state;
};

struct UnkTargetComponent_l6a7ee4 {
    u8 pad_0x0[0x2d08];
    UnkTargetHold_l6a7ee4* hold;
};

struct UnkShakernautLaserState_ovb0037;
void fn_71006a7afc(UnkShakernautLaserState_ovb0037* self, CStrId id, bool flag);
void fn_71006a7448(UnkShakernautLaserState_ovb0037* self, s32 state);

void fn_71006a89b4(UnkShakernautLaser_l6a7ee4* self);
bool fn_71006a8d94(UnkShakernautLaser_l6a7ee4* self);
void fn_71006a8e94(UnkShakernautLaser_l6a7ee4* self);
void fn_71006a84dc(UnkShakernautLaser_l6a7ee4* self, UnkVec3f target, f32 dt);
void fn_71006a8760(UnkShakernautLaser_l6a7ee4* self);
bool fn_71006b03f4(UnkShakernautAI_l6a7ee4* ai, UnkVec3f aim, UnkVec3f* out, f32 blend);
bool fn_71006af8ec(UnkShakernautAI_l6a7ee4* ai, UnkVec3f* aim, UnkVec3f* out, UnkLavaDropsShape_A2_b71* shape,
                   bool* flag);

static inline UnkVec3f Pos(const UnkActor* actor) {
    return reinterpret_cast<const UnkXform*>(actor->field_0x38)->GetTranslation();
}

static inline UnkVec3f operator-(const UnkVec3f& a, const UnkVec3f& b) {
    return UnkVec3f(a.x - b.x, a.y - b.y, a.z - b.z);
}

static inline UnkVec3f operator*(const UnkVec3f& v, f32 s) {
    return UnkVec3f(v.x * s, v.y * s, v.z * s);
}

static inline f32 Dot(const UnkVec3f& a, const UnkVec3f& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline UnkVec3f Normalized(const UnkVec3f& v) {
    return v * (1.0f / (sqrtf(Dot(v, v)) + 0.0001f));
}

static inline f32 Lerp(f32 from, f32 to, f32 t) {
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    return to * t + from * (1.0f - t);
}

// Whether the target is held (its hold state is 3).
static inline bool TargetHeld(UnkActor* owner) {
    if (!fn_7100d7c990(owner)->field_0x208)
        return false;
    UnkTargetHold_l6a7ee4* hold =
        static_cast<UnkTargetComponent_l6a7ee4*>(fn_7100d7c990(owner)->field_0x208)->hold;
    return hold && hold->state == 3;
}

// Whether the target is out of the owner's reach and on the side the laser sweeps toward.
static inline bool TargetAhead(UnkShakernautLaser_l6a7ee4* self) {
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target)
        return true;
    UnkActor* owner = self->owner;
    UnkVec3f from = Pos(owner);
    UnkVec3f to = Pos(target);
    if (owner->field_0x218) {
        f32 reach = fn_7100977b20(owner->field_0x218);
        UnkVec3f d = from - to;
        if (reach * reach > Dot(d, d))
            return false;
    }
    UnkVec3f dir = Normalized(to - from);
    f32 sign = self->side == 2 ? 1.0f : -1.0f;
    UnkVec3f axis(1.0f, 0.0f, 0.0f);
    return Dot(dir, axis * sign) >= 0.0f;
}

void fn_71006a7ee4(UnkShakernautLaser_l6a7ee4* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    fn_710117364c(self->owner->field_0x228, &g_unk_710164a660,
                  reinterpret_cast<UnkGrappleAnswer_b50*>(&self->aim));
    UnkReflObject* object = static_cast<UnkReflObject*>(self->owner->field_0x240);
    UnkShakernautAI_l6a7ee4* ai = object && fn_710009d50c(object->vf_0x0(), fn_71006ad0a4())
                                      ? reinterpret_cast<UnkShakernautAI_l6a7ee4*>(object)
                                      : nullptr;
    UnkShakernautLaserState_ovb0037* state = reinterpret_cast<UnkShakernautLaserState_ovb0037*>(self);
    switch (self->state) {
    case 3: {
        self->aim_timer += dt;
        if (self->aim_timer >= self->timing->aim_time) {
            self->aim_timer = 0.0f;
            self->state = 4;
            fn_71006a7afc(state, g_unk_7172705c88, false);
            return;
        }
        if (!(self->aim_timer <= self->timing->aim_window))
            return;
        UnkVec3f out(0.0f, 0.0f, 0.0f);
        bool reached = fn_71006b03f4(ai, self->aim, &out, self->blend);
        bool clear;
        if (TargetHeld(self->owner) ||
            !static_cast<UnkShakernautAI_l6a7ee4*>(self->owner->field_0x240)->field_0x1220)
            clear = false;
        else
            clear = TargetAhead(self);
        bool flag = true;
        UnkLavaDropsShape_A2_b71 shape(g_unk_710164a674, 10.0f);
        clear &= fn_71006af8ec(ai, &self->aim, &out, &shape, &flag);
        if (UnkActor* target = fn_7100d7c990(self->owner)) {
            UnkVec3f dir = Normalized(Pos(target) - Pos(self->owner));
            UnkVec3f facing(ai->facing_x, ai->facing_y, 0.0f);
            clear &= Dot(facing, dir) >= 0.0f;
        }
        if (reached && clear)
            self->target = out;
        fn_71006a84dc(self, self->target, dt);
        fn_71006a8760(self);
        return;
    }
    case 5:
        self->hold_timer += dt;
        if (self->hold_timer >= self->timing->hold_time) {
            self->hold_timer = 0.0f;
            fn_71006a7448(state, 6);
            return;
        }
        fn_71006a89b4(self);
        if (fn_71006a8d94(self)) {
            if (self->effect)
                self->effect->vf_0x60(false);
            fn_71006a8e94(self);
        }
        return;
    case 6:
        self->fade_timer += dt;
        self->blend = Lerp(self->blend, 0.0f, self->fade_timer / self->timing->fade_time);
        for (s32 i = 0; i < 4; ++i)
            unk_prop_bag::fn_710033187c(
                reinterpret_cast<unk_prop_bag::UnkPropBag*>(
                    static_cast<UnkAnimationComponent*>(self->owner->field_0x220)->field_0x80),
                g_unk_7172705cb0[i], self->blend);
        if (self->fade_timer >= self->timing->fade_time) {
            self->fade_timer = 0.0f;
            fn_71006a7448(state, 7);
        }
        return;
    }
}

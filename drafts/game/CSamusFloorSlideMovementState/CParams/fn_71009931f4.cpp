// CSamusFloorSlideMovementState's unit (tools/tu_map.py unit 1459): the floor slide state's per-frame
// update: turns the owner to face along the slide when grounded and looking against it, tracks the
// distance slid since the last frame, runs the slide's helpers and counts down its timers.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "game/CMovementComponent.h"
#include "types.h"

using base::math::CVector3D;

struct UnkSlideState_ovb0004;
void fn_7100992724(UnkSlideState_ovb0004* self);

struct UnkSlideOwner_l9931f4 {
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
    virtual void SetRotation(const CVector3D* angles, bool apply);  // 0xd8
    u8 pad_0x8[0x30];
    UnkMatrix44 xform;  // 0x38
};

struct UnkSlideMovement_l9931f4 {
    u8 pad_0x0[0x154];
    CVector3D direction;  // 0x154
};

struct UnkFloorSlide_l9931f4 {
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
    UnkSlideOwner_l9931f4* owner;  // 0x30
    UnkSlideMovement_l9931f4* movement;  // 0x38
    u8 pad_0x40[0x158];
    s32 state;  // 0x198
    u8 pad_0x19c[0x18];
    f32 distance;  // 0x1b4
    CVector3D last_position;  // 0x1b8
    u8 pad_0x1c4[0x2c];
    f32 blend;  // 0x1f0
    u8 pad_0x1f4[0xb4];
    f32 timer;  // 0x2a8
    bool notify;  // 0x2ac
};
static_assert(__builtin_offsetof(UnkFloorSlide_l9931f4, last_position) == 0x1b8, "layout");
static_assert(__builtin_offsetof(UnkFloorSlide_l9931f4, blend) == 0x1f0, "layout");
static_assert(__builtin_offsetof(UnkFloorSlide_l9931f4, timer) == 0x2a8, "layout");

void fn_71009933dc(UnkFloorSlide_l9931f4* self);
void fn_710099295c(UnkFloorSlide_l9931f4* self, f32 dt);
void fn_7100992c0c(UnkFloorSlide_l9931f4* self);
void fn_7100994150(UnkFloorSlide_l9931f4* self);
void fn_7100994394(UnkFloorSlide_l9931f4* self);
void fn_710099d738(void* object, void* arg);

static inline f32 Sign_l9931f4(f32 v) { return (f32)((v > 0.0f) - (v < 0.0f)); }

// How far the transform's z axis points along `v`.
inline f32 FacingDot(const UnkMatrix44& m, const CVector3D& v) {
    return m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z;
}

// The owner's position: the translation column of its transform.
static inline CVector3D Position(const UnkSlideOwner_l9931f4* owner) {
    return {owner->xform.m[3], owner->xform.m[7], owner->xform.m[11]};
}

void fn_71009931f4(void* object, void* arg) {
    UnkFloorSlide_l9931f4* self = static_cast<UnkFloorSlide_l9931f4*>(object);
    const f32* dt = static_cast<const f32*>(arg);

    if (fn_71010d1598(reinterpret_cast<UnkGroundPlayer_b52*>(self->movement))) {
        UnkSlideMovement_l9931f4* movement = self->movement;
        if (FacingDot(fn_710002614c(&self->owner->xform), movement->direction) < 0.0f) {
            // A quarter turn about y, toward the slide's direction.
            CVector3D angles = {0.0f, Sign_l9931f4(self->movement->direction.x) * 1.5707964f, 0.0f};
            self->owner->SetRotation(&angles, true);
        }
    }

    fn_71009933dc(self);
    fn_710099295c(self, *dt);
    if (self->vf_38(false, false, nullptr)) {
        self->distance = -1.0f;
    } else {
        if (self->distance < 0.0f) {
            self->distance = 0.0f;
            self->last_position = Position(self->owner);
        } else {
            self->distance += __builtin_fabsf(self->last_position.x - self->owner->xform.m[3]);
            self->last_position = Position(self->owner);
        }
    }
    fn_7100992c0c(self);
    fn_7100994150(self);
    fn_7100994394(self);
    fn_7100992724(reinterpret_cast<UnkSlideState_ovb0004*>(self));

    if (self->timer > 0.0f) {
        f32 left = self->timer - *dt;
        self->timer = left > 0.0f ? left : 0.0f;
        if (self->notify)
            self->vf_19();
    }

    fn_710099d738(self, arg);

    if (self->state == 6) {
        self->blend += *dt * 2.0f;
        if (self->blend > 1.0f)
            self->blend = 1.0f;
    }
}

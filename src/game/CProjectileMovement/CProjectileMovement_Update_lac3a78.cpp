// CProjectileMovement unit (tools/tu_map.py unit 1623): the movement's update: after the base step, either
// finishes (slot 0x358) or, unless the movement stopped or the owner's animation ignores movement, steps the
// movement, derives the velocity from the launch curve, moves the owner to the new position and turns it
// to the new angles. Names and layouts are placeholders.
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CProjectileMovement.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU341.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkSteeringMovement_ovb0023;
void fn_71010d1b60(UnkSteeringMovement_ovb0023* self, f32 dt);
struct UnkVec3;
void fn_71009fa8fc(UnkVec3* out, f32 dirY, f32 dirX, f32 speed, f32 gravity, f32 t);
void fn_7100ac3d74(UnkNailongThornMovement* self, f32 dt);

struct UnkUpdateOwner_lac3a78 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x18c];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x188];
    bool moved;  // 0x3b0
    u8 pad_0x3b1[0xf];
    bool turned;  // 0x3c0
};
static_assert(__builtin_offsetof(UnkUpdateOwner_lac3a78, moved) == 0x3b0, "layout");

struct UnkUpdateDefinition_lac3a78 {
    u8 pad_0x0[0xec];
    bool keep_owner_position;  // 0xec
    u8 pad_0xed[0x43];
    bool follow_curve;  // 0x130
};

struct UnkUpdateMovement_lac3a78;
struct UnkUpdateMovementVtable_lac3a78 {
    u8 pad_0x0[0x308];
    void (*step)(UnkUpdateMovement_lac3a78* self, f32 dt);                           // 0x308
    void (*next_position)(UnkUpdateMovement_lac3a78* self, f32 dt, UnkVec3f* pos);  // 0x310
    void (*next_angle)(UnkUpdateMovement_lac3a78* self, UnkVec3f* angle);            // 0x318
    u8 pad_0x320[0x20];
    bool (*apply_position)(UnkUpdateMovement_lac3a78* self, UnkVec3f* pos);        // 0x340
    u8 pad_0x348[0x10];
    bool (*finished)(UnkUpdateMovement_lac3a78* self, f32 dt);                       // 0x358
};

struct UnkUpdateMovement_lac3a78 {
    UnkUpdateMovementVtable_lac3a78* vtable;
    u8 pad_0x8[0x28];
    UnkUpdateOwner_lac3a78* owner;  // 0x30
    u8 pad_0x38[0x8];
    f32 direction;  // 0x40
    u8 pad_0x44[0x40];
    UnkVec3f position;  // 0x84
    u8 pad_0x90[0x78];
    f32 gravity;  // 0x108
    u8 pad_0x10c[0xf4];
    f32 time;  // 0x200
    u8 pad_0x204[0x2b4];
    UnkVec3f velocity;  // 0x4b8
    u8 pad_0x4c4[0xac];
    bool steer;   // 0x570
    bool turn;    // 0x571
    u8 pad_0x572[6];
    bool stopped;  // 0x578
    u8 pad_0x579[0x3f];
    f32 launch_dir_y;  // 0x5b8
    f32 launch_dir_x;  // 0x5bc
    f32 launch_speed;  // 0x5c0
    u8 pad_0x5c4[4];
    UnkUpdateDefinition_lac3a78* definition;  // 0x5c8
};
static_assert(__builtin_offsetof(UnkUpdateMovement_lac3a78, velocity) == 0x4b8, "layout");
static_assert(__builtin_offsetof(UnkUpdateMovement_lac3a78, definition) == 0x5c8, "layout");

// The zero vector the curve offsets start from.
static inline const UnkVec3f& Zero() { return reinterpret_cast<const UnkVec3f&>(g_unk_710164779c); }

// Whether the owner's animation says to ignore the movement.
static inline bool IgnoresMovement(const UnkUpdateOwner_lac3a78* owner) {
    UnkAnimationComponent* animation = owner->animation;
    return animation && fn_710117b348(animation, fn_7100d10148());
}

void fn_7100ac3a78(UnkNailongThornMovement* movement, f32 dt) {
    UnkUpdateMovement_lac3a78* self = reinterpret_cast<UnkUpdateMovement_lac3a78*>(movement);
    fn_71010d1b60(reinterpret_cast<UnkSteeringMovement_ovb0023*>(movement), dt);
    if (self->vtable->finished(self, dt)) {
        fn_7100232c68(self->owner);
        return;
    }
    if (self->stopped || IgnoresMovement(self->owner))
        return;
    self->vtable->step(self, dt);

    UnkUpdateDefinition_lac3a78* def = self->definition;
    if (def && def->follow_curve) {
        UnkVec3f before = Zero();
        UnkVec3f after = Zero();
        fn_71009fa8fc(reinterpret_cast<UnkVec3*>(&before), self->launch_dir_y, self->launch_dir_x,
                      self->launch_speed, self->gravity, self->time - dt);
        fn_71009fa8fc(reinterpret_cast<UnkVec3*>(&after), self->launch_dir_y, self->launch_dir_x,
                      self->launch_speed, self->gravity, self->time);
        UnkVec3f delta(after.x - before.x, after.y - before.y, after.z - before.z);
        delta.x *= (f32)((self->direction > 0.0f) - (self->direction < 0.0f));
        f32 inv = 1.0f / dt;
        self->velocity = UnkVec3f(inv * delta.x, inv * delta.y, inv * delta.z);
    }
    if (self->steer)
        fn_7100ac3d74(movement, dt);

    UnkVec3f pos = self->position;
    self->vtable->next_position(self, dt, &pos);
    bool applied = self->vtable->apply_position(self, &pos);
    if (self->stopped || IgnoresMovement(self->owner))
        return;
    if (!applied || !self->definition->keep_owner_position) {
        UnkUpdateOwner_lac3a78* owner = self->owner;
        owner->xform.SetTranslation(pos);
        owner->moved = true;
    }
    if (!self->turn)
        return;
    UnkVec3f angle = self->owner->xform.GetAngle();
    self->vtable->next_angle(self, &angle);
    UnkUpdateOwner_lac3a78* owner = self->owner;
    owner->xform.SetAngle(angle);
    owner->turned = true;
}

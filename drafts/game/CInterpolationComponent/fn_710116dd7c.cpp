// CInterpolationComponent unit: the per-frame update. Counts down the delays, then moves the
// interpolated position (+0x40) and angles (+0x4c) towards the owner's, by the owner's interpolation
// tracks when it has them and by the component's own modes otherwise. Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

void fn_710007a050(const UnkVec3f* target, UnkVec3f* angle);

static const f32 kTwoPi = 6.2831855f;

struct UnkInterpolationData {
    u8 pad_0x0[0x28];
    f32 field_0x28;  // blend the position by the component's blend value when set
    u8 pad_0x2c[0x58];
    UnkVec3f start_position;  // 0x84
    UnkVec3f base_angle;      // 0x90
};

struct UnkInterpolationOwner {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x15c];
    UnkInterpolationData* data;  // 0x1f0
    u8 pad_0x1f8[0x28];
    UnkAnimationComponent* animation;  // 0x220
};
static_assert(__builtin_offsetof(UnkInterpolationOwner, data) == 0x1f0, "layout");
static_assert(__builtin_offsetof(UnkInterpolationOwner, animation) == 0x220, "layout");

// The position track: how it moves and how fast.
struct UnkInterpolatePositionTrack {
    u8 pad_0x0[0x18];
    u32 mode;   // 0x18
    f32 speed;  // 0x1c
};

// The rotation track: its time span (for the curve), how it moves, which way it turns and how fast.
struct UnkInterpolateRotationTrack {
    u8 pad_0x0[0x10];
    f32 start;      // 0x10
    f32 end;        // 0x14
    u32 mode;       // 0x18
    s32 direction;  // 0x1c
    f32 speed;      // 0x20
};

struct UnkInterpolation_l16dd7c {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkInterpolationOwner* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkVec3f position;  // 0x40
    UnkVec3f angle;     // 0x4c
    f32 field_0x58;     // delays that restore the default position mode
    f32 field_0x5c;
    u32 position_mode;          // 0x60
    u32 default_position_mode;  // 0x64
    f32 position_speed;         // 0x68
    u32 angle_mode;             // 0x6c
    u8 pad_0x70[0x4];
    f32 angle_speed;  // 0x74
    f32 curve_start;  // 0x78
    UnkVec3f curve_angle;  // 0x7c
    f32 height_delay;  // 0x88
    f32 height;        // 0x8c
    bool started;      // 0x90

    // vtable slot 0x168: how far the position has blended in.
    f32 vf_0x168() { return reinterpret_cast<f32 (*)(UnkInterpolation_l16dd7c*)>(vtable[0x168 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkInterpolation_l16dd7c, started) == 0x90, "layout");

static inline UnkVec3f Approach(const UnkVec3f& from, const UnkVec3f& to, f32 t) {
    return UnkVec3f(t * to.x + (1.0f - t) * from.x, t * to.y + (1.0f - t) * from.y, t * to.z + (1.0f - t) * from.z);
}

static inline UnkVec3f Lerp(const UnkVec3f& from, const UnkVec3f& to, f32 t) {
    return UnkVec3f((1.0f - t) * from.x + t * to.x, (1.0f - t) * from.y + t * to.y, (1.0f - t) * from.z + t * to.z);
}

// Each angle wrapped into (-pi, pi].
static inline UnkVec3f WrapAngles(const UnkVec3f& v) {
    return UnkVec3f(fn_710007a1d0(v.x), fn_710007a1d0(v.y), fn_710007a1d0(v.z));
}

static inline f32 Dot(const UnkVec3f& a, const UnkVec3f& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Counts a delay down; true when it has just run out.
static inline bool CountDown(f32& delay, f32 dt) {
    if (delay > 0.0f) {
        delay -= dt;
        if (delay < 0.0f) {
            delay = 0.0f;
            return true;
        }
    }
    return false;
}

// Makes the yaw turn the way the track asks: 1 the negative way, 0 the positive way.
static inline void TurnWay(const UnkInterpolateRotationTrack* track, const UnkVec3f& from, UnkVec3f* angle) {
    if (track->direction == 0) {
        angle->y = fn_710007a1d0(angle->y);
        if (from.y > angle->y)
            angle->y += kTwoPi;
    } else if (track->direction == 1) {
        angle->y = fn_710007a1d0(angle->y);
        if (from.y < angle->y)
            angle->y -= kTwoPi;
    }
}

// Moves the position by `mode`: 0 towards the owner at `speed` per second, 1 blended in from the
// start (or straight to the owner's), 2 straight to the owner.
static inline void MovePosition(UnkInterpolation_l16dd7c* self, u32 mode, f32 speed, f32 dt, f32 blend,
                                const UnkVec3f& target, const UnkVec3f& start) {
    switch (mode) {
    case 1:
        if (self->owner->data->field_0x28 == 0.0f)
            self->position = self->owner->xform.GetTranslation();
        else
            self->position = blend < 1.0f ? Approach(start, target, blend) : target;
        break;
    case 2:
        self->position = target;
        break;
    case 0: {
        f32 t = speed * dt;
        self->position = t < 1.0f ? Approach(self->position, target, t) : target;
        break;
    }
    }
}

void fn_710116dd7c(UnkInterpolation_l16dd7c* self, f32 dt) {
    bool started = self->started;
    self->started = true;
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));

    if (CountDown(self->field_0x58, dt))
        self->position_mode = self->default_position_mode;
    if (CountDown(self->field_0x5c, dt))
        self->position_mode = self->default_position_mode;
    CountDown(self->height_delay, dt);

    UnkInterpolationOwner* owner = self->owner;
    UnkVec3f target = owner->xform.GetTranslation();
    UnkVec3f start = owner->data->start_position;
    if (self->height_delay > 0.0f) {
        target.z = self->height;
        start.z = self->height;
    }
    if (!started)
        start = target;

    f32 blend = self->vf_0x168();
    UnkTrackQuery position_query = {fn_7100d15248(), nullptr, -1.0f};
    UnkInterpolatePositionTrack* position_track = nullptr;
    if (UnkAnimationComponent* animation = self->owner->animation) {
        fn_710117b08c(animation, &position_query);
        position_track = reinterpret_cast<UnkInterpolatePositionTrack*>(position_query.result);
    }
    if (position_track)
        MovePosition(self, position_track->mode, position_track->speed, dt, blend, target, start);
    else
        MovePosition(self, self->position_mode, self->position_speed, dt, blend, target, start);

    UnkVec3f angle = owner->xform.GetAngle();
    UnkVec3f base = self->owner->data->base_angle;
    UnkVec3f turn = WrapAngles(UnkVec3f(angle.x - base.x, angle.y - base.y, angle.z - base.z));
    angle = UnkVec3f(turn.x + base.x, turn.y + base.y, turn.z + base.z);
    angle.x = fn_710007a1d0(angle.x);
    angle.y = fn_710007a1d0(angle.y);
    angle.z = fn_710007a1d0(angle.z);

    UnkTrackQuery query = {fn_7100d158e8(), nullptr, -1.0f};
    UnkInterpolateRotationTrack* track = nullptr;
    if (UnkAnimationComponent* animation = self->owner->animation) {
        fn_710117b08c(animation, &query);
        track = reinterpret_cast<UnkInterpolateRotationTrack*>(query.result);
    }
    if (track) {
        switch (track->mode) {
        case 0: {
            f32 dot = Dot(self->angle, angle);
            fn_710007a050(&self->angle, &angle);
            track = reinterpret_cast<UnkInterpolateRotationTrack*>(query.result);
            f32 speed = track->speed;
            if (dot < 0.0f && self->owner->animation)
                TurnWay(track, self->angle, &angle);
            if (speed <= 0.0f)
                speed = self->angle_speed;
            f32 t = speed * dt;
            self->angle = t < 1.0f ? Lerp(self->angle, angle, t) : angle;
            break;
        }
        case 1:
            fn_710007a050(&base, &angle);
            self->angle = Lerp(base, angle, blend);
            break;
        case 2:
            fn_710007a050(&base, &angle);
            self->angle = angle;
            break;
        case 3: {
            f32 track_start = track->start;
            if (self->curve_start != track_start) {
                self->curve_angle = self->angle;
                self->curve_start = track_start;
            }
            f32 duration = track->end - self->curve_start;
            f32 dot = Dot(self->curve_angle, angle);
            fn_710007a050(&self->curve_angle, &angle);
            if (dot < 0.0f)
                TurnWay(reinterpret_cast<UnkInterpolateRotationTrack*>(query.result), self->curve_angle, &angle);
            f32 t = (query.value - self->curve_start) / duration;
            if (t < 0.5f)
                t = t * (t + t);
            else
                t = 4.0f * t + t * (-2.0f * t) - 1.0f;
            self->angle = t < 1.0f ? Lerp(self->curve_angle, angle, t) : angle;
            break;
        }
        }
    } else {
        self->curve_start = -1.0f;
        switch (self->angle_mode) {
        case 1:
            fn_710007a050(&base, &angle);
            self->angle = Lerp(base, angle, blend);
            break;
        case 2:
            fn_710007a050(&base, &angle);
            self->angle = angle;
            break;
        case 0: {
            fn_710007a050(&self->angle, &angle);
            f32 t = self->angle_speed * dt;
            self->angle = t < 1.0f ? Lerp(self->angle, angle, t) : angle;
            break;
        }
        }
    }

    self->angle.x = fn_710007a1d0(self->angle.x);
    self->angle.y = fn_710007a1d0(self->angle.y);
    self->angle.z = fn_710007a1d0(self->angle.z);
}

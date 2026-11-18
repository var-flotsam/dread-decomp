// Unit 1118 (no class hint): an AI state starts moving along a direction: its speed ratio against the
// owner's movement (+0x1f0), a velocity scaled from the direction, then a reset of its tracked indices
// and path, and the hand-over of its ready object (fn_71006fc7c4). Names and layouts are placeholders.
#include <math.h>

#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

struct UnkVec3_l6fe9d4 {
    f32 x;
    f32 y;
    f32 z;
};

// An object asked whether it is ready (its slot at +0x20).
struct UnkReadyObject_l6fe894 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();
};

struct UnkMovement_l6fe9d4 {
    u8 pad_0x0[0x118];
    f32 field_0x118;
};

struct UnkAI_l6fe9d4 {
    u8 pad_0x0[0x1794];
    bool field_0x1794;
    u8 pad_0x1795[3];
    f32 field_0x1798;
};

struct UnkOwner_l6fe9d4 {
    u8 pad_0x0[0x1f0];
    UnkMovement_l6fe9d4* movement;  // 0x1f0
    u8 pad_0x1f8[0x48];
    UnkAI_l6fe9d4* ai;  // 0x240
};

struct UnkSettings_l6fe9d4 {
    u8 pad_0x0[0x68];
    f32 speed;  // 0x68
    f32 factor;  // 0x6c
};

// A value the state may set for itself in place of its settings' one.
struct UnkOverride_l6fe9d4 {
    const f32& Get(const f32& fallback) const { return has ? value : fallback; }

    f32 value;
    bool has;
};

struct UnkAIState_l6fe9d4 {
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

    u8 pad_0x8[0x28];
    UnkOwner_l6fe9d4* owner;  // 0x30
    u8 pad_0x38[0x30];
    UnkSettings_l6fe9d4* settings;  // 0x68
    u8 pad_0x70[0x3c];
    s32 current;  // 0xac
    s32 previous;  // 0xb0
    s32 field_0xb4;
    u8 pad_0xb8[0x40];
    f32 speed_ratio;  // 0xf8
    u8 pad_0xfc[0x3c];
    UnkReadyObject_l6fe894* ready;  // 0x138
    bool has_ready;  // 0x140
    u8 pad_0x141[0x13];
    UnkOverride_l6fe9d4 speed;  // 0x154
    u8 pad_0x15c[0x41];
    bool field_0x19d;
    bool field_0x19e;
    u8 pad_0x19f[0x69];
    u8 path[8];  // 0x208
    u8 pad_0x210[0x12a80];
    UnkVec3_l6fe9d4 velocity;  // 0x12c90
};
static_assert(__builtin_offsetof(UnkAIState_l6fe9d4, speed_ratio) == 0xf8, "layout");
static_assert(__builtin_offsetof(UnkAIState_l6fe9d4, field_0x19e) == 0x19e, "layout");
static_assert(__builtin_offsetof(UnkAIState_l6fe9d4, path) == 0x208, "layout");
static_assert(__builtin_offsetof(UnkAIState_l6fe9d4, velocity) == 0x12c90, "layout");

struct UnkAIState_l6fe894;
f32 fn_71010d0c64(UnkMovement_l6fe9d4* movement);
void fn_71006fc7c4(UnkAIState_l6fe894* self, UnkReadyObject_l6fe894* ready);

static inline bool IsNearZero(const UnkVec3_l6fe9d4& v) {
    const f32 epsilon = 0.001f;
    return v.x + epsilon >= 0.0f && v.x - epsilon <= 0.0f && v.y + epsilon >= 0.0f && v.y - epsilon <= 0.0f &&
           v.z + epsilon >= 0.0f && v.z - epsilon <= 0.0f;
}

static inline f32 Length(const UnkVec3_l6fe9d4& v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

void fn_71006fe9d4(UnkAIState_l6fe9d4* self, const UnkVec3_l6fe9d4* direction) {
    if (self->field_0x19e && self->field_0x19d)
        return;
    UnkVec3_l6fe9d4& velocity = self->velocity;
    self->vf_0x50();
    UnkMovement_l6fe9d4* movement = self->owner->movement;
    f32 length;
    if (movement && !IsNearZero(*direction) && (length = Length(*direction)) > 0.0f) {
        f32 speed = self->speed.Get(self->settings->speed);
        self->speed_ratio = length / fn_71010d0c64(movement);
        f32 scale = 1.0f / (Length(*direction) + 0.0001f);
        f32 x = direction->x * scale;
        f32 y = direction->y * scale;
        f32 z = scale * direction->z;
        f32 factor = self->settings->factor;
        velocity.x = speed * x * factor;
        velocity.y = speed * y * factor;
        velocity.z = speed * z * factor;
        if (self->speed_ratio > 1.0f) {
            if (UnkAI_l6fe9d4* ai = self->owner->ai) {
                ai->field_0x1794 = true;
                ai->field_0x1798 = -1.0f;
            }
        }
    } else if (movement && movement->field_0x118 > 0.0f && fn_71010d0c64(movement) > 0.0f) {
        self->speed_ratio = movement->field_0x118 / fn_71010d0c64(movement);
    } else {
        self->speed_ratio = 0.0f;
    }
    self->current = 0x80000001;
    self->previous = 0x80000001;
    self->field_0xb4 = 0x80000001;
    fn_71002adc8c(self->path);
    UnkReadyObject_l6fe894* ready = nullptr;
    if (self->has_ready) {
        ready = self->ready;
        if (ready && !ready->vf_0x20())
            ready = nullptr;
    }
    fn_71006fc7c4(reinterpret_cast<UnkAIState_l6fe894*>(self), ready);
    velocity.x = 0.0f;
    velocity.y = 0.0f;
    velocity.z = 0.0f;
}

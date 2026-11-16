// Unit 2573: samples a followed object's position, derives its velocity over a time step and keeps the
// largest velocity seen along the directions the owner asks for.
// Names and layouts are placeholders.
#include "types.h"

struct UnkTU2573Vec3 {
    f32 x, y, z;

    UnkTU2573Vec3() = default;
    UnkTU2573Vec3(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkTU2573Target {
    u8 pad_0x0[0x38];
    f32 transform[3][4];  // 0x38, translation in the last column
    u8 pad_0x68[0x198];
    void* field_0x200;

    UnkTU2573Vec3 GetPosition() const {
        return UnkTU2573Vec3(transform[0][3], transform[1][3], transform[2][3]);
    }
};

struct UnkTU2573TargetHolder {
    u8 pad_0x0[0x10];
    UnkTU2573Target* target;
};

struct UnkTU2573VelocityTracker {
    u8 pad_0x0[0x21];
    bool resetEachStep;  // 0x21
    u8 pad_0x22[0x4e];
    UnkTU2573TargetHolder* holder;  // 0x70
    u8 pad_0x78[0x28];
    f32 peakX;  // 0xa0
    f32 peakY;  // 0xa4
    f32 field_0xa8;
    UnkTU2573Vec3 lastPos;  // 0xac
    f32 dirX;   // 0xb8
    f32 dirY;   // 0xbc
};
static_assert(__builtin_offsetof(UnkTU2573VelocityTracker, peakX) == 0xa0, "");
static_assert(__builtin_offsetof(UnkTU2573VelocityTracker, dirY) == 0xbc, "");
static_assert(__builtin_offsetof(UnkTU2573Target, field_0x200) == 0x200, "");

// The velocity component when it points the same way as the wanted direction, else zero.
static inline f32 AlongDirection(f32 velocity, f32 direction) {
    return (velocity > 0.0f && direction > 0.0f) || (velocity < 0.0f && direction < 0.0f) ? velocity : 0.0f;
}

void fn_7100ecd074(UnkTU2573VelocityTracker* self, f32 dt) {
    UnkTU2573TargetHolder* holder = self->holder;
    UnkTU2573Target* target;
    if (holder != nullptr && (target = holder->target) != nullptr) {
        if (dt > 0.0f && target->field_0x200 != nullptr) {
            UnkTU2573Vec3 prev = self->lastPos;
            self->lastPos = target->GetPosition();
            f32 velX = (self->lastPos.x - prev.x) / dt;
            f32 velY = (self->lastPos.y - prev.y) / dt;

        f32 dirX = self->dirX;
        f32 speedX = AlongDirection(velX, dirX);
        f32 dirY = self->dirY;
        f32 speedY = AlongDirection(velY, dirY);

        if (dirX >= 1.0f) {
            if (self->peakX < speedX)
                self->peakX = speedX;
        } else if (dirX <= -1.0f) {
            if (self->peakX > speedX)
                self->peakX = speedX;
        } else if (dirY >= 1.0f) {
            if (self->peakY < speedY)
                self->peakY = speedY;
        } else if (!(dirY > -1.0f)) {
            if (self->peakY > speedY)
                self->peakY = speedY;
        }
        }
    }
    if (self->resetEachStep) {
        self->peakX = 0.0f;
        self->peakY = 0.0f;
        self->field_0xa8 = 0.0f;
    }
}

// UnkTU211: writes each listed entry of an output table with a 3-vector blended between three keyed vectors by
// the progress of a timed object. Names and layouts are placeholders.
#include "types.h"

struct UnkTimed_ovb0167;
f32 fn_710016cdd0(UnkTimed_ovb0167* self, u32 index);

struct UnkVec3 {
    f32 x;
    f32 y;
    f32 z;

    UnkVec3() {}
    UnkVec3(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3(const UnkVec3& other) : x(other.x), y(other.y), z(other.z) {}
    UnkVec3& operator=(const UnkVec3& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

struct UnkBlendTarget {
    u8 pad_0x0[8];
    UnkTimed_ovb0167* timed_0x8;
    u8 pad_0x10[0x10];
    UnkVec3* table_0x20;
};

struct UnkBlendKeys {
    u8 pad_0x0[8];
    UnkBlendTarget* target_0x8;
    f32 end_0x10;
    f32 hold_0x14;
    UnkVec3 start_0x18;
    UnkVec3 middle_0x24;
    UnkVec3 last_0x30;
};

static inline UnkVec3 Scale(const UnkVec3& v, f32 k) {
    return UnkVec3(v.x * k, v.y * k, v.z * k);
}

static inline UnkVec3 Add(const UnkVec3& a, const UnkVec3& b) {
    return UnkVec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline UnkVec3 Mix(const UnkVec3& from, const UnkVec3& to, f32 t) {
    return Add(Scale(from, 1.0f - t), Scale(to, t));
}

void fn_7100194890(UnkBlendKeys* self, const u32* indices, u32 count) {
    UnkTimed_ovb0167* timed = self->target_0x8->timed_0x8;
    for (; count != 0; --count, ++indices) {
        u32 index = *indices;
        f32 time = fn_710016cdd0(timed, index);
        UnkVec3 value;
        if (time <= 0.0f) {
            value = self->start_0x18;
        } else if (time < self->end_0x10) {
            f32 t = time / self->end_0x10;
            if (t >= 1.0f)
                value = self->middle_0x24;
            else
                value = Mix(self->start_0x18, self->middle_0x24, t);
        } else if (time < self->hold_0x14) {
            value = self->middle_0x24;
        } else {
            value = self->last_0x30;
            if (time < 1.0f) {
                f32 t = (1.0f - time) / (1.0f - self->hold_0x14);
                if (t >= 1.0f)
                    value = self->middle_0x24;
                else
                    value = Mix(value, self->middle_0x24, t);
            }
        }
        self->target_0x8->table_0x20[index] = value;
    }
}

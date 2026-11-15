// UnkTU211: writes each listed entry of an output table with a value blended between three keyed values
// by the progress of a timed object. Names and layouts are placeholders.
#include "types.h"

struct UnkTimed_ovb0167;
f32 fn_710016cdd0(UnkTimed_ovb0167* self, u32 index);

struct UnkKey {
    f32 value;

    UnkKey() {}
    UnkKey(f32 v) : value(v) {}
    UnkKey(const UnkKey& other) : value(other.value) {}
    UnkKey& operator=(const UnkKey& other) {
        value = other.value;
        return *this;
    }
};

struct UnkBlendTarget {
    u8 pad_0x0[8];
    UnkTimed_ovb0167* timed_0x8;
    u8 pad_0x10[8];
    UnkKey* table_0x18;
};

struct UnkBlendKeys {
    u8 pad_0x0[8];
    UnkBlendTarget* target_0x8;
    f32 end_0x10;
    f32 hold_0x14;
    UnkKey start_0x18;
    UnkKey middle_0x1c;
    UnkKey last_0x20;
};

static inline UnkKey Mix(const UnkKey& from, const UnkKey& to, f32 t) {
    return UnkKey((1.0f - t) * from.value + t * to.value);
}

void fn_7100193ae8(UnkBlendKeys* self, const u32* indices, u32 count) {
    UnkTimed_ovb0167* timed = self->target_0x8->timed_0x8;
    for (; count != 0; --count, ++indices) {
        u32 index = *indices;
        f32 time = fn_710016cdd0(timed, index);
        UnkKey value;
        if (time <= 0.0f) {
            value = self->start_0x18;
        } else if (time < self->end_0x10) {
            f32 t = time / self->end_0x10;
            if (t >= 1.0f)
                value = self->middle_0x1c;
            else if (!(t < 0.0f))
                value = Mix(self->start_0x18, self->middle_0x1c, t);
            else
                value = self->start_0x18;
        } else if (time < self->hold_0x14) {
            value = self->middle_0x1c;
        } else if (time >= 1.0f) {
            value = self->last_0x20;
        } else {
            f32 t = (1.0f - time) / (1.0f - self->hold_0x14);
            if (t >= 1.0f)
                value = self->middle_0x1c;
            else if (!(t < 0.0f))
                value = Mix(self->last_0x20, self->middle_0x1c, t);
            else
                value = self->last_0x20;
        }
        self->target_0x8->table_0x18[index] = value;
    }
}

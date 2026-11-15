// UnkTU211: writes each listed entry of an output table with a pair blended between three keyed pairs by
// the progress of a timed object. Names and layouts are placeholders.
#include "types.h"

struct UnkTimed_ovb0167;
f32 fn_710016cdd0(UnkTimed_ovb0167* self, u32 index);

struct UnkPair {
    f32 first;
    f32 second;

    UnkPair() {}
    UnkPair(f32 a, f32 b) : first(a), second(b) {}
    UnkPair(const UnkPair& other) : first(other.first), second(other.second) {}
    UnkPair& operator=(const UnkPair& other) {
        first = other.first;
        second = other.second;
        return *this;
    }
};

struct UnkBlendTarget {
    u8 pad_0x0[8];
    UnkTimed_ovb0167* timed_0x8;
    u8 pad_0x10[8];
    UnkPair* table_0x18;
};

struct UnkBlendKeys {
    u8 pad_0x0[8];
    UnkBlendTarget* target_0x8;
    f32 end_0x10;
    f32 hold_0x14;
    UnkPair start_0x18;
    UnkPair middle_0x20;
    UnkPair last_0x28;
};

static inline UnkPair Mix(const UnkPair& from, const UnkPair& to, f32 t) {
    f32 rest = 1.0f - t;
    f32 first = rest * from.first;
    f32 second = rest * from.second;
    first += t * to.first;
    second += t * to.second;
    return UnkPair(first, second);
}

void fn_710019413c(UnkBlendKeys* self, const u32* indices, u32 count) {
    UnkTimed_ovb0167* timed = self->target_0x8->timed_0x8;
    for (; count != 0; --count, ++indices) {
        u32 index = *indices;
        f32 time = fn_710016cdd0(timed, index);
        UnkPair value;
        if (time <= 0.0f) {
            value = self->start_0x18;
        } else if (time < self->end_0x10) {
            f32 t = time / self->end_0x10;
            if (t >= 1.0f)
                value = self->middle_0x20;
            else if (!(t < 0.0f))
                value = Mix(self->start_0x18, self->middle_0x20, t);
            else
                value = self->start_0x18;
        } else if (time < self->hold_0x14) {
            value = self->middle_0x20;
        } else {
            value = self->last_0x28;
            if (time < 1.0f) {
                f32 t = (1.0f - time) / (1.0f - self->hold_0x14);
                if (t >= 1.0f)
                    value = self->middle_0x20;
                else if (!(t < 0.0f))
                    value = Mix(value, self->middle_0x20, t);
            }
        }
        self->target_0x8->table_0x18[index] = value;
    }
}

// GUI::IDisplayObjectTrack: the flag of a flag track at a time: binary search for the last key at or before
// the time, answered as the flag of that key (and, on a track that interpolates, the key after it).
// Names and layouts are placeholders.
#include "types.h"

struct UnkFlagKey1671 {
    s32 time;
    bool value;  // 0x4
};

struct UnkFlagTrack1671 {
    u8 pad_0x0[0x38];
    u32 field_0x38;
    u32 field_0x3c;
    UnkFlagKey1671* keys;  // 0x40
    u32 count;             // 0x48
};

extern u8 g_unk_7172760c81;
UnkFlagKey1671* fn_7100b18bfc(void* keys, u32 index);

bool fn_7100b188d0(UnkFlagTrack1671* self, s32 time, bool wrap) {
    u8 fallback = g_unk_7172760c81;
    s32 count = self->count;
    UnkFlagKey1671* keys = self->keys;
    if (count < 1 || keys[0].time > time)
        return fallback != 0;
    s32 lo = 0;
    s32 hi = count;
    while (hi - lo > 1) {
        s32 sum = lo + hi;
        u32 mid = sum / 2;
        if (keys[mid].time > time)
            hi = mid;
        else
            lo = mid;
    }
    if (lo < 0)
        return fallback != 0;
    UnkFlagKey1671* key = fn_7100b18bfc(&self->keys, lo);
    bool result = key->value;
    if (self->field_0x38 == 0 || key->time >= time)
        return result;
    u32 next = lo + 1;
    if (lo >= (s32)self->count && wrap)
        next = next - self->count + self->field_0x3c;
    if (next >= self->count)
        return result;
    fn_7100b18bfc(&self->keys, lo);
    fn_7100b18bfc(&self->keys, next);
    fn_7100b18bfc(&self->keys, lo);
    return self->keys[lo].value;
}

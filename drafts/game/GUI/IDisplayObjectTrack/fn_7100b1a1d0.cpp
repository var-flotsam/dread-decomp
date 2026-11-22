// GUI::IDisplayObjectTrack: the string of a string track at a time: binary search for the last key at or
// before the time, answered as a copy of that key's string (and, on a track that interpolates, the key after it).
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "types.h"

struct UnkStringKey1671 {
    s32 time;
    u8 pad_0x4[4];
    base::global::CRntString value;  // 0x8
};
static_assert(sizeof(UnkStringKey1671) == 0x38, "UnkStringKey1671 layout");

struct UnkStringTrack1671 {
    u8 pad_0x0[0x38];
    u32 field_0x38;
    u32 field_0x3c;
    UnkStringKey1671* keys;  // 0x40
    u32 count;               // 0x48
};

extern const char* g_unk_7101a9a4b8;
UnkStringKey1671* fn_7100b1a568(void* keys, u32 index);

base::global::CRntString fn_7100b1a1d0(UnkStringTrack1671* self, s32 time, bool wrap) {
    base::global::CRntString result(g_unk_7101a9a4b8);
    s32 count = self->count;
    if (count < 1 || self->keys[0].time > time)
        return result;
    UnkStringKey1671* keys = self->keys;
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
        return result;
    UnkStringKey1671* key = fn_7100b1a568(&self->keys, lo);
    CRntString_Assign(&result, &key->value);
    if (self->field_0x38 == 0 || key->time >= time)
        return result;
    u32 next = lo + 1;
    if (lo >= (s32)self->count && wrap)
        next = next - self->count + self->field_0x3c;
    if (next >= self->count)
        return result;
    fn_7100b1a568(&self->keys, lo);
    fn_7100b1a568(&self->keys, next);
    fn_7100b1a568(&self->keys, lo);
    base::global::CRntString current(self->keys[lo].value);
    CRntString_Assign(&result, &current);
    return result;
}

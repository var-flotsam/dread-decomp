// Unit 356: assigns one counted list of 12-byte entries from another.
// Names and layouts are placeholders.
#include "types.h"

struct UnkEntry356 {
    u16 field_0x0;
    u8 field_0x2;
    u8 pad_0x3;
    u32 field_0x4;
    u32 field_0x8;
};

struct UnkList356 {
    u32 count;
    UnkEntry356 entries[1];
};

void fn_7100286420(UnkList356* dst, UnkList356* src) {
    if (dst == src)
        return;
    u32 old = dst->count;
    u32 n = src->count;
    if (old > n) {
        if (n == 0)
            goto done;
        UnkEntry356* d = dst->entries;
        const UnkEntry356* s = src->entries;
        for (u64 i = 0; i < src->count; i++) {
            u16 lo = s->field_0x0;
            u8 mid = s->field_0x2;
            d->field_0x2 = mid;
            d->field_0x0 = lo;
            ((u32*)d)[1] = ((const u32*)s)[1];
            ((u32*)d)[2] = ((const u32*)s)[2];
            s++;
            d++;
        }
    } else {
        UnkEntry356* d = dst->entries;
        const UnkEntry356* s = src->entries;
        for (u64 i = 0; i < old; i++) {
            u16 lo = s->field_0x0;
            u8 mid = s->field_0x2;
            d->field_0x2 = mid;
            d->field_0x0 = lo;
            ((u32*)d)[1] = ((const u32*)s)[1];
            ((u32*)d)[2] = ((const u32*)s)[2];
            s++;
            d++;
        }
        n = src->count;
    }
    if (old < n) {
        u64 i = old;
        do {
            u32* d = (u32*)&dst->entries[(u32)i];
            const u32* s = (const u32*)&src->entries[(u32)i];
            i++;
            d[0] = s[0];
            d[1] = s[1];
            d[2] = s[2];
            dst->count = (u32)i;
            n = src->count;
        } while (i < n);
    }
done:
    dst->count = n;
}

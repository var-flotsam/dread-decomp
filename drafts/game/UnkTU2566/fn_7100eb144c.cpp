// Unit 2566 (minimap): equality of two keyed collections of position lists: every key of one is found in the other and
// the lists under it match (positions within a tolerance). Names and layouts are placeholders.
#include "types.h"

extern const f32 g_unk_7101647774;

struct UnkApproxPos_leb144c {
    f32 x, y;

    bool operator==(const UnkApproxPos_leb144c& o) const {
        return x - g_unk_7101647774 <= o.x && g_unk_7101647774 + x >= o.x && y - g_unk_7101647774 <= o.y &&
               g_unk_7101647774 + y >= o.y;
    }
};

struct UnkKeyedPos_leb144c {
    u64 key;
    UnkApproxPos_leb144c pos;
};

struct UnkPosList_leb144c {
    UnkKeyedPos_leb144c* data;
    u32 count;
    u8 pad_0xc[0x28 - 0xc];
};

struct UnkPosListMap_leb144c {
    UnkPosList_leb144c* lists;
    u64* keys;
    u32 count;
};

static inline bool ListEquals(const UnkPosList_leb144c& a, const UnkPosList_leb144c& b) {
    u32 n = a.count;
    if (n != b.count)
        return false;
    if (n) {
        const UnkKeyedPos_leb144c* pa = a.data;
        const UnkKeyedPos_leb144c* pb = b.data;
        u64 left = n;
        do {
            if (!(pa->key == pb->key && pa->pos == pb->pos))
                return false;
            --left;
            ++pa;
            ++pb;
        } while (left);
    }
    return true;
}

// Index of the first key equal to `key`, or `count` when there is none.
static inline u64 FindKey(const u64* keys, u64 count, u64 key) {
    u64 i = 0;
    for (; i != count; ++i) {
        if (keys[i] == key)
            break;
    }
    return i;
}

bool fn_7100eb144c(const UnkPosListMap_leb144c* a, const UnkPosListMap_leb144c* b) {
    u32 n = a->count;
    if (n != b->count)
        return false;
    for (u32 i = 0; i != n; ++i) {
        u32 j = FindKey(b->keys, n, a->keys[i]);
        if (j == n)
            return false;
        if (!ListEquals(a->lists[i], b->lists[j]))
            return false;
    }
    return true;
}

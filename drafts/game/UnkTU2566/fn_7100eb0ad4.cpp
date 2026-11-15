// Unit 2566 (minimap): equality of two reflected values holding an id and a list of keyed positions, each member
// inline or (mode 2) through a pointer; positions match within a tolerance. Names and layouts are placeholders.
#include "types.h"

extern const f32 g_unk_7101647774;

struct UnkApproxPos_leb0ad4 {
    f32 x, y;

    bool operator==(const UnkApproxPos_leb0ad4& o) const {
        return x - g_unk_7101647774 <= o.x && g_unk_7101647774 + x >= o.x && y - g_unk_7101647774 <= o.y &&
               g_unk_7101647774 + y >= o.y;
    }
};

struct UnkKeyedPos_leb0ad4 {
    u64 key;
    UnkApproxPos_leb0ad4 pos;

    bool operator==(const UnkKeyedPos_leb0ad4& o) const { return key == o.key && pos == o.pos; }
};

struct UnkPosList_leb0ad4 {
    UnkKeyedPos_leb0ad4* data;
    u32 count;
};

struct UnkHolder_leb0ad4 {
    union {
        u64 value;
        const u64* ptr;
    } id;
    union {
        UnkPosList_leb0ad4 value;
        const UnkPosList_leb0ad4* ptr;
    } list;
    u8 pad_0x18[0x30 - 0x18];
    u32 mode;

    const u64& Id() const { return mode == 2 ? *id.ptr : id.value; }
    const UnkPosList_leb0ad4& List() const { return mode == 2 ? *list.ptr : list.value; }
};

bool fn_7100eb0ad4(const UnkHolder_leb0ad4* a, const UnkHolder_leb0ad4* b) {
    if (a->Id() != b->Id())
        return false;
    const UnkPosList_leb0ad4& x = a->List();
    const UnkPosList_leb0ad4& y = b->List();
    if (x.count != y.count)
        return false;
    for (u64 i = 0; i < x.count; i++) {
        if (!(y.data[i] == x.data[i]))
            return false;
    }
    return true;
}

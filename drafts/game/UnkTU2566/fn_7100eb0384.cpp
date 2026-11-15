// Unit 2566 (minimap): equality of two reflected values holding an id and a list of keyed positions, each member
// inline or (mode 2) through a pointer; positions match within a tolerance. Names and layouts are placeholders.
#include "types.h"

extern const f32 g_unk_7101647774;

struct UnkApproxPos_leb0384 {
    f32 x, y;

    bool operator==(const UnkApproxPos_leb0384& o) const {
        return x - g_unk_7101647774 <= o.x && g_unk_7101647774 + x >= o.x && y - g_unk_7101647774 <= o.y &&
               g_unk_7101647774 + y >= o.y;
    }
};

struct UnkKeyedPos_leb0384 {
    u64 key;
    UnkApproxPos_leb0384 pos;

    bool operator==(const UnkKeyedPos_leb0384& o) const { return key == o.key && pos == o.pos; }
};

struct UnkPosList_leb0384 {
    UnkKeyedPos_leb0384* data;
    u32 count;
};

struct UnkHolder_leb0384 {
    union {
        u64 value;
        const u64* ptr;
    } id;
    union {
        UnkPosList_leb0384 value;
        const UnkPosList_leb0384* ptr;
    } list;
    u8 pad_0x18[0x30 - 0x18];
    u32 mode;

    const u64& Id() const { return mode == 2 ? *id.ptr : id.value; }
    const UnkPosList_leb0384& List() const { return mode == 2 ? *list.ptr : list.value; }
};

static inline bool Equal(const void* a_, const void* b_) {
    const UnkHolder_leb0384* a = static_cast<const UnkHolder_leb0384*>(a_);
    const UnkHolder_leb0384* b = static_cast<const UnkHolder_leb0384*>(b_);
    if (!(a->Id() == b->Id()))
        return false;
    const UnkPosList_leb0384& x = a->List();
    const UnkPosList_leb0384& y = b->List();
    if (x.count != y.count)
        return false;
    for (u64 i = 0; i < x.count; i++)
        if (!(y.data[i] == x.data[i]))
            return false;
    return true;
}

bool fn_7100eb0384(const void* a, const void* b) { return Equal(a, b); }

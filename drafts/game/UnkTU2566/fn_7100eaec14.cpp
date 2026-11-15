// Unit 2566 (minimap): equality of two (u32, keyed position) pairs, each held in place or by reference (state 2); the
// position matches within a tolerance. Names and layouts are placeholders.
#include "types.h"

extern const f32 g_unk_7101647774;

struct UnkApproxPos_leaec14 {
    f32 x, y;

    bool operator==(const UnkApproxPos_leaec14& o) const {
        return x - g_unk_7101647774 <= o.x && x + g_unk_7101647774 >= o.x && y - g_unk_7101647774 <= o.y &&
               y + g_unk_7101647774 >= o.y;
    }
};

struct UnkKeyedPos_leaec14 {
    u64 key;
    UnkApproxPos_leaec14 pos;

    bool operator==(const UnkKeyedPos_leaec14& o) const { return key == o.key && pos == o.pos; }
};

struct UnkIdKeyedPosPair {
    union {
        struct {
            u32 x;
            UnkKeyedPos_leaec14 y;
        } values;
        struct {
            u32* x;
            UnkKeyedPos_leaec14* y;
        } refs;
    };
    s32 state;

    u32& GetX() { return state == 2 ? *refs.x : values.x; }
    UnkKeyedPos_leaec14& GetY() { return state == 2 ? *refs.y : values.y; }
};

bool fn_7100eaec14(UnkIdKeyedPosPair* a, UnkIdKeyedPosPair* b) {
    if (a->GetX() == b->GetX()) {
        UnkKeyedPos_leaec14& ya = a->GetY();
        UnkKeyedPos_leaec14& yb = b->GetY();
        if (ya.key == yb.key)
            return ya.pos == yb.pos;
    }
    return false;
}

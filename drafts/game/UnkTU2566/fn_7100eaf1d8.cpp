// Unit 2566 (minimap): equality of two (u32, keyed position) pairs, each held in place or by reference (state 2); the
// position matches within a tolerance. Names and layouts are placeholders.
#include "types.h"

extern const f32 g_unk_7101647774;

struct UnkApproxPos_leaf1d8 {
    f32 x, y;

    bool operator==(const UnkApproxPos_leaf1d8& o) const {
        return x - g_unk_7101647774 <= o.x && x + g_unk_7101647774 >= o.x && y - g_unk_7101647774 <= o.y &&
               y + g_unk_7101647774 >= o.y;
    }
};

struct UnkKeyedPos_leaf1d8 {
    u64 key;
    UnkApproxPos_leaf1d8 pos;

    bool operator==(const UnkKeyedPos_leaf1d8& o) const {
        if (key != o.key)
            return false;
        return pos == o.pos;
    }
};

struct UnkIdKeyedPosPair_leaf1d8 {
    union {
        struct {
            u32 x;
            UnkKeyedPos_leaf1d8 y;
        } values;
        struct {
            u32* x;
            UnkKeyedPos_leaf1d8* y;
        } refs;
    };
    s32 state;

    u32& GetX() { return state == 2 ? *refs.x : values.x; }
    UnkKeyedPos_leaf1d8& GetY() { return state == 2 ? *refs.y : values.y; }
};

bool fn_7100eaf1d8(UnkIdKeyedPosPair_leaf1d8* a, UnkIdKeyedPosPair_leaf1d8* b) {
    return a->GetX() == b->GetX() && a->GetY() == b->GetY();
}

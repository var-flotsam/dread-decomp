// CSwarmControllerComponent's unit: sets the value kept for a key in the component's list of
// (key, value) pairs, adding the pair when the key is not there. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

#include <algorithm>

#include "../../../src/game_s7/UnkIter.h"

struct UnkSwarmPair_l74bdd4 {
    u32 key;
    f32 value;

    UnkSwarmPair_l74bdd4(u32 key, f32 value) : key(key), value(value) {}
    UnkSwarmPair_l74bdd4(UnkSwarmPair_l74bdd4&& other) : key(other.key), value(other.value) {}
    ~UnkSwarmPair_l74bdd4() {}
};

struct UnkSwarmComponent_l74bdd4 {
    u8 pad_0x0[0x168];
    base::global::CRntVector<UnkSwarmPair_l74bdd4> pairs;  // 0x168
};
static_assert(__builtin_offsetof(UnkSwarmComponent_l74bdd4, pairs) == 0x168, "layout");

void fn_710074bdd4(UnkSwarmComponent_l74bdd4* self, u32 key, f32 value) {
    UnkSwarmPair_l74bdd4* begin = self->pairs.data;
    UnkSwarmPair_l74bdd4* end = begin + self->pairs.count;
    auto it = std::find_if(UnkIter<UnkSwarmPair_l74bdd4>{begin}, UnkIter<UnkSwarmPair_l74bdd4>{end},
                           [key](const UnkSwarmPair_l74bdd4& pair) { return pair.key == key; });
    if (it.p != end) {
        it.p->value = value;
        return;
    }
    self->pairs.EmplaceBack(UnkSwarmPair_l74bdd4(key, value));
}

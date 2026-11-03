// tools/tu_map.py unit 25 (no class hint): two equality tests of 16-byte keys (two 64-bit words,
// both compared). A first-word mismatch is the expected case (it lays the "false" return out first,
// as in the original). Names are placeholders.
#include "types.h"

struct UnkKeyPair25 {
    u64 first;
    u64 second;
};

static inline bool PairEquals25(const UnkKeyPair25* a, const UnkKeyPair25* b) {
    if (__builtin_expect(a->first != b->first, 1)) {
        return false;
    }
    return a->second == b->second;
}

bool fn_710001ca94(const UnkKeyPair25* a, const UnkKeyPair25* b) {
    return PairEquals25(a, b);
}

bool fn_710001ce70(const UnkKeyPair25* a, const UnkKeyPair25* b) {
    return PairEquals25(a, b);
}

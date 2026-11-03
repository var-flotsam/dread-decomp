// CBossRushSurvivalConfiguration unit: assignments of a two-word alternative (index 2) into a
// variant whose index sits at +0x10; alternatives 1 and 2 are reset to the empty state first.
// Names and layouts are placeholders.
#include "types.h"

struct UnkBossRushVariant {
    u64 a, b;
    s32 index;
};

struct UnkBossRushPair {
    u64 a, b;
};

static inline void AssignPair(UnkBossRushVariant* self, u64 a, u64 b) {
    if ((u32)(self->index - 1) < 2)
        self->index = 0;
    self->index = 2;
    self->a = a;
    self->b = b;
}

UnkBossRushVariant* fn_7100dceb44(UnkBossRushVariant* self, const UnkBossRushVariant* other) {
    if (self != other) {
        u64 a = other->a, b = other->b;
        AssignPair(self, a, b);
    }
    return self;
}

UnkBossRushVariant* fn_7100dcede8(UnkBossRushVariant* self, const UnkBossRushVariant* other) {
    if (self != other) {
        u64 a = other->a, b = other->b;
        AssignPair(self, a, b);
    }
    return self;
}

// Stores the pair with its words swapped into the variant and returns the pair.
const UnkBossRushPair* fn_7100dd0cf4(const UnkBossRushPair* pair, UnkBossRushVariant* dst) {
    u64 a = pair->a, b = pair->b;
    AssignPair(dst, b, a);
    return pair;
}

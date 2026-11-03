// CBossRushSurvivalConfiguration unit (see CBossRushSurvivalConfiguration_VariantAssign.cpp): stores a pair of
// element pointers (two arrays indexed by the word at +0x10) as alternative 2 of a variant whose index sits at
// +0x10; alternatives 1 and 2 are reset to the empty state first. Names and layouts are placeholders.
#include "types.h"

struct UnkBossRushVariant2 {
    u32* a;
    u32* b;
    s32 index;
};

struct UnkBossRushArrays {
    u32* first;
    u32* second;
    u32 index;
};

void fn_7100dd0d1c(const UnkBossRushArrays* arrays, UnkBossRushVariant2* dst) {
    u32* second = arrays->second + arrays->index;
    u32* first = arrays->first + arrays->index;
    if ((u32)(dst->index - 1) < 2)
        dst->index = 0;
    dst->index = 2;
    dst->a = second;
    dst->b = first;
}

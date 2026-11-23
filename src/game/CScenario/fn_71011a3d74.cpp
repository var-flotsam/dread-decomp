// The CScenario unit: looks `*key` up in the three arrays of 0x60-byte groups of the object behind the
// game manager's +0x608 (game/UnkGameManager.h), each group searched by fn_71002636b0; null when no
// group has it. Names and layouts are placeholders.
#include "game/UnkGameManager.h"

struct UnkScenarioGroup_011a3d74 {
    u8 data[0x60];
};

struct UnkScenarioGroups_011a3d74 {
    UnkScenarioGroup_011a3d74* data;
    u8 pad_0x8[8];
    u32 count;
};

struct UnkScenarioPoints {
    u8 pad_0x0[0x28];
    UnkScenarioGroups_011a3d74 first;  // 0x28
    u8 pad_0x40[0xb0];
    UnkScenarioGroups_011a3d74 second;  // 0xf0
    u8 pad_0x108[0xb0];
    UnkScenarioGroups_011a3d74 third;  // 0x1b8
};
static_assert(__builtin_offsetof(UnkScenarioPoints, third) == 0x1b8, "UnkScenarioPoints layout");

void* fn_71002636b0(UnkScenarioGroup_011a3d74* group, const u64* key);

static inline void* Find(UnkScenarioGroups_011a3d74* groups, const u64* key) {
    for (UnkScenarioGroup_011a3d74 *g = groups->data, *end = g + groups->count; g != end; ++g)
        if (void* found = fn_71002636b0(g, key))
            return found;
    return nullptr;
}

// NON_MATCHING(minor): same code, but an empty third array returns through its own copy of the
// epilogue (the original shares one).
void* fn_71011a3d74(UnkGameManagerSub_0x608* points, const u64* key) {
    auto* self = reinterpret_cast<UnkScenarioPoints*>(points);
    if (void* found = Find(&self->first, key))
        return found;
    if (void* found = Find(&self->second, key))
        return found;
    return Find(&self->third, key);
}

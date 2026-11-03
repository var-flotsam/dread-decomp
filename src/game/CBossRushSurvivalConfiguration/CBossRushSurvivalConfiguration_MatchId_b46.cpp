// CBossRushSurvivalConfiguration's unit (tools/tu_map.py unit 2544): while active (+0xb8) with a
// current stage (+0x100, 1 to 12), compares an object's id (+0x18) with one of the stage's two ids
// (0xa8-byte stage records): a match with the first marks the configuration (+0xa44, kind 0), a
// match with the second goes on to fn_7100dca36c (kind 1). Empty ids never match. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkSurvivalObject_b46a3 {
    u8 pad_0x0[0x18];
    base::global::UnkStringData* id;  // 0x18
};

struct UnkSurvivalStage_b46a3 {
    CStrId first;   // 0x0
    u8 pad_0x8[0x8];
    CStrId second;  // 0x10
    u8 pad_0x18[0x90];
};
static_assert(sizeof(UnkSurvivalStage_b46a3) == 0xa8, "UnkSurvivalStage_b46a3 layout");

struct UnkSurvivalConfiguration_b46a3 {
    u8 pad_0x0[0xb8];
    bool active;  // 0xb8
    u8 pad_0xb9[0x47];
    u32 stage;  // 0x100
    u8 pad_0x104[0x34];
    UnkSurvivalStage_b46a3 stages[13];  // 0x138
    u8 pad_0x9c0[0x84];
    bool matched;  // 0xa44
};

void fn_7100dca36c(UnkSurvivalConfiguration_b46a3* self, UnkSurvivalObject_b46a3* object);

// Written on integers: as pointers Clang swaps the cmp operands.
static inline bool IsEmpty(const CStrId& id) {
    return reinterpret_cast<u64>(id.data) == reinterpret_cast<u64>(g_unk_7101d47108) + 0x10;
}

void fn_7100dd1358(UnkSurvivalConfiguration_b46a3* self, UnkSurvivalObject_b46a3* object, u32 kind) {
    if (!object || !self->active)
        return;
    u32 stage = self->stage;
    if (stage - 1 > 11)
        return;
    switch (kind) {
    case 0: {
        const CStrId& id = self->stages[stage].first;
        if (!IsEmpty(id) && object->id == id.data)
            self->matched = true;
        break;
    }
    case 1: {
        const CStrId& id = self->stages[stage].second;
        if (!IsEmpty(id) && object->id == id.data)
            fn_7100dca36c(self, object);
        break;
    }
    }
}

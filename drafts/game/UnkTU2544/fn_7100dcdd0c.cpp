// CBossRushSurvivalConfiguration's unit (unit 2544): false while inactive or out of stages, else
// false only when the object's id equals the stage's chosen id. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkSurvivalObject_dcdd0c {
    u8 pad_0x0[0x18];
    base::global::UnkStringData* id;  // 0x18
};

struct UnkSurvivalStage_dcdd0c {
    u8 pad_0x0[0x8];
    CStrId primary;    // 0x8
    CStrId secondary;  // 0x10
    u8 pad_0x18[0x90];
};
static_assert(sizeof(UnkSurvivalStage_dcdd0c) == 0xa8, "layout");

struct UnkSurvivalConfiguration_dcdd0c {
    u8 pad_0x0[0xb8];
    bool active;  // 0xb8
    u8 pad_0xb9[0x47];
    u32 stage;  // 0x100
    u8 pad_0x104[0x34];
    UnkSurvivalStage_dcdd0c stages[13];  // 0x138
};

static inline bool IsEmpty(const CStrId& id) {
    return reinterpret_cast<u64>(id.data) == reinterpret_cast<u64>(g_unk_7101d47108) + 0x10;
}

bool fn_7100dcdd0c(UnkSurvivalConfiguration_dcdd0c* self, UnkSurvivalObject_dcdd0c* object) {
    if (self->active) {
        u32 stage = self->stage;
        if (stage - 1 <= 11) {
            UnkSurvivalStage_dcdd0c& s = self->stages[stage];
            const CStrId* pick = IsEmpty(s.primary) ? &s.secondary : &s.primary;
            if (!object || IsEmpty(*pick) || object->id != pick->data)
                return true;
        }
    }
    return false;
}

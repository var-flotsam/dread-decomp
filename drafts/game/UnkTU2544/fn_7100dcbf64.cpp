// CBossRushSurvivalConfiguration's unit (unit 2544): two floats for a stage: the amount found in the
// stage's (or the shared) entry list for the max-life property, and that amount less the stage's ramp
// start, eased by the player's value (at least 1). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkIdHolder_dcbf64 {
    base::global::UnkStringData* id;
};

struct UnkEntryObject_dcbf64 {
    void** vtable;
    UnkIdHolder_dcbf64* vf_0x28() { return reinterpret_cast<UnkIdHolder_dcbf64* (*)(UnkEntryObject_dcbf64*)>(vtable[5])(this); }
};

struct UnkEntry_dcbf64 {
    UnkEntryObject_dcbf64* object;  // 0x0
    f32 value;                      // 0x8
    u32 pad_0xc;
};

struct UnkEntryList_dcbf64 {
    UnkEntry_dcbf64* entries;  // 0x0
    u32 count;                 // 0x8
    u32 pad_0xc;
};

struct UnkStage_dcbf64 {
    u8 pad_0x0[0x58];
    UnkEntryList_dcbf64 list;  // 0x58
    u8 pad_0x68[0x40];
};
static_assert(sizeof(UnkStage_dcbf64) == 0xa8, "layout");

struct UnkRamp_dcbf64 {
    f32 start;
    f32 pad_0x4;
    f32 pad_0x8;
};

struct UnkPlayerState_dcbf64 {
    u8 pad_0x0[0x54];
    f32 value;  // 0x54
};

struct UnkPlayerView_dcbf64 {
    u8 pad_0x0[0x1b8];
    UnkPlayerState_dcbf64* state;  // 0x1b8
};

struct UnkSurvivalConfiguration_dcbf64 {
    u8 pad_0x0[0xba];
    bool flag_0xba;
    bool shared;  // 0xbb
    u8 pad_0xbc[0x14];
    u32 field_0xd0;
    u8 pad_0xd4[0x4];
    UnkEntryList_dcbf64 shared_list;  // 0xd8
    u8 pad_0xe8[0x18];
    u32 stage;  // 0x100
    u8 pad_0x104[0x34];
    UnkStage_dcbf64 stages[13];  // 0x138
};

void fn_7100dcbf64(UnkSurvivalConfiguration_dcbf64* self, u32 index, f32* found, f32* result) {
    *found = -1.0f;
    *result = -1.0f;
    UnkEntryList_dcbf64* list = self->shared ? &self->shared_list : &self->stages[index].list;
    u32 count = list->count;
    if (count != 0) {
        UnkEntry_dcbf64* entry = list->entries;
        for (u32 i = 0; i < count; i++, entry++) {
            if (entry->object->vf_0x28()->id == g_unk_717275a058.name.data)
                *result = entry->value;
        }
    }
    if (self->flag_0xba) {
        *found = *result;
        return;
    }
    if (self->shared) {
        if (self->field_0xd0 == 0) {
            *found = *result;
            return;
        }
    } else if (static_cast<s32>(index) < 1 ? self->stage <= 1 : index == 1) {
        *found = *result;
        return;
    }
    s32 ramp_index = static_cast<s32>(index) - 1;
    if (ramp_index < 1)
        ramp_index = 12;
    UnkRamp_dcbf64* ramp = reinterpret_cast<UnkRamp_dcbf64*>(reinterpret_cast<char*>(self) + 0x99c) + ramp_index;
    f32 start = ramp->start;
    if (UnkPlayerView_dcbf64* player = reinterpret_cast<UnkPlayerView_dcbf64*>(fn_7100dec788(g_unk_7101d47310))) {
        if (player->state)
            start = player->state->value;
    }
    f32 value = *result - (ramp->start - start);
    *found = value > 1.0f ? value : 1.0f;
}

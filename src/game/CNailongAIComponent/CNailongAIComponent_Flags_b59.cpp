// CNailongAIComponent's unit (tools/tu_map.py): two handlers with one body. For each string id in the
// list at +0x14898 (count at +0x148a0), when the owner (+0x30) has its component at +0x228, hands
// the id's hash to it (fn_7101172764 or fn_7101172780, as in
// src/game/CGooplotAIComponent/CGooplotAIComponent_Reset_b49.cpp); then, when the owner's timeline
// (+0x2e8) knows its id (g_unk_710164a2f8 or g_unk_710164a2f0), sets that id to the all-flags mask
// (as src/game/CKraidAIComponent/fn_7100600c98.cpp). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkFlagTarget;
void fn_7101172780(UnkFlagTarget* target, const char* name);
void fn_7101172764(UnkFlagTarget* target, const char* name);

struct UnkNailongKey_b59a4;
extern UnkNailongKey_b59a4 g_unk_710164a2f8;  // reached through the GOT
extern UnkNailongKey_b59a4 g_unk_710164a2f0;  // reached through the GOT

struct UnkNailongStrId_b59a4 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkNailongOwner_b59a4 {
    u8 pad_0x0[0x228];
    UnkFlagTarget* field_0x228;
    u8 pad_0x230[0x2e8 - 0x230];
    UnkTimelineComponent* field_0x2e8;
};

struct UnkNailongAI_b59a4 {
    u8 pad_0x0[0x30];
    UnkNailongOwner_b59a4* owner;
    u8 pad_0x38[0x14898 - 0x38];
    UnkNailongStrId_b59a4** ids;
    u32 count;
};

template <void (*Set)(UnkFlagTarget*, const char*), UnkNailongKey_b59a4* Key>
static inline void SetFlags_b59a4(UnkNailongAI_b59a4* self) {
    base::reflection::UnkReflTypeIdArg mask;
    u64 hash;
    for (UnkNailongStrId_b59a4 **it = self->ids, **end = it + self->count; it != end; ++it) {
        if (UnkFlagTarget* target = self->owner->field_0x228) {
            hash = (*it)->hash;
            Set(target, reinterpret_cast<const char*>(&hash));
        }
    }
    const u64* key = reinterpret_cast<const u64*>(Key);
    UnkTimelineComponent* timeline = self->owner->field_0x2e8;
    if (timeline && fn_7100a2c13c(timeline, key)) {
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, key, &mask.id, false, -1.0f);
    }
}

void fn_7100621020(UnkNailongAI_b59a4* self) {
    SetFlags_b59a4<fn_7101172764, &g_unk_710164a2f8>(self);
}

void fn_7100621d38(UnkNailongAI_b59a4* self) {
    SetFlags_b59a4<fn_7101172780, &g_unk_710164a2f0>(self);
}

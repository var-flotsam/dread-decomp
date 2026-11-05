// CChozoCommanderAIComponent's unit (the name from reflection): a hit filter (as CChozoWarriorAIComponent's,
// src/game/CChozoWarriorAIComponent/CChozoWarriorAIComponent_HitFilter_b67a2.cpp). A hit whose info flags (+0xb4)
// have 0x4000 set writes 0 to *out and returns 1, unless the component's +0x1140 is set and the owner's object
// at +0x260 is missing or fn_710113d83c refuses it; everything else defers to fn_71011061ac.
// Names and layouts are placeholders.
#include "types.h"

// As src/game/CGoliathAIComponent/CGoliathAIComponent_HitFilter_b55.cpp declares it.
struct UnkGoliathAI_b55;
struct UnkGoliathHitInfo_b55;
struct UnkGoliathHitTarget_b55;
u32 fn_71011061ac(UnkGoliathAI_b55* self, u32* out, void* arg2, UnkGoliathHitInfo_b55* info,
                  UnkGoliathHitTarget_b55** hit);
bool fn_710113d83c(void*);

struct UnkChozoCommanderOwner_A2_b69 {
    u8 pad_0x0[0x260];
    void* field_0x260;
};

struct UnkChozoCommanderHitAI_A2_b69 {
    u8 pad_0x0[0x30];
    UnkChozoCommanderOwner_A2_b69* owner;  // +0x30
    u8 pad_0x38[0x1140 - 0x38];
    u32 field_0x1140;
};

struct UnkChozoCommanderHitInfo_A2_b69 {
    u8 pad_0x0[0xb4];
    u32 flags;
};

u32 fn_7100452068(UnkChozoCommanderHitAI_A2_b69* self, u32* out, void* arg2, UnkChozoCommanderHitInfo_A2_b69* info,
                  UnkGoliathHitTarget_b55** hit) {
    if (self->field_0x1140 != 0) {
        if (info->flags & 0x4000) {
            void* object = self->owner->field_0x260;
            if (object && fn_710113d83c(object)) {
                *out = 0;
                return 1;
            }
        }
    } else if (info->flags & 0x4000) {
        *out = 0;
        return 1;
    }
    return fn_71011061ac(reinterpret_cast<UnkGoliathAI_b55*>(self), out, arg2,
                         reinterpret_cast<UnkGoliathHitInfo_b55*>(info), hit);
}

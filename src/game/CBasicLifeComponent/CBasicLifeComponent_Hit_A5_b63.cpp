// CBasicLifeComponent's unit (tools/tu_map.py): after the base call fn_710116064c, keeps `info` at +0x270;
// sets one id on the owner's timeline (+0x30, +0x2e8) to the all-flags mask when the timeline holds it, and
// another on the timeline of the actor `info` names (+0x8, +0x10) when bit 6 of +0xb4 is set; calls the
// callback at +0x260 with its context (+0x258) and `info`; then, when the +0x268 object's flag at +0x118 is
// set, runs fn_7100232c68 on the owner and, when fn_71004dad10 finds something and +0x270 is set, passes
// both to fn_71007c1ce0 and clears +0x270. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTU341.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkLifeBase_A5_b63;
struct UnkLifeArg_A5_b63;
struct UnkHitInfo_A5_b63;
void fn_710116064c(UnkLifeBase_A5_b63* self, UnkLifeArg_A5_b63* arg, UnkHitInfo_A5_b63* info);
char* fn_71004dad10(UnkActor*);
void fn_71007c1ce0(char* p, UnkHitInfo_A5_b63* info);

extern u64 g_unk_710164af78;
extern u64 g_unk_710164af80;

struct UnkHitLink_A5_b63 {
    u8 pad_0x0[0x10];
    UnkActor* actor;
};

struct UnkHitInfo_A5_b63 {
    u8 pad_0x0[0x8];
    UnkHitLink_A5_b63* link;
    u8 pad_0x10[0xb4 - 0x10];
    u8 flags;  // 0xb4
};

struct UnkLifeConfig_A5_b63 {
    u8 pad_0x0[0x118];
    bool field_0x118;
};

struct UnkLife_A5_b63 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x258 - 0x38];
    void* context;                                       // 0x258
    void (*callback)(void* context, UnkHitInfo_A5_b63* info);  // 0x260
    UnkLifeConfig_A5_b63* config;                        // 0x268
    UnkHitInfo_A5_b63* info;                             // 0x270
};

// Sets `key` to the all-flags mask (inlined: its slot comes first in the frame).
__attribute__((always_inline)) static inline void SetAll_A5_b63(UnkTimelineComponent* timeline, const u64* key) {
    goto start;  // the mask gets no lifetime markers and keeps its own slot
    base::reflection::UnkReflTypeIdArg mask;
start:
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, key, &mask.id, false, -1.0f);
}

void fn_71008a3e64(UnkLife_A5_b63* self, UnkLifeArg_A5_b63* arg, UnkHitInfo_A5_b63* info) {
    fn_710116064c(reinterpret_cast<UnkLifeBase_A5_b63*>(self), arg, info);
    self->info = info;
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    if (timeline && fn_7100a2c13c(timeline, &g_unk_710164af78)) {
        goto first;  // no lifetime markers either
        base::reflection::UnkReflTypeIdArg mask;
    first:
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &g_unk_710164af78, &mask.id, false, -1.0f);
    }
    if ((info->flags >> 6 & 1) && info->link) {
        if (UnkActor* actor = info->link->actor) {
            if (auto* other = static_cast<UnkTimelineComponent*>(actor->field_0x2e8)) {
                SetAll_A5_b63(other, &g_unk_710164af80);
            }
        }
    }
    if (self->callback)
        self->callback(self->context, info);
    if (self->config->field_0x118) {
        fn_7100232c68(self->owner);
        if (char* p = fn_71004dad10(self->owner)) {
            if (self->info) {
                fn_71007c1ce0(p, self->info);
                self->info = nullptr;
            }
        }
    }
}

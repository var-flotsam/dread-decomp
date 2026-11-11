// CBigkranXAIComponent's unit (tools/tu_map.py): a message handler override. After the base AI component's handler
// (fn_710036974c), whose result it returns: with the message's flag (+8) set, a pending +0x14ba1 becomes +0x14ba2.
// Otherwise a pending +0x14ba2 raises +0x14ba1 (clearing +0x14b4c first unless it was raised), and with the owner's
// (+0x30) +0x220 object and timeline (+0x2e8) present, sets g_unk_7101649398 (the object in kind 0xb) or
// g_unk_71016493a0 on the timeline to the all-flags mask. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CAnimationComponent.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

// As src/game/CObsydomithonAIComponent/CObsydomithonAIComponent_Message_b60.cpp declares them.
struct UnkSclawkAI_b44a2;
struct UnkSclawkMessage_b44a2;
struct UnkTakumakuComponent;

struct UnkBigkranXKey_b61a2;
extern UnkBigkranXKey_b61a2 g_unk_7101649398;  // reached through the GOT
extern UnkBigkranXKey_b61a2 g_unk_71016493a0;  // reached through the GOT

struct UnkBigkranXOwner_b61a2 {
    u8 pad_0x0[0x220];
    UnkTakumakuComponent* field_0x220;
    u8 pad_0x228[0x2e8 - 0x228];
    void* field_0x2e8;
};

struct UnkBigkranXMessage_b61a2 {
    u8 pad_0x0[0x8];
    bool field_0x8;
};

struct UnkBigkranXSub_b61a2 {
    u32 field_0x0;
    u8 pad_0x4[0x51];
    bool field_0x55;
    bool field_0x56;
};

struct UnkBigkranXAI_b61a2 {
    u8 pad_0x0[0x30];
    UnkBigkranXOwner_b61a2* owner;
    u8 pad_0x38[0x14b4c - 0x38];
    UnkBigkranXSub_b61a2 field_0x14b4c;
};

u32 fn_71003f4d28(UnkBigkranXAI_b61a2* self, UnkBigkranXMessage_b61a2* msg) {
    UnkBigkranXSub_b61a2* sub = &self->field_0x14b4c;
    u32 result = fn_710036974c(reinterpret_cast<UnkSclawkAI_b44a2*>(self), reinterpret_cast<UnkSclawkMessage_b44a2*>(msg));
    if (msg->field_0x8) {
        if (sub->field_0x55) {
            sub->field_0x55 = false;
            sub->field_0x56 = true;
        }
        return result;
    }
    if (sub->field_0x56) {
        if (!sub->field_0x55)
            sub->field_0x0 = 0;
        sub->field_0x55 = true;
    }
    UnkBigkranXOwner_b61a2* owner = self->owner;
    UnkTakumakuComponent* object = owner->field_0x220;
    if (!object)
        return result;
    auto* timeline = static_cast<UnkTimelineComponent*>(owner->field_0x2e8);
    if (!timeline)
        return result;
    // Jumped over: the first mask gets no lifetime markers, so it keeps its own slot.
    goto start;
    base::reflection::UnkReflTypeIdArg kind_mask;
start:
    if (fn_7101178e6c(object, 0xb)) {
        kind_mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_7101649398), &kind_mask.id, false, -1.0f);
        return result;
    }
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_71016493a0), &mask.id, false, -1.0f);
    return result;
}

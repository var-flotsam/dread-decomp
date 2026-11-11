// CObsydomithonAIComponent's unit (tools/tu_map.py): a message handler override. After the base AI
// component's handler (fn_710036974c): with the message's flag (+8) set, the owner's (+0x30) timeline
// (+0x2e8) gets g_unk_710164a340 set to the all-flags mask when its +0x220 object is in kind 0xb
// (fn_7101178e6c), that object gets fn_710117ab64 with 1 and the hidden g_unk_71726f9430, and the object
// at +0x147a8 its slot 12 with 0. Otherwise the value at +0x1475c is copied to +0x14760 and, while the
// owner's +0x1b8 object is not in state 2, the attack at +0x146c8 is stopped (fn_710062a268) when its
// value at +0x50 is set and not 0x7fffffff, and g_unk_710164a348 set in kind 0xb; the +0x220 object then
// gets fn_710117ab64 with 0. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CAnimationComponent.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "game/CObsydomithonAttack.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

// As src/game/CSclawkAIComponent/CSclawkAIComponent_b44.cpp declares it.
struct UnkSclawkAI_b44a2;
struct UnkSclawkMessage_b44a2;

// As src/game/CTakumakuAIComponent/CTakumakuAIComponent_b35.cpp declares it.
struct UnkTakumakuComponent;

// As src/game/CRockDiverAIComponent/CRockDiverAIComponent_Send_b57.cpp declares it.
struct UnkAnimationComponent;

extern char g_unk_71726f9430[] __attribute__((visibility("hidden")));

struct UnkObsydomithonKey_b60a4;
extern UnkObsydomithonKey_b60a4 g_unk_710164a340;  // reached through the GOT
extern UnkObsydomithonKey_b60a4 g_unk_710164a348;  // reached through the GOT

struct UnkObsydomithonState_b60a4 {
    u8 pad_0x0[0x70];
    s32 state;
};

struct UnkObsydomithonOwner_b60a4 {
    u8 pad_0x0[0x1b8];
    UnkObsydomithonState_b60a4* field_0x1b8;
    u8 pad_0x1c0[0x220 - 0x1c0];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x2e8 - 0x228];
    UnkTimelineComponent* field_0x2e8;
};

struct UnkObsydomithonListener_b60a4 {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    virtual void vf_3();
    virtual void vf_4();
    virtual void vf_5();
    virtual void vf_6();
    virtual void vf_7();
    virtual void vf_8();
    virtual void vf_9();
    virtual void vf_10();
    virtual void vf_11();
    virtual void vf_12(u32 value);
};

struct UnkObsydomithonAttack_b60a4 {
    u8 pad_0x0[0x50];
    s32 field_0x50;
};

struct UnkObsydomithonAI_b60a4 {
    u8 pad_0x0[0x30];
    UnkObsydomithonOwner_b60a4* owner;
    u8 pad_0x38[0x146c8 - 0x38];
    UnkObsydomithonAttack_b60a4 field_0x146c8;
    u8 pad_0x14718[0x1475c - 0x14718 - 4];
    u32 field_0x1475c;
    u32 field_0x14760;
    u8 pad_0x14764[0x147a8 - 0x14764];
    UnkObsydomithonListener_b60a4* field_0x147a8;
};
static_assert(__builtin_offsetof(UnkObsydomithonAI_b60a4, field_0x1475c) == 0x1475c, "layout");
static_assert(__builtin_offsetof(UnkObsydomithonAI_b60a4, field_0x147a8) == 0x147a8, "layout");

struct UnkObsydomithonMessage_b60a4 {
    u8 pad_0x0[0x8];
    bool field_0x8;
};

static inline void SetAll_b60a4(UnkTimelineComponent* timeline, UnkObsydomithonKey_b60a4* key,
                                base::reflection::UnkReflTypeIdArg& mask) {
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(key), &mask.id, false, -1.0f);
}

u32 fn_710062f9c8(UnkObsydomithonAI_b60a4* self, UnkObsydomithonMessage_b60a4* msg) {
    u32 result = fn_710036974c(reinterpret_cast<UnkSclawkAI_b44a2*>(self),
                               reinterpret_cast<UnkSclawkMessage_b44a2*>(msg));
    // The masks are declared past a goto (no lifetime markers), so they keep their own slots.
    goto start;
    base::reflection::UnkReflTypeIdArg mask1;
    base::reflection::UnkReflTypeIdArg mask2;
start:
    UnkObsydomithonOwner_b60a4* owner = self->owner;
    UnkTimelineComponent* timeline = owner->field_0x2e8;
    UnkAnimationComponent* anim = owner->field_0x220;
    if (msg->field_0x8) {
        if (timeline && anim && fn_7101178e6c(reinterpret_cast<UnkTakumakuComponent*>(anim), 0xb))
            SetAll_b60a4(timeline, &g_unk_710164a340, mask1);
        if (anim)
            fn_710117ab64(anim, 1, g_unk_71726f9430);
        if (UnkObsydomithonListener_b60a4* listener = self->field_0x147a8)
            listener->vf_12(0);
        return result;
    }
    self->field_0x14760 = self->field_0x1475c;
    if (owner->field_0x1b8 && owner->field_0x1b8->state != 2) {
        s32 v = self->field_0x146c8.field_0x50;
        if (v != 0 && v != 0x7fffffff)
            fn_710062a268(&self->field_0x146c8, true);
        if (timeline && anim) {
            if (fn_7101178e6c(reinterpret_cast<UnkTakumakuComponent*>(anim), 0xb))
                SetAll_b60a4(timeline, &g_unk_710164a348, mask2);
            goto send;
        }
    }
    if (!anim)
        return result;
send:
    fn_710117ab64(anim, 0, g_unk_71726f9430);
    return result;
}

// CEmmyProtoAIComponent's unit (tools/tu_map.py unit 791): after fn_71005447a0, enters state g_unk_71726debd0
// (on) and clears the id property g_unk_71726debd8 of the blackboard section g_unk_71726b7cb8 (unless the
// blackboard is locked); stops layers 8 and 10 of the owner's animation and sends it "groundwalk" with mode
// 2; asks for state 2 against the actor the owner acts against; resets every slot of the owner's model;
// and, when fn vf_0x818 says so, registers the callback record (+0x2a710) in the set of the kind-3 entry
// of that actor's info. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkDeferredSet.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "game/CAbilityShinespark.h"
#include "game/CAnimationComponent.h"
#include "game/CBlackboard.h"
#include "game/UnkActor.h"
#include "game/UnkSlots.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU811.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "game/behaviortree/CBehavior.h"
#include "game/behaviortree/CEmmyChaseAction.h"
#include "types.h"

using base::global::CStrId;

struct UnkEmmyActor_b55;
struct UnkActorTarget;
extern u32 g_unk_7101649db8;
extern const base::math::CVector4D g_unk_7101649d08 __attribute__((visibility("hidden")));

void fn_71005447a0(UnkEmmyActor_b55* self);
void fn_71004fd510(base::UnkDeferredSet<void>* set, void* item);
void fn_71004fd6a0(base::UnkDeferredSet<void>* set, void* item);

struct UnkEmmyModel_l4faea8 {
    u8 pad_0x0[0x138];
    UnkSlots* slots;  // 0x138
};

struct UnkEmmyRender_l4faea8 {
    u8 pad_0x0[0x50];
    UnkEmmyModel_l4faea8* model;  // 0x50
};

struct UnkEmmyOwner_l4faea8 {
    u8 pad_0x0[0x220];
    UnkActorTarget* field_0x220;
    UnkEmmyRender_l4faea8* field_0x228;
};

struct UnkGrabEntry_l4faea8 {
    u8 pad_0x0[0x10];
    u32 kind;  // 0x10
    u8 pad_0x14[0x48 - 0x14];
    base::UnkDeferredSet<void> set;  // 0x48
};

// The entries are 4-byte aligned in the original (the array follows the count directly).
struct __attribute__((packed)) UnkGrabInfo_l4faea8 {
    u8 pad_0x0[0x2d18];
    u32 count;                          // 0x2d18
    UnkGrabEntry_l4faea8* entries[1];  // 0x2d1c
};

struct UnkGrabActor_l4faea8 {
    u8 pad_0x0[0x208];
    UnkGrabInfo_l4faea8* info;  // 0x208
};

struct UnkEmmyProto_l4faea8 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkEmmyOwner_l4faea8* owner;  // 0x30
    u8 pad_0x38[0x1260 - 0x38];
    CBlackboard blackboard;  // 0x1260
    u8 pad_after[0x2a710 - 0x1260 - sizeof(CBlackboard)];
    base::UnkDeferredSet<void>* registered;  // 0x2a710

    bool vf_0x818() { return reinterpret_cast<bool (*)(UnkEmmyProto_l4faea8*)>(vtable[0x818 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkEmmyProto_l4faea8, registered) == 0x2a710, "layout");

// The typed value lives in the caller's frame for the whole function: the original gives it its own slot.
static inline void SetSectionId(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name, CStrId* value,
                                UnkTypedValue& v) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            v.type = fn_7100009658();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

void fn_71004faea8(UnkEmmyProto_l4faea8* self) {
    UnkTypedValue v;
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    fn_71005447a0(reinterpret_cast<UnkEmmyActor_b55*>(self));
    fn_7100544a34(reinterpret_cast<UnkEmmyActor_b55*>(self), g_unk_71726debd0, true);
    SetSectionId(&self->blackboard, g_unk_71726b7cb8, &g_unk_71726debd8, &base::global::CStrId::Empty(), v);
    if (self->owner->field_0x220) {
        u32 layer = g_unk_7101649db8;
        fn_710117662c(self->owner->field_0x220, 8, layer, false, false, -1.0f);
        fn_710117662c(self->owner->field_0x220, 10, layer + 1, false, false, -1.0f);
        UnkActorTarget* target = self->owner->field_0x220;
        u64 hash = fn_7100001570("groundwalk", 10);
        mode.id = 2;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b968, &g_unk_7172769740, &mask);
        fn_7101176a88(self->owner->field_0x220, 1);
    }
    fn_7100547428(reinterpret_cast<UnkEmmyActor_b55*>(self), 2,
                  fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner)), true);

    if (UnkEmmyModel_l4faea8* model = self->owner->field_0x228->model) {
        u32 count = model->slots->info->count;
        for (u32 i = 0; i < count; ++i)
            fn_71001c2764(model->slots, i, 1, &g_unk_7101649d08, 1, true);
    }

    if (!self->vf_0x818())
        return;
    auto* actor = reinterpret_cast<UnkGrabActor_l4faea8*>(fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner)));
    if (!actor->info)
        return;
    UnkGrabInfo_l4faea8* info =
        reinterpret_cast<UnkGrabActor_l4faea8*>(fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner)))->info;
    UnkGrabEntry_l4faea8* found = nullptr;
    for (UnkGrabEntry_l4faea8 **it = info->entries, **end = it + info->count; it != end; ++it) {
        if ((*it)->kind == 3) {
            found = *it;
            break;
        }
    }
    if (!found)
        return;
    if (self->registered) {
        fn_71004fd510(self->registered, &self->registered);
        self->registered = nullptr;
    }
    self->registered = &found->set;
    fn_71004fd6a0(self->registered, &self->registered);
}

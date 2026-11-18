// CEmmyChaseAction unit. fn_7100512b94: ends the chase: clears the two words at +0x19e0 of the owner's
// enemy movement, turns off every door's EMMY effect (when the tunable's +0x28 is set), runs
// fn_7100511f04, stops layer 10 of the owner's +0x220, clears the EMMY's blackboard id and flag, drops the
// weak handle at +0x78, hands a default parameter block to the owner's +0x298 and leaves with the base
// step (fn_710051db44). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityShinespark.h"
#include "game/CAnimationComponent.h"
#include "game/CBlackboard.h"
#include "game/CDoorEmmyFXComponent.h"
#include "game/UnkTypedValue.h"
#include "game/behaviortree/CBehavior.h"
#include "game/behaviortree/CEmmyChaseAction.h"
#include "game/behaviortree/CEmmyStateAction.h"
#include "game_s2/UnkChargeComponent.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkComponent;
struct UnkDoorEmmyFX_b50;

struct UnkDoorList_b0193 {
    base::reflection::UnkReflObject** items;
    u32 count;
};

extern UnkPropertyName g_unk_71726df1a8;

void fn_71007b6be4(UnkDoorEmmyFX_b50* self, bool add);

// The tunable's switch for the doors' EMMY effect.
struct UnkEmmyDoorTunable_l512b94 {
    u8 pad_0x0[0x28];
    bool doors_fx;  // 0x28
};

struct UnkEmmyDoor_l512b94 {
    u8 pad_0x0[0x10];
    UnkReflObject* fx;  // 0x10
};

struct UnkEmmyMovement_l512b94 {
    u8 pad_0x0[0x19e0];
    u64 field_0x19e0;
    u64 field_0x19e8;
};

struct UnkEmmyAI_l512b94 {
    u8 pad_0x0[0x1260];
    CBlackboard blackboard;  // 0x1260
};

struct UnkEmmyChaseOwner_l512b94 {
    u8 pad_0x0[0x1f0];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x220 - 0x1f8];
    UnkActorTarget* field_0x220;
    u8 pad_0x228[0x240 - 0x228];
    UnkReflObject* ai;  // 0x240
    u8 pad_0x248[0x298 - 0x248];
    void* field_0x298;
};

struct UnkEmmyChaseAction_l512b94 {
    u8 pad_0x0[0x18];
    UnkEmmyChaseOwner_l512b94* owner;  // 0x18
    u8 pad_0x20[0x60 - 0x20];
    u8 field_0x60[0x18];
    UnkWeakHandle* field_0x78;
};

void fn_7100511f04(UnkEmmyChaseAction_l512b94* self, u32 a, bool b, f32 time);

template <class T>
static inline void SetProperty(CBlackboard* blackboard, UnkPropertyName* name, T* value) {
    if (CBlackboard::CSection* section = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = base::reflection::UnkReflTypeOf<T>::Get();
            v.value = value;
            fn_7100332c80(section, name, &v);
        }
    }
}

void fn_7100512b94(UnkEmmyChaseAction_l512b94* self, u32 arg) {
    UnkReflObject* movement = self->owner->movement;
    if (movement)
        movement = fn_710009d50c(movement->vf_0x0(), fn_71010c74e0()) ? movement : nullptr;
    // Faithful to the original: the words are cleared without checking that the cast succeeded.
    UnkEmmyMovement_l512b94* mover = reinterpret_cast<UnkEmmyMovement_l512b94*>(movement);
    mover->field_0x19e0 = 0;
    mover->field_0x19e8 = 0;

    if (reinterpret_cast<UnkEmmyDoorTunable_l512b94*>(&g_unk_71726e0aa8)->doors_fx) {
        UnkDoorList_b0193* doors = fn_71007b5d90();
        for (u32 i = 0; i < doors->count; ++i) {
            UnkEmmyDoor_l512b94* door = reinterpret_cast<UnkEmmyDoor_l512b94*>(doors->items[i]);
            if (!door)
                continue;
            UnkReflObject* fx = door->fx;
            if (fx && fn_710009d50c(fx->vf_0x0(), fn_71007b55e4()))
                fn_71007b6be4(reinterpret_cast<UnkDoorEmmyFX_b50*>(fx), false);
        }
    }

    fn_7100511f04(self, 0, true, 0.0f);
    if (self->owner->field_0x220)
        fn_7101178e08(self->owner->field_0x220, 10, false, false, -1.0f);

    UnkReflObject* ai = self->owner->ai;
    if (ai)
        ai = fn_710009d50c(ai->vf_0x0(), fn_710053b934()) ? ai : nullptr;
    CBlackboard* blackboard = &reinterpret_cast<UnkEmmyAI_l512b94*>(ai)->blackboard;
    SetProperty(blackboard, &g_unk_71726debd8, &g_unk_7101d470e0);
    bool chasing = false;
    SetProperty(blackboard, &g_unk_71726df1a8, &chasing);

    if (UnkWeakHandle* handle = self->field_0x78) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->field_0x60);
        ResetRef(&self->field_0x78);
    }

    if (void* target = self->owner->field_0x298) {
        UnkParams params;
        __builtin_memset(params.field_0x8, 0, 13);
        params.field_0x30 = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.field_0x34 = 5;
        params.field_0x18 = 0;
        params.field_0x1c = 1;
        fn_71006f4b0c(target, &params, 1);
    }
    fn_710051db44(reinterpret_cast<UnkComponent*>(self), arg);
}

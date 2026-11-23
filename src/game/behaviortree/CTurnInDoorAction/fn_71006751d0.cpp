// behaviortree::CTurnInDoorAction unit (tools/tu_map.py, unit 996): the action's start. After the behaviour base,
// a Sabotoru's AI is marked and its controller updated, the door of its spawn point is turned toward the side
// the owner faces, the enemy movement gets its parameter blocks, and the animation is sent the turn to one side
// or the other. Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CBossAIComponent.h"
#include "game/CDoorShieldLifeComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkTU1376.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU367.h"
#include "game/behaviortree/CBehavior.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::reflection::CClass;

struct UnkAnimationComponent;


// An object reached through its vtable only.
struct UnkTurnObject_l6751d0 {
    void** vtable;

    const CClass* GetClass() { return reinterpret_cast<const CClass* (*)(UnkTurnObject_l6751d0*)>(vtable[0])(this); }
};

template <const CClass* (*Class)()>
static inline bool IsKindOf(UnkTurnObject_l6751d0* object) {
    return fn_710009d50c(object->GetClass(), Class());
}

struct UnkTurnController_l6751d0 : UnkTurnObject_l6751d0 {
    u8 pad_0x8[0x48];
    f32 field_0x50;

    void vf_0x168(f32 value, bool flag) {
        reinterpret_cast<void (*)(UnkTurnController_l6751d0*, f32, bool)>(vtable[0x168 / 8])(this, value, flag);
    }
    void vf_0x170(f32 value) {
        reinterpret_cast<void (*)(UnkTurnController_l6751d0*, f32)>(vtable[0x170 / 8])(this, value);
    }
};

struct UnkSabotoruAI_l6751d0 : UnkTurnObject_l6751d0 {
    u8 pad_0x8[0x146d0];
    f32 field_0x146d8;
    u8 pad_0x146dc[0x9c];
    u32 field_0x14778;
};
static_assert(__builtin_offsetof(UnkSabotoruAI_l6751d0, field_0x14778) == 0x14778, "layout");

struct UnkSabotoruSpawnPoint_l6751d0 : UnkTurnObject_l6751d0 {
    u8 pad_0x8[0x3b0];
    u8 door_link[0x8];  // 0x3b8
};

struct UnkSpawnPointHolder_l6751d0 {
    u8 pad_0x0[0x10];
    UnkSabotoruSpawnPoint_l6751d0* spawn_point;
};

struct UnkDoorLinkTarget_l6751d0 {
    u8 pad_0x0[0x10];
    UnkWeakTarget* door;
};

struct UnkTurnMovement_l6751d0 : UnkTurnObject_l6751d0 {
    u8 pad_0x8[0xf1];
    bool field_0xf9;

    void SetField_0xf9(bool value) {
        if (field_0xf9 != value) {
            field_0xf9 = value;
            reinterpret_cast<void (*)(UnkTurnMovement_l6751d0*)>(vtable[0x260 / 8])(this);
        }
    }
};

struct UnkTurnOwner_l6751d0 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x140];
    UnkTurnController_l6751d0* controller;  // 0x1b8
    u8 pad_0x1c0[0x30];
    UnkTurnMovement_l6751d0* movement;  // 0x1f0
    u8 pad_0x1f8[0x28];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkSabotoruAI_l6751d0* ai;  // 0x240
    u8 pad_0x248[0x50];
    void* field_0x298;
    u8 pad_0x2a0[0xf8];
    UnkSpawnPointHolder_l6751d0* spawn_point;  // 0x398
};
static_assert(__builtin_offsetof(UnkTurnOwner_l6751d0, spawn_point) == 0x398, "layout");

struct UnkTurnInDoorAction_l6751d0 {
    void* vtable;
    u8 pad_0x8[0x10];
    UnkTurnOwner_l6751d0* owner;  // 0x18
    u8 pad_0x20[0xc];
    bool field_0x2c;
};

// The enemy movement's parameter block of this kind.
struct UnkTurnMovementParams_l6751d0 {
    void** vtable;
    u8 pad_0x8[4];
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 field_0x38;
    u8 field_0x3c;
    u8 field_0x3d;
};

// The door the spawn point's link holds, if any.
static inline UnkWeakTarget* LinkedDoor(UnkSabotoruSpawnPoint_l6751d0* spawn_point) {
    UnkDoorLinkTarget_l6751d0* target = *reinterpret_cast<UnkDoorLinkTarget_l6751d0**>(
        fn_71003b0c4c(reinterpret_cast<UnkZiplineLink_b49*>(spawn_point->door_link)));
    return target ? target->door : nullptr;
}

void fn_71006751d0(UnkTurnInDoorAction_l6751d0* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    self->field_0x2c = false;
    UnkAnimationComponent* animation = self->owner->animation;

    if (self->owner->controller) {
        UnkSabotoruAI_l6751d0* ai = self->owner->ai;
        if (ai && IsKindOf<fn_7100677dd4>(ai)) {
            ai->field_0x14778 = 1;
            self->owner->controller->vf_0x170(ai->field_0x146d8);
            UnkTurnController_l6751d0* controller = self->owner->controller;
            controller->vf_0x168(controller->field_0x50, true);
        }
    }

    if (UnkSpawnPointHolder_l6751d0* holder = self->owner->spawn_point) {
        UnkSabotoruSpawnPoint_l6751d0* spawn_point = holder->spawn_point;
        if (spawn_point && IsKindOf<fn_7100a1b7b4>(spawn_point) && LinkedDoor(spawn_point)) {
            if (UnkDoorLock* lock = fn_71002af860(LinkedDoor(spawn_point))) {
                UnkDoorLife_b44a2* door = reinterpret_cast<UnkDoorLife_b44a2*>(lock);
                if (fn_710002614c(&self->owner->transform).m[2] > 0.0f)
                    fn_71008db24c(lock, self->owner);
                else
                    fn_71008db440(lock, self->owner);
                fn_71008d498c(door, 0, 0, 0);
            }
        }
    }

    UnkTurnMovement_l6751d0* movement = self->owner->movement;
    if (movement && IsKindOf<fn_71010c74e0>(movement)) {
        if (void* target = self->owner->field_0x298) {
            UnkParams params;
            params.field_0x1c = 0;
            __builtin_memset(params.field_0x20, 0, 16);
            params.field_0x30 = 0;
            params.vtable = g_unk_7101a25b10 + 2;
            __builtin_memset(params.field_0x8, 0, 13);
            params.field_0x39 = 0;
            params.field_0x3b = 0;
            params.field_0x3d = 0;
            params.field_0x34 = 5;
            fn_71006f4b0c(target, &params, 1);
        }
        UnkTurnMovementParams_l6751d0 params;
        params.field_0xc = 0;
        params.field_0x14 = 0;
        __builtin_memset(params.field_0x18, 0, 13);
        __builtin_memset(params.field_0x28, 0, 13);
        params.vtable = g_unk_7101a2e208 + 2;
        params.field_0x38 = 3;
        params.field_0x3c = 0;
        params.field_0x3d = 1;
        fn_71010c7e6c(reinterpret_cast<UnkEffectSink*>(movement), &params, 1);
        movement->SetField_0xf9(true);
    }

    if (!animation)
        return;
    // The two turn requests' arguments, one pair for each side. Declared past a goto (no lifetime markers), so
    // each keeps its own slot.
    goto send;
    base::reflection::UnkReflTypeIdArg mode_up;
    base::reflection::UnkReflTypeIdArg mask_up;
    base::reflection::UnkReflTypeIdArg mode_down;
    base::reflection::UnkReflTypeIdArg mask_down;
send:
    {
        if (fn_710002614c(&self->owner->transform).m[5] > 0.0f) {
            mode_up.id = 0;
            mask_up.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, 0xc2d1d284f785eda6, 0xf4061ff25a2163d6, &mode_up, &g_unk_710164b958,
                          &g_unk_7172769740, &mask_up);
        } else {
            mode_down.id = 0;
            mask_down.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, 0x24ed25e5223bca66, 0xf4061ff25a2163d6, &mode_down, &g_unk_710164b958,
                          &g_unk_7172769740, &mask_down);
        }
    }
}

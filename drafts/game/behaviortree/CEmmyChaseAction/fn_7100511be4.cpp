// behaviortree::CEmmyChaseAction unit (tools/tu_map.py unit 801): starts the chase. After the base start it
// resets the action's timers, takes the chase speed from the EMMY's AI list into the owner's animation values,
// reads the chase id from the AI's blackboard, turns on every door's EMMY effect (when the tunable says so),
// listens to the owner's animation component and its enemy movement, and refreshes the AI's ids. Names and
// layouts are placeholders.
#include "../../../../src/game_s7/UnkPropBagS7.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CBlackboard.h"
#include "game/CDoorEmmyFXComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/behaviortree/CBehavior.h"
#include "game/behaviortree/CEmmyChaseAction.h"
#include "game_s2/UnkChargeComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkEmmyChaseAction_l511be4;
struct UnkPatrolBase_ovb0118;
void fn_710051dae0(UnkPatrolBase_ovb0118* self);
struct UnkEmmyChaseAction_l512b94;
void fn_7100511f04(UnkEmmyChaseAction_l512b94* self, u32 a, bool b, f32 time);
struct UnkEmmyChaseAction3_ovb0001;
void fn_710051270c(UnkEmmyChaseAction3_ovb0001* self, f32 value);
struct UnkEmmyAI3_ovb0001;
void fn_7100546654(UnkEmmyAI3_ovb0001* ai);
struct UnkEmmyAILists_b46a1;
bool fn_710055b574(UnkEmmyAILists_b46a1* self, f32* out);
void fn_71005553a8(char* self, bool refresh);
struct UnkDoorEmmyFX_b50;
void CDoorEmmyFXComponent_SetEmmyChaseKey(UnkDoorEmmyFX_b50* self, bool add);

// The chase's callbacks, handed to the animation component and the enemy movement.
void fn_7100514074(UnkEmmyChaseAction_l511be4* self);
void fn_710051408c(UnkEmmyChaseAction_l511be4* self);

extern CStrId g_unk_71726dec08 __attribute__((visibility("hidden")));

// The EMMY's AI component (CEmmyAIComponent).
struct UnkEmmyAI_l511be4 : UnkReflObject {
    u8 pad_0x8[0x1260 - 0x8];
    CBlackboard blackboard;  // 0x1260
};
static_assert(__builtin_offsetof(UnkEmmyAI_l511be4, blackboard) == 0x1260, "layout");

void fn_710054b44c(UnkEmmyAI_l511be4* self);
void fn_7100546674(UnkEmmyAI_l511be4* self);

struct UnkChaseDelegate_l511be4 {
    UnkEmmyChaseAction_l511be4* object;
    void (*function)(UnkEmmyChaseAction_l511be4*);
};

// What the action registers with the animation component: a link and the callback.
struct UnkChaseListener_l511be4 {
    void* field_0x0;
    UnkChaseDelegate_l511be4 callback;  // 0x8
};

struct UnkEmmyMovement_l511be4 {
    u8 pad_0x0[0x19e0];
    UnkChaseDelegate_l511be4 callback;  // 0x19e0
};

// The tunable's switch for the doors' EMMY effect.
struct UnkEmmyDoorTunable_l511be4 {
    u8 pad_0x0[0x28];
    bool doors_fx;  // 0x28
};

struct UnkDoor_l511be4 {
    u8 pad_0x0[0x10];
    UnkReflObject* fx;  // 0x10
};

struct UnkDoorList_l511be4 {
    UnkDoor_l511be4** items;
    u32 count;
};

struct UnkChaseOwner_l511be4 {
    u8 pad_0x0[0x1f0];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x220 - 0x1f8];
    UnkWeakTarget* animation;  // 0x220
    u8 pad_0x228[0x240 - 0x228];
    UnkEmmyAI_l511be4* ai;  // 0x240
};

struct UnkEmmyChaseAction_l511be4 {
    u8 pad_0x0[0x18];
    UnkChaseOwner_l511be4* owner;  // 0x18
    u8 pad_0x20[0x2c - 0x20];
    f32 field_0x2c;
    u8 pad_0x30[0x4];
    f32 field_0x34;
    f32 field_0x38;
    u8 pad_0x3c[0x44 - 0x3c];
    f32 field_0x44;
    f32 speed;  // 0x48
    f32 field_0x4c;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5c;
    UnkChaseListener_l511be4 listener;  // 0x60
    UnkWeakHandle* animation;           // 0x78
    bool field_0x80;
    u8 pad_0x81[0x3];
    u32 field_0x84;
    u32 field_0x88;
    u32 field_0x8c;
    u32 field_0x90;
    u8 pad_0x94[0x98 - 0x94];
    UnkChaseDelegate_l511be4 movement_callback;  // 0x98
};
static_assert(__builtin_offsetof(UnkEmmyChaseAction_l511be4, listener) == 0x60, "layout");
static_assert(__builtin_offsetof(UnkEmmyChaseAction_l511be4, movement_callback) == 0x98, "layout");

// A string with 0x100 bytes of inline storage.
struct UnkInlineString_l511be4 {
    u8 str[0x30];
    char* storage_vtable;  // 0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkInlineString_l511be4() {
        CRntString_InitWithAllocator(Str(), reinterpret_cast<base::IAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
    }
    ~UnkInlineString_l511be4() {
        CRntString_Clear(Str());
        CRntString_Destroy(Str());
    }
};

void fn_7100511be4(UnkEmmyChaseAction_l511be4* self) {
    fn_710051dae0(reinterpret_cast<UnkPatrolBase_ovb0118*>(self));
    UnkEmmyAI_l511be4* ai = self->owner->ai;
    if (ai)
        ai = fn_710009d50c(ai->vf_0x0(), fn_710053b934()) ? ai : nullptr;

    self->field_0x80 = false;
    self->field_0x5c = 0.0f;
    self->field_0x2c = 0.0f;
    self->field_0x84 = 0;
    self->field_0x88 = 0;
    self->field_0x8c = 0;
    self->field_0x90 = 0;
    self->field_0x34 = 0.75f;
    self->field_0x38 = 0.0f;
    fn_7100511f04(reinterpret_cast<UnkEmmyChaseAction_l512b94*>(self), 0, true, 0.0f);
    self->field_0x44 = 5.0f / 6.0f;

    if (!fn_710055b574(reinterpret_cast<UnkEmmyAILists_b46a1*>(ai), &self->speed))
        self->speed = 0.0f;
    self->field_0x4c = self->speed;
    self->field_0x50 = 0.0f;
    UnkChaseOwner_l511be4* owner = self->owner;
    self->field_0x54 = 0.0f;
    self->field_0x58 = 0.0f;
    UnkAnimationComponent* animation = reinterpret_cast<UnkAnimationComponent*>(owner->animation);
    unk_prop_bag::fn_710033187c(reinterpret_cast<unk_prop_bag::UnkPropBag*>(animation->field_0x80),
                                g_unk_71726dec08, self->speed);

    UnkInlineString_l511be4 chase_id;
    if (CBlackboard::CSection* section = fn_7101047270(&ai->blackboard, g_unk_71726b7cb8))
        fn_7100395c38(section, &g_unk_71726debd8, chase_id.Str());
    fn_710054b44c(ai);
    const char* id = chase_id.Str()->data;
    if (!id || !*id)
        fn_7100546654(reinterpret_cast<UnkEmmyAI3_ovb0001*>(ai));
    fn_7100546674(ai);
    fn_710051270c(reinterpret_cast<UnkEmmyChaseAction3_ovb0001*>(self), 0.0f);

    if (reinterpret_cast<UnkEmmyDoorTunable_l511be4*>(&g_unk_71726e0aa8)->doors_fx) {
        UnkDoorList_l511be4* doors = reinterpret_cast<UnkDoorList_l511be4*>(fn_71007b5d90());
        for (u64 i = 0; i < doors->count; ++i) {
            UnkDoor_l511be4* door = doors->items[i];
            if (!door)
                continue;
            UnkReflObject* fx = door->fx;
            if (fx && fn_710009d50c(fx->vf_0x0(), fn_71007b55e4()))
                CDoorEmmyFXComponent_SetEmmyChaseKey(reinterpret_cast<UnkDoorEmmyFX_b50*>(fx), true);
        }
    }

    UnkChaseListener_l511be4* listener = &self->listener;
    listener->callback = {self, fn_7100514074};
    UnkWeakTarget* target = self->owner->animation;
    if (UnkWeakHandle* handle = self->animation) {
        if (UnkWeakTarget* old = handle->target)
            fn_710117b3f0(old, listener);
        ResetRef(&self->animation);
    }
    if (target) {
        CAbilityGhostAura_AssignWeakHandleToRef(target, &self->animation);
        fn_710117b3e8(self->animation ? self->animation->target : nullptr, listener);
    }

    UnkReflObject* movement = self->owner->movement;
    if (movement)
        movement = fn_710009d50c(movement->vf_0x0(), fn_71010c74e0()) ? movement : nullptr;
    self->movement_callback = {self, fn_710051408c};
    // Faithful to the original: the callback is stored without checking that the cast succeeded.
    reinterpret_cast<UnkEmmyMovement_l511be4*>(movement)->callback = self->movement_callback;
    fn_71005553a8(reinterpret_cast<char*>(ai), true);
}

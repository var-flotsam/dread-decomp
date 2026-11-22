// Unit 2726 (gui/scripts/msemenu_ingame.lua): puts the two actors the game manager keeps at +0x29a8 and
// +0x29b0 where the player stands, facing the player's side ("left" or "right"), dressed in the player's
// suit, and fades the environment back to white. Names and layouts are placeholders.
#include "../../game_s7/UnkPropBagS7.h"
#include "base/color/CColor4F.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "base/math/CVector4D.h"
#include "base/reflection.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU341.h"
#include "game/UnkTU57.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CRntString;
using base::global::CStrId;
using base::math::UnkVector4D;
using base::reflection::UnkReflTypeId;
using base::reflection::UnkReflTypeIdArg;
using base::reflection::UnkReflTypeIdTag;
using unk_prop_bag::Key;

struct UnkEnvironmentFade;
struct UnkTimelineComponentFull_ovb0089;
void fn_7100f97f64(f32 time, UnkEnvironmentFade* self, const UnkVector4D* to);
void fn_7100a2bc3c(UnkTimelineComponentFull_ovb0089* self);

struct UnkIngameMenu_l18c7a0 {
    u8 pad_0x0[0x260];
    u32 field_0x260;
    f32 field_0x264;
};

// One of the two actors placed where the player stands.
struct UnkPlacedActor_l18c7a0 {
    struct VTable {
        void* slots[26];
        void (*SetPosition)(UnkPlacedActor_l18c7a0* self, const UnkVec3f* pos, bool flag);  // 0xd0
        void (*SetAngle)(UnkPlacedActor_l18c7a0* self, const UnkVec3f* angle, bool flag);   // 0xd8
    };
    VTable* vtable;
    u8 pad_0x8[0x218];
    void* animation;  // 0x220
};

struct UnkGameManagerView_l18c7a0 {
    u8 pad_0x0[0x29a8];
    UnkWeakHandle* field_0x29a8;
    UnkWeakHandle* field_0x29b0;
};

struct UnkPlayerView_l18c7a0 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x17c];
    void* field_0x210;
    u8 pad_0x218[0x58];
    UnkInventory* inventory;  // 0x270
    u8 pad_0x278[0x70];
    UnkTimelineComponentFull_ovb0089* timeline;  // 0x2e8
};
static_assert(__builtin_offsetof(UnkPlayerView_l18c7a0, timeline) == 0x2e8, "layout");

struct UnkEnvironmentManager_l18c7a0 {
    u8 pad_0x0[0x28];
    u8 fade[0x4a0];  // an UnkEnvironmentFade (src/game/UnkEnvironmentFade.cpp)
};

static inline UnkPlacedActor_l18c7a0* Target_l18c7a0(UnkWeakHandle* handle) {
    return handle ? reinterpret_cast<UnkPlacedActor_l18c7a0*>(handle->target) : nullptr;
}

// The id's whole string, counting the read.
static inline const CRntString& StringOf_l18c7a0(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

// The item's qualified id, built on first use.
static inline const UnkQualifiedId& ItemId_l18c7a0(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// A string literal with its length.
struct UnkName_l18c7a0 {
    const char* text;
    u64 length;
    UnkName_l18c7a0() {}
    UnkName_l18c7a0(const char* str) : text(str), length(__builtin_strlen(str)) {}
};

// Tells the actor's timeline which suit it wears.
static inline void SetSuit_l18c7a0(UnkPlacedActor_l18c7a0* actor, const UnkName_l18c7a0& suit) {
    u64 key = fn_7100001570(suit.text, suit.length);
    fn_7100d7c828(reinterpret_cast<UnkActor*>(actor), &key, false);
}

// Fades the environment to `color` over `time`.
static inline void FadeTo_l18c7a0(UnkEnvironmentFade* fade, const CColor4F& color, f32 time) {
    UnkVector4D target = fn_7100030878(&color);
    fn_7100f97f64(time, fade, &target);
}

void fn_710118c7a0(UnkIngameMenu_l18c7a0* self) {
    self->field_0x260 = 4;
    self->field_0x264 = 0.2f;

    UnkGameManagerView_l18c7a0* manager = reinterpret_cast<UnkGameManagerView_l18c7a0*>(g_unk_7101d47310);
    UnkPlacedActor_l18c7a0* first = Target_l18c7a0(manager->field_0x29a8);
    UnkPlacedActor_l18c7a0* second = Target_l18c7a0(manager->field_0x29b0);
    bool raised = reinterpret_cast<UnkPlayerView_l18c7a0*>(fn_7100dec788(g_unk_7101d47310))->field_0x210 != nullptr;
    fn_7100df31b8(g_unk_7101d47310, true, true, 1, g_unk_7101d470e0, false);
    UnkPlayerView_l18c7a0* player = reinterpret_cast<UnkPlayerView_l18c7a0*>(fn_7100dec788(g_unk_7101d47310));

    UnkVec3f at = player->xform.GetTranslation();
    UnkVec3f pos = raised ? UnkVec3f(at.x + 0.0f, at.y + -110.0f, at.z + 0.0f) : at;
    first->vtable->SetPosition(first, &pos, true);
    second->vtable->SetPosition(second, &pos, true);
    UnkVec3f angle = player->xform.GetAngle();
    first->vtable->SetAngle(first, &angle, true);
    second->vtable->SetAngle(second, &angle, true);

    f32 facing = player->xform.GetMatrix().m[2];
    CRntString side(StringOf_l18c7a0(Key(0xd78)));
    CRntString_AppendCString(&side, facing > 0.0f ? "right" : "left");

    void* first_animation = first->animation;
    u64 first_key;
    fn_7100066630(&first_key, &side);
    UnkReflTypeIdArg first_mode;
    first_mode.id = 1;
    UnkReflTypeIdArg first_type;
    first_type.id = UnkReflTypeId<UnkReflTypeIdTag>::id();
    fn_7101176690(first_animation, first_key, 0xf4061ff25a2163d6, &first_mode, &g_unk_710164b968,
                  &g_unk_7172769740, &first_type);
    void* second_animation = second->animation;
    u64 second_key;
    fn_7100066630(&second_key, &side);
    UnkReflTypeIdArg second_mode;
    second_mode.id = 1;
    UnkReflTypeIdArg second_type;
    second_type.id = UnkReflTypeId<UnkReflTypeIdTag>::id();
    fn_7101176690(second_animation, second_key, 0xf4061ff25a2163d6, &second_mode, &g_unk_710164b968,
                  &g_unk_7172769740, &second_type);

    fn_71002334fc(reinterpret_cast<UnkStruct_71002334fc*>(player), false);
    if (player->timeline)
        fn_7100a2bc3c(player->timeline);
    if (UnkInventory* inventory = player->inventory) {
        UnkName_l18c7a0 suit;
        if (fn_71011428bc(inventory, ItemId_l18c7a0(g_unk_717275a418)))
            suit = "hypersuit";
        else if (fn_71011428bc(inventory, ItemId_l18c7a0(g_unk_717275a458)))
            suit = "gravitysuit";
        else if (fn_71011428bc(inventory, ItemId_l18c7a0(g_unk_717275a498)))
            suit = "variasuit";
        else
            suit = "powersuit";
        SetSuit_l18c7a0(first, suit);
        SetSuit_l18c7a0(second, suit);
    }
    fn_7100232864(reinterpret_cast<UnkEntityState*>(first), true);
    fn_7100232864(reinterpret_cast<UnkEntityState*>(second), true);

    void* environment = g_unk_7101d47368;
    if (!environment) {
        environment = fn_71000839a0(0x4c8, 0x3f);
        fn_7100d8ced8(environment);
        g_unk_7101d47368 = environment;
    }
    FadeTo_l18c7a0(reinterpret_cast<UnkEnvironmentFade*>(static_cast<UnkEnvironmentManager_l18c7a0*>(environment)->fade),
                   CColor4F(1.0f, 1.0f, 1.0f, 1.0f), 0.0f);
}

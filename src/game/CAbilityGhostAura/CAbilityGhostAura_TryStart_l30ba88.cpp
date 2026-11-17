// CAbilityGhostAura's unit (tools/tu_map.py, unit 430): tries to start the aura. Refreshes the cached values from
// the owner, then, when the owner has a movement, no "ignore ghost aura" track and the ability is idle, starts it
// (state 2) facing the owner's direction if the gravity suit is owned or the game forces it; otherwise arms the
// retry delay. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAbilityGhostAura.h"
#include "game/CInventoryComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2553.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkPart;
struct CAbilityComponent;
struct UnkMovement2_ovb0048;
struct UnkTunable;

// As src/game/UnkStateMachineUpdateA2_b63.cpp declares it.
void* fn_7100300bd0(void*, u32);
// As src/game/CMovementComponent/CMovementComponent_Motion_ovb0048.cpp defines it.
UnkVec3f fn_71010d2b08(UnkMovement2_ovb0048* self);

// CAbilityGhostAura::CTunableAbilityGhostAura (src/reflection/CAbilityGhostAura.cpp).
extern UnkTunable g_unk_71726af3a0;

struct UnkGhostAuraTunables_l30ba88 {
    u8 pad_0x0[0x94];
    f32 field_0x94;
    f32 retry_delay;  // 0x98
};

static inline const UnkGhostAuraTunables_l30ba88& Tunables() {
    return reinterpret_cast<const UnkGhostAuraTunables_l30ba88&>(g_unk_71726af3a0);
}

struct UnkGhostAuraSource_l30ba88 {
    u8 pad_0x0[0x9ec];
    f32 x;  // 0x9ec
    f32 y;
    f32 z;
};

struct UnkGhostAuraActor_l30ba88 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x160];
    UnkGhostAuraSource_l30ba88* source;  // 0x1d8
    u8 pad_0x1e0[0x28];
    void* movement;  // 0x208
    u8 pad_0x210[0x10];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkGhostAuraMeter_l30ba88 {
    u8 pad_0x0[0x40];
    f32 max;    // 0x40
    f32 value;  // 0x44
};

struct UnkGhostAuraMeterHolder_l30ba88 {
    u8 pad_0x0[0x10];
    UnkGhostAuraMeter_l30ba88* meter;  // 0x10
};

struct UnkGhostAuraLink_l30ba88 {
    u8 pad_0x0[0x30];
    UnkGhostAuraActor_l30ba88* actor;  // 0x30
    u8 pad_0x38[0xb0];
    UnkGhostAuraMeterHolder_l30ba88* meters;  // 0xe8
};

struct UnkIgnoreTrack_l30ba88 {
    u8 pad_0x0[0x18];
    bool ignore;  // 0x18
};

struct UnkSuitPlayer_l30ba88 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;  // 0x270
};

struct UnkGhostAura_l30ba88 {
    u8 pad_0x0[0xc0];
    UnkGhostAuraLink_l30ba88* link;  // 0xc0
    u8 pad_0xc8[0x328];
    UnkVec3f dir;  // 0x3f0
    u8 pad_0x3fc[0x30];
    f32 source_x;  // 0x42c
    f32 source_y;
    f32 source_z;
    u8 pad_0x438[0x10];
    s32 state;  // 0x448
    s32 field_0x44c;
    u32 start_arg;  // 0x450
    u8 pad_0x454[0x24];
    f32 field_0x478;
    u8 pad_0x47c[0x8];
    f32 retry_timer;  // 0x484
    u8 pad_0x488[0x13];
    u8 field_0x49b;
};
static_assert(__builtin_offsetof(UnkGhostAura_l30ba88, field_0x49b) == 0x49b, "layout");

void fn_710030a8fc(UnkGhostAura_l30ba88* self);

// An item's qualified id, built on first use.
static inline const UnkQualifiedId& ItemId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

void fn_710030ba88(UnkGhostAura_l30ba88* self, u32 arg) {
    UnkGhostAuraLink_l30ba88* link = self->link;
    UnkGhostAuraActor_l30ba88* actor = link->actor;
    if (!actor)
        return;
    void* movement = actor->movement;
    UnkGhostAuraMeter_l30ba88* meter = link->meters ? link->meters->meter : nullptr;
    UnkAnimationComponent* animation = actor->animation;
    if (UnkGhostAuraSource_l30ba88* source = actor->source) {
        self->source_x = source->x;
        self->source_y = source->y;
        self->source_z = source->z;
    }
    bool not_full = meter ? !(meter->value >= meter->max) : true;
    UnkTrackQuery query = {fn_7100d0b788(), nullptr, -1.0f};
    fn_710117b08c(animation, &query);
    bool ignored = query.result ? reinterpret_cast<UnkIgnoreTrack_l30ba88*>(query.result)->ignore : false;
    self->field_0x49b = self->field_0x49b ? not_full : 0;
    if (!movement || ignored || self->state != 0)
        return;

    UnkGameManager* game = g_unk_7101d47310;
    bool forced = game->field_0xc8 == 0 && !game->field_0x10c;
    bool has_suit = false;
    auto* player = reinterpret_cast<UnkSuitPlayer_l30ba88*>(fn_7100dec788(game));
    if (player && player->inventory)
        has_suit = fn_71011428bc(player->inventory, ItemId(g_unk_717275a458));  // ITEM_GRAVITY_SUIT
    if (forced || has_suit) {
        fn_71009eaa88(movement);
        const UnkMatrix44& m = fn_710002614c(&actor->transform);
        self->dir = UnkVec3f(m.m[2], m.m[6], m.m[10]);
        if (fn_71009e12d0(movement))
            self->dir = fn_71010d2b08(static_cast<UnkMovement2_ovb0048*>(movement));
        self->start_arg = arg;
        fn_7100309fd8(reinterpret_cast<UnkGhostAura_A2_b70*>(self), 2, 1);
        self->field_0x478 = Tunables().field_0x94;
        if (self->field_0x478 <= 0.0f)
            fn_710030a8fc(self);
    } else if (self->retry_timer <= 0.0f && self->state != 1) {
        self->retry_timer = Tunables().retry_delay;
        fn_7100300bd0(self->link, 0);
        self->field_0x44c = 1;
    }
}

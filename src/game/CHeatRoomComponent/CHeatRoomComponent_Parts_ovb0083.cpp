// CHeatRoomComponent's unit (tools/tu_map.py unit 1538): the passes that hand each of the heat
// room's parts' effect objects a value (vtable slots 0x168 and 0x170), whether the room hurts the
// player (not with the varia or gravity suit), the environment switch when an actor enters or
// leaves, and the destructor. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/snd/CSoundSystem.h"
#include "game/CActorComponent.h"
#include "game/CCharClassBaseDamageTriggerComponent.h"
#include "game/CHeatRoomComponent.h"
#include "game/CInventoryComponent.h"
#include "game/CPlayerInfo.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU0.h"
#include "game/UnkTU23.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

struct UnkHeatPair_ovb0083 {
    f32 x;
    f32 y;
};

struct UnkHeatParams_ovb0083 {
    f32 field_0x0;
    f32 field_0x4;
    UnkHeatPair_ovb0083 field_0x8;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    bool field_0x1c;
};

struct UnkHeatCurrent_ovb0083 {
    f32 field_0x0;
    f32 field_0x4;
    UnkHeatPair_ovb0083 field_0x8;
    f32 field_0x10;
    f32 field_0x14;
};

struct UnkBreakablePart;
struct UnkBreakableTarget {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkHeatOwner_ovb0083 {
    u8 pad_0x0[0x28];
    base::reflection::UnkReflObject* char_class;  // 0x28
    u8 field_0x30[8];
};

struct UnkHeatRoom_ovb0083 {
    u8 pad_0x0[0x30];
    UnkHeatOwner_ovb0083* owner;  // 0x30
    u8 pad_0x38[0x44];
    f32 field_0x7c;
    u8 pad_0x80[0x938];
    u32 field_0x9b8;
    u8 pad_0x9bc[0x204];
    UnkHeatCurrent_ovb0083 current;  // 0xbc0
    u8 pad_0xbd8[0xc];
    UnkHeatParams_ovb0083 normal;  // 0xbe4
    UnkHeatParams_ovb0083 alt;     // 0xc04
    u32 use_alt;                   // 0xc24
    u8* parts;                     // 0xc28, 0x40-byte parts
    u32 part_count;                // 0xc30
    u8 pad_0xc34[0x1c];
    u8 sound_path[0x8];  // 0xc50
    CStrId environment;           // 0xc58
    CStrId previous_environment;  // 0xc60
    bool sets_environment;        // 0xc68
    u8 pad_0xc69[0x17];
    bool normal_was_set;  // 0xc80
    bool alt_was_set;     // 0xc81
};
static_assert(__builtin_offsetof(UnkHeatRoom_ovb0083, sound_path) == 0xc50, "UnkHeatRoom_ovb0083 layout");

struct UnkHeatRoom_b49;
void fn_7100a45cec(UnkHeatRoom_ovb0083* self);
struct UnkSoundMgr;
struct UnkPathNode;
struct UnkStruct_710024fa0c;
void* fn_710082e510(void* key);
struct UnkPlayer;
struct UnkInventory;

struct UnkHeatEffectVtable_ovb0083 {
    u8 pad_0x0[0x168];
    void (*set_0x168)(void* self, f32 value);
    void (*set_0x170)(void* self, f32 value);
};

static inline void Refresh(UnkHeatRoom_ovb0083* self) {
    fn_7100a26828(reinterpret_cast<UnkHeatRoom_b49*>(self));
}

// Hands `value` to slot `Slot` of every part's effect object.
template <int Slot>
static inline void SetOnParts(UnkHeatRoom_ovb0083* self, f32 value) {
    for (u8 *it = self->parts, *end = it + (u64)self->part_count * 0x40; it != end; it += 0x40) {
        UnkBreakablePart* part = reinterpret_cast<UnkBreakablePart*>(it);
        UnkBreakableTarget* target = *fn_71002a76a4(part);
        if (!target || !target->field_0x10)
            continue;
        target = *fn_71002a76a4(part);
        if (void* effect = fn_710082e510(target ? target->field_0x10 : nullptr)) {
            auto* vtable = *reinterpret_cast<UnkHeatEffectVtable_ovb0083**>(effect);
            if (Slot == 0x168)
                vtable->set_0x168(effect, value);
            else
                vtable->set_0x170(effect, value);
        }
    }
}

void fn_7100a27cac(UnkHeatRoom_ovb0083* self) {
    SetOnParts<0x168>(self, 1.0f);
    Refresh(self);
}

void fn_7100a27bdc(UnkHeatRoom_ovb0083* self, f32 value) {
    if (value <= 0.0f) {
        fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(self), false);
        Refresh(self);
    }
    SetOnParts<0x170>(self, value);
}

void fn_7100a27b08(UnkHeatRoom_ovb0083* self, f32 value) {
    if (value >= 1.0f) {
        fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(self), true);
        Refresh(self);
    }
    SetOnParts<0x168>(self, value);
}

void fn_7100a270f4(UnkHeatRoom_ovb0083* self) {
    fn_7100108094(g_unk_7101d4d820, reinterpret_cast<UnkPathNode*>(self->sound_path));
    Refresh(self);
    if (self->field_0x7c >= 1.0f)
        SetOnParts<0x168>(self, 1.0f);
    fn_7100a45cec(self);
}

struct UnkSuitPlayer_ovb0083 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;  // 0x270
};

// An item's qualified id, built on first use.
static inline const UnkQualifiedId& ItemId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// Whether the room hurts: never with the varia or gravity suit, else while the normal set is in use.
bool fn_7100a279fc(UnkHeatRoom_ovb0083* self) {
    auto* player = reinterpret_cast<UnkSuitPlayer_ovb0083*>(fn_7100dec788(g_unk_7101d47310));
    if (player && player->inventory) {
        UnkInventory* inventory = player->inventory;
        if (fn_71011428bc(inventory, ItemId(g_unk_717275a498)) ||  // ITEM_VARIA_SUIT
            fn_71011428bc(inventory, ItemId(g_unk_717275a458)))    // ITEM_GRAVITY_SUIT
            return false;
    }
    return self->use_alt == 0;
}

struct UnkEnvironmentManager;
struct UnkHeatActor_ovb0083;
void fn_7100a45f04(UnkHeatRoom_ovb0083* self, UnkHeatActor_ovb0083* actor);

struct UnkHeatActorState_ovb0083 {
    u8 pad_0x0[0x70];
    s32 field_0x70;
};

struct UnkHeatActor_ovb0083 {
    u8 pad_0x0[0x30];
    u8 field_0x30[0x188];
    UnkHeatActorState_ovb0083* state;  // 0x1b8
};

// The environment manager, created on first use.
static inline UnkEnvironmentManager* EnvironmentManager() {
    if (!g_unk_7101d47368) {
        void* m = fn_71000839a0(0x4c8, 0x3f);
        fn_7100d8ced8(m);
        g_unk_7101d47368 = m;
    }
    return static_cast<UnkEnvironmentManager*>(g_unk_7101d47368);
}

// When the room is cold and set to change the environment, an actor (whose state at +0x1b8 is not
// 2) switches the environment manager to the room's (keeping the current one at +0xc60); then the
// base handler runs.
void fn_7100a27718(UnkHeatRoom_ovb0083* self, UnkHeatActor_ovb0083* actor) {
    if (self->current.field_0x0 <= 0.0f && self->use_alt == 0 && self->sets_environment) {
        if (fn_7100d8efc8(EnvironmentManager())->data != self->environment.data && actor->state &&
            actor->state->field_0x70 != 2) {
            self->previous_environment = *fn_7100d8efc8(EnvironmentManager());
            fn_7100d8e32c(EnvironmentManager(), &self->environment, false, false);
        }
    }
    fn_7100a45f04(self, actor);
}

struct UnkLiquidPoolEnter_ovb0075;
struct UnkLiquidPoolActor_ovb0075;

// The base handler first; then, when the room is hot (+0x9b8 clear) and set to change the
// environment, an actor (state at +0x1b8 not 2) switches the manager back to the kept environment
// while the room's is current; a tracked actor (fn_7100df5c38) is also handed to fn_7100f0e98c
// with the owner's character class.
void fn_7100a27884(UnkHeatRoom_ovb0083* self, UnkHeatActor_ovb0083* actor, const UnkVec3f* pos) {
    fn_7100a45f08(reinterpret_cast<UnkLiquidPoolEnter_ovb0075*>(self),
                  reinterpret_cast<UnkLiquidPoolActor_ovb0075*>(actor), pos);
    if (self->field_0x9b8 == 0 && self->sets_environment &&
        self->previous_environment.data != &g_unk_7101d47108->empty && actor->state &&
        actor->state->field_0x70 != 2) {
        if (fn_7100d8efc8(EnvironmentManager())->data == self->environment.data)
            fn_7100d8e32c(EnvironmentManager(), &self->previous_environment, false, false);
    }
    if (!fn_7100df5c38(g_unk_7101d47310, actor))
        return;
    u8* manager_0x38 = reinterpret_cast<u8*>(g_unk_7101d47310) + 0x38;
    base::reflection::UnkReflObject* c = self->owner->char_class;
    c = c && fn_710009d50c(c->vf_0x0(), fn_71011abbd4()) ? c : nullptr;
    fn_7100f0e98c(manager_0x38, reinterpret_cast<u8*>(c) + 0x98, self->owner->field_0x30, actor->field_0x30);
}

// The part list (+0xc28) and the three ids (+0xc50 on the file-path pool, +0xc58, +0xc60) as the
// destructor sees them.
struct UnkHeatPart_ovb0083 {
    virtual ~UnkHeatPart_ovb0083();
    u8 pad_0x8[0x38];
};

struct UnkHeatRoomMembers_ovb0083 {
    char* vtable;
    u8 pad_0x8[0xc20];
    base::UnkVector<UnkHeatPart_ovb0083> parts;  // 0xc28
    base::UnkStrIdMemberOf<g_unk_7101d47200> sound_path;  // 0xc50
    base::UnkStrIdMember environment;  // 0xc58
    base::UnkStrIdMember previous_environment;  // 0xc60
};
static_assert(__builtin_offsetof(UnkHeatRoomMembers_ovb0083, sound_path) == 0xc50, "UnkHeatRoomMembers_ovb0083 layout");

extern char g_unk_7101a8d620[];
struct UnkLiquidPoolDtor_ovb0075;

// The destructor: the ids, then the parts, then the base's (fn_7100a45a84).
__attribute__((flatten)) void fn_7100a26518(UnkHeatRoomMembers_ovb0083* self) {
    self->vtable = g_unk_7101a8d620 + 0x10;
    self->previous_environment.~UnkStrIdMemberOf();
    self->environment.~UnkStrIdMemberOf();
    self->sound_path.~UnkStrIdMemberOf();
    self->parts.Free();
    fn_7100a45a84(reinterpret_cast<UnkLiquidPoolDtor_ovb0075*>(self));
}

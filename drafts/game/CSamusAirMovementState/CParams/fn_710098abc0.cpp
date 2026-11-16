// CSamusAirMovementState::CParams's unit (tools/tu_map.py unit 1455): whether the air state may hand over
// to the magnet glove: the inventory must hold ITEM_MAGNET_GLOVE, the state must not be 13 and the
// ability (fn_7100df554c) must not be in states 3 to 6 unless forced; then the glove state's own checks
// decide (fn_71009a64cc, or fn_71009a6310 with the owner's two magnet parameters). Names and layouts are
// placeholders.
#include "base/math/CVector.h"
#include "game/CAbilityGhostAura.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkInventory;
struct UnkAirMoveState_B71a6;
struct UnkAirWords_B71a6;
struct UnkAirRequest_B71a6;
struct UnkMagnetCheckState_ovb0082;
struct UnkMagnetRequest_ovb0082;

bool fn_71009a64cc(UnkAirMoveState_B71a6* state, const UnkAirWords_B71a6* words, s32 a, UnkAirRequest_B71a6* req,
                   s32 b);
bool fn_71009a6310(UnkMagnetCheckState_ovb0082* self, f32 x, f32 y, f32 speed, const UnkAirWords_B71a6* words,
                   UnkMagnetRequest_ovb0082* req);
u32 fn_71009a60fc();

struct UnkAirModeState_l98abc0 {
    u8 pad_0x0[0x10];
    u32 kind;  // 0x10
};

#pragma pack(push, 4)
struct UnkAirMovement_l98abc0 {
    u8 pad_0x0[0x2d18];
    u32 state_count;                       // 0x2d18
    UnkAirModeState_l98abc0* states[1];  // 0x2d1c, 4-byte aligned
};
#pragma pack(pop)

struct UnkMagnetParams_l98abc0 {
    u8 pad_0x0[0x9ec];
    base::math::CVector2D stick;  // 0x9ec
};

struct UnkAirOwner_l98abc0 {
    u8 pad_0x0[0x1d8];
    UnkMagnetParams_l98abc0* params;  // 0x1d8
    u8 pad_0x1e0[0x90];
    UnkInventory* inventory;  // 0x270
};

struct UnkAirParams_l98abc0 {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
    u8 pad_0x14[0x1c];
    UnkAirOwner_l98abc0* owner;        // 0x30
    UnkAirMovement_l98abc0* movement;  // 0x38
};

static inline const UnkQualifiedId& GetQualifiedId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// The movement's state of this kind, if it has one.
static inline UnkAirModeState_l98abc0* FindState(UnkAirMovement_l98abc0* movement, u32 kind) {
    UnkAirModeState_l98abc0** end = movement->states + movement->state_count;
    for (UnkAirModeState_l98abc0** it = movement->states; it != end; ++it) {
        if ((*it)->kind == kind)
            return *it;
    }
    return nullptr;
}

// Whether the ability (fn_7100df554c) is in one of its states 3 to 6, unless the hand-over is forced.
static inline bool AbilityBusy(bool forced) {
    UnkAbility* ability = fn_7100df554c(g_unk_7101d47310);
    if (!ability || forced)
        return false;
    u32 mode = fn_7100309568(ability);
    return mode >= 3 && mode <= 6;
}

static inline bool IsNearZero(f32 v) {
    return v + 0.001f >= 0.0f && v - 0.001f <= 0.0f;
}

bool fn_710098abc0(UnkAirParams_l98abc0* self, const UnkAirWords_B71a6* words, UnkAirRequest_B71a6* req, bool direct,
                   bool forced) {
    UnkInventory* inventory = self->owner->inventory;
    if (fn_7101142928(inventory, GetQualifiedId(g_unk_717275a9d8)) == 0.0f || self->state == 13 ||
        AbilityBusy(forced))
        return false;
    if (direct) {
        UnkAirMovement_l98abc0* movement = self->movement;
        u32 kind = fn_71009a60fc();
        UnkAirModeState_l98abc0* state = kind <= 14 ? FindState(movement, kind) : nullptr;
        return fn_71009a64cc(reinterpret_cast<UnkAirMoveState_B71a6*>(state), words, 1, req, 0);
    }
    base::math::CVector2D stick = self->owner->params->stick;
    f32 speed = IsNearZero(stick.x) && IsNearZero(stick.y) ? 0.0f : 1.0f;
    UnkAirMovement_l98abc0* movement = self->movement;
    u32 kind = fn_71009a60fc();
    UnkAirModeState_l98abc0* state = kind <= 14 ? FindState(movement, kind) : nullptr;
    return fn_71009a6310(reinterpret_cast<UnkMagnetCheckState_ovb0082*>(state), stick.x, stick.y, speed, words,
                         reinterpret_cast<UnkMagnetRequest_ovb0082*>(req));
}

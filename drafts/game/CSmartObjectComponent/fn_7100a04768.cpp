// CSmartObjectComponent unit: whether the player stands on the smart object: its bounds (the collider under
// the id table's name, widened by every "smartobject_fake" collider) must hold the player's x, widened by
// most of the player's half width, and the player's y must be within 10 of its top raised by the player's
// half height. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharacterMovement.h"
#include "game/CSmartObjectComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU2553.h"
#include "game/UnkVec3Bits.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkStruct_71008651c8_Entry;
struct UnkStruct_71008651c8;
using UnkOut_71008651c8 = base::global::CRntVector<UnkStruct_71008651c8_Entry*>;
void CCollisionComponent_CollectRecordsBySecondaryId(UnkStruct_71008651c8*, void* const*, UnkOut_71008651c8*);

// A collider with a 2D box whose bounds vf_0x98 recomputes while they are out of date.
struct UnkSmartCollider_la04768 : UnkCollisionObject {
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();

    u8 pad_0x8[0x38];
    f32 min_x;  // 0x40
    f32 min_y;  // 0x44
    f32 max_x;  // 0x48
    f32 max_y;  // 0x4c
    u8 pad_0x50[0x10];
    bool dirty;  // 0x60

    void UpdateBounds() {
        if (dirty)
            vf_0x98();
    }
};
static_assert(__builtin_offsetof(UnkSmartCollider_la04768, dirty) == 0x60, "UnkSmartCollider_la04768 layout");

struct UnkShapeBounds_la04768 {
    f32 min_x;
    f32 min_y;
    f32 min_z;
    f32 max_x;
    f32 max_y;
    f32 max_z;
};

// The movement's shape: vf_0x28 gives the bounds of its +0x8.
struct UnkMoveShape_la04768 {
    void** vtable;
    u8 field_0x8[1];

    UnkShapeBounds_la04768 GetBounds() {
        return reinterpret_cast<UnkShapeBounds_la04768 (*)(UnkMoveShape_la04768*, void*)>(vtable[0x28 / 8])(
            this, field_0x8);
    }
};

struct UnkMoveBody_la04768 {
    u8 pad_0x0[0x158];
    UnkMoveShape_la04768* shape;  // 0x158
};

struct UnkSmartMovement_la04768 : UnkReflObject {
    u8 pad_0x8[0x3b0];
    UnkMoveBody_la04768* body;          // 0x3b8
    UnkMoveBody_la04768* body_override; // 0x3c0

    UnkMoveBody_la04768* Body() { return body_override ? body_override : body; }
};
static_assert(__builtin_offsetof(UnkSmartMovement_la04768, body_override) == 0x3c0, "layout");

struct UnkSmartPlayer_la04768 {
    u8 pad_0x0[0x1f0];
    UnkReflObject* movement;  // 0x1f0
};

struct UnkSmartOwner_la04768 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkSmartObject_la04768 {
    u8 pad_0x0[0x30];
    UnkSmartOwner_la04768* owner;  // 0x30
};

bool fn_7100a04768(UnkPlatformSm_A5_b72* component) {
    auto* self = reinterpret_cast<UnkSmartObject_la04768*>(component);
    auto* player = reinterpret_cast<UnkSmartPlayer_la04768*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return false;

    UnkReflObject* object = player->movement;
    auto* movement = object && fn_710009d50c(object->vf_0x0(), fn_7100975b3c())
                         ? static_cast<UnkSmartMovement_la04768*>(object)
                         : nullptr;

    auto* collider = static_cast<UnkSmartCollider_la04768*>(
        fn_7100864194(self->owner->collision, &GameManagerIdTable().field_0x20f0));
    collider->UpdateBounds();
    f32 min_x = collider->min_x;
    f32 min_y = collider->min_y;
    f32 max_x = collider->max_x;
    f32 max_y = collider->max_y;

    UnkOut_71008651c8 fakes(0xf);
    static CStrId s_fake("smartobject_fake", true);
    CCollisionComponent_CollectRecordsBySecondaryId(reinterpret_cast<UnkStruct_71008651c8*>(self->owner->collision),
                  reinterpret_cast<void* const*>(&s_fake), &fakes);
    for (UnkStruct_71008651c8_Entry **it = fakes.data, **end = fakes.data + fakes.count; it != end; ++it) {
        auto* fake =
            static_cast<UnkSmartCollider_la04768*>(reinterpret_cast<UnkCollisionRecord*>(*it)->field_0x28);
        if (!fake)
            continue;
        fake->UpdateBounds();
        min_x = min_x < fake->min_x ? min_x : fake->min_x;
        min_y = min_y < fake->min_y ? min_y : fake->min_y;
        max_x = max_x > fake->max_x ? max_x : fake->max_x;
        max_y = max_y > fake->max_y ? max_y : fake->max_y;
    }

    f32 x = fn_7100979a60(reinterpret_cast<UnkPlace*>(movement)).x;
    f32 half_width;
    f32 half_height;
    if (movement) {
        UnkShapeBounds_la04768 bounds = movement->Body()->shape->GetBounds();
        f32 height = bounds.max_y - bounds.min_y;
        f32 width = bounds.max_x - bounds.min_x;
        half_height = height * 0.5f;
        half_width = width * 0.5f * 0.9f;
    } else {
        half_width = 0.0f;
        half_height = 0.0f;
    }

    bool on;
    if (x >= min_x - half_width && x <= max_x + half_width) {
        f32 y = fn_7100979a60(reinterpret_cast<UnkPlace*>(movement)).y;
        f32 center_y = (min_y + max_y) * 0.5f;
        f32 extent_y = (max_y - min_y) * 0.5f;
        f32 top = extent_y + (center_y + half_height);
        on = y - 10.0f <= top && y + 10.0f >= top;
    } else {
        on = false;
    }
    fakes.Free();
    return on;
}

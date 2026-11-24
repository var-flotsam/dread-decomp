// CSamusFloorSlideMovementState (unit 1459): whether there is room for the slide's collision body (kind 8)
// at `pos` (or the owner's position), its capsule shrunk slightly, against the collision tree.
// Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU39.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CCapsuleShape2D.h"
#include "types.h"

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

void fn_7100d60184(void* shape, f32 scale);
void fn_7100d6c334(void* shape, const UnkVec3f* offset);

// The slide's collision body: its kind (slot 0x18), its offset (slot 0x68) and its capsule dimensions.
struct UnkRoomBody_l994570 {
    void** vtable;
    u8 pad_0x8[0xa0];
    f32 field_0xa8;
    f32 field_0xac;
    f32 field_0xb0;
    u32 field_0xb4;

    s32 GetKind() { return reinterpret_cast<s32 (*)(UnkRoomBody_l994570*)>(vtable[0x18 / 8])(this); }
    UnkVec3f GetOffset() {
        return reinterpret_cast<UnkVec3f (*)(UnkRoomBody_l994570*)>(vtable[0x68 / 8])(this);
    }
};

struct UnkRoomCollisionObject_l994570 {
    u8 pad_0x0[0x158];
    UnkRoomBody_l994570* body;
};

// A capsule collision shape (0xc0 bytes); slot 0x80 recomputes it after its dimensions change.
struct UnkRoomCapsule_l994570 {
    void** vtable;
    u8 pad_0x8[0xa0];
    f32 field_0xa8;
    f32 field_0xac;
    f32 field_0xb0;
    u32 field_0xb4;
    u8 pad_0xb8[0x8];

    UnkRoomCapsule_l994570() { fn_7100d5ff7c(this); }
    ~UnkRoomCapsule_l994570() { fn_7100d6006c(this); }
    void Update() { reinterpret_cast<void (*)(UnkRoomCapsule_l994570*)>(vtable[0x80 / 8])(this); }
};

struct UnkRoomMovement_l994570 {
    void** vtable;
    u8 pad_0x8[0x3c8];
    u32 collision_mask;  // 0x3d0

    UnkRoomCollisionObject_l994570* GetCollisionObject() {
        return reinterpret_cast<UnkRoomCollisionObject_l994570* (*)(UnkRoomMovement_l994570*)>(
            vtable[0x2a8 / 8])(this);
    }
};

struct UnkRoomOwner_l994570 {
    u8 pad_0x0[0x38];
    UnkXform xform;
};

struct UnkRoomState_l994570 {
    u8 pad_0x0[0x30];
    UnkRoomOwner_l994570* owner;        // 0x30
    UnkRoomMovement_l994570* movement;  // 0x38
    u8 pad_0x40[0x1b8];
    u8 timer[0xb0];  // 0x1f8
};

// Where to test: the given position, or else the owner's.
static inline UnkVec3f QueryPosition(UnkRoomState_l994570* self, const UnkVec3f* pos) {
    if (pos)
        return *pos;
    return self->owner->xform.GetTranslation();
}

bool fn_7100994570(UnkRoomState_l994570* self, bool unused, bool skip_second, const UnkVec3f* pos) {
    UnkRoomCollisionObject_l994570* object = self->movement->GetCollisionObject();
    if (!object)
        return false;
    UnkRoomBody_l994570* body = object->body;
    if (!body || body->GetKind() != 8)
        return false;
    UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
    if (!sub)
        return false;
    UnkTree* tree = static_cast<UnkTree*>(sub->field_0x820);
    if (!tree)
        return false;

    UnkVec3f at = QueryPosition(self, pos);
    UnkMatrix44_7100078c9c xf = BuildTranslationMatrixFromVec3(at);
    UnkRoomCapsule_l994570 capsule;
    capsule.field_0xa8 = body->field_0xa8;
    capsule.field_0xac = body->field_0xac;
    capsule.field_0xb0 = body->field_0xb0;
    capsule.field_0xb4 = body->field_0xb4;
    capsule.Update();
    fn_7100d60184(&capsule, 0.999f);
    UnkVec3f offset = body->GetOffset();
    fn_7100d6c334(&capsule, &offset);

    const UnkMtx44* mtx = reinterpret_cast<const UnkMtx44*>(&xf);
    UnkShape* shape = reinterpret_cast<UnkShape*>(&capsule);
    bool free = !fn_7100d5b484(tree, self->movement->collision_mask, shape, mtx, nullptr, 1) &&
                !fn_710002fb70(self->timer);
    if (!skip_second)
        free = free && !fn_7100d5b484(tree, 0x100000, shape, mtx, nullptr, 1);
    return free;
}

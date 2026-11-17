// CProjectileMovement (unit 1623): looks up the owner's two collision objects named by the
// definition's ids at +0x108 and +0x110 and keeps weak references to them at +0x4a8 and +0x4b0.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkCollisionRecord.h"
#include "game_s2/UnkWeakHandle.h"

struct UnkProjectileMovementDef {
    u8 pad_0x0[0x108];
    base::global::CStrId collider_id;  // 0x108
    base::global::CStrId other_collider_id;  // 0x110
};

struct UnkProjectileMovementOwner {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;
};

struct UnkProjectileMovement {
    u8 pad_0x0[0x30];
    UnkProjectileMovementOwner* owner;
    u8 pad_0x38[0x4a8 - 0x38];
    UnkWeakHandle* collider;  // 0x4a8
    UnkWeakHandle* other_collider;  // 0x4b0
    u8 pad_0x4b8[0x5c8 - 0x4b8];
    UnkProjectileMovementDef* def;  // 0x5c8
};

void fn_7100ac3604(UnkProjectileMovement* self) {
    if (self->def->collider_id.data != &g_unk_7101d47108->empty) {
        if (UnkCollisionContainer* collision = self->owner->collision) {
            UnkWeakTarget* target = reinterpret_cast<UnkWeakTarget*>(
                fn_71008640a4(collision, &self->def->collider_id));
            AssignRef(&self->collider, target ? GetWeakHandle(target) : nullptr);
        }
    }
    if (self->def->other_collider_id.data != &g_unk_7101d47108->empty) {
        if (UnkCollisionContainer* collision = self->owner->collision) {
            UnkWeakTarget* target = reinterpret_cast<UnkWeakTarget*>(
                fn_71008640a4(collision, &self->def->other_collider_id));
            AssignRef(&self->other_collider, target ? GetWeakHandle(target) : nullptr);
        }
    }
}

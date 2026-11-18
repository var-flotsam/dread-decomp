// CCharacterMovement's unit (tools/tu_map.py unit 1445): the update. After the base update, the collider
// asked for by the animation's CCharClassChangeCharacterColliderTrack is looked up (kept at +0x3c0), and the
// weak references at +0x3f8 count down, the dead or expired ones erased. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkCharacterMovement_l976468;
void fn_71010d18e4(UnkCharacterMovement_l976468* self, f32 dt);

// As src/game/CCharacterMovement/CCharacterMovement_Movement_ovb0021.cpp defines it.
struct UnkTimedRef_ovb0021;
UnkTimedRef_ovb0021* fn_7100979360(base::global::CRntVector<UnkTimedRef_ovb0021>* list, UnkTimedRef_ovb0021* it);

// A weak reference with a countdown (16 bytes).
struct UnkTimedRef_l976468 {
    UnkWeakHandle* handle;
    f32 time;
};

struct UnkMovementCollider_l976468 {
    u8 pad_0x0[0x20];
    base::global::CStrId name;  // 0x20
};

struct UnkMovementCollision_l976468 {
    void** vtable;

    void vf_0x178(UnkMovementCollider_l976468* collider, bool arg) {
        reinterpret_cast<void (*)(UnkMovementCollision_l976468*, UnkMovementCollider_l976468*, bool)>(
            vtable[0x178 / 8])(this, collider, arg);
    }
};

struct UnkMovementOwner_l976468 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2a0 - 0x228];
    UnkMovementCollision_l976468* collision;  // 0x2a0
};

struct UnkCharacterMovement_l976468 {
    u8 pad_0x0[0x30];
    UnkMovementOwner_l976468* owner;  // 0x30
    u8 pad_0x38[0x3b8 - 0x38];
    UnkMovementCollider_l976468* defaultCollider;  // 0x3b8
    UnkMovementCollider_l976468* collider;  // 0x3c0
    u8 pad_0x3c8[0x3f8 - 0x3c8];
    base::global::CRntVector<UnkTimedRef_l976468> timed;  // 0x3f8
};

void fn_7100976468(UnkCharacterMovement_l976468* self, f32 dt) {
    fn_71010d18e4(self, dt);
    self->collider = nullptr;
    UnkTrackQuery query = {fn_7100cf1e58(), nullptr, -1.0f};
    UnkAnimationComponent* animation = self->owner->animation;
    if (animation) {
        fn_710117b08c(animation, &query);
        if (query.result) {
            UnkMovementCollider_l976468* current = self->collider ? self->collider : self->defaultCollider;
            if (UnkMovementCollision_l976468* collision = self->owner->collision)
                collision->vf_0x178(self->defaultCollider, false);
            if (!(current->name == query.result->sources))
                self->collider = reinterpret_cast<UnkMovementCollider_l976468*>(fn_71008640a4(
                    reinterpret_cast<UnkCollisionContainer*>(self->owner->collision), &query.result->sources));
        }
    }

    for (UnkTimedRef_l976468* it = self->timed.begin(); self->timed.end() != it;) {
        if (it->handle && it->handle->target && it->time > 0.0f && (it->time -= dt) > 0.0f) {
            ++it;
        } else {
            it = reinterpret_cast<UnkTimedRef_l976468*>(
                fn_7100979360(reinterpret_cast<base::global::CRntVector<UnkTimedRef_ovb0021>*>(&self->timed),
                              reinterpret_cast<UnkTimedRef_ovb0021*>(it)));
        }
    }
}

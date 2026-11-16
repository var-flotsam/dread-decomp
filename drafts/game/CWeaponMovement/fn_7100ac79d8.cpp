// CWeaponMovement unit: whether the hit list already holds a finished entry for `actor` and the part
// it was hit on (the collision record index, or the result of fn_7100ac6ed0).
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkRefl_fn_710031cd8c;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_710031cd8c>(void*);

s32 fn_710086427c(UnkCollisionContainer* self, const UnkCollisionObject* object);
s32 fn_7100ac6ed0(void* actor, void* info);

struct UnkHitActor_lac79d8 {
    u8 pad_0x0[0x2a0];
    UnkReflObject* collision;  // 0x2a0
};

struct UnkHitInfo_lac79d8 {
    UnkWeakHandle* source;
};

struct UnkHitEntry_lac79d8 {
    UnkWeakHandle* actor;
    s32 part;
    s32 state;
    u64 field_0x10;

    UnkWeakTarget* Actor() const { return actor ? actor->target : nullptr; }
};

struct UnkWeaponMovement_lac79d8 {
    u8 pad_0x0[0x450];
    UnkHitEntry_lac79d8* hits;  // 0x450
    u32 hit_count;              // 0x458
};

// A counted reference to an actor's weak handle.
struct UnkWeakRef_lac79d8 {
    UnkWeakHandle* handle;

    explicit UnkWeakRef_lac79d8(UnkWeakTarget* target) : handle(target ? fn_7100d78378(target) : nullptr) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkWeakRef_lac79d8() {
        if (handle) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
    UnkWeakTarget* Get() const { return handle ? handle->target : nullptr; }
};

static const s32 kNoPart = -0x7fffffff - 1;

bool fn_7100ac79d8(void* self_, void* actor_, void* info_) {
    auto* self = static_cast<UnkWeaponMovement_lac79d8*>(self_);
    auto* actor = static_cast<UnkHitActor_lac79d8*>(actor_);
    auto* info = static_cast<UnkHitInfo_lac79d8*>(info_);

    UnkWeakRef_lac79d8 ref(reinterpret_cast<UnkWeakTarget*>(actor));
    s32 part;
    UnkReflObject* collision;
    if (actor && (collision = actor->collision) && fn_710009d50c(collision->vf_0x0(), fn_7100723914())) {
        UnkWeakTarget* source = info->source ? info->source->target : nullptr;
        part = fn_710086427c(reinterpret_cast<UnkCollisionContainer*>(collision),
                             reinterpret_cast<const UnkCollisionObject*>(source));
    } else if (base::reflection::refl_FindTyped<UnkRefl_fn_710031cd8c>(actor)) {
        part = fn_7100ac6ed0(actor, info);
    } else {
        part = kNoPart;
    }

    for (UnkHitEntry_lac79d8* hit = self->hits; hit != self->hits + self->hit_count; ++hit) {
        if (hit->Actor() != ref.Get())
            continue;
        if (part == hit->part || part == (hit->part < 0 ? -hit->part : hit->part)) {
            if (hit->state == kNoPart)
                return true;
        }
    }
    return false;
}

// CWeaponMovement's unit (tools/tu_map.py, unit 1624): records a hit actor. A counted reference to the
// actor's weak handle is appended to the hit list (+0x450) with the hit's id: the record index in the
// actor's CBillboardCollisionComponent, or fn_7100ac6ed0's answer. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkGunShot_ovb0070;
void fn_710085c338(base::global::CRntVector<UnkGunShot_ovb0070>* shots, u32 n);

struct UnkRefl_fn_710031cd8c;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_710031cd8c>(void*);

struct UnkWeaponHitCollision_lac7bec;
struct UnkWeaponHitCollisionVtbl_lac7bec {
    const base::reflection::CType* (*GetClass)(UnkWeaponHitCollision_lac7bec* self);
};
struct UnkWeaponHitCollision_lac7bec {
    UnkWeaponHitCollisionVtbl_lac7bec* vtable;
};

struct UnkWeaponTarget_lac7bec {
    u8 pad_0x0[0x2a0];
    UnkWeaponHitCollision_lac7bec* collision;  // 0x2a0
};

struct UnkWeaponHitShape_lac7bec {
    u8 pad_0x0[0x10];
    UnkCollisionObject* object;  // 0x10
};

struct UnkWeaponHit_lac7bec {
    UnkWeaponHitShape_lac7bec* shape;
};

s32 fn_710086427c(UnkCollisionContainer* self, const UnkCollisionObject* object);
s32 fn_7100ac6ed0(UnkWeaponTarget_lac7bec* target, UnkWeaponHit_lac7bec* hit);

// A counted reference to a weak handle.
struct UnkHandleRef_lac7bec {
    UnkWeakHandle* handle;

    explicit UnkHandleRef_lac7bec(UnkWeakHandle* p) : handle(p) {
        if (handle)
            ++handle->ref_count;
    }
    UnkHandleRef_lac7bec(const UnkHandleRef_lac7bec& other) : handle(nullptr) { *this = other; }
    UnkHandleRef_lac7bec& operator=(const UnkHandleRef_lac7bec& other) {
        UnkWeakHandle* p = other.handle;
        if (p)
            ++p->ref_count;
        if (UnkWeakHandle* old = handle) {
            if (--old->ref_count == 0)
                old->vf_0x10();
        }
        handle = p;
        return *this;
    }
    ~UnkHandleRef_lac7bec() {
        if (handle && --handle->ref_count == 0)
            handle->vf_0x10();
    }
};

struct UnkWeaponHitEntry_lac7bec {
    UnkHandleRef_lac7bec target;
    s32 id;
    s32 field_0xc;
    f32 field_0x10;

    UnkWeaponHitEntry_lac7bec(const UnkHandleRef_lac7bec& target, s32 id)
        : target(target), id(id), field_0xc(-0x7fffffff - 1), field_0x10(3.40282347e+38f) {}
};
static_assert(sizeof(UnkWeaponHitEntry_lac7bec) == 0x18, "UnkWeaponHitEntry_lac7bec layout");

struct UnkWeaponMovement_lac7bec {
    u8 pad_0x0[0x450];
    base::global::CRntVector<UnkWeaponHitEntry_lac7bec> hits;  // 0x450

    // Appends a hit, growing the list like CRntVector::PushBackCopy (including its hang).
    void AddHit(const UnkHandleRef_lac7bec& target, s32 id) {
        u32 n = hits.count;
        u32 new_capacity = hits.capacity;
        if (new_capacity <= n && hits.step != 0)
            new_capacity = (n / hits.step + 1) * hits.step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        fn_710085c338(reinterpret_cast<base::global::CRntVector<UnkGunShot_ovb0070>*>(&hits), new_capacity);
        new (&hits.data[hits.count]) UnkWeaponHitEntry_lac7bec(target, id);
        ++hits.count;
    }
};

void fn_7100ac7bec(UnkWeaponMovement_lac7bec* self, UnkWeaponTarget_lac7bec* target, UnkWeaponHit_lac7bec* hit) {
    if (!target)
        return;
    UnkHandleRef_lac7bec ref(fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(target)));
    s32 id;
    UnkWeaponHitCollision_lac7bec* collision = target->collision;
    if (collision && fn_710009d50c(collision->vtable->GetClass(collision),
                                   reinterpret_cast<const base::reflection::CType*>(fn_7100723914()))) {
        id = fn_710086427c(reinterpret_cast<UnkCollisionContainer*>(collision),
                           hit->shape ? hit->shape->object : nullptr);
    } else if (base::reflection::refl_FindTyped<UnkRefl_fn_710031cd8c>(target)) {
        id = fn_7100ac6ed0(target, hit);
    } else {
        id = -0x7fffffff - 1;
    }
    self->AddHit(ref, id);
}

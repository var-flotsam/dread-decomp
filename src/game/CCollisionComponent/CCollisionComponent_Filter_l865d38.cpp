// CCollisionComponent unit (tools/tu_map.py unit 1319): decides a contact between two colliders: 1
// when either collider's actor ignores the other by its collision-mask track and the other does not
// collide against ignored colliders, else the result of the component's filter lists. Names and
// layouts are placeholders.
#include <string.h>

#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCollisionComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;

struct UnkCollider_l865d38 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual u32 vf_0x58();
    u8 unk_0x8[0x8];
    UnkReflObject* component;  // 0x10
};

struct UnkColliderRef_l865d38 {
    u8 unk_0x0[0x10];
    UnkCollider_l865d38* collider;  // 0x10
};

struct UnkCollisionPair_l865d38 {
    u8 unk_0x0[0x10];
    UnkColliderRef_l865d38* first;   // 0x10
    UnkColliderRef_l865d38* second;  // 0x18
};

struct UnkCollisionActor_l865d38 {
    u8 unk_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkCollisionComponentView_l865d38 {
    u8 unk_0x0[0x30];
    UnkCollisionActor_l865d38* owner;  // 0x30
};

// The record of a CCharClassIgnoreCollisionMaskTrack.
struct UnkIgnoreMaskTrack_l865d38 {
    u8 unk_0x0[0x28];
    u32 mask;    // 0x28
    u32 except;  // 0x2c
};

struct UnkCollisionFilter_l865d38 {
    virtual u32 vf_0x0(UnkCollisionPair_l865d38* pair);
};

// Up to eight filters in place, the count first.
struct UnkCollisionFilterList_l865d38 {
    u32 count;
    u8 storage[8 * sizeof(UnkCollisionFilter_l865d38*)];

    UnkCollisionFilter_l865d38** begin() { return reinterpret_cast<UnkCollisionFilter_l865d38**>(storage); }
    UnkCollisionFilter_l865d38** end() { return begin() + count; }
    const UnkCollisionFilter_l865d38* const* begin() const {
        return reinterpret_cast<const UnkCollisionFilter_l865d38* const*>(storage);
    }

    UnkCollisionFilterList_l865d38(const UnkCollisionFilterList_l865d38& other) {
        memcpy(begin(), other.begin(), other.count * sizeof(UnkCollisionFilter_l865d38*));
        count = other.count;
    }
};
static_assert(sizeof(UnkCollisionFilterList_l865d38) == 0x44, "filter list size");

struct UnkCollisionFilters_l865d38 {
    u8 unk_0x0[0xd8];
    UnkCollisionFilterList_l865d38* lists;  // 0xd8
    u8 unk_0xe0[0x8];
    u32 listCount;  // 0xe8
};

static inline bool IsCollisionComponent(UnkReflObject* component) {
    const CType* type = component->vf_0x0();
    fn_710085fc44();
    return fn_710009d50c(type, &g_unk_7172728bf0);
}

static inline UnkAnimationComponent* Animation(UnkReflObject* component) {
    return reinterpret_cast<UnkCollisionComponentView_l865d38*>(component)->owner->animation;
}

// Whether `self`'s actor ignores `other` by its collision-mask track.
static inline bool Ignores(UnkCollider_l865d38* self, UnkCollider_l865d38* other) {
    UnkReflObject* component = self->component;
    if (!component || !IsCollisionComponent(component))
        return false;
    UnkAnimationComponent* animation = Animation(component);
    if (!animation)
        return false;
    UnkTrackQuery query = {fn_7100c291f8(), nullptr, -1.0f};
    fn_710117b08c(animation, &query);
    UnkIgnoreMaskTrack_l865d38* track = reinterpret_cast<UnkIgnoreMaskTrack_l865d38*>(query.result);
    if (!track)
        return false;
    u32 mask = track->mask;
    u32 except = track->except;
    return (other->vf_0x58() & mask) && !(other->vf_0x58() & except);
}

// Whether `collider`'s actor collides against colliders that ignore it.
static inline bool CollidesAgainstIgnored(UnkCollider_l865d38* collider) {
    UnkReflObject* component = collider->component;
    if (!component || !IsCollisionComponent(component))
        return false;
    UnkAnimationComponent* animation = Animation(component);
    return !animation || fn_710117b348(animation, fn_7100bf88b8());
}

u32 fn_7100865d38(UnkCollisionFilters_l865d38* self, UnkCollisionPair_l865d38* pair) {
    if (pair) {
        UnkCollider_l865d38* a = pair->second ? pair->second->collider : nullptr;
        UnkCollider_l865d38* b = pair->first ? pair->first->collider : nullptr;
        if (a && b) {
            if (Ignores(a, b) && !CollidesAgainstIgnored(b))
                return 1;
            if (Ignores(b, a) && !CollidesAgainstIgnored(a))
                return 1;
        }
    }
    u32 result = 0;
    for (UnkCollisionFilterList_l865d38* list = self->lists; list != self->lists + self->listCount; ++list) {
        UnkCollisionFilterList_l865d38 filters = *list;
        for (UnkCollisionFilter_l865d38** filter = filters.begin(); filter != filters.end(); ++filter) {
            result = (*filter)->vf_0x0(pair);
            if (result == 1)
                break;
        }
    }
    return result;
}

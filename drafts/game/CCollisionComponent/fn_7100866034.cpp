// CCollisionComponent unit (tools/tu_map.py unit 1319): decides a contact between two colliders: 1
// when either collider's actor ignores the other by its collision-mask track and the other does not
// collide against ignored colliders, else the result of the component's filter lists. Names and
// layouts are placeholders.
#include <string.h>

#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;

extern CType g_unk_7172728bf0 __attribute__((visibility("hidden")));  // CCollisionComponent's type

struct UnkCollider_l866034 {
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

struct UnkColliderRef_l866034 {
    u8 unk_0x0[0x10];
    UnkCollider_l866034* collider;  // 0x10
};

struct UnkCollisionPair_l866034 {
    u8 unk_0x0[0x10];
    UnkColliderRef_l866034* first;   // 0x10
    UnkColliderRef_l866034* second;  // 0x18
};

struct UnkCollisionActor_l866034 {
    u8 unk_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkCollisionComponentView_l866034 {
    u8 unk_0x0[0x30];
    UnkCollisionActor_l866034* owner;  // 0x30
};

// The record of a CCharClassIgnoreCollisionMaskTrack.
struct UnkIgnoreMaskTrack_l866034 {
    u8 unk_0x0[0x28];
    u32 mask;    // 0x28
    u32 except;  // 0x2c
};

struct UnkCollisionFilter_l866034 {
    virtual void vf_0x0();
    virtual u32 vf_0x8(UnkCollisionPair_l866034* pair);
};

#pragma pack(push, 4)
// Up to eight filters, the count first.
struct UnkCollisionFilterList_l866034 {
    u32 count;
    UnkCollisionFilter_l866034* items[8];

    UnkCollisionFilterList_l866034(const UnkCollisionFilterList_l866034& other) {
        memcpy(items, other.items, other.count * sizeof(items[0]));
        count = other.count;
    }
};
#pragma pack(pop)
static_assert(sizeof(UnkCollisionFilterList_l866034) == 0x44, "filter list size");

struct UnkCollisionFilters_l866034 {
    u8 unk_0x0[0xd8];
    UnkCollisionFilterList_l866034* lists;  // 0xd8
    u8 unk_0xe0[0x8];
    u32 listCount;  // 0xe8
};

static inline bool IsCollisionComponent(UnkReflObject* component) {
    const CType* type = component->vf_0x0();
    fn_710085fc44();
    return fn_710009d50c(type, &g_unk_7172728bf0);
}

static inline UnkAnimationComponent* Animation(UnkReflObject* component) {
    return reinterpret_cast<UnkCollisionComponentView_l866034*>(component)->owner->animation;
}

// Whether `self`'s actor ignores `other` by its collision-mask track.
static inline bool Ignores(UnkCollider_l866034* self, UnkCollider_l866034* other) {
    UnkReflObject* component = self->component;
    if (!component || !IsCollisionComponent(component))
        return false;
    UnkAnimationComponent* animation = Animation(component);
    if (!animation)
        return false;
    UnkTrackQuery query = {fn_7100c291f8(), nullptr, -1.0f};
    fn_710117b08c(animation, &query);
    UnkIgnoreMaskTrack_l866034* track = reinterpret_cast<UnkIgnoreMaskTrack_l866034*>(query.result);
    if (!track)
        return false;
    u32 mask = track->mask;
    u32 except = track->except;
    return (other->vf_0x58() & mask) && !(other->vf_0x58() & except);
}

// Whether `collider`'s actor collides against colliders that ignore it.
static inline bool CollidesAgainstIgnored(UnkCollider_l866034* collider) {
    UnkReflObject* component = collider->component;
    if (!component || !IsCollisionComponent(component))
        return false;
    UnkAnimationComponent* animation = Animation(component);
    return !animation || fn_710117b348(animation, fn_7100bf88b8());
}

u32 fn_7100866034(UnkCollisionFilters_l866034* self, UnkCollisionPair_l866034* pair) {
    if (pair) {
        UnkCollider_l866034* a = pair->second ? pair->second->collider : nullptr;
        UnkCollider_l866034* b = pair->first ? pair->first->collider : nullptr;
        if (a && b) {
            if (Ignores(a, b) && !CollidesAgainstIgnored(b))
                return 1;
            if (Ignores(b, a) && !CollidesAgainstIgnored(a))
                return 1;
        }
    }
    u32 result = 0;
    for (UnkCollisionFilterList_l866034* list = self->lists; list != self->lists + self->listCount; ++list) {
        UnkCollisionFilterList_l866034 filters = *list;
        for (u32 i = 0; i != filters.count; ++i) {
            result = filters.items[i]->vf_0x8(pair);
            if (result == 1)
                break;
        }
    }
    return result;
}

// Unit 437 (aim component): whether a hit is accepted: none of the listed colliders is the hit's
// collider, and the collider's owner plays none of the listed animation classes. Names and layouts
// are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::UnkReflObject;

struct UnkFilterOwner {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkFilterComponent : UnkReflObject {
    u8 pad_0x8[0x28];
    UnkFilterOwner* owner;  // 0x30
};

struct UnkFilterRef {
    u8 pad_0x0[0x10];
    UnkFilterComponent* component;  // 0x10
};

struct UnkFilterHit {
    u8 pad_0x0[0x10];
    UnkFilterRef* collider;  // 0x10
};

struct UnkFilterMessage {
    UnkFilterHit* hit;
};

struct __attribute__((packed)) UnkColliderList {
    u32 count;
    UnkFilterComponent* items[1];
};

struct __attribute__((packed)) UnkClassList358 {
    u32 count;
    const CClass* classes[1];
};

struct UnkHitFilter {
    u8 pad_0x0[0x10];
    UnkColliderList* colliders;  // 0x10
    UnkClassList358* classes;    // 0x18
};

static inline UnkFilterComponent* HitComponent(UnkFilterMessage* msg) {
    return msg->hit && msg->hit->collider ? msg->hit->collider->component : nullptr;
}

bool fn_7100358d68(UnkHitFilter* self, UnkFilterMessage* msg) {
    bool accepted = true;
    UnkColliderList* colliders = self->colliders;
    UnkFilterComponent* const* it = colliders->items;
    for (UnkFilterComponent* const* end = it + colliders->count; it != end; ++it) {
        if (!accepted)
            continue;
        UnkFilterComponent* item = *it;
        if (!item || !msg->hit || !msg->hit->collider || !msg->hit->collider->component)
            accepted = true;
        else
            accepted = item != msg->hit->collider->component;
    }
    if (!accepted)
        return false;
    UnkClassList358* list = self->classes;
    bool result = true;
    const CClass* const* cit = list->classes;
    for (const CClass* const* end = cit + list->count; cit != end; ++cit) {
        const CClass* cls = *cit;
        bool playing = false;
        UnkFilterComponent* component = HitComponent(msg);
        if (component && fn_710009d50c(component->vf_0x0(), fn_710085fc44()) && component->owner &&
            component->owner->animation)
            playing = fn_710117b348(component->owner->animation, cls);
        result = result ? !playing : false;
    }
    return result;
}

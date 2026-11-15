// Unit 437 (aim component): true unless the hit's collider is a CCollisionComponent whose owner is
// playing one of the listed animation classes. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::UnkReflObject;

struct UnkColliderOwner {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkColliderComponent : UnkReflObject {
    u8 pad_0x8[0x28];
    UnkColliderOwner* owner;  // 0x30
};

struct UnkColliderRef {
    u8 pad_0x0[0x10];
    UnkReflObject* component;  // 0x10
};

struct UnkColliderHit {
    u8 pad_0x0[0x10];
    UnkColliderRef* collider;  // 0x10
};

struct __attribute__((packed)) UnkClassList {
    u32 count;
    const CClass* classes[1];
};

struct UnkColliderFilter {
    u8 pad_0x0[0x10];
    UnkClassList* ignored;  // 0x10
};

struct UnkColliderMessage {
    UnkColliderHit* hit;
};

bool fn_7100354b34(UnkColliderFilter* self, UnkColliderMessage* msg) {
    if (!msg->hit)
        return true;
    UnkColliderRef* collider = msg->hit->collider;
    if (!collider || !collider->component)
        return true;
    if (!fn_710009d50c(collider->component->vf_0x0(), fn_710085fc44()))
        return true;
    UnkReflObject* object = msg->hit->collider->component;
    UnkColliderComponent* component =
        fn_710009d50c(object->vf_0x0(), fn_710085fc44()) ? static_cast<UnkColliderComponent*>(object) : nullptr;
    UnkColliderOwner* owner = component->owner;
    if (!owner)
        return true;
    UnkClassList* list = self->ignored;
    UnkAnimationComponent* animation = owner->animation;
    u32 count = list->count;
    const CClass* const* it = list->classes;
    if (count == 0 || !animation)
        return true;
    for (const CClass* const* end = it + count; it != end; ++it) {
        if (fn_710117b348(animation, *it))
            return false;
    }
    return true;
}

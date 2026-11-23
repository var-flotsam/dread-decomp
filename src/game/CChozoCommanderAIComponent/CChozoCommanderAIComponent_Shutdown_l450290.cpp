// CChozoCommanderAIComponent unit (tools/tu_map.py): runs the base step, drops the two weak handles
// (unlinking their listeners first), then kills the owner of every sentence sphere, energy-shards sphere
// and energy-shards fragment in the sphere list. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CWeaponMovement.h"
#include "game/UnkSphereList.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::UnkReflObject;

struct UnkSphereEntry {
    u8 pad_0x0[0x10];
    UnkReflObject* object;  // 0x10
};

struct UnkChozoSphereMovement_l450290 : UnkReflObject {
    u8 pad_0x8[0x28];
    void* owner;  // 0x30
};

struct UnkChozoCommanderAI_l450290 {
    u8 pad_0x0[0x15f08];
    u8 link0[0x18];          // 0x15f08
    UnkWeakHandle* handle0;  // 0x15f20
    u8 link1[0x18];          // 0x15f28
    UnkWeakHandle* handle1;  // 0x15f40
};

static inline void Unlink(UnkWeakHandle** slot, void* link) {
    if (UnkWeakHandle* handle = *slot) {
        if (handle->target)
            fn_710117b3f0(handle->target, link);
        ResetRef(slot);
    }
}

template <const CClass* (*Getter)()>
static inline UnkChozoSphereMovement_l450290* SphereAs(UnkSphereEntry* entry) {
    UnkReflObject* object = entry ? entry->object : nullptr;
    if (object && fn_710009d50c(object->vf_0x0(), Getter()))
        return static_cast<UnkChozoSphereMovement_l450290*>(object);
    return nullptr;
}

void fn_7100450290(UnkChozoCommanderAI_l450290* self) {
    fn_71010ff2b8(reinterpret_cast<char*>(self));
    Unlink(&self->handle0, self->link0);
    Unlink(&self->handle1, self->link1);
    UnkSphereList* list = fn_7100ac73b0();
    for (UnkSphereEntry **it = list->data, **end = list->data + list->count; it != end; ++it) {
        if (UnkChozoSphereMovement_l450290* sphere = SphereAs<fn_710045b404>(*it))
            fn_7100232c68(sphere->owner);
        else if (UnkChozoSphereMovement_l450290* sphere = SphereAs<fn_7100459b48>(*it))
            fn_7100232c68(sphere->owner);
        else if (UnkChozoSphereMovement_l450290* sphere = SphereAs<fn_71004594e4>(*it))
            fn_7100232c68(sphere->owner);
    }
}

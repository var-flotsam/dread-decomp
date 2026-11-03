// CProjectileMovement unit (tools/tu_map.py unit 1623): when what `hit` refers to is a collision
// component, passes its actor, the position of `at` (its translation), an empty record, the hit and
// the target of its second handle on to the movement's virtual at slot 0x330. Names and layouts
// are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"

using base::reflection::CClass;

struct UnkOnHitComponent_b46a3 {
    virtual const CClass* vf_0x0();
    u8 unk_0x8[0x28];
    UnkActor* actor;  // 0x30
};

struct UnkOnHitTarget_b46a3 {
    u8 unk_0x0[0x10];
    UnkOnHitComponent_b46a3* component;  // 0x10
};

struct UnkOnHitInfo_b46a3 {
    UnkWeakHandle* handle;
    UnkWeakHandle* other;
};

struct UnkOnHitRecord_b46a3 {
    void* field_0x0;
    u32 field_0x8;

    UnkOnHitRecord_b46a3() : field_0x0(nullptr), field_0x8(0) {}
};

struct UnkProjectileMovement8_b46a3 {
    void** vtable;

    u32 vf_0x330(UnkActor* actor, const UnkVec3f* pos, UnkOnHitRecord_b46a3* record, UnkOnHitInfo_b46a3* hit,
                  UnkWeakTarget* other) {
        return reinterpret_cast<u32 (*)(UnkProjectileMovement8_b46a3*, UnkActor*, const UnkVec3f*,
                                         UnkOnHitRecord_b46a3*, UnkOnHitInfo_b46a3*, UnkWeakTarget*)>(
            vtable[0x330 / 8])(this, actor, pos, record, hit, other);
    }
};

u32 fn_7100ac62c8(UnkProjectileMovement8_b46a3* self, UnkActor* at, UnkOnHitInfo_b46a3* hit) {
    UnkWeakHandle* handle = hit->handle;
    if (!handle || !handle->target)
        return false;
    UnkOnHitComponent_b46a3* component = reinterpret_cast<UnkOnHitTarget_b46a3*>(handle->target)->component;
    if (!component)
        return false;
    if (!fn_710009d50c(component->vf_0x0(), fn_710085fc44()))
        return false;
    UnkActor* actor = component->actor;
    UnkVec3f pos = reinterpret_cast<UnkXform*>(at->field_0x38)->GetTranslation();
    UnkOnHitRecord_b46a3 record;
    return self->vf_0x330(actor, &pos, &record, hit, hit->other ? hit->other->target : nullptr) & 1;
}

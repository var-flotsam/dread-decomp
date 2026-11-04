// CForcedMovementAreaComponent's unit: on an area event (flag 4, or flag 2 unless the collider's
// vf_0x58 says no), when the collider's entity (+0x10) is of the class fn_710085fc44 names, hands
// the collider to fn_71007eb998. Two copies: the second is the override seen from the base at +0x38.
// Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkAreaCollider_A5_b65 {
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
    u64 field_0x8;
    UnkReflObject* entity;  // 0x10
};

struct UnkForcedMovementArea_A5_b65;
void fn_71007eb998(UnkForcedMovementArea_A5_b65* self, UnkAreaCollider_A5_b65* collider);

__attribute__((always_inline)) static inline bool Accepts_A5_b65(UnkAreaCollider_A5_b65* collider, u32 flags) {
    if (!(flags & 4)) {
        if (!(flags & 2))
            return false;
        if (collider->vf_0x58())
            return false;
    }
    UnkReflObject* entity = collider->entity;
    if (!entity)
        return false;
    return fn_710009d50c(entity->vf_0x0(), fn_710085fc44());
}

void fn_71007ec4d4(UnkForcedMovementArea_A5_b65* self, UnkAreaCollider_A5_b65* collider, u32 flags) {
    if (Accepts_A5_b65(collider, flags))
        fn_71007eb998(self, collider);
}

void fn_71007ec55c(char* self, UnkAreaCollider_A5_b65* collider, u32 flags) {
    if (Accepts_A5_b65(collider, flags))
        fn_71007eb998(reinterpret_cast<UnkForcedMovementArea_A5_b65*>(self - 0x38), collider);
}

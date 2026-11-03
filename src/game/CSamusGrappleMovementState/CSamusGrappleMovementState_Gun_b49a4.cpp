// CSamusGrappleMovementState unit (tools/tu_map.py): the owner's grapple beam gun: the object
// fn_710083cd30 finds for the component at owner->+0x30->+0x2a8, when it is a CGrappleBeamGun
// (reflection), else null. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

struct UnkComponent;
void* fn_710083cd30(UnkComponent* component);

struct UnkGrappleOwnerActor_b49a4 {
    u8 pad_0x0[0x2a8];
    UnkComponent* field_0x2a8;
};
struct UnkGrappleOwner_b49a4 {
    u8 pad_0x0[0x30];
    UnkGrappleOwnerActor_b49a4* field_0x30;
};
struct UnkGrappleMovement_b49a4 {
    u8 pad_0x0[0x8];
    UnkGrappleOwner_b49a4* field_0x8;
};
struct UnkGrappleState_b49a4 {
    u8 pad_0x0[0x8];
    UnkGrappleMovement_b49a4* field_0x8;
};

base::reflection::UnkReflObject* fn_710099afa0(UnkGrappleState_b49a4* self) {
    UnkComponent* c = self->field_0x8->field_0x8->field_0x30->field_0x2a8;
    if (c == nullptr)
        return nullptr;
    auto* obj = static_cast<base::reflection::UnkReflObject*>(fn_710083cd30(c));
    if (obj == nullptr)
        return nullptr;
    return fn_710009d50c(obj->vf_0x0(), fn_7100832034()) ? obj : nullptr;
}

// CSamusGrappleMovementState's unit (tools/tu_map.py, unit 1461): fn_710099b9e0 turns the aim target's
// offset from the owner into a pitch angle in degrees (the angle between the target's height above
// the plane and its horizontal distance) and stores it in the owner's property bag under
// g_unk_717273bb60. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "game/UnkXformNode.h"
#include "types.h"
#include "../../game_s7/UnkPropBagS7.h"

extern char g_unk_717273bb60[] __attribute__((visibility("hidden")));

struct UnkAimTarget_b9e0 {
    void** vtable;

    UnkVec3f vf_0x188() { return reinterpret_cast<UnkVec3f (*)(UnkAimTarget_b9e0*)>(vtable[0x188 / 8])(this); }
};

struct UnkAimHolder_b9e0 {
    u8 pad_0x0[0x10];
    UnkAimTarget_b9e0* target;  // 0x10
};

struct UnkAnimOwner_b9e0 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x220 - 0x68];
    char* props;  // 0x220
};

struct UnkGrappleState_b9e0 {
    u8 pad_0x0[0x30];
    UnkAnimOwner_b9e0* owner;  // 0x30
    u8 pad_0x38[0xb18 - 0x38];
    UnkAimHolder_b9e0* aim;  // 0xb18
};

void fn_710099b9e0(UnkGrappleState_b9e0* self) {
    UnkAimTarget_b9e0* target = self->aim ? self->aim->target : nullptr;
    base::math::CVector3D delta;
    UnkVec3f pos = target->vf_0x188();
    UnkAnimOwner_b9e0* owner = self->owner;
    f32 dx = pos.x - owner->x;
    f32 dy = pos.y - owner->y;
    f32 dz = pos.z - owner->z;
    delta.x = dx;
    delta.y = dy;
    delta.z = dz;
    base::math::CVector2D flat;
    fn_710007b430(&flat, &delta);
    f32 angle = atan2f(flat.y, fabsf(flat.x));
    unk_prop_bag::UnkPropBag* bag = reinterpret_cast<unk_prop_bag::UnkPropBag*>(self->owner->props + 0x80);
    f32 degrees = static_cast<f32>(static_cast<f64>(angle * 180.0f) / 3.14159265358979323846);
    unk_prop_bag::fn_710033187c(bag, *reinterpret_cast<const unk_prop_bag::CStrId*>(g_unk_717273bb60), degrees);
}

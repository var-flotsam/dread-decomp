// CAISmartObjectComponent's unit (tools/tu_map.py): the rotation angles (fn_7100076acc) of the frame
// fn_7100079c60 builds from the owner's up axis (column 1 of its matrix, fn_710002614c on +0x38) and its
// side axis (column 0) scaled by the negated side (+0x54), orthonormalised by fn_7100077bbc. Names and
// layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkMat4f {
    f32 m[4][4];
};
UnkMat4f fn_7100079c60(const UnkVec3f* from, const UnkVec3f* to);
UnkVec3f fn_7100076acc(const UnkXform* x);

struct UnkSmartObjectOwner_ovb0069 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
};

struct UnkSmartObjectComponent_ovb0069 {
    u8 pad_0x0[0x30];
    UnkSmartObjectOwner_ovb0069* owner;  // 0x30
    u8 pad_0x38[0x1c];
    s32 side;  // 0x54
};

static inline UnkVec3f Up(UnkSmartObjectComponent_ovb0069* self) {
    const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
    return UnkVec3f(m.m[1], m.m[5], m.m[9]);
}

static inline UnkVec3f Side(UnkSmartObjectComponent_ovb0069* self) {
    const UnkMatrix44& n = fn_710002614c(&self->owner->transform);
    f32 s = static_cast<f32>(-self->side);
    return UnkVec3f(n.m[0] * s, n.m[4] * s, n.m[8] * s);
}

UnkVec3f fn_71003601ac(UnkSmartObjectComponent_ovb0069* self) {
    UnkMat4f frame = fn_7100079c60(&static_cast<const UnkVec3f&>(Up(self)), &static_cast<const UnkVec3f&>(Side(self)));
    fn_7100077bbc(&frame);
    return fn_7100076acc(reinterpret_cast<const UnkXform*>(&frame));
}

// Returns the target object's position (the origin's unused), pulled inside the area the
// commander's stage defines: when the commander is found and is of the right class, each axis is
// moved to stay a margin (25 plus the component's value at 0x5dc) away from the area's edge.
#include "base/math/CVector.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

using base::math::CVector3D;
using base::reflection::CClass;

struct UnkTargetPos {
    u8 pad_0x0[0x44];
    u32 field_0x44;
    u8 pad_0x48[0xc];
    u32 field_0x54;
    u8 pad_0x58[0xc];
    u32 field_0x64;
};

struct UnkArea {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
};

struct UnkCommander {
    void** vtable;
    u8 pad_0x8[0x15d14];
    UnkArea field_0x15d1c;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkCommander*)>(vtable[0])(this); }
};

struct UnkCommanderOwner {
    u8 pad_0x0[0x240];
    UnkCommander* field_0x240;
};

struct UnkCommanderHolder {
    u8 pad_0x0[0x10];
    UnkCommanderOwner* field_0x10;
};

struct UnkClamper {
    u8 pad_0x0[0x30];
    UnkActor* field_0x30;
    u8 pad_0x38[0x1f8];
    UnkCommanderHolder* field_0x230;
    u8 pad_0x238[0x3a4];
    f32 field_0x5dc;
};

const CClass* fn_710044e708();  // CChozoCommanderAIComponent::refl_GetClass

union UnkBits {
    u32 u;
    f32 f;
};

// Not trivially copyable, so it is returned in memory.
struct UnkVec3Bits {
    UnkBits x;
    UnkBits y;
    UnkBits z;

    UnkVec3Bits(u32 x, u32 y, u32 z) : x{x}, y{y}, z{z} {}
    UnkVec3Bits(const UnkVec3Bits& other) : x(other.x), y(other.y), z(other.z) {}
};

UnkVec3Bits fn_710045bd10(UnkClamper* self) {
    UnkTargetPos* target = reinterpret_cast<UnkTargetPos*>(fn_7100d7c990(self->field_0x30));
    UnkVec3Bits result(target->field_0x44, target->field_0x54, target->field_0x64);
    if (!self->field_0x230 || !self->field_0x230->field_0x10)
        return result;
    UnkCommander* commander = self->field_0x230->field_0x10->field_0x240;
    if (!commander || !fn_710009d50c(commander->vf_0x0(), fn_710044e708()))
        return result;
    f32 margin = self->field_0x5dc;
    UnkArea* area = &commander->field_0x15d1c;
    f32 x = result.x.f;
    f32 y = result.y.f;
    if (margin + x + 25.0f > area->max_x)
        result.x.f = area->max_x - margin + -25.0f;
    else if (x - margin + -25.0f < area->min_x)
        result.x.f = margin + area->min_x + 25.0f;
    if (margin + y + 25.0f > area->max_y)
        result.y.f = area->max_y - margin + -25.0f;
    else if (y - margin + -25.0f < area->min_y)
        result.y.f = margin + area->min_y + 25.0f;
    return result;
}

// Moves a position towards the target object's position (the origin when there is none) by a
// step: the speed is interpolated between two values of the boss attack's tunables by how far
// the target is (distance over a maximum, capped at 1), scaled by `scale`, and never more than
// the distance itself.
#include "base/math/CVector.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::math::CVector3D;
using base::reflection::CClass;

struct UnkTargetPos {
    u8 pad_0x0[0x44];
    f32 field_0x44;
    u8 pad_0x48[0xc];
    f32 field_0x54;
    u8 pad_0x58[0xc];
    f32 field_0x64;
};

struct UnkAttackTunables {
    u8 pad_0x0[0x11c];
    f32 field_0x11c;
    f32 field_0x120;
    f32 field_0x124;
};

struct UnkAttackObject {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkAttackObject*)>(vtable[0])(this); }
};

struct UnkMover {
    u8 pad_0x0[0x30];
    UnkTargetPos* field_0x30;
    u8 pad_0x38[0x15118];
    UnkAttackObject* field_0x15150;
};

UnkTargetPos* fn_7100d7c990(UnkTargetPos* object);
const CClass* fn_71004add20();  // CCharClassCooldownXBossLavaDropsAttack::refl_GetClass

// NON_MATCHING(minor): the two argument copies at the start (position, scale) are in the other order;
// behaviour is identical
void fn_71004d24d0(UnkMover* self, CVector3D* position, f32 scale) {
    UnkTargetPos* target = fn_7100d7c990(self->field_0x30);
    CVector3D to = target ? CVector3D{target->field_0x44, target->field_0x54, target->field_0x64} : CVector3D{0.0f, 0.0f, 0.0f};
    f32 dx = to.x - position->x;
    f32 dy = to.y - position->y;
    f32 dz = to.z - position->z;
    f32 distance = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    UnkAttackObject* object = self->field_0x15150;
    UnkAttackTunables* tunables =
        fn_710009d50c(object->vf_0x0(), fn_71004add20()) ? reinterpret_cast<UnkAttackTunables*>(object) : nullptr;
    f32 ratio = distance / *reinterpret_cast<volatile f32*>(&tunables->field_0x124);
    f32 t = !(ratio <= 0.0f) ? (ratio >= 1.0f ? 1.0f : ratio) : 0.0f;
    f32 from = *reinterpret_cast<volatile f32*>(&tunables->field_0x11c);
    f32 until = *reinterpret_cast<volatile f32*>(&tunables->field_0x120);
    f32 speed = (from + (until - from) * t) * scale;
    f32 step = speed < distance ? speed : distance;
    if (distance != 0.0f) {
        f32 inv = 1.0f / distance;
        CVector3D direction = {dx * inv, dy * inv, dz * inv};
        f32 mx = direction.x * step;
        f32 my = direction.y * step;
        f32 mz = direction.z * step;
        position->x += mx;
        position->y += my;
        position->z += mz;
    }
}

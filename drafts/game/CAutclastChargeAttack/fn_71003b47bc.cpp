// CAutclastChargeAttack (tools/tu_map.py): whether the charge may start. After the base check, the
// owner's movement direction (+0x218, fn_71010c9a1c) must be close to an axis, the owner's coordinate
// along its facing axis must lie near the middle of a 100-unit cell (within twice g_unk_7101a2adf8 of
// 40), and both of the attack's own checks must pass; then the owner's position, moved to the middle of
// its cell along the axis it moves on, must have a clear path along its up axis (fn_71003b514c).
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "game/CAttack.h"
#include "game/UnkActor.h"
#include "game/UnkTU2523.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkVec2f_A5 {
    f32 x, y;
    UnkVec2f_A5(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkVec2f_A5(const UnkVec2f_A5& o) : x(o.x), y(o.y) {}
};

struct UnkEnemyMovement_A5;
UnkVec2f_A5 fn_71010c9a1c(UnkEnemyMovement_A5* self);

// A position returned in memory (x8): 16 bytes, not trivially copyable.
struct UnkActorPosition {
    f32 x, y, z, w;
    UnkActorPosition(const UnkActorPosition& other);
};

struct UnkAutclastBody_ovb0088;
bool fn_71003b514c(UnkAutclastBody_ovb0088* actor, const CVector2D* from, const CVector2D* to);

bool fn_71003b49b8(CAttack* self);
bool fn_71003b4c7c(CAttack* self);

extern f32 g_unk_7101a2adf8;

inline const UnkMatrix44& Matrix(UnkActor* actor) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
}

// The matrix's second column: the actor's up axis.
inline CVector3D UpAxis(UnkActor* actor) {
    const UnkMatrix44& m = Matrix(actor);
    return CVector3D{m.m[1], m.m[5], m.m[9]};
}

// Whether the owner moves along an axis: its movement direction's x near 0 or near 1.
inline bool MovesAlongAxis(UnkActor* owner) {
    UnkVec2f_A5 move = fn_71010c9a1c(static_cast<UnkEnemyMovement_A5*>(owner->field_0x218));
    f32 along = __builtin_fabsf(move.x);
    return along < 0.01f || along > 0.99f;
}

inline s32 Sign(s32 v) {
    return (v > 0) - (v < 0);
}

// Moves a coordinate to the middle of its 100-unit cell.
inline void SnapToCellMiddle(f32* c) {
    s32 cell = (s32)*c / 100 * 100;
    *c = (f32)(cell + Sign(cell) * 50);
}

bool fn_71003b47bc(CAttack* self, void* arg, u32 mode) {
    if (!fn_7100360b84(self, arg, mode))
        return false;

    if (!MovesAlongAxis(self->field_0x8))
        return false;

    const UnkMatrix44& m = Matrix(self->field_0x8);
    UnkActor* owner = self->field_0x8;
    const f32& coordinate = __builtin_fabsf(m.m[2]) > 0.9 ? owner->field_0x44 : owner->field_0x54;
    s32 whole = (s32)coordinate;
    s32 offset = (s32)__builtin_fabsf((f32)whole) % 100 - 40;
    if (!(__builtin_fabsf((f32)offset) < 2.0f * g_unk_7101a2adf8))
        return false;
    if (!fn_71003b49b8(self))
        return false;
    if (!fn_71003b4c7c(self))
        return false;

    CVector2D up;
    {
        CVector3D axis = UpAxis(self->field_0x8);
        fn_710007b430(&up, &axis);
    }
    UnkActorPosition position = fn_7100d7b7a8(self->field_0x8);
    CVector2D from;
    fn_710007b430(&from, reinterpret_cast<const CVector3D*>(&position));
    if (__builtin_fabsf(up.x) > 0.95f)
        SnapToCellMiddle(&from.y);
    else if (__builtin_fabsf(up.y) > 0.95f)
        SnapToCellMiddle(&from.x);
    return fn_71003b514c(reinterpret_cast<UnkAutclastBody_ovb0088*>(self->field_0x8), &from, &up);
}

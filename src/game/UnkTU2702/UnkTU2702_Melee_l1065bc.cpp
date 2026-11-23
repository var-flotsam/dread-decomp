// Unit 2702 (hint CAIComponent): picks a melee attack: when the target is close enough along x to
// the path end the owner faces and close enough in space, the request gets the melee id
// ("melee2m", or "meleesuccess2m" / "meleerunsuccessbigai" for mode 2). Names and layouts are
// placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "base/math/UnkSegment2D.h"
#include "game/CCharacterMovement.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkMeleeRequest_l1065bc.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

struct UnkMeleeAttack_l1065bc {
    u8 pad_0x0[0x19b];
    u8 run_success;  // 0x19b
    u8 pad_0x19c[0x358];
    f32 max_dx;  // 0x4f4, negative: the two radii
    f32 max_distance;  // 0x4f8, negative: the two radii
};
static_assert(__builtin_offsetof(UnkMeleeAttack_l1065bc, max_dx) == 0x4f4, "UnkMeleeAttack_l1065bc layout");

struct UnkMeleeSelf_l1065bc {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x30];
    CVector3D* points;  // 0x68
    u32 count;          // 0x70
    u8 pad_0x74[0x20];
    u8 closed;  // 0x94
    u8 pad_0x95[0x157b];
    UnkMeleeAttack_l1065bc* attack;  // 0x1610

    // The path's points other than a closing one.
    u32 Open() const {
        bool open = closed == 0;
        return (count != 0) & open;
    }
};
static_assert(__builtin_offsetof(UnkMeleeSelf_l1065bc, attack) == 0x1610, "UnkMeleeSelf_l1065bc layout");

static inline const CVector2D& Pt_l1065bc(const CVector3D* p) {
    return *reinterpret_cast<const CVector2D*>(p);
}

// x of the path's first point.
static inline f32 FirstX_l1065bc(const UnkMeleeSelf_l1065bc* self) {
    CVector3D* points = self->points;
    UnkSegment2D seg(Pt_l1065bc(points), Pt_l1065bc(self->count == 1 ? points : points + 1));
    return seg.start.x;
}

// x of the end of the path's last segment, 0 when there is none.
static inline f32 LastX_l1065bc(const UnkMeleeSelf_l1065bc* self) {
    u32 n = self->count - self->Open();
    if (n == 0)
        return 0.0f;
    u32 last = n - 1;
    CVector3D* points = self->points;
    UnkSegment2D seg(Pt_l1065bc(&points[last]), Pt_l1065bc(self->Open() ? &points[n] : &points[0]));
    return seg.end.x;
}

static inline f32 Radius(UnkActor* actor) {
    if (actor->field_0x1f8)
        return fn_7100977b20(actor->field_0x1f8);
    return actor->field_0x37c;
}

static inline const UnkMatrix44& Matrix(UnkActor* actor) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
}

static inline CVector3D Position(const UnkActor* actor) {
    return CVector3D{actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

void fn_71011065bc(UnkMeleeSelf_l1065bc* self, s32 mode, UnkMeleeRequest_l1065bc* request, bool* out) {
    static CStrId s_melee("melee2m", true);
    static CStrId s_success("meleesuccess2m", true);
    static CStrId s_run_success("meleerunsuccessbigai", true);

    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target || !self->attack)
        return;

    f32 max_dx = self->attack->max_dx;
    if (max_dx < -0.001f)
        max_dx = Radius(target) + Radius(self->owner);
    f32 max_distance = self->attack->max_distance;
    if (max_distance < -0.001f)
        max_distance = Radius(target) + Radius(self->owner);

    void* movement = self->owner->field_0x218;
    if (!movement || fn_71010cc3a4(movement) != 1)
        return;

    f32 x;
    if (Matrix(self->owner).m[2] > 0) {
        if (self->count == self->Open())
            return;
        x = FirstX_l1065bc(self);
    } else {
        if (self->count == self->Open())
            return;
        x = LastX_l1065bc(self);
    }
    UnkActor* owner = self->owner;
    f32 dx = __builtin_fabsf(owner->field_0x44 - x);
    CVector3D target_pos = Position(target);
    CVector3D owner_pos = Position(owner);
    // The original fetches the owner's matrix once more here and does not use it.
    Matrix(owner);
    if (!(dx < max_dx))
        return;
    if (!(base::math::LengthSq(target_pos - owner_pos) < max_distance * max_distance))
        return;

    if (mode == 2) {
        request->id = s_success;
        request->key = s_success.data->Key();
        if (self->attack && self->attack->run_success) {
            request->id = s_run_success;
            request->key = s_run_success.data->Key();
        }
    } else {
        request->id = s_melee;
        request->key = s_melee.data->Key();
    }
    request->field_0x24 = 1;
    *out = false;
}

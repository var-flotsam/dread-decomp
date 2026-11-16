// Unit 110 (2D geometry): a moving box against a still one, as its centre swept against the still box
// grown by its half size; the contact point is moved back onto the moving box's face and kept inside
// the still box. Names and layouts are placeholders.
#include "base/math/CVectorOps.h"
#include "game/malloc.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkSweep2D {
    CVector2D start;
    CVector2D end;
    CVector2D delta;
};

struct UnkBox2D {
    f32 x0;
    f32 y0;
    f32 x1;
    f32 y1;
};

struct UnkContact2D {
    CVector3D point;
    CVector3D normal_a;
    CVector3D normal_b;
    f32 param;
    u32 flags;

    void SetPoint(f32 x, f32 y) {
        point.x = x;
        point.y = y;
        point.z = 0.0f;
        flags |= 8;
    }
    void SetParam(f32 t) {
        param = t;
        flags |= 0x200;
    }
    void SetNormalA(const CVector2D& n) {
        normal_a.x = n.x;
        normal_a.y = n.y;
        normal_a.z = 0.0f;
        flags |= 0x20;
    }
    void SetNormalB(const CVector2D& n) {
        normal_b.x = n.x;
        normal_b.y = n.y;
        normal_b.z = 0.0f;
        flags |= 0x10;
    }
};

// One contact point (0x2c bytes): the position, two vectors that belong to the first and the
// second shape of the pair (swapped when the pair is reported the other way round), and flags.
struct UnkContactPoint {
    CVector3D pos;
    CVector3D field_0xc;
    CVector3D field_0x18;
    u32 field_0x24;
    u32 flags;

    UnkContactPoint() = default;
    // Copied word by word (fn_7100d574c4 too).
    UnkContactPoint(const UnkContactPoint& o)
        : pos{o.pos.x, o.pos.y, o.pos.z}, field_0xc{o.field_0xc.x, o.field_0xc.y, o.field_0xc.z},
          field_0x18{o.field_0x18.x, o.field_0x18.y, o.field_0x18.z}, field_0x24(o.field_0x24), flags(o.flags) {}
};
static_assert(sizeof(UnkContactPoint) == 0x2c, "UnkContactPoint layout");

static inline CVector2D Vec(f32 x, f32 y) {
    CVector2D v;
    v.x = x;
    v.y = y;
    return v;
}

static inline const f32& Max(const f32& a, const f32& b) {
    return a > b ? a : b;
}
static inline const f32& Min(const f32& a, const f32& b) {
    return a < b ? a : b;
}

bool fn_710012dbd0(const UnkBox2D* a, void* x5, const void* b, UnkContactPoint* out) {
    const CVector2D* move = static_cast<const CVector2D*>(x5);
    const UnkBox2D* target = static_cast<const UnkBox2D*>(b);
    UnkContact2D* contact = reinterpret_cast<UnkContact2D*>(out);

    CVector2D lo = Vec(a->x0, a->y0);
    CVector2D hi = Vec(a->x1, a->y1);
    CVector2D center = (lo + hi) * 0.5f;
    CVector2D half = (hi - lo) * 0.5f;

    CVector2D grownLo = Vec(target->x0, target->y0) - half;
    CVector2D grownHi = half + Vec(target->x1, target->y1);
    UnkBox2D grown;
    grown.x0 = grownLo.x;
    grown.y0 = grownLo.y;
    grown.x1 = grownHi.x;
    grown.y1 = grownHi.y;
    UnkSweep2D sweep;
    sweep.end = center + *move;
    sweep.start = center;
    sweep.delta = sweep.end - sweep.start;
    if (!fn_710012b7f0(&sweep, &grown, contact, 2)) {
        return false;
    }

    // Back from the centre onto the face that was hit.
    if (contact->normal_b.x < 0.0f || contact->normal_b.x > 0.0f) {
        contact->SetPoint(half.x + contact->point.x, contact->point.y + half.x * (move->y / move->x));
    } else {
        contact->SetPoint(contact->point.x + half.y * (move->x / move->y), half.y + contact->point.y);
    }
    contact->normal_a = -contact->normal_b;
    contact->flags |= 0x20;

    // Kept inside the still box.
    contact->SetPoint(Max(contact->point.x, target->x0), Max(contact->point.y, target->y0));
    contact->SetPoint(Min(contact->point.x, target->x1), Min(contact->point.y, target->y1));
    return true;
}

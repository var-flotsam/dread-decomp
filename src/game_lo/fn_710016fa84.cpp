// Builds two axes perpendicular to an up vector: either from a rotation matrix around that
// up vector (columns 0/1, re-orthogonalised with cross products) or from fixed unit axes.
#include <math.h>

#include "base/math/CVector.h"

using base::math::CVector3D;

struct UnkMatrix_710016fa84_0x40 {
    f32 m[4][4];
};

struct UnkOwner_710016fa84 {
    u8 pad[0x438];
    bool field_0x438;
    bool field_0x439;
};

extern const CVector3D unk_71016481b0 __attribute__((visibility("hidden")));
extern const CVector3D unk_71016481bc __attribute__((visibility("hidden")));

void fn_7100078df0(UnkMatrix_710016fa84_0x40* out, f32 angle);
void fn_7100079584(const CVector3D* axis, UnkMatrix_710016fa84_0x40* out, f32 angle);
void fn_710007a620(const CVector3D* axis, const CVector3D* rot, CVector3D* out);

static inline void Cross(CVector3D* out_, const CVector3D* a_, const CVector3D* b_) {
    f32* out = &out_->x;
    const f32* a = &a_->x;
    const f32* b = &b_->x;
    out[0] = a[1] * b[2] - a[2] * b[1];
    out[1] = a[2] * b[0] - a[0] * b[2];
    out[2] = a[0] * b[1] - a[1] * b[0];
}

static inline void Normalize(CVector3D* v) {
    f32 len = sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
    f32 inv = 1.0f / (len + 0.0001f);
    f32 x = v->x, y = v->y, z = v->z;
    v->x = x * inv;
    v->y = y * inv;
    v->z = inv * z;
}

// Fills the rotation matrix around the up vector, if this owner uses one.
static inline bool BuildMatrix(const UnkOwner_710016fa84* self, const CVector3D* up,
                               const CVector3D* rot, UnkMatrix_710016fa84_0x40* m) {
    if (self->field_0x438 && self->field_0x439) {
        f32 angle = rot->z;
        if (up->z < -0.8f)
            fn_7100078df0(m, -angle);
        else
            fn_7100079584(up, m, angle);
        return true;
    }
    if (self->field_0x439) {
        fn_7100079584(up, m, rot->z);
        return true;
    }
    return false;
}

void fn_710016fa84(UnkOwner_710016fa84* self, const CVector3D* up, const CVector3D* rot,
                   CVector3D* side, CVector3D* fwd) {
    {
        UnkMatrix_710016fa84_0x40 m;
        if (BuildMatrix(self, up, rot, &m)) {
            if (self->field_0x438) {
                side->x = m.m[0][1];
                side->y = m.m[1][1];
                side->z = m.m[2][1];
                Cross(fwd, side, up);
                Normalize(fwd);
            } else {
                fwd->x = m.m[0][0];
                fwd->y = m.m[1][0];
                fwd->z = m.m[2][0];
            }
            Cross(side, up, fwd);
            return;
        }
    }
    fn_710007a620(&unk_71016481b0, rot, side);
    if (self->field_0x438) {
        Cross(fwd, side, up);
        Normalize(fwd);
    } else {
        fn_710007a620(&unk_71016481bc, rot, fwd);
    }
}

// CCameraBoundaryCtrl unit: the four corners of the view a camera at `pos` looking along `dir` with the
// given field of view has where it meets the plane through `plane`, optionally inset by a margin. Flat
// views look straight down the z axis. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVectorOps.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU311.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

f32 fn_71000764a0(const void* a, const void* b, void* c, void* d, const void* e, void* f);
u32 fn_7100110cd4(const CVector3D* a, const CVector3D* b, const CVector3D* planePoint,
                  const CVector3D* normal, CVector3D* out);

// The plane's normal.
__attribute__((visibility("hidden"))) extern const CVector3D g_unk_710164aa78;

static const f64 kPi = 3.14159265358979323846;

// How far the inset view stays inside each side and the top and bottom.
static const CVector2D kInsetMargin = {40.0f, 24.0f};
static const CVector2D kNoMargin = {0.0f, 0.0f};

inline f32 ViewMax_l7654b0(f32 a, f32 b) {
    return a > b ? a : b;
}

inline f32 ViewMin_l7654b0(f32 a, f32 b) {
    return a < b ? a : b;
}

// The corners of the rectangle the view covers at `distance` around `center`, `right` and `up` spanning it.
inline void ViewRectangle_l7654b0(const CVector3D& center, const CVector3D& right, const CVector3D& up,
                                  f32 distance, f32 half_fov, const CVector2D& margin, CVector3D* c0,
                                  CVector3D* c1, CVector3D* c2, CVector3D* c3) {
    f32 half_w = tanf(half_fov) * distance - margin.x;
    f32 scale = fn_7100132fa0();
    f32 vertical_fov = fn_7100217ee0((f32)(half_fov * 180.0f / kPi), scale);
    f32 half_h = distance * tanf((f32)(vertical_fov * kPi / 180.0)) - margin.y;
    CVector3D across = right * half_w;
    CVector3D along = up * half_h;
    *c0 = center + across + along;
    *c1 = center - across + along;
    *c2 = center + across - along;
    *c3 = center - across - along;
}

// Moves a corner to where the ray from `pos` through it crosses the plane.
inline void OntoPlane_l7654b0(const CVector3D* pos, const CVector3D* plane, CVector3D* corner) {
    CVector3D beyond = *corner + (*corner - *pos);
    fn_7100110cd4(pos, &beyond, plane, &g_unk_710164aa78, corner);
}

void fn_71007654b0(const CVector3D* pos, f32 fov, const CVector3D* dir, const CVector3D* plane, CVector3D* c0,
                   CVector3D* c1, CVector3D* c2, CVector3D* c3, bool flat, bool inset) {
    CVector2D margin = inset ? kInsetMargin : kNoMargin;
    f32 half_fov = (f32)(fov * kPi / 180.0) * 0.5f;

    if (flat) {
        f32 distance = pos->z - plane->z;
        CVector3D center = {pos->x, pos->y, plane->z};
        ViewRectangle_l7654b0(center, CVector3D{1.0f, 0.0f, 0.0f}, CVector3D{0.0f, 1.0f, 0.0f}, distance,
                              half_fov, margin, c0, c1, c2, c3);
        return;
    }

    CVector3D probe = *dir * 500.0f + *pos;
    CVector3D hit = {};
    fn_71000764a0(pos, &probe, const_cast<CVector3D*>(plane), &hit, nullptr, nullptr);
    f32 distance = Distance(hit, *pos);
    CVector3D right = Cross(*dir, CVector3D{0.0f, 1.0f, 0.0f});
    CVector3D up = Cross(right, *dir);
    ViewRectangle_l7654b0(hit, right, up, distance, half_fov, margin, c0, c1, c2, c3);

    OntoPlane_l7654b0(pos, plane, c0);
    OntoPlane_l7654b0(pos, plane, c1);
    OntoPlane_l7654b0(pos, plane, c2);
    OntoPlane_l7654b0(pos, plane, c3);

    c0->y = c1->y = ViewMax_l7654b0(c0->y, c1->y);
    c2->y = c3->y = ViewMin_l7654b0(c2->y, c3->y);
    c0->x = c2->x = ViewMax_l7654b0(c0->x, c2->x);
    c1->x = c3->x = ViewMax_l7654b0(c1->x, c3->x);
}

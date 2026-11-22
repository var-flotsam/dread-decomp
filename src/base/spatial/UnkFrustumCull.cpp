// From unit 244 of tools/tu_map.py (0x71001b76ec-0x71001bb60c, no reflected class): a view
// frustum's six planes (at +0x178 of the view object) tested against a bounding sphere. The sphere
// remembers the plane that last rejected it and the test starts there, walking the planes in the
// order a next-plane table gives. Names and layouts are placeholders.
// Holds fn_71001bab30 and the frustum's update, fn_71001b83f8.
#include "game/UnkPoolBox.h"
#include "game/UnkTU204.h"
#include "game/UnkTU225.h"
#include "game/UnkTU244.h"
#include "game/UnkTU308.h"
#include "types.h"

// A plane: its normal and its distance term.
struct UnkPlane {
    f32 x;
    f32 y;
    f32 z;
    f32 d;
};

struct UnkFrustumView {
    u8 pad_0x0[0x178];
    UnkPlane planes[6];
};

// A plane's index (0..5). An enum type: its stores don't alias the sphere's floats.
enum class UnkPlaneIndex : u8 {};

struct UnkBoundSphere {
    u8 pad_0x0[0x8];
    f32 radius;
    u8 pad_0xc[0xc];
    f32 center_x;
    f32 center_y;
    f32 center_z;
    u8 pad_0x24[0x29];
    UnkPlaneIndex plane;  // the plane the test starts with
};
static_assert(__builtin_offsetof(UnkBoundSphere, plane) == 0x4d, "UnkBoundSphere placeholder layout");

// The plane tested after each plane.
extern const u8 g_unk_71015e9322[6] __attribute__((visibility("hidden")));

// 1 when the sphere is outside a plane, -1 when it is inside all six (or too small to test),
// 0 when it crosses one. The sphere's start plane moves on with every plane tested.
// NON_MATCHING(minor): the original computes the fifth plane's inside test (cset) first; the same
// six compares and ands, scheduled in another order.
s32 fn_71001bab30(const UnkFrustumView* self, UnkBoundSphere* sphere) {
    f32 r = sphere->radius;
    if (r < 0.0001f)
        return -1;
    u32 index = (u32)sphere->plane;
    bool inside = true;
    for (u32 i = 0; i < 6; ++i) {
        const UnkPlane& p = self->planes[index];
        f32 dist = p.x * sphere->center_x + p.y * sphere->center_y + p.z * sphere->center_z + p.d;
        if (dist > sphere->radius)
            return 1;
        if (dist > -r)
            inside = false;
        index = g_unk_71015e9322[index];
        sphere->plane = (UnkPlaneIndex)index;
    }
    return inside ? -1 : 0;
}

// ---- The view's cull object (fn_71001b7a60) ----

#include "base/global/CRntDictionary.h"
#include "base/global/CRntVector.h"
#include "base/math/CVectorOps.h"
#include "game/UnkSphere.h"

// The registry every view's cull object is entered in: a chained hash set of pointers at +8 (the
// layout of base/UnkHashMap.h: buckets indexed by the key's low 32 bits modulo the capacity, and a
// list of all nodes). src/game_lo/fn_710019e100.cpp removes a key from it.
struct UnkKeyOwner;

struct UnkCullRegistry {
    u8 pad_0x0[0x8];
    base::global::CRntDictionary<u64, u64> objects;  // +0x8: each object its own value
};

// The 0xb8-byte cull object.
struct UnkCullObject;
void fn_71001691ac(UnkCullObject* self);  // its constructor

// ---- The view's constructor (fn_71001b76ec) ----

// The box's base constructor, and the vtable of the box class (GOT 0x7101cc7410).
void fn_7100215dcc(UnkPoolBox* self, const UnkVec3f* min, const UnkVec3f* max);
extern const char g_unk_7101a20090[];

// A box and its corners quantised to tenths (0x38 bytes).
struct UnkViewBox {
    UnkPoolBox box;
    s32 cellMin[3];  // 0x20
    s32 cellMax[3];  // 0x2c
};
static_assert(sizeof(UnkViewBox) == 0x38, "UnkViewBox layout");

// Constructors of three small members (other units).
void fn_710016931c(void* self);  // +0x128 (0x24 bytes)
void fn_71001695ec(void* self);  // +0x14c (0x10 bytes)
void fn_710016947c(void* self);  // +0x15c (0x14 bytes)

extern "C" f32 sinf(f32);
extern "C" f32 cosf(f32);

// The view object (UnkFrustumView above: the frustum planes at +0x178). The defaults suggest a
// camera: a 45-degree field of view at +0x80, near/far-like distances (100, 100000, 1000) and a
// 1280x720 viewport at +0xa0.
struct UnkCullView {
    u32 field_0x0;
    u32 field_0x4;
    s32 field_0x8;
    u32 field_0xc;
    UnkCullView* field_0x10;
    u32 field_0x18;
    u32 field_0x1c;
    UnkSphere sphere;  // 0x20
    bool field_0x70;
    f32 field_0x74;
    f32 field_0x78;
    f32 field_0x7c;
    f32 fov;  // 0x80, degrees
    f32 field_0x84;
    f32 field_0x88;
    f32 field_0x8c;
    f32 field_0x90;
    f32 field_0x94;
    f32 field_0x98;
    f32 field_0x9c;
    u32 width;   // 0xa0
    u32 height;  // 0xa4
    u64 field_0xa8;
    u64 field_0xb0;
    u32 field_0xb8;
    f32 field_0xbc;
    f32 field_0xc0;
    f32 field_0xc4;
    f32 field_0xc8;
    f32 field_0xcc;
    f32 field_0xd0;
    f32 field_0xd4;
    f32 field_0xd8;
    f32 field_0xdc;
    f32 field_0xe0;
    f32 field_0xe4[14];  // 0xe4
    u32 field_0x11c;
    f32 field_0x120;
    f32 field_0x124;
    u8 field_0x128[0x24];
    u8 field_0x14c[0x10];
    u8 field_0x15c[0x14];
    UnkCullObject* cull;  // 0x170, entered in g_unk_716e34c2e0
    UnkPlane planes[6];   // 0x178
    u8 pad_0x1d8[0x90];
    f32 sinHalfFov;     // 0x268
    f32 cosHalfFov;     // 0x26c
    f32 invCosHalfFov;  // 0x270
    f32 cosHalfFovSq;   // 0x274
    f32 tanHalfFov;     // 0x278
    u16 field_0x27c;
    u8 field_0x27e;
    void* field_0x280[12];
    bool field_0x2e0;
    UnkViewBox boxes[9];  // 0x2e8
};
static_assert(__builtin_offsetof(UnkCullView, cull) == 0x170, "UnkCullView layout");
static_assert(__builtin_offsetof(UnkCullView, sinHalfFov) == 0x268, "UnkCullView layout");
static_assert(sizeof(UnkCullView) == 0x4e0, "UnkCullView layout");

// The constructor: defaults, the sphere and three members, zeroed planes, nine empty boxes with
// their quantised corners, then the half field of view's sine, cosine and tangent.
// NON_MATCHING(minor): of the fourteen 8-byte zero stores to +0x268..+0x2d8, the one to +0x270 gets
// its address register (x10) before the others where the original computes it second (in x8); same
// stores, nothing between them.
void fn_71001b76ec(UnkCullView* self) {
    self->field_0x0 = 0;
    self->field_0x4 = 0;
    self->field_0x8 = -1;
    self->field_0x18 = 0;
    self->field_0x10 = nullptr;
    fn_710021685c(&self->sphere);
    self->field_0x70 = false;
    self->field_0x74 = 1000.0f;
    self->field_0x78 = 0.0f;
    self->field_0x7c = 0.0f;
    self->fov = 45.0f;
    self->field_0x84 = 45.0f;
    self->field_0x88 = 100000.0f;
    self->field_0x8c = 100.0f;
    self->field_0x90 = 0.0f;
    self->field_0x94 = 1.0f;
    self->field_0x98 = 1.0f;
    self->field_0x9c = 1000.0f;
    self->field_0xa8 = 0;
    self->field_0xb0 = 0;
    self->field_0xb8 = 0;
    self->field_0xbc = 1.0f;
    self->field_0xc0 = 0.0f;
    self->field_0xc4 = 1.0f;
    self->field_0xc8 = 0.0f;
    self->field_0xcc = 1.0f;
    self->field_0xd0 = 0.0f;
    self->field_0xd4 = 0.0f;
    self->field_0xd8 = 0.0f;
    self->field_0xdc = 0.0f;
    self->field_0xe0 = 1.0f;
    for (f32& f : self->field_0xe4)
        f = 0.0f;
    self->field_0x11c = 0;
    self->field_0x120 = 1.0f;
    self->field_0x124 = 0.0f;
    fn_710016931c(self->field_0x128);
    fn_71001695ec(self->field_0x14c);
    fn_710016947c(self->field_0x15c);
    self->cull = nullptr;
    for (UnkPlane& p : self->planes) {
        p.x = 0.0f;
        p.y = 0.0f;
        p.z = 0.0f;
    }
    self->field_0x27c = 0;
    self->field_0x27e = 0;
    self->sinHalfFov = 0.0f;
    self->cosHalfFov = 0.0f;
    self->invCosHalfFov = 0.0f;
    self->cosHalfFovSq = 0.0f;
    self->field_0x280[0] = nullptr;
    self->field_0x280[1] = nullptr;
    self->field_0x280[2] = nullptr;
    self->field_0x280[3] = nullptr;
    self->field_0x280[4] = nullptr;
    self->field_0x280[5] = nullptr;
    self->field_0x280[6] = nullptr;
    self->field_0x280[7] = nullptr;
    self->field_0x280[8] = nullptr;
    self->field_0x280[9] = nullptr;
    self->field_0x280[10] = nullptr;
    self->field_0x280[11] = nullptr;
    self->field_0x2e0 = true;
    for (UnkViewBox& b : self->boxes) {
        UnkPoolBox* box = &b.box;
        {
            UnkVec3f min(0.0f, 0.0f, 0.0f);
            UnkVec3f max(0.0f, 0.0f, 0.0f);
            fn_7100215dcc(box, &min, &max);
        }
        *reinterpret_cast<const char**>(box) = g_unk_7101a20090 + 0x10;
        UnkVec3f min = box->GetMin();
        UnkVec3f max = box->GetMax();
        b.cellMin[0] = (s32)(min.x * 10.0f);
        b.cellMin[1] = (s32)(min.y * 10.0f);
        b.cellMin[2] = (s32)(min.z * 10.0f);
        b.cellMax[0] = (s32)(max.x * 10.0f);
        b.cellMax[1] = (s32)(max.y * 10.0f);
        b.cellMax[2] = (s32)(max.z * 10.0f);
    }
    self->field_0x0 = 8;
    self->field_0x4 = 1;
    f32 half = (f32)(self->fov * 3.141592653589793 / 180.0) * 0.5f;
    self->field_0x10 = self;
    self->sinHalfFov = sinf(half);
    f32 c = cosf(half);
    self->cosHalfFov = c;
    self->cosHalfFovSq = c * c;
    self->invCosHalfFov = 1.0f / c;
    self->width = 1280;
    self->height = 720;
    self->tanHalfFov = self->invCosHalfFov * self->sinHalfFov;
}

// Replaces the view's cull object with a new one, entered in the registry.
void fn_71001b7a60(UnkCullView* self) {
    if (self->cull) {
        fn_710019e100(static_cast<UnkKeyOwner*>(g_unk_716e34c2e0), (u64)self->cull);
        self->cull = nullptr;
    }
    auto* registry = static_cast<UnkCullRegistry*>(g_unk_716e34c2e0);
    auto* cull = static_cast<UnkCullObject*>(fn_71000839a0(0xb8, 0x34));
    fn_71001691ac(cull);
    if (cull)
        registry->objects.InsertChecked((u64)cull, (u64)cull);
    self->cull = cull;
}

// ---- fn_71001ba864: aiming the view at a point ----

#include "game/UnkXformNode.h"

// The same view as the eye, its direction and its target: what fn_71001ba864 sets (the transform
// node of game/UnkXformNode.h hands it its target position).
struct UnkLookAtView {
    u8 pad_0x0[0x9c];
    f32 distance;  // 0x9c, at least 1
    u8 pad_0xa0[0x8];
    UnkVec3f eye;  // 0xa8
    UnkVec3f dir;  // 0xb4, unit length
    u8 pad_0xc0[0x18];
    UnkVec3f target;  // 0xd8
    u8 pad_0xe4[0x198];
    bool field_0x27c;
    u8 pad_0x27d;
    bool field_0x27e;  // with field_0x27c clear, the view keeps its aim
};
static_assert(__builtin_offsetof(UnkLookAtView, field_0x27e) == 0x27e, "UnkLookAtView placeholder layout");

void fn_71001b7eb4(UnkLookAtView* self);  // recomputes the view from the aim

// Aims the view at `pos`: the direction from the eye, its length (at least 1) and the target.
void fn_71001ba864(UnkXformTarget* target, const UnkVec3f* pos) {
    auto* self = reinterpret_cast<UnkLookAtView*>(target);
    if (self->field_0x27e && !self->field_0x27c)
        return;
    f32 dx = pos->x - self->eye.x;
    f32 dy = pos->y - self->eye.y;
    f32 dz = pos->z - self->eye.z;
    self->dir.x = dx;
    self->dir.y = dy;
    self->dir.z = dz;
    f32 d = sqrtf(dx * dx + dy * dy + dz * dz);
    self->distance = fmaxf(d, 1.0f);
    f32 inv = 1.0f / (sqrtf(self->dir.x * self->dir.x + self->dir.y * self->dir.y + self->dir.z * self->dir.z) +
                      0.0001f);
    f32 x = self->dir.x * inv;
    f32 y = self->dir.y * inv;
    f32 z = inv * self->dir.z;
    self->dir.x = x;
    self->dir.y = y;
    self->dir.z = z;
    self->target.x = pos->x;
    self->target.y = pos->y;
    self->target.z = pos->z;
    fn_71001b7eb4(self);
}

// ---- The frustum's update (fn_71001b83f8) ----

namespace {

using base::math::CVector3D;

// The view's fields this update reads and writes, by offset (the constructor's layout above keeps
// them as raw words): the eye at +0xa8, its forward, right and up axes after it, the corners of the
// two cross-sections at +0x280 (the one at +0x8c's distance) and +0x2b0 (+0x88's), the quantised
// planes at +0x1d8 and the orthographic size at +0x15c.
inline CVector3D& Vec(UnkCullView* v, u32 offset) { return *reinterpret_cast<CVector3D*>(reinterpret_cast<u8*>(v) + offset); }
inline f32 Word(const UnkCullView* v, u32 offset) {
    return *reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(v) + offset);
}

struct UnkQuantPlane {
    s32 x, y, z;  // the normal, in thousandths
    u32 pad_0xc;
    s64 d;        // the distance term, in ten-thousandths
};
static_assert(sizeof(UnkQuantPlane) == 0x18, "UnkQuantPlane placeholder layout");

// Rounded toward zero by floor or ceiling, as the original does.
inline s32 Quantise(f32 v, f32 scale) {
    f32 x = v * scale;
    return (s32)(x > 0.0f ? floorf(x) : ceilf(x));
}

// A cross-section's four corners at the given distance along the view's forward axis, from the
// right and up axes scaled to the half sizes (r, u): (+r, -u), (+r, +u), (-r, -u), (-r, +u).
__attribute__((always_inline)) inline void Corners(UnkCullView* self, CVector3D* out, f32 distance, CVector3D r, CVector3D u) {
    CVector3D eye = Vec(self, 0xa8);
    CVector3D forward = Vec(self, 0xb4);
    CVector3D c = {eye.x + forward.x * distance, eye.y + forward.y * distance, eye.z + forward.z * distance};
    CVector3D cr = c + r;
    CVector3D cl = c - r;
    out[0] = cr - u;
    out[1] = u + cr;
    out[2] = cl - u;
    out[3] = u + cl;
}

// The plane through p0, a and b (its normal cross(a - p0, b - p0), normalised).
__attribute__((always_inline)) inline void PlaneFrom(UnkPlane* plane, const CVector3D& p0, const CVector3D& a, const CVector3D& b) {
    CVector3D v1 = a - p0;
    CVector3D v2 = b - p0;
    f32 nx = v1.y * v2.z - v1.z * v2.y;
    f32 ny = v1.z * v2.x - v1.x * v2.z;
    f32 nz = v1.x * v2.y - v1.y * v2.x;
    plane->x = nx;
    plane->y = ny;
    plane->z = nz;
    f32 inv = 1.0f / (sqrtf(nx * nx + ny * ny + nz * nz) + 0.0001f);
    plane->x = plane->x * inv;
    plane->y = plane->y * inv;
    plane->z = plane->z * inv;
    plane->d = -(plane->x * p0.x + plane->y * p0.y + plane->z * p0.z);
}

}  // namespace

// The corners, planes and boxes from the two cross-sections' distances and half sizes (inlined
// into both of the update's branches).
__attribute__((always_inline)) static inline void Rebuild(UnkCullView* self, CVector3D* near, CVector3D* far, f32 near_d,
                                                          f32 far_d, CVector3D near_r, CVector3D near_u, CVector3D far_r, CVector3D far_u) {
    Corners(self, near, near_d, near_r, near_u);
    Corners(self, far, far_d, far_r, far_u);

    PlaneFrom(&self->planes[0], far[0], near[1], near[0]);
    PlaneFrom(&self->planes[1], far[2], near[2], near[3]);
    PlaneFrom(&self->planes[2], far[2], near[0], near[2]);
    PlaneFrom(&self->planes[3], far[1], near[3], near[1]);
    PlaneFrom(&self->planes[4], near[0], near[1], near[3]);
    PlaneFrom(&self->planes[5], far[1], far[0], far[2]);
    auto* quant = reinterpret_cast<UnkQuantPlane*>(reinterpret_cast<u8*>(self) + 0x1d8);
    for (u32 i = 0; i < 6; ++i) {
        quant[i].x = Quantise(self->planes[i].x, 1000.0f);
        quant[i].y = Quantise(self->planes[i].y, 1000.0f);
        quant[i].z = Quantise(self->planes[i].z, 1000.0f);
        quant[i].d = Quantise(self->planes[i].d, 10000.0f);
    }

    // The frustum's four edges, from the far corners to the near ones, cut into nine slabs.
    CVector3D step[4];
    for (u32 k = 0; k < 4; ++k)
        step[k] = (near[k] - far[k]) * 0.11111111f;
    for (u32 i = 0; i < 9; ++i) {
        CVector3D points[8];
        for (u32 k = 0; k < 4; ++k) {
            points[k] = step[k] * (f32)i + far[k];
            points[4 + k] = step[k] * (f32)(i + 1) + far[k];
        }
        UnkPoolBox* box = &self->boxes[i].box;
        {
            UnkVec3f lo(__FLT_MAX__, __FLT_MAX__, __FLT_MAX__);
            UnkVec3f hi(-__FLT_MAX__, -__FLT_MAX__, -__FLT_MAX__);
            box->SetMin(&lo);
            box->SetMax(&hi);
        }
        // The box's value wins a comparison with a NaN.
        for (u64 k = 0; k < 8; ++k) {
            const CVector3D& p = points[k];
            {
                UnkVec3f m = box->GetMin();
                UnkVec3f grown(m.x < p.x ? m.x : p.x, m.y < p.y ? m.y : p.y, m.z < p.z ? m.z : p.z);
                box->SetMin(&grown);
            }
            {
                UnkVec3f m = box->GetMax();
                UnkVec3f grown(m.x > p.x ? m.x : p.x, m.y > p.y ? m.y : p.y, m.z > p.z ? m.z : p.z);
                box->SetMax(&grown);
            }
        }
    }
}

// Rebuilds the frustum from the view: the two cross-sections' corners (perspective, from the
// distances at +0x88 and +0x8c and the half-size factors at +0x78/+0x7c; orthographic when the cull
// object's flag at +0xb0 is set, from the size at +0x15c), the six planes and their quantised copies,
// then the nine boxes, each bounding a ninth of the frustum along its depth.
// NON_MATCHING(minor): the same blocks, calls, arithmetic, compares, constants and stores (compared
// block by block), 6980 bytes against 7024: the original reloads more of the stored corners and
// plane terms instead of keeping them in registers (and multiplies one reloaded plane distance by
// 10000 where we fold its negation into -10000), so the scheduling and registers differ.
void fn_71001b83f8(UnkCullView* self) {
    CVector3D* far = &Vec(self, 0x280);
    CVector3D* near = &Vec(self, 0x2b0);
    if (reinterpret_cast<const u8*>(self->cull)[0xb0]) {
        f32 near_d = self->field_0x88;
        // The size times the axis, halved.
        CVector3D r = Vec(self, 0xc0) * Word(self, 0x160) * 0.5f;
        CVector3D u = Vec(self, 0xcc) * Word(self, 0x15c) * 0.5f;
        Rebuild(self, near, far, near_d, self->field_0x8c, r, u, r, u);
    } else {
        f32 near_d = self->field_0x88;
        f32 far_d = self->field_0x8c;
        Rebuild(self, near, far, near_d, far_d, Vec(self, 0xc0) * (near_d * self->field_0x78),
                Vec(self, 0xcc) * (near_d * self->field_0x7c), Vec(self, 0xc0) * (self->field_0x78 * far_d),
                Vec(self, 0xcc) * (self->field_0x7c * far_d));
    }
}

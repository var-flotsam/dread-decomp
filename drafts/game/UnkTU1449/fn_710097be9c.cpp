// Unit 1449: whether the body can take a step along a level surface: not while its state is 3; yes
// when the surface direction is not level, or when the target is behind the surface or the surface
// faces up; otherwise when the navigation mesh has a walkable cell behind the surface's point (or above
// or below it, on the target's side), which it then remembers. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU1450.h"
#include "game/UnkXformNode.h"
#include "types.h"

// The target: a position source with a virtual 0x188 that gives its position.
class UnkTarget_l97be9c {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual UnkVec3f vf_0x188();
};

// The owner: a 3x4 transform at +0x38 whose translation is read.
struct UnkOwner_l97be9c {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkState_l97be9c {
    u8 pad_0x0[0x10];
    s32 id;  // 0x10
};

struct UnkSelf_l97be9c {
    u8 pad_0x0[0x40];
    UnkState_l97be9c* state;  // 0x40
    u8 pad_0x48[0x278 - 0x48];
    bool stepped;  // 0x278
};

// A surface: a point on it, its normal and its direction.
struct UnkSurface_l97be9c {
    u8 pad_0x0[0x14];
    base::math::CVector3D point;   // 0x14
    base::math::CVector3D normal;  // 0x20
    base::math::CVector3D dir;     // 0x2c
};

static inline bool IsAbout(f32 v, f32 target) {
    return v - 0.1f <= target && v + 0.1f >= target;
}

static inline f32 Dot(const base::math::CVector3D& a, const base::math::CVector3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline base::math::CVector3D Sub(const UnkVec3f& a, const UnkOwner_l97be9c* b) {
    base::math::CVector3D r;
    r.x = a.x - b->x;
    r.y = a.y - b->y;
    r.z = a.z - b->z;
    return r;
}

static inline base::math::CVector2D Offset(const base::math::CVector2D& p, f32 dx, f32 dy) {
    base::math::CVector2D r;
    r.x = p.x + dx;
    r.y = p.y + dy;
    return r;
}

static inline UnkNavMeshManager* NavManager() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

static inline bool IsWalkable(UnkNavMesh* mesh, const base::math::CVector2D& p) {
    u16 cell = fn_710027a41c(mesh, &p, 0);
    return cell != 0xffff && fn_71002b005c(fn_710027a328(mesh, cell));
}

// A surface along an axis: unless the target is behind it or it faces up, whether there is ground
// behind its point.
static inline bool CanStepAlong(UnkSelf_l97be9c* self, const UnkSurface_l97be9c* surface,
                                const base::math::CVector3D& along) {
    UnkUnit1452_b56* unit = reinterpret_cast<UnkUnit1452_b56*>(self);
    const base::math::CVector3D up = {0.0f, 1.0f, 0.0f};
    base::math::CVector3D to_target =
        Sub(reinterpret_cast<UnkTarget_l97be9c*>(fn_710097f1e0(unit))->vf_0x188(),
            reinterpret_cast<UnkOwner_l97be9c*>(fn_710097f1b8(unit)));
    if (!(Dot(to_target, surface->normal) > 0.0f))
        return true;
    if (Dot(along, up) > 0.0f)
        return true;

    base::math::CVector2D dir;
    fn_710007b430(&dir, &along);
    if (!IsAbout(__builtin_fabsf(along.x), 1.0f))
        return false;
    UnkUnit1452Owner_b56* owner = fn_710097f1b8(unit);
    UnkNavMeshManager* manager = NavManager();
    if (!owner)
        return false;
    UnkNavMesh* mesh = manager->navmesh;
    if (!mesh)
        return false;

    base::math::CVector2D behind;
    fn_710007b430(&behind, &surface->point);
    behind.x = behind.x - dir.x * 50.0f;
    behind.y = behind.y - dir.y * 50.0f;
    bool blocked = !IsWalkable(mesh, behind);
    f32 rise = Dot(to_target, up);
    if (rise >= 0.0f && blocked && IsWalkable(mesh, Offset(behind, 0.0f, 100.0f)))
        blocked = false;
    if (rise <= 0.0f && blocked && IsWalkable(mesh, Offset(behind, 0.0f, -100.0f)))
        blocked = false;
    if (blocked)
        return false;
    self->stepped = true;
    return true;
}

bool fn_710097be9c(UnkSelf_l97be9c* self, const UnkSurface_l97be9c* surface) {
    if (self->state && self->state->id == 3)
        return false;
    const base::math::CVector3D& along = surface->dir;
    if (IsAbout(__builtin_fabsf(along.x), 1.0f) || IsAbout(__builtin_fabsf(along.y), 1.0f))
        return CanStepAlong(self, surface, along);
    return true;
}

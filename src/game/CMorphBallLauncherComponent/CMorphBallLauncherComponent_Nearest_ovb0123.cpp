// CMorphBallLauncherComponent's unit (tools/tu_map.py): the entity of the list fn_7100aa9ecc gives
// that is nearest the component's entity (+0x30, by the translation of their transforms), or null.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/math/CVector.h"
#include "game/CBombMovement.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkLauncherXform_ovb0123 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

static inline base::math::CVector3D Pos(const UnkLauncherXform_ovb0123* t) {
    return {t->x, t->y, t->z};
}

struct UnkLauncherTarget_ovb0123 {
    u8 pad_0x0[0x18];
    UnkReflObject* entity;  // 0x18, its transform too
};

struct UnkLauncherEntry_ovb0123 {
    u8 pad_0x0[0x10];
    UnkLauncherTarget_ovb0123* target;  // 0x10
};

struct UnkLauncherList_ovb0123 {
    UnkLauncherEntry_ovb0123** data;
    u32 count;
};
struct UnkFanBombList_ovb0122;

static inline UnkLauncherList_ovb0123* List() {
    return reinterpret_cast<UnkLauncherList_ovb0123*>(fn_7100aa9ecc());
}

static inline UnkLauncherEntry_ovb0123** End(UnkLauncherList_ovb0123* list) {
    return list->data + list->count;
}

struct UnkMorphBallLauncher_ovb0123 {
    u8 pad_0x0[0x30];
    UnkLauncherXform_ovb0123* xform;  // 0x30
};

UnkReflObject* fn_71008880bc(UnkMorphBallLauncher_ovb0123* self) {
    UnkLauncherEntry_ovb0123** it = List()->data;
    UnkReflObject* best = nullptr;
    f32 bestDist = -1.0f;
    for (; it != End(List());) {
        UnkReflObject* e = (*it)->target->entity;
        UnkReflObject* cast = fn_710009d50c(e->vf_0x0(), fn_7100d70c20()) ? e : nullptr;
        const UnkLauncherXform_ovb0123* a = reinterpret_cast<const UnkLauncherXform_ovb0123*>((*it++)->target->entity);
        base::math::CVector3D p = Pos(a);
        base::math::CVector3D q = Pos(self->xform);
        f32 dx = p.x - q.x;
        f32 dy = p.y - q.y;
        f32 d = dx * dx + dy * dy;
        f32 dz = p.z - q.z;
        d += dz * dz;
        if (!best || bestDist > d) {
            bestDist = d;
            best = cast;
        }
    }
    return best;
}

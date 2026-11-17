// CAbilityGhostAura unit: casts a shape back along a direction through the collision tree and, on a hit
// nearer than `*distance` (measured along the direction), shortens `*distance` to it, with allowances for
// breakable tile groups and forced-movement areas. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a22d68[];
extern __attribute__((visibility("hidden"))) u64 g_unk_71726af4b8;

struct UnkRefl_fn_710030d850;
template <>
base::reflection::UnkReflObject* base::reflection::refl_FindTyped<UnkRefl_fn_710030d850>(void*);

// A counted reference held by a hit.
struct UnkHitRef_l30d464 {
    UnkRefObject* ptr;

    UnkHitRef_l30d464() : ptr(nullptr) {}
    ~UnkHitRef_l30d464() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkCollisionHit_l30d464 {
    UnkHitRef_l30d464 object;
    UnkHitRef_l30d464 field_0x8;
    s32 id;
    UnkVec3f point;
    UnkVec3f field_0x20;
    UnkVec3f normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_l30d464()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};

struct UnkRay_l30d464 {
    bool field_0x0;
    u8 pad_0x1[0x7];
    UnkVec3f origin;       // 0x8
    const UnkVec3f* dir;   // 0x18
    u32 mask;              // 0x20

    UnkRay_l30d464(const UnkVec3f& o, const UnkVec3f* d) : field_0x0(false), origin(o), dir(d), mask(0x20000000) {}
};

// The query (vtable g_unk_7101a22d68): its nearest hit and the ray it was cast along.
struct UnkRayQuery_l30d464 {
    void** vtable;
    UnkCollisionHit_l30d464 hit;  // 0x8
    UnkRay_l30d464 ray;           // 0x50
    UnkRay_l30d464 field_0x78;

    UnkRayQuery_l30d464(const UnkVec3f& from, const UnkVec3f* dir) : ray(from, dir), field_0x78(from, dir) {
        vtable = g_unk_7101a22d68 + 2;
    }
    ~UnkRayQuery_l30d464() { vtable = g_unk_7101a20ed8 + 2; }
};
static_assert(sizeof(UnkRayQuery_l30d464) == 0xa0, "query");

struct UnkHitComponent_l30d464 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x28];
    void* entity;  // 0x30
};

// What a hit's handle points at.
struct UnkHitTarget_l30d464 {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkHitComponent_l30d464* component;  // 0x10
    u8 pad_0x18[0x8];
    u64 field_0x20;
    u8 pad_0x28[0x18];
    f32 field_0x40;
    u8 pad_0x44[0x4];
    f32 field_0x48;
    u8 pad_0x4c[0x14];
    bool field_0x60;

    u32 vf_0x58() { return reinterpret_cast<u32 (*)(UnkHitTarget_l30d464*)>(vtable[0x58 / 8])(this); }
    void vf_0x98() { reinterpret_cast<void (*)(UnkHitTarget_l30d464*)>(vtable[0x98 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkHitTarget_l30d464, field_0x60) == 0x60, "target");

struct UnkHitHandle_l30d464 : UnkRefObject {
    UnkHitTarget_l30d464* target;  // 0x10
};

static inline f32 Clamp(f32 v, f32 lo, f32 hi) {
    v = !(v >= hi) ? v : hi;
    return !(v < lo) ? v : lo;
}

bool fn_710030d464(void* self, UnkShape* shape, const UnkVec3f* from, const UnkVec3f* dir, f32* distance) {
    UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
    UnkTree* tree = sub ? static_cast<UnkTree*>(sub->field_0x820) : nullptr;
    UnkVec3f back = -*dir;
    UnkRayQuery_l30d464 query(*from, &back);
    const UnkMtx44* identity = reinterpret_cast<const UnkMtx44*>(&g_unk_7101a1a680);
    bool nearer = false;
    if (fn_7100d5b484(tree, 0x409, shape, identity, &query, 0) ||
        fn_7100d5b484(tree, 0x10000, shape, identity, &query, 1)) {
        UnkVec3f delta = query.hit.point - *from;
        UnkVec3f forward = Normalized(*dir);
        UnkVec3f toward = Normalized(delta);
        f32 angle = acosf(Clamp(Dot(forward, toward), -1.0f, 1.0f));
        f32 along = Length(delta) * cosf(angle);
        u32 flags = query.hit.flags;
        if ((flags & 0x20) && (flags & 0x10) && along < *distance) {
            if (UnkHitHandle_l30d464* handle = static_cast<UnkHitHandle_l30d464*>(query.hit.object.ptr)) {
                UnkHitTarget_l30d464* target = handle->target;
                if (target && target->component) {
                    UnkHitComponent_l30d464* component = target->component;
                    if (fn_710009d50c(component->vf_0x0(), fn_7100ca0484())) {
                        along += -100.0f;
                    } else if ((target->vf_0x58() & 0x10000) || target->field_0x20 == g_unk_71726af4b8) {
                        if (!fn_710009d50c(component->vf_0x0(), fn_71010c23d0()) || !component->entity)
                            return false;
                        if (!base::reflection::refl_FindTyped<UnkRefl_fn_710030d850>(component->entity))
                            return false;
                        if (target->field_0x60)
                            target->vf_0x98();
                        f32 half = (target->field_0x48 - target->field_0x40) * 0.5f;
                        along += half + half + 200.0f;
                        *distance = along;
                    }
                }
            }
            nearer = true;
            *distance = along;
        }
    }
    return nearer;
}

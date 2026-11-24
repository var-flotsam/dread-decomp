// CCharacterMovement's unit (1445): pushes `pos` out of whatever the shape (or the owner's, slot 0x2a8)
// overlaps in the collision tracker, up to `iterations` steps: each step moves it by the shortest push
// (fn_7100977338) along a hit's normal or straight back, scaled by 1.01. True once nothing overlaps, with
// the last hit's normal. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CAABoxShape2D.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern base::reflection::UnkTunable g_unk_717273a7a0;

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

// A shape's bounds: its two corners.
struct UnkShapeBounds_l976c94 {
    base::math::CVector3D min;
    base::math::CVector3D max;
};

struct UnkBox2D_l976c94 {
    base::math::CVector2D min;
    base::math::CVector2D max;
};
struct UnkCollisionShape {
    u8 data[0xc0];
};
// A collision shape (0xc0 bytes) built from a 2D box.
struct UnkBox2D {
    f32 min_x, min_y, max_x, max_y;

    UnkBox2D(f32 min_x_, f32 min_y_, f32 max_x_, f32 max_y_)
        : min_x(min_x_), min_y(min_y_), max_x(max_x_), max_y(max_y_) {}
};

// The movement tunables: the largest push a step may take at +0x24.
struct UnkMovementTunable_l976dd4 {
    u8 pad_0x0[0x24];
    f32 max_push;  // 0x24
};

// The shape pushed out: slot 0x10 releases it, 0x28 gives its bounds under its transform (+0x8), 0x30
// scales it and 0x40 makes a copy.
struct UnkPushShape_l976dd4 {
    void** vtable;
    u8 field_0x8[1];

    void Release() { reinterpret_cast<void (*)(UnkPushShape_l976dd4*)>(vtable[0x10 / 8])(this); }
    UnkShapeBounds_l976c94 GetBounds(const void* xf) {
        return reinterpret_cast<UnkShapeBounds_l976c94 (*)(UnkPushShape_l976dd4*, const void*)>(
            vtable[0x28 / 8])(this, xf);
    }
    void Scale(f32 factor) {
        reinterpret_cast<void (*)(UnkPushShape_l976dd4*, f32)>(vtable[0x30 / 8])(this, factor);
    }
    UnkPushShape_l976dd4* Clone() {
        return reinterpret_cast<UnkPushShape_l976dd4* (*)(UnkPushShape_l976dd4*)>(vtable[0x40 / 8])(this);
    }
};

struct UnkShapeOwner_l976dd4 {
    u8 pad_0x0[0x158];
    UnkPushShape_l976dd4* field_0x158;
};

struct UnkMovement_l976dd4 {
    void** vtable;

    UnkShapeOwner_l976dd4* vf_0x2a8() {
        return reinterpret_cast<UnkShapeOwner_l976dd4* (*)(UnkMovement_l976dd4*)>(vtable[0x2a8 / 8])(this);
    }
};

// A hit of the query: two counted references, the point hit and its normal.
struct __attribute__((packed)) UnkPushHit_l976dd4 {
    UnkRefObject* field_0x0;
    UnkRefObject* field_0x8;
    u8 pad_0x10[0x4];
    UnkVec3f point;   // 0x14
    u8 pad_0x20[0xc];
    UnkVec3f normal;  // 0x2c
    u8 pad_0x38[0x10];
};
static_assert(sizeof(UnkPushHit_l976dd4) == 0x48, "size");

static inline void ReleaseRef_l976dd4(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

// Up to eight hits after the count; the destructor (vtable g_unk_7101a23478) releases both references.
struct __attribute__((packed)) UnkPushHits_l976dd4 {
    void** vtable;
    u32 count;
    UnkPushHit_l976dd4 items[8];

    UnkPushHits_l976dd4() {
        vtable = g_unk_7101a23478 + 2;
        count = 0;
    }
    ~UnkPushHits_l976dd4() {
        vtable = g_unk_7101a23478 + 2;
        for (u64 i = 0; i < count; i++) {
            ReleaseRef_l976dd4(items[i].field_0x8);
            ReleaseRef_l976dd4(items[i].field_0x0);
        }
        count = 0;
    }
    UnkPushHit_l976dd4* begin() { return items; }
    UnkPushHit_l976dd4* end() { return &items[count]; }
};

UnkVec3f fn_7100977338(const UnkVec3f* offset, const UnkVec3f* dir, const UnkShapeBounds_l976c94* bounds);

// The push a result gives: its larger horizontal component, when it is a real push at all.
static inline bool IsShorter(const UnkVec3f& push, f32 best, f32* size) {
    f32 ay = __builtin_fabsf(push.y);
    f32 ax = __builtin_fabsf(push.x);
    f32 sum = ax + ay;
    *size = ax > ay ? ax : ay;
    return sum > 0.001f && *size < best;
}

bool fn_7100976dd4(UnkMovement_l976dd4* self, UnkVec3f* pos, UnkVec3f* normal_out, u32 mask, UnkPushShape_l976dd4* shape,
                   u32 iterations) {
    UnkVec3f p = *pos;
    UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
    if (!sub)
        return false;
    void* tracker = sub->field_0x820;
    if (!shape) {
        UnkShapeOwner_l976dd4* owner = self->vf_0x2a8();
        shape = owner ? owner->field_0x158 : nullptr;
    }

    UnkShapeBounds_l976c94 bounds = shape->GetBounds(shape->field_0x8);
    UnkCollisionShape area;
    {
        UnkBox2D_l976c94 box;
        fn_710007b430(&box.min, &bounds.min);
        fn_710007b430(&box.max, &bounds.max);
        fn_7100d5e1f8(&area, reinterpret_cast<const UnkBox2D*>(&box));
    }
    UnkPushShape_l976dd4* probe = shape->Clone();
    probe->Scale(0.999f);
    UnkMatrix44_7100078c9c xf = BuildTranslationMatrixFromVec3(p);

    bool ok = false;
    UnkVec3f normal(0.0f, 0.0f, 0.0f);
    if (iterations) {
        u32 step = 0;
        bool moved;
        do {
            xf.m[3] = p.x;
            xf.m[7] = p.y;
            xf.m[11] = p.z;
            ok = true;
            if (!fn_7100d5b484(static_cast<UnkTree*>(tracker), mask, reinterpret_cast<UnkShape*>(probe),
                               reinterpret_cast<const UnkMtx44*>(&xf), nullptr, 1))
                break;
            moved = true;
            UnkPushHits_l976dd4 hits;
            if (fn_7100d5b484(static_cast<UnkTree*>(tracker), mask, reinterpret_cast<UnkShape*>(&area),
                              reinterpret_cast<const UnkMtx44*>(&xf), &hits, 1)) {
                moved = false;
                if (hits.begin() == hits.end()) {
                    ok = true;
                } else {
                    f32 best = reinterpret_cast<UnkMovementTunable_l976dd4&>(g_unk_717273a7a0).max_push;
                    UnkVec3f push(0.0f, 0.0f, 0.0f);
                    for (UnkPushHit_l976dd4* it = hits.begin(); it != hits.end(); ++it) {
                        f32 dy = it->point.y - p.y;
                        f32 dz = it->point.z - p.z;
                        f32 dx = it->point.x - p.x;
                        UnkVec3f offset(dx, dy, dz);
                        f32 size;
                        UnkVec3f along = fn_7100977338(&offset, &it->normal, &bounds);
                        if (IsShorter(along, best, &size)) {
                            moved = true;
                            push = along;
                            best = size;
                            normal = it->normal;
                        } else {
                            UnkVec3f back(-dx, -dy, -dz);
                            UnkVec3f away = fn_7100977338(&offset, &back, &bounds);
                            if (IsShorter(away, best, &size)) {
                                moved = true;
                                push = away;
                                best = size;
                            }
                        }
                    }
                    ok = false;
                    if (moved) {
                        f32 len2 = push.x * push.x + push.y * push.y + push.z * push.z;
                        if (len2 > 0.0f) {
                            // Pushes shorter than one are taken at unit length.
                            UnkVec3f dir = push;
                            if (len2 < 1.0f) {
                                f32 inv = 1.0f / sqrtf(len2);
                                dir.x = push.x * inv;
                                dir.y = push.y * inv;
                                dir.z = push.z * inv;
                            }
                            p.x = dir.x * 1.01f + p.x;
                            p.y = dir.y * 1.01f + p.y;
                            p.z = dir.z * 1.01f + p.z;
                        } else {
                            moved = false;
                        }
                    }
                }
            } else {
                ok = true;
            }
        } while (++step < iterations && (moved & !ok));
    }
    if (probe)
        probe->Release();
    fn_7100d5e2c8(&area);
    if (!ok)
        return false;
    *pos = p;
    if (normal_out)
        *normal_out = normal;
    return true;
}

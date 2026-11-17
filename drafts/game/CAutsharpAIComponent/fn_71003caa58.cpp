// CAutsharpAIComponent unit (tools/tu_map.py unit 572): fn_71003caa58, whether the Autsharp has reached a
// ledge it can drop from: ground under it but none a step ahead, then two floor probes either way along its
// facing whose hits are 100 or 200 units apart pick the drop kind and its heading. Names and layouts are
// placeholders.
#include <math.h>

#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkAutsharpOwner_b0117 {
    void** vtable;
    u8 pad_0x8[0x38 - 0x8];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1f0 - 0x94];
    void* field_0x1f0;
    u8 pad_0x1f8[0x218 - 0x1f8];
    void* field_0x218;
    void* field_0x220;
    void* field_0x228;
    u8 pad_0x230[0x298 - 0x230];
    void* field_0x298;
    u8 pad_0x2a0[0x37c - 0x2a0];
    f32 radius;  // 0x37c
    u8 pad_0x380[0x18];
    void* field_0x398;
};
static_assert(__builtin_offsetof(UnkAutsharpOwner_b0117, field_0x398) == 0x398, "owner layout");

// The tuning object at +0x146d0: its vf_0xb0 gives a speed for the owner.
struct UnkAutsharpTuning_b0117 {
    void** vtable;
    u8 pad_0x8[0x530 - 0x8];
    f32 field_0x530;
    f32 field_0x534;
    f32 field_0x538;
    f32 field_0x53c;
    f32 field_0x540;
    f32 field_0x544;
    f32 field_0x548;

    f32 GetSpeed(UnkAutsharpOwner_b0117* owner) {
        return reinterpret_cast<f32 (*)(UnkAutsharpTuning_b0117*, UnkAutsharpOwner_b0117*)>(vtable[0xb0 / 8])(this,
                                                                                                              owner);
    }
};

struct UnkPair_b0117 {
    u64 first;
    u64 second;
};

struct UnkAutsharpAI_b0117 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkAutsharpOwner_b0117* owner;  // 0x30
    u8 pad_0x38[0x30];
    CVector3D* points;  // 0x68
    u32 point_count;    // 0x70
    u8 pad_0x74[0x20];
    u8 open;  // 0x94
    u8 pad_0x95[0x146d0 - 0x95];
    UnkAutsharpTuning_b0117* tuning;  // 0x146d0
    s32 state;                        // 0x146d8
    u8 pad_0x146dc[4];
    UnkPair_b0117 field_0x146e0;
    u8 link[0x18];       // 0x146f0
    bool field_0x14708;
    bool field_0x14709;
    bool field_0x1470a;
    bool field_0x1470b;
    bool field_0x1470c;
    u8 pad_0x1470d[3];
    f32 field_0x14710;
    f32 timer;  // 0x14714
    u8 pad_0x14718[8];
    f32 field_0x14720;
    f32 field_0x14724;
    f32 field_0x14728;
    f32 field_0x1472c;
    f32 field_0x14730;
    u8 pad_0x14734[8];
    bool field_0x1473c;
    u8 pad_0x1473d[0xb];
    bool field_0x14748;
    u8 pad_0x14749[3];
    CVector2D field_0x1474c;
    CVector3D field_0x14754;
    UnkVec3f field_0x14760;
    CVector2D field_0x1476c;
    CVector2D field_0x14774;
    u32 field_0x1477c;
    bool field_0x14780;
    u8 pad_0x14781[4];
    bool field_0x14785;
    u8 pad_0x14786[2];
    f32 field_0x14788;
    UnkVec3f field_0x1478c;
    u8 pad_0x14798[0x11];
    bool field_0x147a9;
    bool field_0x147aa;
    bool field_0x147ab;
};
static_assert(__builtin_offsetof(UnkAutsharpAI_b0117, field_0x147ab) == 0x147ab, "layout");
static_assert(__builtin_offsetof(UnkAutsharpAI_b0117, field_0x14785) == 0x14785, "layout");

// ---- the collision-tree segment query (as src/game/CCharacterMovement.cpp spells it) ----

struct UnkHitRef_b0117 {
    base::UnkRefCounted* ptr;

    UnkHitRef_b0117() : ptr(nullptr) {}
    ~UnkHitRef_b0117() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkHitVec_b0117 {
    f32 x, y, z;
    UnkHitVec_b0117(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkCollisionHit_b0117 {
    UnkHitRef_b0117 object;
    UnkHitRef_b0117 field_0x8;
    s32 id;
    UnkHitVec_b0117 point;
    UnkHitVec_b0117 field_0x20;
    UnkHitVec_b0117 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_b0117()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};

struct UnkSegmentHitQuery_b0117 {
    void** vtable;
    UnkCollisionHit_b0117 hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSegmentHitQuery_b0117(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkSegmentHitQuery_b0117() { vtable = g_unk_7101a20ed8 + 2; }
};

struct UnkHitSegment_b0117 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkHitSegment_b0117(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

struct UnkTree;
struct UnkBox;

static inline CVector3D Position_l3caa58(UnkAutsharpOwner_b0117* owner) {
    return CVector3D{owner->xform.m[0][3], owner->xform.m[1][3], owner->xform.m[2][3]};
}

// Whether a and b are within one unit of each other.
static inline bool Near_l3caa58(f32 a, f32 b) {
    return a - 1.0f <= b && b <= a + 1.0f;
}

bool fn_71003caa58(UnkAutsharpAI_b0117* self) {
    f32 radius = self->owner->radius;
    UnkVec3f pos = self->owner->xform.GetTranslation();
    const UnkVec3f& facing = self->field_0x14760;

    // Ground under the Autsharp, and ground a step ahead: no ledge.
    const UnkMatrix44& m = self->owner->xform.GetMatrix();
    CVector2D here = {pos.x, pos.y};
    CVector2D below = {pos.x - radius * 1.1f * m.m[1], pos.y - radius * 1.1f * m.m[5]};
    UnkHitSegment_b0117 down(here, below);
    UnkSegmentHitQuery_b0117 ground(Position_l3caa58(self->owner));
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    if (fn_7100d5c518(tree, 0x80409, reinterpret_cast<const UnkBox*>(&down), &ground, 2)) {
        CVector3D now = Position_l3caa58(self->owner);
        pos = UnkVec3f(now.x + facing.x * 50.0f, now.y + facing.y * 50.0f, now.z + facing.z * 50.0f);
        const UnkMatrix44& n = self->owner->xform.GetMatrix();
        CVector2D ahead = {pos.x, pos.y};
        CVector2D ahead_below = {pos.x - radius * 1.1f * n.m[1], pos.y - radius * 1.1f * n.m[5]};
        UnkHitSegment_b0117 ahead_down(ahead, ahead_below);
        UnkSegmentHitQuery_b0117 ahead_ground(CVector3D{pos.x, pos.y, pos.z});
        if (fn_7100d5c518(tree, 0x80409, reinterpret_cast<const UnkBox*>(&ahead_down), &ahead_ground, 2))
            return false;
    }

    // Probe the floor below, forward and back along the facing.
    {
        const UnkMatrix44& n = self->owner->xform.GetMatrix();
        CVector2D base = {pos.x - radius * 1.1f * n.m[1], pos.y - radius * 1.1f * n.m[5]};
        CVector2D span = {facing.x * 200.0f, facing.y * 200.0f};
        CVector2D front_end = {base.x + span.x, base.y + span.y};
        CVector2D back_end = {base.x - span.x, base.y - span.y};
        UnkHitSegment_b0117 front(base, front_end);
        UnkHitSegment_b0117 back(base, back_end);
        UnkSegmentHitQuery_b0117 front_hit(Position_l3caa58(self->owner));
        UnkSegmentHitQuery_b0117 back_hit(Position_l3caa58(self->owner));
        bool front_found = fn_7100d5c518(tree, 0x80409, reinterpret_cast<const UnkBox*>(&front), &front_hit, 2);
        bool back_found = fn_7100d5c518(tree, 0x80409, reinterpret_cast<const UnkBox*>(&back), &back_hit, 2);
        if (front_found && back_found) {
            CVector3D a = {front_hit.hit.point.x, front_hit.hit.point.y, front_hit.hit.point.z};
            CVector3D b = {back_hit.hit.point.x, back_hit.hit.point.y, back_hit.hit.point.z};
            CVector3D now = Position_l3caa58(self->owner);
            const UnkMatrix44& k = self->owner->xform.GetMatrix();
            f32 fx = now.x - radius * 1.1f * k.m[1] - a.x;
            f32 fy = now.y - radius * 1.1f * k.m[5] - a.y;
            f32 fz = now.z - radius * 1.1f * k.m[9] - a.z;
            f32 half = radius * 0.5f;
            if (fx * fx + fy * fy + fz * fz < half * half)
                return false;
            f32 gx = a.x - b.x;
            f32 gy = a.y - b.y;
            f32 gz = a.z - b.z;
            f32 gap = gx * gx + gy * gy + gz * gz;
            u32 kind = 0;
            if (Near_l3caa58(gap, 200.0f * 200.0f))
                kind = 2;
            else if (Near_l3caa58(gap, 100.0f * 100.0f))
                kind = 1;
            if (kind != 0) {
                self->field_0x1477c = kind;
                const UnkHitVec_b0117& hit = front_hit.hit.point;
                CVector3D target = {hit.x - facing.x * 50.0f, hit.y - facing.y * 50.0f, hit.z - facing.z * 50.0f};
                UnkAutsharpOwner_b0117* owner = self->owner;
                CVector3D to = {target.x - owner->xform.m[0][3], target.y - owner->xform.m[1][3],
                                target.z - owner->xform.m[2][3]};
                CVector2D dir;
                fn_710007b430(&dir, &to);
                f32 len = sqrtf(dir.x * dir.x + dir.y * dir.y);
                if (len != 0.0f) {
                    dir.x /= len;
                    dir.y /= len;
                }
                self->field_0x1476c.x = dir.x;
                self->field_0x1476c.y = dir.y;
                CVector2D goal;
                fn_710007b430(&goal, &target);
                self->field_0x14774.x = goal.x;
                self->field_0x14774.y = goal.y;
                self->field_0x14780 = true;
                return true;
            }
        }
    }
    return false;
}
// CCharacterMovement's unit (tools/tu_map.py unit 1445): the navigation hit behind a point. The point
// (optionally adjusted by vf_0x300) is probed along the owner's backward direction (fn_7100ebfa7c), 4000
// long, as wide as the collider's half extent; the hit found is returned, an empty one otherwise.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/CCharClassAIComponent.h"
#include "game/UnkNavMeshHit.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU362.h"
#include "game/UnkTU55.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkSource_7100286798;

struct UnkProbePoint_l979cd4 {
    f32 x;
    f32 y;
    UnkProbePoint_l979cd4(f32 x_, f32 y_) : x(x_), y(y_) {}
};

struct UnkProbeSegment_l979cd4 {
    UnkProbePoint_l979cd4 start;
    UnkProbePoint_l979cd4 end;
    UnkProbePoint_l979cd4 delta;
    UnkProbeSegment_l979cd4(const UnkProbePoint_l979cd4& s, const UnkProbePoint_l979cd4& e)
        : start(s), end(e), delta(e.x - s.x, e.y - s.y) {}
};

// fn_7100ebfa7c's result: no cell (0xffff), no side (0xff), points cleared.
struct UnkProbeResult_l979cd4 {
    u16 cell;
    u8 side;
    UnkProbePoint_l979cd4 points[5];
    f32 distance;
    UnkProbeResult_l979cd4()
        : cell(0xffff), side(0xff),
          points{{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}}, distance(0.0f) {}
};

struct UnkProbeNavSource_l979cd4 {
    u8 pad_0x0[0x1610];
    UnkSource_7100286798* source;  // 0x1610
};

struct UnkProbeOwner_l979cd4 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
    u8 pad_0x78[0x240 - 0x78];
    UnkProbeNavSource_l979cd4* nav;  // 0x240
};

struct UnkProbeCollider_l979cd4 {
    void** vtable;
    u8 pad_0x8[0x40 - 0x8];
    f32 min;  // 0x40
    u8 pad_0x44[0x4];
    f32 max;  // 0x48
    u8 pad_0x4c[0x60 - 0x4c];
    bool dirty;  // 0x60

    void vf_0x98() { reinterpret_cast<void (*)(UnkProbeCollider_l979cd4*)>(vtable[0x98 / 8])(this); }
};

struct UnkProbeMovement_l979cd4 {
    void** vtable;
    u8 pad_0x8[0x30 - 0x8];
    UnkProbeOwner_l979cd4* owner;  // 0x30
    u8 pad_0x38[0x3b8 - 0x38];
    UnkProbeCollider_l979cd4* defaultCollider;  // 0x3b8
    UnkProbeCollider_l979cd4* collider;  // 0x3c0

    void vf_0x300(CVector2D* point) {
        reinterpret_cast<void (*)(UnkProbeMovement_l979cd4*, CVector2D*)>(vtable[0x300 / 8])(this, point);
    }
};

static inline UnkNavMeshManager* NavMeshManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
UnkNavMeshHit fn_7100979cd4(UnkProbeMovement_l979cd4* self, const CVector3D* pos, bool adjust) {
    UnkNavMeshHit hit;
    UnkProbeNavSource_l979cd4* nav = self->owner->nav;
    u32 flags = nav ? fn_7100413fd0(nav->source) : 0;
    CVector2D from;
    fn_710007b430(&from, pos);
    if (adjust)
        self->vf_0x300(&from);

    CVector2D dir;
    {
        const UnkMatrix44& m = fn_710002614c(&self->owner->xform);
        f32 x = -m.m[1];
        f32 y = -m.m[5];
        f32 z = -m.m[9];
        CVector3D back;
        back.x = x;
        back.y = y;
        back.z = z;
        fn_710007b430(&dir, &back);
    }

    UnkProbeCollider_l979cd4* collider = self->collider ? self->collider : self->defaultCollider;
    f32 halfWidth = 0.0f;
    if (collider) {
        if (collider->dirty)
            collider->vf_0x98();
        halfWidth = (collider->max - collider->min) * 0.5f;
    }

    UnkProbePoint_l979cd4 start(from.x - dir.x, from.y - dir.y);
    UnkProbePoint_l979cd4 end(from.x + dir.x * 4000.0f, from.y + dir.y * 4000.0f);
    UnkProbeSegment_l979cd4 segment(start, end);
    UnkProbeResult_l979cd4 result;
    if (fn_7100ebfa7c(NavMeshManager(), reinterpret_cast<Vec2*>(&segment), reinterpret_cast<Result*>(&result),
                      halfWidth, flags, -1.0f)) {
        hit.cell = result.cell;
        hit.side = result.side;
        fn_710007b444(&hit.point, reinterpret_cast<const UnkVec3*>(&result.points[0]));
    }
    return hit;
}

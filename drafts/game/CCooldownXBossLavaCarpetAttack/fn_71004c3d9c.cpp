// CCooldownXBossLavaCarpetAttack's unit (tools/tu_map.py unit 747): probes the lower edge of the attack's
// box for a floor (a hit whose normal points up) and, when one is found far enough ahead, sets where the
// carpet lands and where it goes next. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/spatial/COBox2D.h"
#include "game/CAbilityShinespark.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

#include <math.h>

using base::math::CVector2D;
using base::math::CVector3D;
using base::spatial::COBox2D;

UnkVec2Result fn_7100110420(const COBox2D* self, u32 index);

struct UnkHitObj_l4c3d9c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkHitRef_l4c3d9c {
    UnkHitObj_l4c3d9c* ptr;

    UnkHitRef_l4c3d9c() : ptr(nullptr) {}
    ~UnkHitRef_l4c3d9c() {
        if (UnkHitObj_l4c3d9c* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

// A point.
struct UnkVec3_l4c3d9c {
    f32 x, y, z;

    UnkVec3_l4c3d9c(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkCollisionHit_l4c3d9c {
    UnkHitRef_l4c3d9c object;
    UnkHitRef_l4c3d9c field_0x8;
    s32 id;
    UnkVec3_l4c3d9c point;
    UnkVec3_l4c3d9c field_0x20;
    UnkVec3_l4c3d9c normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_l4c3d9c()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};

// The hit query the segment test fills.
struct UnkSegmentQuery_l4c3d9c {
    void** vtable;
    UnkCollisionHit_l4c3d9c hit;
    bool field_0x50;

    UnkSegmentQuery_l4c3d9c() {
        vtable = g_unk_7101a23590 + 2;
        field_0x50 = false;
    }
    ~UnkSegmentQuery_l4c3d9c() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkSegment_l4c3d9c {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkSegment_l4c3d9c(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

struct UnkPieceInfo_l4c3d9c {
    u8 pad_0x0[0x8];
    u32 kind;
};

struct UnkPieceSlot_l4c3d9c {
    UnkPieceInfo_l4c3d9c* info;
};

struct UnkPieceList_l4c3d9c {
    u8 pad_0x0[0x28];
};

UnkPieceSlot_l4c3d9c* fn_71004a8064(UnkPieceList_l4c3d9c* list, u32 index);

struct UnkLavaParams_l4c3d9c {
    u8 pad_0x0[0xec];
    f32 forward;  // 0xec
    u8 pad_0xf0[0x10];
    UnkPieceList_l4c3d9c* lists;  // 0x100
};

struct UnkLavaPath_l4c3d9c {
    u8 pad_0x0[0x8];
    u32 kind;
};

struct UnkLavaPoint_l4c3d9c {
    u8 pad_0x0[0x10];
    UnkVec3_l4c3d9c pos;
    u8 pad_0x1c[0x14];
};
static_assert(sizeof(UnkLavaPoint_l4c3d9c) == 0x30, "UnkLavaPoint_l4c3d9c layout");

struct UnkLavaCarpetAttack_l4c3d9c {
    u8 pad_0x0[0x30];
    COBox2D box;  // 0x30
    UnkShape* shape;  // 0x58
    u8 pad_0x60[0x8];
    UnkLavaParams_l4c3d9c* params;  // 0x68
    u8 pad_0x70[0x28];
    s32 facing;  // 0x98
    u8 pad_0x9c[0x124];
    u32 list;  // 0x1c0
    u32 piece_count;  // 0x1c4
    u8 pad_0x1c8[0x8];
    UnkLavaPath_l4c3d9c* path;  // 0x1d0
    u32 point_count;  // 0x1d8
    UnkLavaPoint_l4c3d9c points[30];  // 0x1dc
    u8 pad_0x77c[0x784 - 0x77c];
    bool reset;  // 0x784
};
static_assert(__builtin_offsetof(UnkLavaCarpetAttack_l4c3d9c, points) == 0x1dc, "layout");
static_assert(__builtin_offsetof(UnkLavaCarpetAttack_l4c3d9c, reset) == 0x784, "layout");

// The translation of a transform.
static inline CVector3D Translation(const UnkMatrix44& m) {
    CVector3D t;
    t.x = m.m[3];
    t.y = m.m[7];
    t.z = m.m[11];
    return t;
}

// Corner `index` of the box, placed at `pos`.
static inline CVector2D Corner(const COBox2D* box, u32 index, const CVector2D& pos) {
    UnkVec2Result c = fn_7100110420(box, index);
    CVector2D r;
    r.x = pos.x + c.x;
    r.y = pos.y + c.y;
    return r;
}

bool fn_71004c3d9c(UnkLavaCarpetAttack_l4c3d9c* self, UnkVec3_l4c3d9c* target, UnkVec3_l4c3d9c* out, bool* started,
                   f32* time_a, f32* time_b, f32 min_distance, f32 time) {
    UnkGameManagerSub_0x608* collision = g_unk_7101d47310->field_0x608;
    if (!collision)
        return false;
    UnkTree* tree = static_cast<UnkTree*>(collision->field_0x820);
    if (!tree)
        return false;

    CVector2D pos;
    CVector3D origin = Translation(fn_7100d36a2c(self->shape));
    fn_710007b430(&pos, &origin);
    CVector2D c0 = Corner(&self->box, 0, pos);
    CVector2D c1 = Corner(&self->box, 1, pos);
    CVector2D c3 = Corner(&self->box, 3, pos);
    CVector2D c2 = Corner(&self->box, 2, pos);

    // The lower edge in the direction the attack faces.
    const CVector2D& start = self->facing == 0 ? c0 : c3;
    const CVector2D& end = self->facing == 0 ? c1 : c2;
    UnkSegmentQuery_l4c3d9c query;
    UnkSegment_l4c3d9c segment(start, end);
    if (!fn_7100d5c518(tree, 9, reinterpret_cast<const UnkBox*>(&segment), &query, 2))
        return false;
    if (query.hit.normal.y < 0.9f)
        return false;
    if (!*started) {
        *time_a = time;
        *time_b = time;
        *started = true;
        *target = query.hit.point;
        target->y += 20.0f;
        if (!self->reset && self->point_count >= 2) {
            u32 last = self->point_count - 2;
            UnkPieceSlot_l4c3d9c* piece = fn_71004a8064(&self->params->lists[self->list], self->piece_count - 2);
            if (self->path->kind == 4) {
                f32 offset = self->facing == 0 ? 50.0f : -50.0f;
                *target = self->points[last].pos;
                target->x = offset + target->x;
            } else if ((piece->info->kind & ~1u) == 2) {
                *target = self->points[last].pos;
            }
        }
        self->reset = false;
    }
    *out = query.hit.point;
    f32 forward = self->params->forward;
    out->x = (self->facing == 0 ? forward : -forward) + query.hit.point.x;
    if (fabsf(target->x - out->x) < min_distance)
        return false;
    *out = *target;
    out->x = (self->facing == 0 ? min_distance : -min_distance) + out->x;
    return true;
}

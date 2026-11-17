// CEmmyAIComponent's unit (tools/tu_map.py unit 811): after the base step, while the Emmy is in state 5
// or 6, casts a 650-unit probe across each listed navigation-mesh edge that is nearly vertical and
// appends what the probe hits, when the caller's filter accepts it. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/CCharacterMovement.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU362.h"
#include "game/UnkTU39.h"
#include "game/UnkTU811.h"
#include "types.h"

// As src/game_hi/fn_7100ebfa7c.cpp defines it (its point and result types are local there).
struct Vec2;
struct Result;
bool fn_7100ebfa7c(UnkNavMeshManager* self, Vec2* pos, Result* out, f32 a, u32 flags, f32 b);

struct UnkNavPoint_l556e20 {
    f32 x;
    f32 y;
    UnkNavPoint_l556e20(f32 x_, f32 y_) : x(x_), y(y_) {}
};

// A segment with its direction (end - start).
struct UnkNavSegment_l556e20 {
    UnkNavPoint_l556e20 start;
    UnkNavPoint_l556e20 end;
    UnkNavPoint_l556e20 delta;
    UnkNavSegment_l556e20(const UnkNavPoint_l556e20& s, const UnkNavPoint_l556e20& e)
        : start(s), end(e), delta(e.x - s.x, e.y - s.y) {}
};

// fn_7100ebfa7c's result: no cell (0xffff), no side (0xff), points cleared.
struct UnkNavResult_l556e20 {
    u16 cell;
    u8 side;
    UnkNavPoint_l556e20 points[5];
    f32 distance;
    UnkNavResult_l556e20()
        : cell(0xffff), side(0xff),
          points{{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}}, distance(0.0f) {}
};

// A side of a cell and a point on it (constructed by fn_7100287598 or fn_71002875d8, destroyed by
// fn_710028760c).
struct UnkEdgeHit_l556e20 {
    u16 cell;
    u8 side;
    f32 x;
    f32 y;

    UnkEdgeHit_l556e20() {}
    UnkEdgeHit_l556e20(const UnkEdgeHit_l556e20& other) : cell(other.cell), side(other.side), x(other.x), y(other.y) {}
};

void fn_71002875d8(UnkEdgeHit_l556e20* self, u16 cell, u8 side, const base::math::CVector2D* at);

struct UnkEdgeHits_l556e20 {
    u32 count;
    UnkEdgeHit_l556e20 items[1];

    // The element at `index`, constructing the ones up to it first.
    UnkEdgeHit_l556e20& At(u64 index) {
        if (index >= count) {
            for (u32 i = count; i < index + 1; i++)
                fn_7100287598(reinterpret_cast<UnkNavMeshHit*>(&items[i]));
            count = index + 1;
        }
        return items[index];
    }
};

struct UnkEdgeFilter_l556e20 {
    u8 pad_0x0[0x10];
    void* user;  // 0x10
    bool (*accept)(void* user, u16 cell, u8 side, void* a, void* b, void* data, u32 field_0x6c,
                   u32 field_0x68);  // 0x18
    u8 pad_0x20[0x68 - 0x20];
    u32 field_0x68;
    u32 field_0x6c;
    u8 pad_0x70[0x190 - 0x70];
    void* data;  // 0x190
};

struct UnkEmmyEntity_l556e20 {
    u8 pad_0x0[0x218];
    void* movement;  // 0x218
};

struct UnkEmmyMode_l556e20 {
    u32 active;
    u8 pad_0x4[0x10];
    u32 kind;  // 0x14
};

struct UnkEmmyAI_l556e20 {
    u8 pad_0x0[0x30];
    UnkEmmyEntity_l556e20* entity;  // 0x30
    u8 pad_0x38[0x14b48 - 0x38];
    UnkEmmyMode_l556e20 mode;       // 0x14b48
    u8 pad_0x14b60[0x29418 - 0x14b60];
    u8 timer[0x29ee0 - 0x29418];    // 0x29418
    void* settings;                 // 0x29ee0
    u8 pad_0x29ee8[0x29f26 - 0x29ee8];
    bool disabled;                  // 0x29f26
};

void fn_710110f31c(UnkEmmyAI_l556e20* self, void* arg, UnkEdgeFilter_l556e20* filter, UnkEdgeHits_l556e20* hits);
u32 fn_7100413fd0(void* settings);

static inline UnkNavMeshManager* NavMeshManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

void fn_7100556e20(UnkEmmyAI_l556e20* self, void* arg, UnkEdgeFilter_l556e20* filter, UnkEdgeHits_l556e20* hits) {
    fn_710110f31c(self, arg, filter, hits);
    if (fn_710002fb70(self->timer) || self->disabled)
        return;
    if (!fn_7100543b30(reinterpret_cast<UnkEmmyAIView*>(self)) &&
        !fn_71005436e8(reinterpret_cast<UnkEmmyAIComponent_b46a1*>(self)))
        return;
    if (self->mode.active == 0 || self->mode.kind - 5 > 1)
        return;
    UnkNavMesh* navmesh = NavMeshManager()->navmesh;
    if (!navmesh)
        return;
    u32 n = hits->count;
    if (n == 0)
        return;

    for (u64 i = 0; i != n; i++) {
        UnkEdgeHit_l556e20& hit = hits->At(i);
        if (hit.cell == 0xffff || hit.side == 0xff)
            continue;
        UnkNavMeshCell* cell = fn_710027a33c(navmesh, hit.cell);
        if (fn_71002b005c(cell))
            continue;
        UnkNavMeshEdge* edge = fn_71002afbe4(cell, &hit.side);
        if (!(fabsf(edge->field_0xc * 0.0f - edge->field_0x10) > 0.9f))
            continue;

        f32 sign = (f32)((edge->field_0x10 < 0.0f) - (edge->field_0x10 > 0.0f));
        f32 reach = -1.0f;
        if (self->entity->movement)
            reach = fn_7100977b20(self->entity->movement);
        UnkNavPoint_l556e20 dir(sign, sign * 0.0f);
        UnkNavPoint_l556e20 start(hits->items[i].x + dir.x, dir.y + hits->items[i].y);
        UnkNavPoint_l556e20 end(dir.x * 650.0f + hits->items[i].x, dir.y * 650.0f + hits->items[i].y);
        UnkNavResult_l556e20 result;
        UnkNavSegment_l556e20 segment(start, end);
        UnkNavMeshManager* manager = NavMeshManager();
        if (!fn_7100ebfa7c(manager, reinterpret_cast<Vec2*>(&segment), reinterpret_cast<Result*>(&result), reach,
                           fn_7100413fd0(self->settings), -1.0f))
            continue;
        if (filter->accept &&
            !filter->accept(filter->user, result.cell, result.side, nullptr, nullptr, filter->data,
                            filter->field_0x6c, filter->field_0x68))
            continue;

        base::math::CVector2D at;
        fn_710007b430(&at, reinterpret_cast<const base::math::CVector3D*>(result.points));
        UnkEdgeHit_l556e20 found;
        fn_71002875d8(&found, result.cell, result.side, &at);
        new (&hits->items[hits->count]) UnkEdgeHit_l556e20(found);
        hits->count++;
        fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(&found));
    }
}

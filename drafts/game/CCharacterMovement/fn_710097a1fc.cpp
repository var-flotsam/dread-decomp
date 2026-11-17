// Unit 1445 (CCharacterMovement): from a point on a navmesh cell side, finds where the walkable
// floor along the sides ends, walking side to side round the cell corners while the sides stay
// floor-like, and reports which way the last side slopes. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU363.h"
#include "types.h"

using base::math::CVector2D;

struct UnkNavMeshPoint_b0252 {
    u16 cell;  // 0xffff for none
    u8 side;   // 0xff for none
    u8 pad_0x3;
    f32 x;  // 0x4
    f32 y;  // 0x8
};

f32 fn_7100287748(UnkNavMeshPoint_b0252* self, UnkNavMesh* mesh);
bool fn_710027c460(UnkNavMesh* self, u16 cell, u8 side, bool forward, const CVector2D* widths, u32 flags,
                   u16* out_cell, u8* out_side);

// A position on a cell side and the length walked to reach it.
struct UnkNavMeshWalkPos_l97a1fc {
    u16 cell;
    u8 side;
    f32 x;
    f32 y;
    f32 length;
    UnkNavMeshWalkPos_l97a1fc() { fn_71002878dc(reinterpret_cast<UnkMsg_b152*>(this)); }
    ~UnkNavMeshWalkPos_l97a1fc() { fn_7100287954(reinterpret_cast<UnkMsg_b152*>(this)); }
    UnkNavMeshWalkPos_l97a1fc& operator=(const UnkNavMeshWalkPos_l97a1fc& o) {
        cell = o.cell;
        side = o.side;
        x = o.x;
        y = o.y;
        length = o.length;
        return *this;
    }
};

struct UnkNavMeshEdgeInfo_l97a1fc {
    u8 pad_0x0[0x8];
    f32 weight;  // 0x8
    f32 nx;      // 0xc, the side's normal
    f32 ny;      // 0x10
};

struct UnkNavMeshCellEntry_l97a1fc {
    UnkNavMeshCell cell;
    u8 pad_0x68[0x8];
};
static_assert(sizeof(UnkNavMeshCellEntry_l97a1fc) == 0x70, "UnkNavMeshCellEntry_l97a1fc layout");

struct UnkNavMeshCells_l97a1fc {
    u8 pad_0x0[0x290];
    UnkNavMeshCellEntry_l97a1fc* cells;  // 0x290
};

struct UnkCharacterMovement_l97a1fc {
    UnkNavMeshPoint_b0252* point;
    u8 pad_0x8[0x8];
    UnkNavMeshManager* manager;  // 0x10
};

static inline UnkNavMeshCell* CellAt(UnkNavMesh* mesh, u16 id) {
    return &reinterpret_cast<UnkNavMeshCells_l97a1fc*>(mesh)->cells[id].cell;
}

static inline const UnkNavMeshEdgeInfo_l97a1fc* Info(const UnkNavMeshEdge* edge) {
    return reinterpret_cast<const UnkNavMeshEdgeInfo_l97a1fc*>(edge);
}

// How far the side's normal points down: below -0.5 the side is floor.
static inline f32 Downness(const UnkNavMeshEdgeInfo_l97a1fc* e) { return e->ny * -0.0f - e->nx; }

static inline f32 Slope(const UnkNavMeshEdgeInfo_l97a1fc* e) { return e->ny + e->nx * 0.0f; }

void fn_710097a1fc(UnkCharacterMovement_l97a1fc* self, bool forward, UnkNavMeshWalkPos_l97a1fc* out,
                   bool* slopes_forward) {
    UnkNavMeshSideSegment seg;
    u16 next_cell;
    u8 next_side;
    UnkNavMeshSideSegment next_seg;

    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (manager == nullptr) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    UnkNavMeshPoint_b0252* point = self->point;
    UnkNavMesh* mesh = manager->navmesh;
    f32 t = fn_7100287748(point, self->manager->navmesh);
    UnkNavMeshWalkPos_l97a1fc pos;
    CVector2D widths = {45.0f, 90.0f};
    const u8* side = &point->side;
    if (point->cell != 0xffff && *side != 0xff) {
        UnkNavMeshCell* cell = CellAt(mesh, point->cell);
        const UnkNavMeshEdgeInfo_l97a1fc* edge = Info(fn_71002afbe4(cell, side));
        seg = {};
        seg.field_0x10 = -1.0f;
        fn_71002afea8(cell, side, &seg);
        pos.length = 0.0f;
        pos.x = seg.start.x + t * (seg.end.x - seg.start.x);
        pos.y = seg.start.y + t * (seg.end.y - seg.start.y);
        pos.cell = point->cell;
        pos.side = *side;
        if (Downness(edge) <= -0.5f) {
            f32 left = forward ? 1.0f - t : t;
            const CVector2D& end = forward ? seg.end : seg.start;
            pos.x = end.x;
            pos.y = end.y;
            const CVector2D* next_end = forward ? &next_seg.end : &next_seg.start;
            next_cell = 0xffff;
            next_side = 0xff;
            pos.length = left * edge->weight;
            while (fn_710027c460(mesh, pos.cell, pos.side, forward, &widths, 0, &next_cell, &next_side)) {
                UnkNavMeshCell* c = CellAt(mesh, next_cell);
                const UnkNavMeshEdgeInfo_l97a1fc* e = Info(fn_71002afbe4(c, &next_side));
                if (!(Downness(e) <= -0.5f)) {
                    if (slopes_forward) {
                        f32 d = Slope(e);
                        if (forward)
                            *slopes_forward = d > 0.0f;
                        else
                            *slopes_forward = d < 0.0f;
                    }
                    break;
                }
                next_seg = {};
                next_seg.field_0x10 = -1.0f;
                fn_71002afea8(c, &next_side, &next_seg);
                pos.length = e->weight + pos.length;
                pos.x = next_end->x;
                pos.y = next_end->y;
                pos.cell = next_cell;
                pos.side = next_side;
                next_side = 0xff;
                next_cell = 0xffff;
            }
        } else if (slopes_forward) {
            f32 d = Slope(edge);
            if (forward)
                *slopes_forward = d > 0.0f;
            else
                *slopes_forward = d < 0.0f;
        }
    }
    *out = pos;
}

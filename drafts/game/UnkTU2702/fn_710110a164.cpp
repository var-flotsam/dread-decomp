// Unit 2702 (hint CAIComponent): finds, among the navigation mesh cells in a box around the target, the
// nearest upward-facing side whose end links to a vertical side of the next cell, and stores it and the
// target's tracked entry in `out`. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/CCharClassAIComponent.h"
#include "game/CCharacterMovement.h"
#include "game/CMovementComponent.h"
#include "game/UnkActor.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU362.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkAIReach {
    f32 radius;
    f32 field_0x4;

    UnkAIReach() : radius(0.0f), field_0x4(0.0f) {}
    UnkAIReach(const UnkAIReach& other);
};

struct UnkSegment2D_71001262e0;  // a segment's two points (src/game/malloc/fn_71001262e0.cpp)
f32 fn_71001262e0(const UnkSegment2D_71001262e0* seg, const CVector2D* p, CVector2D* closest, f32* t);

// A box on the mesh plane.
struct UnkNavBox_l10a164 {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
};

// The cells a box query returns.
struct UnkNavCells_l10a164 {
    u32 count;
    u16 keys[0x402];
};

void fn_710027a92c(UnkNavMesh* mesh, const UnkNavBox_l10a164* box, u32 layer, bool a, bool b,
                   UnkNavCells_l10a164* out);
bool fn_710027d100(UnkNavMesh* mesh, u16* cell, u8* side, u32 layer);
void fn_710027c460(UnkNavMesh* mesh, u16 cell, u8 side, bool forward, UnkAIReach* reach, u32 layer, u16* next_cell,
                   u8* next_side);

// A point on the mesh: the cell, its side and a position.
struct UnkNavPoint_l10a164 {
    u16 cell;
    u8 side;
    CVector2D pos;

    ~UnkNavPoint_l10a164() { fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(this)); }
};

struct UnkNavPointData_l10a164 {
    u16 cell;
    u8 side;
    CVector2D pos;
};

struct UnkMovement_l10a164 {
    void** vtable;

    UnkNavPoint_l10a164 vf_0x2d8() {
        return reinterpret_cast<UnkNavPoint_l10a164 (*)(UnkMovement_l10a164*)>(vtable[0x2d8 / 8])(this);
    }
};

struct UnkStatus_l10a164 {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
};

struct UnkTarget_l10a164 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x188];
    UnkStatus_l10a164* status;  // 0x1f0
    UnkMovement_l10a164* movement;  // 0x1f8
    u8 pad_0x200[0x8];
    void* field_0x208;
};
static_assert(__builtin_offsetof(UnkTarget_l10a164, field_0x208) == 0x208, "layout");

struct UnkLedgeAI_l10a164 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x15d8];
    UnkSource_7100286798* source;  // 0x1610
};

struct UnkLedgeOut_l10a164 {
    u8 pad_0x0[0x14];
    u16 cell;  // 0x14
    u8 side;  // 0x16
    u8 pad_0x17;
    CVector2D from;  // 0x18
    CVector2D to;  // 0x20
    UnkNavPointData_l10a164 target;  // 0x28
    CVector2D target_pos;  // 0x34
};
static_assert(__builtin_offsetof(UnkLedgeOut_l10a164, target_pos) == 0x34, "layout");

static inline UnkNavMeshManager* NavMeshManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

// Whether `v` is within 0.001 of `target`.
static inline bool Near(f32 v, f32 target) {
    return target <= v + 0.001f && v - 0.001f <= target;
}

bool fn_710110a164(UnkLedgeAI_l10a164* self, UnkLedgeOut_l10a164* out) {
    UnkTarget_l10a164* target = reinterpret_cast<UnkTarget_l10a164*>(fn_7100d7c990(self->owner));
    if (!target || !target->movement)
        return false;

    UnkAIReach reach = self->owner->field_0x1f8 ? fn_7100977b90(target->movement) : UnkAIReach();
    bool flagged = target->status && target->status->field_0xf9;
    bool queried = target->field_0x208 ? fn_71009e12d0(target->field_0x208) : false;
    if (flagged || queried) {
        if (UnkNavMesh* mesh = NavMeshManager()->navmesh) {
            bool upward_only = self->owner->field_0x1f8
                                   ? !fn_71010d0e38(static_cast<UnkMovement_71010d0e38*>(self->owner->field_0x1f8))
                                   : false;
            CVector2D pos;
            {
                f32 x = target->x;
                f32 y = target->y;
                f32 z = target->z;
                CVector3D at = {x, y, z};
                fn_710007b430(&pos, &at);
            }
            UnkNavBox_l10a164 box;
            box.min_x = pos.x - 250.0f;
            box.min_y = pos.y - 600.0f;
            box.max_x = pos.x + 250.0f;
            box.max_y = pos.y + 250.0f;
            UnkNavCells_l10a164 cells;
            cells.count = 0;
            fn_710027a92c(mesh, &box, fn_7100413fd0(self->source), false, false, &cells);
            f32 best = 3.4028235e+38f;
            for (u16* it = cells.keys; it != cells.keys + cells.count; ++it) {
                u16 key = *it;
                UnkNavMeshCell* cell = fn_710027a33c(mesh, key);
                for (u8 side = 0; side < cell->field_0x10; ++side) {
                    if (upward_only && !fn_710027d100(mesh, &key, &side, fn_7100413fd0(self->source)))
                        continue;
                    UnkNavMeshSideSegment segment;
                    segment.start = {0.0f, 0.0f};
                    segment.end = {0.0f, 0.0f};
                    segment.field_0x10 = -1.0f;
                    fn_71002afea8(cell, &side, &segment);
                    UnkVec2Result normal = fn_7100125e78(reinterpret_cast<UnkSegment2D*>(&segment));
                    if (normal.x * 0.0f + normal.y * 1.0f < 0.70710677f && upward_only)
                        continue;
                    CVector2D closest = {0.0f, 0.0f};
                    f32 t;
                    f32 dist = fn_71001262e0(reinterpret_cast<UnkSegment2D_71001262e0*>(&segment), &pos, &closest, &t);
                    if (!(box.min_x <= closest.x && closest.x <= box.max_x && box.min_y <= closest.y &&
                          dist < best && closest.y <= box.max_y))
                        continue;
                    if (Near(t, 1.0f)) {
                        // The end of the side: the next cell's side must face down.
                        u16 next_cell = 0xffff;
                        u8 next_side = 0xff;
                        fn_710027c460(mesh, key, side, true, &reach, fn_7100413fd0(self->source), &next_cell,
                                      &next_side);
                        if (next_cell == 0xffff || next_side == 0xff)
                            continue;
                        UnkNavMeshEdge* edge = fn_71002afbe4(fn_710027a33c(mesh, next_cell), &next_side);
                        if (!Near(edge->field_0xc, 0.0f) || !Near(edge->field_0x10, -1.0f))
                            continue;
                    } else {
                        // The start of the side: the next cell's side must face up.
                        if (!Near(t, 0.0f))
                            continue;
                        u16 next_cell = 0xffff;
                        u8 next_side = 0xff;
                        fn_710027c460(mesh, key, side, false, &reach, fn_7100413fd0(self->source), &next_cell,
                                      &next_side);
                        if (next_cell == 0xffff || next_side == 0xff)
                            continue;
                        UnkNavMeshEdge* edge = fn_71002afbe4(fn_710027a33c(mesh, next_cell), &next_side);
                        if (!Near(edge->field_0xc, 0.0f) || !Near(edge->field_0x10, 1.0f))
                            continue;
                    }
                    out->to = closest;
                    out->from = closest;
                    out->cell = key;
                    out->side = side;
                    best = dist;
                }
            }
        }
    }

    {
        UnkNavPoint_l10a164 point = target->movement->vf_0x2d8();
        out->target.side = point.side;
        out->target.cell = point.cell;
        out->target.pos = point.pos;
    }
    out->target_pos = out->target.pos;
    return true;
}

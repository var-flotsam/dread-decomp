// UnkTU811 (CEmmyAIComponent's unit): the cost of a path step for the EMMI, the base cost raised when the step
// crosses a navigation-mesh side facing away, when the target lies near the segment, or when it changes area.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU811.h"
#include "types.h"

struct UnkSelf_n;
struct UnkOther_n;
struct UnkSlot_n;
f32 fn_710110f4f4(UnkSelf_n* self, UnkOther_n* other, u32 a, u32 b, f32* pair, UnkSlot_n* slot, f32 value);

struct UnkPathPoint_l5571e0 {
    u8 pad_0x0[2];
    s16 cell;  // 0x2
    f32 x;     // 0x4
    f32 y;     // 0x8
    u8 pad_0xc[0x18 - 0xc];
    u32 area;  // 0x18
};
static_assert(sizeof(UnkPathPoint_l5571e0) == 0x1c, "UnkPathPoint_l5571e0 layout");

struct UnkPathStep_l5571e0 {
    UnkPathPoint_l5571e0 from;  // 0x0
    UnkPathPoint_l5571e0 to;    // 0x1c
};

struct UnkPoint2_l5571e0 {
    f32 x;
    f32 y;
};

struct UnkPathTarget_l5571e0 {
    u8 pad_0x0[0x84];
    UnkPoint2_l5571e0 pos;  // 0x84
};

struct UnkEmmyAI_l5571e0 {
    void** vtable;
    bool vf_0x7b8() { return reinterpret_cast<bool (*)(UnkEmmyAI_l5571e0*)>(vtable[0x7b8 / 8])(this); }
};

// An axis-aligned 2D box that grows to take in points.
struct UnkBox2_l5571e0 {
    f32 minX, minY, maxX, maxY;

    void Add(f32 x, f32 y) {
        minX = minX < x ? minX : x;
        minY = minY < y ? minY : y;
        maxX = maxX > x ? maxX : x;
        maxY = maxY > y ? maxY : y;
    }
    bool Contains(const UnkPoint2_l5571e0& p) const {
        return p.x >= minX && p.x <= maxX && minY <= p.y && maxY >= p.y;
    }
};

static inline UnkNavMeshManager* NavManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(sizeof(UnkNavMeshManager), 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

f32 fn_71005571e0(UnkEmmyAI_l5571e0* self, UnkOther_n* other, u32 cell, u32 sideIndex, f32* pair,
                  UnkPathStep_l5571e0* step, UnkPathTarget_l5571e0* target, f32 value) {
    u8 side = sideIndex;
    f32 cost = fn_710110f4f4(reinterpret_cast<UnkSelf_n*>(self), other, cell, sideIndex, pair,
                             reinterpret_cast<UnkSlot_n*>(step), value);

    if (side != 0xff && self->vf_0x7b8()) {
        UnkNavMeshManager* manager = NavManager();
        if (manager->navmesh) {
            UnkNavMeshEdge* edge = fn_71002afbe4(fn_710027a328(manager->navmesh, cell), &side);
            // The side's normal against the left direction.
            f32 facing = edge->field_0xc * -1.0f + edge->field_0x10 * -0.0f;
            if (facing > 0.1f)
                cost *= 4.0f;
        }
    }

    bool viewing = fn_7100543b30(reinterpret_cast<UnkEmmyAIView*>(self));
    if (step && viewing) {
        UnkPoint2_l5571e0 pos = target->pos;
        const UnkPathPoint_l5571e0& to = step->to;
        UnkBox2_l5571e0 box = {to.x - 250.0f, to.y - 0.0f, to.x + 250.0f, to.y + 0.0f};
        box.Add(step->from.x, step->from.y);
        if (!box.Contains(pos)) {
            box.Add(to.x - 500.0f, to.y - 0.0f);
            box.Add(to.x + 500.0f, to.y + 0.0f);
            box.Add(step->from.x, step->from.y);
            if (box.Contains(pos))
                cost += 250.0f;
        } else {
            cost += 500.0f;
        }
    }

    if (step && step->from.cell != -1 && step->to.cell != -1) {
        if (step->from.area != step->to.area)
            cost += 500.0f;
    }
    return cost;
}

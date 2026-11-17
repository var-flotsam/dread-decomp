// CChozoRobotSoldierAIComponent's unit (tools/tu_map.py unit 694): the cost of a navigation side, after the
// base's (fn_710110f4f4): raised when another soldier stands on it or plans to pass it, lowered when this
// one already plans to, and raised again when it lies behind the soldier. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU362.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkSelf_n;
struct UnkOther_n;
struct UnkSlot_n;
f32 fn_710110f4f4(UnkSelf_n* self, UnkOther_n* other, u32 a, u32 b, f32* pair, UnkSlot_n* slot, f32 value);

extern char g_unk_71726cca00[] __attribute__((visibility("hidden")));

// A navigation cell and side, as the position object reports it.
struct UnkNavCell_l474048 {
    u16 cell;
    u8 side;
    u8 pad_0x3[0xd];

    ~UnkNavCell_l474048() { fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(this)); }
};

struct UnkPositionObject_l474048 {
    void** vtable;

    UnkNavCell_l474048 Cell() {
        return reinterpret_cast<UnkNavCell_l474048 (*)(UnkPositionObject_l474048*)>(vtable[0x2d8 / 8])(this);
    }
};

// Whether the position object reports the cell and side.
static bool IsAt(UnkPositionObject_l474048* position, u32 cell, u32 side) {
    UnkNavCell_l474048 at = position->Cell();
    return at.cell != 0xffff && at.cell == (u16)cell && at.side != 0xff && at.side == (u8)side;
}

struct UnkSoldierOwner_l474048 {
    u8 pad_0x0[0x1f8];
    UnkPositionObject_l474048* position;  // 0x1f8
};

// A planned path's cell and side.
struct UnkPathStep_l474048 {
    u8 pad_0x0[0x8];
    u16 cell;
    u8 side;
    u8 pad_0xb[0x115];
};

struct UnkPathState_l474048 {
    void** vtable;

    bool IsValid() { return reinterpret_cast<bool (*)(UnkPathState_l474048*)>(vtable[0x20 / 8])(this); }
};

struct UnkPathSteps_l474048 {
    void** vtable;

    u32 Count() { return reinterpret_cast<u32 (*)(UnkPathSteps_l474048*)>(vtable[0x28 / 8])(this); }
};

// What every soldier on the list has: its owner and its planned path.
struct UnkSoldierBase_l474048 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkSoldierOwner_l474048* owner;  // 0x30
    u8 pad_0x38[0x19c0 - 0x38];
    bool has_path;  // 0x19c0
    u8 pad_0x19c1[0x7];
    UnkPathState_l474048 path_state;  // 0x19c8
    u8 pad_0x19d0[0x8];
    UnkPathSteps_l474048 path_steps;  // 0x19d8
    u8 pad_0x19e0[0x4];
    UnkPathStep_l474048 steps[0x10];  // 0x19e4
};

struct UnkSoldierAI_l474048 : UnkSoldierBase_l474048 {
    u8 pad_0x2be8[0x27618 - 0x2be8];
    CVector3D facing;  // 0x27618

    f32 vf_0x6f0() { return reinterpret_cast<f32 (*)(UnkSoldierAI_l474048*)>(vtable[0x6f0 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkSoldierAI_l474048, steps) == 0x19e4, "layout");
static_assert(__builtin_offsetof(UnkSoldierAI_l474048, facing) == 0x27618, "layout");

struct UnkSoldierEntry_l474048 {
    u8 pad_0x0[0x10];
    UnkSoldierBase_l474048* ai;  // 0x10
};

struct UnkSoldierList_l474048 {
    UnkSoldierEntry_l474048** data;
    u32 count;

    UnkSoldierEntry_l474048** begin() { return data; }
    UnkSoldierEntry_l474048** end() { return data + count; }
};

struct UnkCostTarget_l474048 {
    u8 pad_0x0[0x10c];
    f32 x;  // 0x10c
    f32 y;  // 0x110
    u8 pad_0x114[0xc];
    void* field_0x120;
};

// Whether `ai`'s planned path passes the cell and side; for this soldier's own path, notes it in `mine`
// instead.
static bool PathPasses(UnkSoldierBase_l474048* ai, UnkSoldierAI_l474048* self, u32 cell, u32 side, bool* mine) {
    if (ai->has_path && ai->path_state.IsValid()) {
        for (u64 i = 0; i < ai->path_steps.Count(); i++) {
            const UnkPathStep_l474048& step = ai->steps[i];
            if (step.cell == (u16)cell && step.side == (u8)side) {
                if (static_cast<UnkSoldierAI_l474048*>(ai) != self)
                    return true;
                *mine = true;
            }
        }
    }
    return false;
}

static inline bool IsNear_l474048(f32 a, f32 b) {
    return a + 0.001f >= b && a - 0.001f <= b;
}

f32 fn_7100474048(UnkSoldierAI_l474048* self, UnkCostTarget_l474048* target, u32 cell, u32 side, f32* pair,
                  UnkSlot_n* slot, f32 value) {
    f32 cost = fn_710110f4f4(reinterpret_cast<UnkSelf_n*>(self), reinterpret_cast<UnkOther_n*>(target), cell,
                             side, pair, slot, value);
    UnkSoldierList_l474048* list = reinterpret_cast<UnkSoldierList_l474048*>(
        fn_7100de94cc(g_unk_7101d47310, reinterpret_cast<const u64*>(g_unk_71726cca00)));
    if (list->count != 0) {
        bool occupied = false;
        bool passed = false;
        bool mine = false;
        for (UnkSoldierEntry_l474048* entry : *list) {
            if (!entry)
                continue;
            UnkSoldierBase_l474048* ai = entry->ai;
            if (!ai)
                continue;
            if (static_cast<UnkSoldierAI_l474048*>(ai) != self && fn_71011057fc(reinterpret_cast<UnkRobotSoldierAI_A2_b69*>(ai)) &&
                ai->owner->position) {
                if (IsAt(ai->owner->position, cell, side)) {
                    occupied = true;
                    break;
                }
            }
            passed = passed || PathPasses(ai, self, cell, side, &mine);
        }
        // Another soldier stands there; another plans to pass there; this one already plans to.
        if (occupied)
            cost += 100000.0f;
        else if (passed)
            cost = cost != 0.0f ? cost * 20.0f : cost + 1000.0f;
        else if (mine)
            cost *= 0.4f;
    }
    if (target && !target->field_0x120 && !(IsNear_l474048(pair[0], target->x) && IsNear_l474048(pair[1], target->y))) {
        f32 dx = pair[0] - target->x;
        f32 dy = pair[1] - target->y;
        f32 length = sqrtf(dx * dx + dy * dy);
        if (length != 0.0f) {
            dx /= length;
            dy /= length;
        }
        CVector2D facing;
        fn_710007b430(&facing, &self->facing);
        if (dx * facing.x + dy * facing.y < 0.7f)
            cost += self->vf_0x6f0();
    }
    return cost;
}

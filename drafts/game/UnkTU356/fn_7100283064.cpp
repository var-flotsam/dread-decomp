// Unit 356 (navmesh query state): looks for a point where an agent's cells meet the query's target cells
// and records it as the query's exit point. Names and layouts are placeholders.
#include "types.h"

struct UnkNavCellSide_l283064 {
    u16 cell;
    u8 side;

    bool operator==(const UnkNavCellSide_l283064& other) const {
        return cell == other.cell && side == other.side;
    }
};

struct UnkNavPoint_l283064 {
    f32 x;
    f32 y;
    UnkNavCellSide_l283064 id;
};

struct UnkNavTargetEntry_l283064 {
    UnkNavCellSide_l283064 id;
    f32 x;
    f32 y;
};

struct UnkNavTarget_l283064 {
    bool enabled;  // 0x0
    UnkNavPoint_l283064 single;  // 0x4
    u32 count;  // 0x10
    UnkNavTargetEntry_l283064 entries[23];  // 0x14
    u8 pad_0x128[8];
    bool direct;  // 0x130: the agent may not end in the target cells themselves
    u8 pad_0x131[7];
    UnkNavPoint_l283064 exit;  // 0x138

    // The entry for `id`, or null.
    const UnkNavTargetEntry_l283064* Find(const UnkNavCellSide_l283064& id) const {
        for (u32 i = 0; i < count; i++) {
            if (entries[i].id == id)
                return &entries[i];
        }
        return nullptr;
    }
};
static_assert(__builtin_offsetof(UnkNavTarget_l283064, direct) == 0x130, "target");
static_assert(__builtin_offsetof(UnkNavTarget_l283064, exit) == 0x138, "target");

struct UnkNavQueryState_l283064 {
    u8 pad_0x0[0x5dca0];
    UnkNavTarget_l283064 target;  // 0x5dca0
};

struct UnkNavAgent_l283064 {
    u8 pad_0x0[2];
    s16 field_0x2;
    u8 pad_0x4[0x1a];
    s16 field_0x1e;
    f32 x;  // 0x20
    f32 y;  // 0x24
    u8 pad_0x28[0x19];
    bool field_0x41;
    u8 pad_0x42[0xa];
    bool field_0x4c;
    u8 pad_0x4d[0x33];
    UnkNavCellSide_l283064* cells;  // 0x80
    u32 cell_count;  // 0x88
    u8 pad_0x8c[0x74];
    UnkNavCellSide_l283064 id;  // 0x100
    u32 mode;  // 0x104
};
static_assert(__builtin_offsetof(UnkNavAgent_l283064, cells) == 0x80, "agent");
static_assert(__builtin_offsetof(UnkNavAgent_l283064, mode) == 0x104, "agent");

bool fn_7100283064(UnkNavQueryState_l283064* self, const UnkNavAgent_l283064* agent) {
    if (agent == nullptr)
        return false;
    UnkNavTarget_l283064& target = self->target;
    if (!target.enabled)
        return false;

    if (!target.direct) {
        if (agent->mode == 1) {
            if (agent->id.cell == target.single.id.cell) {
                target.exit.x = target.single.x;
                target.exit.y = target.single.y;
                target.exit.id.cell = agent->id.cell;
                return true;
            }
        } else if (agent->mode == 2) {
            if (const UnkNavTargetEntry_l283064* entry = target.Find(agent->id)) {
                target.exit.x = entry->x;
                target.exit.y = entry->y;
                target.exit.id.cell = entry->id.cell;
                target.exit.id.side = entry->id.side;
                return true;
            }
        }
    }

    if (agent->field_0x2 == -1 || agent->field_0x1e == -1)
        return false;
    if (!agent->field_0x4c && !agent->field_0x41)
        return false;
    for (const UnkNavCellSide_l283064* cell = agent->cells; cell != agent->cells + agent->cell_count; cell++) {
        if (target.Find(*cell) != nullptr) {
            target.exit.x = agent->x;
            target.exit.y = agent->y;
            target.exit.id.cell = cell->cell;
            target.exit.id.side = cell->side;
            return true;
        }
    }
    return false;
}

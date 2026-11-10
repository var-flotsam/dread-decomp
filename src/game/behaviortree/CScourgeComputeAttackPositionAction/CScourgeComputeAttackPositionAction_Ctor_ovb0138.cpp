// behaviortree::CScourgeComputeAttackPositionAction's unit (tools/tu_map.py): the action's constructor
// (the shape of fn_71003b3028, src/game/behaviortree/CAutclastComputeAttackPositionAction/
// fn_71003b3028.cpp): the base node's constructor (fn_7100393e68), the vtable, three string ids at
// +0x30, +0x38 and +0x40 made the empty string (creating the pool first when there is none), and two
// cleared fields. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkBehaviorTree.h"
#include "types.h"

using base::global::UnkStringData;

extern void* g_unk_7101a61d18[];

struct UnkScourgeComputeAttackPosition_ovb0138 : UnkBehaviorTreeNodeBase {
    UnkStringData* ids[3];  // 0x30
    u64 field_0x48;
    u32 field_0x50;
};
static_assert(__builtin_offsetof(UnkScourgeComputeAttackPosition_ovb0138, ids) == 0x30, "layout");

static inline void MakeEmpty(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_710069d668(UnkScourgeComputeAttackPosition_ovb0138* self) {
    fn_7100393e68(self);
    self->vtable = g_unk_7101a61d18 + 2;
    MakeEmpty(&self->ids[0]);
    MakeEmpty(&self->ids[1]);
    MakeEmpty(&self->ids[2]);
    self->field_0x48 = 0;
    self->field_0x50 = 0;
}

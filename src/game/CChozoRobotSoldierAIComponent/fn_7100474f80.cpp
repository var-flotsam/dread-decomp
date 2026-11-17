// CChozoRobotSoldierAIComponent unit: whether a nav-mesh cell passes the filter's optional predicate and
// side `side` of it lies wholly at or below the filter's height limit. Names and layouts are placeholders.
#include "game/UnkNavMeshCell.h"
#include "types.h"

struct UnkCellFilter_l474f80 {
    void* context;
    bool (*accept)(void* context, u32 key);
    u8 pad_0x10[0x4];
    f32 limit;
    UnkNavMesh* mesh;
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
bool fn_7100474f80(UnkCellFilter_l474f80* self, u32 key, u8 side) {
    if (self->accept && !self->accept(self->context, key))
        return false;
    if (!self->mesh)
        return false;
    UnkNavMeshSideSegment seg = {};
    seg.field_0x10 = -1.0f;
    fn_71002afea8(fn_710027a33c(self->mesh, key), &side, &seg);
    if (seg.start.y <= self->limit && seg.end.y <= self->limit)
        return true;
    return false;
}

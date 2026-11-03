// CGoliathXBurstProjectionBombMovement unit (tools/tu_map.py, the name from reflection): keeps a weak
// handle to `target` (fn_7100d78378), or none when there is no target. Same shape as fn_710097a58c.
// Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self);

struct UnkBurstBombMovementTarget {
    u8 unk_0x0[0x230];
    UnkWeakHandle* handle;  // 0x230
};

void fn_710058c55c(UnkBurstBombMovementTarget* self, UnkWeakTarget* target) {
    AssignRef(&self->handle, target ? fn_7100d78378(target) : nullptr);
}

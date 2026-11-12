// CSynchronizedBlockCameraCtrl unit (tools/tu_map.py, the name from reflection): keeps a weak handle to
// `target` (fn_7100d78378), or none when there is no target. Same shape as fn_710097a58c.
// Names and layouts are placeholders.
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkSyncBlockCameraTarget {
    u8 unk_0x0[0x2a0];
    UnkWeakHandle* handle;
};

void fn_7100a20d7c(UnkSyncBlockCameraTarget* self, UnkWeakTarget* target) {
    AssignRef(&self->handle, target ? fn_7100d78378(target) : nullptr);
}

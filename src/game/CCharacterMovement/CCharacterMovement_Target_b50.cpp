// CCharacterMovement unit (tools/tu_map.py, the name from reflection): keeps a weak handle (+0x398) to
// `target` (fn_7100d78378), or none when there is no target. Names and layouts are placeholders.
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkCharacterMovementTarget {
    u8 unk_0x0[0x398];
    UnkWeakHandle* handle;
};

void fn_710097a58c(UnkCharacterMovementTarget* self, UnkWeakTarget* target) {
    AssignRef(&self->handle, target ? fn_7100d78378(target) : nullptr);
}

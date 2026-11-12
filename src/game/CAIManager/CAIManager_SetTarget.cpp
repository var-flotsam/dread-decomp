// CAIManager (unit 2540 of tools/tu_map.py, the name from reflection): keeps a weak handle
// (+0xbf0) to `target` unless the handle it holds still points at something. Names and layouts
// are placeholders.
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkAIManagerTarget {
    u8 unk_0x0[0xbf0];
    UnkWeakHandle* handle;
};

void fn_7100daae64(UnkAIManagerTarget* self, UnkWeakTarget* target) {
    if (!target)
        return;
    if (self->handle && self->handle->target)
        return;
    AssignRef(&self->handle, fn_7100d78378(target));
}

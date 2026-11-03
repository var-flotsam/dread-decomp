// Unit 1038 of tools/tu_map.py: a copy of fn_7100daae64 (CAIManager_SetTarget.cpp) for another
// owner; keeps a weak handle (+0x2d0) to `target` unless the one it holds still points at something.
// Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self);

struct UnkUnit1038Owner {
    u8 unk_0x0[0x2d0];
    UnkWeakHandle* handle;
};

void fn_71006a23c4(UnkUnit1038Owner* self, UnkWeakTarget* target) {
    if (!target)
        return;
    if (self->handle && self->handle->target)
        return;
    AssignRef(&self->handle, fn_7100d78378(target));
}

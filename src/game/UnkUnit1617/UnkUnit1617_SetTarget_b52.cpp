// From unit 1617 of tools/tu_map.py (no reflection hint): points the counted weak reference at +0x0
// at `target`'s weak handle (created on first use; game_s2/UnkWeakHandle.h), then clears the flag at
// +0x15. Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkWeakRefHolder_b52 {
    UnkWeakHandle* handle;
    u8 pad_0x8[0xd];
    bool field_0x15;
};

void fn_7100ab9910(UnkWeakRefHolder_b52* self, UnkWeakTarget* target) {
    AssignRef(&self->handle, GetWeakHandle(target));
    self->field_0x15 = false;
}

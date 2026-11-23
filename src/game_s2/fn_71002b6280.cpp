// Makes the weak reference at +0xf8 refer to the weak handle of `other` (created on first use), or to
// nothing when `other` is null or has no handle slot, releasing what it referred to before.
#include "game_s2/UnkWeakHandle.h"

struct UnkSelf_2b6280 {
    u8 pad_0x0[0xf8];
    UnkWeakHandle* field_0xf8;
};

void fn_71002b6280(UnkSelf_2b6280* self, UnkWeakTarget* other) {
    UnkWeakHandle* handle = other ? GetWeakHandle(other) : nullptr;
    AssignRef(&self->field_0xf8, handle);
}

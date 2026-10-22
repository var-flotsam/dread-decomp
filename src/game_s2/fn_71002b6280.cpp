// Makes the weak reference at +0xf8 refer to the weak handle of `other` (created on first use), or to
// nothing when `other` is null or has no handle slot, releasing what it referred to before.
#include "game_s2/UnkWeakHandle.h"

struct UnkSelf {
    u8 pad_0x0[0xf8];
    UnkWeakHandle* field_0xf8;
};

void fn_71002b6280(UnkSelf* self, UnkWeakTarget* other) {
    UnkWeakHandle* handle = other ? GetWeakHandle(other) : nullptr;
    AssignRef(&self->field_0xf8, handle);
}

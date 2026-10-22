// Makes the slot at +0x14890 refer to the weak handle of `other` (created on first use), or to
// nothing when `other` is null or has no handle slot, releasing what it referred to before.
#include "game_s2/UnkWeakHandle.h"

struct UnkSelf {
    u8 pad_0x0[0x14890];
    UnkWeakHandle* field_0x14890;
};

void fn_71004dc590(UnkSelf* self, UnkWeakTarget* other) {
    UnkWeakHandle* handle = other ? GetWeakHandle(other) : nullptr;
    AssignRef(&self->field_0x14890, handle);
}

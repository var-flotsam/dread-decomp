// Makes the weak reference at +0x29558 refer to the weak handle of `other` (created on first use), or
// to nothing when `other` is null or has no handle slot, then runs fn_71005416bc on `self` with 1.
#include "game_s2/UnkWeakHandle.h"

struct UnkSelf {
    u8 pad_0x0[0x29558];
    UnkWeakHandle* field_0x29558;
};

UnkSelf* fn_71005416bc(UnkSelf* self, u32 flag);

UnkSelf* fn_7100541860(UnkSelf* self, UnkWeakTarget* other) {
    UnkWeakHandle* handle = other ? GetWeakHandle(other) : nullptr;
    AssignRef(&self->field_0x29558, handle);
    return fn_71005416bc(self, 1);
}

// An object's weak handle, created on first use, and a setter making the weak reference at 0x398
// refer to another object's handle.
#include "game_s2/UnkWeakHandle.h"

UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self) { return GetWeakHandle(self); }

struct UnkWeakRef398 {
    u8 pad_0x0[0x398];
    UnkWeakHandle* field_0x398;
};

void fn_7100d7b964(UnkWeakRef398* self, UnkWeakTarget* other) {
    UnkWeakHandle* handle = other ? GetWeakHandle(other) : nullptr;
    AssignRef(&self->field_0x398, handle);
}

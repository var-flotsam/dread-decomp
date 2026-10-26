// Points the weak reference at +0x970 of `self` at `other`'s weak handle (created on first use) and
// copies the value `name` refers to into +0x978; without an `other`, drops the reference and stores
// the hash of the empty string (fn_7100001570, the CRC64 of a string) instead. The word is kept
// pointer-typed: as a u64 its store gets other alias info and the code changes.
#include "base/global/CStrId.h"
#include "game_s2/UnkWeakHandle.h"

struct UnkSelf {
    u8 pad_0x0[0x970];
    UnkWeakHandle* field_0x970;
    void* field_0x978;
};

void fn_7100343f7c(UnkSelf* self, UnkWeakTarget* other, void* const* name) {
    if (!other) {
        ResetRef(&self->field_0x970);
        self->field_0x978 = reinterpret_cast<void*>(fn_7100001570("", 0));
        return;
    }
    UnkWeakHandle* handle = GetWeakHandle(other);
    AssignRef(&self->field_0x970, handle);
    self->field_0x978 = *name;
}

// Points the object at +0x5e8 of `self` (through its weak handle) at nothing and links `self` to
// `other` instead: the weak handle slot at +0x5e8 is made to refer to `other`'s handle (created on
// first use), and when that leads to an object, that object's slot at +0x5f0 is made to refer to
// the weak handle of `self`.
#include "game_s2/UnkWeakHandle.h"

struct UnkLinked : UnkWeakTarget {
    u8 pad_0x8[0x5e0];
    UnkWeakHandle* field_0x5e8;
    UnkWeakHandle* field_0x5f0;
};

void fn_71004d7980(UnkLinked* self, UnkWeakTarget* other) {
    if (UnkWeakHandle* old = self->field_0x5e8) {
        if (UnkLinked* linked = static_cast<UnkLinked*>(old->target))
            ResetRef(&linked->field_0x5f0);
    }
    UnkWeakHandle* handle = other ? GetWeakHandle(other) : nullptr;
    AssignRef(&self->field_0x5e8, handle);
    if (handle && handle->target) {
        UnkWeakHandle* own = GetWeakHandle(self);
        UnkLinked* linked = self->field_0x5e8 ? static_cast<UnkLinked*>(self->field_0x5e8->target) : nullptr;
        AssignRef(&linked->field_0x5f0, own);
    }
}

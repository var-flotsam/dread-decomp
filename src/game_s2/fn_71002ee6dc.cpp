// Move assignment of a record: a first word, a string id (released, then taking the other's), 28 bytes of
// other data, an owning pointer (the old object is destroyed through its vtable slot at +0x10) and a last
// word.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkOwned {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

struct UnkRecord {
    u64 field_0x0;
    CStrId field_0x8;
    u8 field_0x10[28];
    UnkOwned* field_0x30;
    u64 field_0x38;
};

UnkRecord* fn_71002ee6dc(UnkRecord* self, UnkRecord* other) {
    if (self != other) {
        self->field_0x0 = other->field_0x0;
        self->field_0x8.ReleaseAndTake(other->field_0x8);
        __builtin_memcpy(self->field_0x10, other->field_0x10, sizeof(self->field_0x10));
        UnkOwned* taken = other->field_0x30;
        other->field_0x30 = nullptr;
        UnkOwned* old = self->field_0x30;
        self->field_0x30 = taken;
        if (old)
            old->vf_0x10();
        self->field_0x38 = other->field_0x38;
    }
    return self;
}

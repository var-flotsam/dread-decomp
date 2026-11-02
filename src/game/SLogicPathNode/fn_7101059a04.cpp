// From SLogicPathNode's unit: calls one virtual function (slot 6) with the same argument on every
// element of an array (+0x8, count at +0x10) of 0x20-byte polymorphic objects. Names and layouts
// are placeholders.
#include "types.h"

struct UnkPathElement {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30(void* arg);
    u8 pad_0x8[0x18];
};

struct UnkPathElementArray {
    u64 field_0x0;
    UnkPathElement* data;
    u32 count;
};

void fn_7101059a04(UnkPathElementArray* self, void* arg) {
    for (UnkPathElement *it = self->data, *end = self->data + self->count; it != end; ++it)
        it->vf_0x30(arg);
}

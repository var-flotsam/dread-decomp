// SLogicPathNode unit: two owners' arrays of polymorphic elements (0x50 bytes at +0x10, 0x20 bytes at
// +0x8) cleared in place: each element destroyed through vtable slot 1, then the count zeroed. The
// shape of base::UnkVector::Clear (include/base/UnkVector.h). Names and layouts are placeholders.
#include "types.h"

template <u64 Size>
struct UnkPathNodePolyElem {
    void** vtable;
    u64 words[(Size - 8) / 8];
};

template <u64 Size>
struct UnkPathNodePolyArray {
    UnkPathNodePolyElem<Size>* data;
    u32 count;

    void Clear() {
        for (u32 i = 0; i < count; ++i)
            reinterpret_cast<void (*)(UnkPathNodePolyElem<Size>*)>(data[i].vtable[1])(&data[i]);
        count = 0;
    }
};

struct UnkPathNodeOwnerA {
    u64 unk_0x0[2];
    UnkPathNodePolyArray<0x50> nodes;
};

struct UnkPathNodeOwnerB {
    u64 unk_0x0;
    UnkPathNodePolyArray<0x20> nodes;
};

void fn_7101059f8c(UnkPathNodeOwnerA* self) {
    self->nodes.Clear();
}

void fn_710105a2d0(UnkPathNodeOwnerB* self) {
    self->nodes.Clear();
}

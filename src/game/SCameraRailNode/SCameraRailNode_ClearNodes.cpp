// SCameraRailNode unit: two owners' arrays of polymorphic elements (0x50 bytes at +0x10, 0x58 bytes at
// +0x8) cleared in place: each element destroyed through vtable slot 1, then the count zeroed. The
// shape of base::UnkVector::Clear (include/base/UnkVector.h). Names and layouts are placeholders.
#include "types.h"

template <u64 Size>
struct UnkRailPolyElem {
    void** vtable;
    u64 words[(Size - 8) / 8];
};

template <u64 Size>
struct UnkRailPolyArray {
    UnkRailPolyElem<Size>* data;
    u32 count;

    void Clear() {
        for (u32 i = 0; i < count; ++i)
            reinterpret_cast<void (*)(UnkRailPolyElem<Size>*)>(data[i].vtable[1])(&data[i]);
        count = 0;
    }
};

struct UnkRailOwnerA {
    u64 unk_0x0[2];
    UnkRailPolyArray<0x50> nodes;
};

struct UnkRailOwnerB {
    u64 unk_0x0;
    UnkRailPolyArray<0x58> nodes;
};

void fn_7100c04380(UnkRailOwnerA* self) {
    self->nodes.Clear();
}

void fn_7100c046c8(UnkRailOwnerB* self) {
    self->nodes.Clear();
}

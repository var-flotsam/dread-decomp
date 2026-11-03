// SCameraRailNode and SLogicPathNode units: an owner's array of polymorphic elements (0x50 bytes at
// +0x10) visited in place, each element's vtable slot 4 called with `arg`. Names and layouts are
// placeholders.
#include "types.h"

struct UnkPolyNode50 {
    void** vtable;
    u64 words[9];

    void vf_0x20(void* arg) { reinterpret_cast<void (*)(UnkPolyNode50*, void*)>(vtable[4])(this, arg); }
};

struct UnkPolyNodeOwner {
    u64 unk_0x0[2];
    UnkPolyNode50* data;  // 0x10
    u32 count;            // 0x18

    void VisitAll(void* arg) {
        for (UnkPolyNode50 *it = data, *end = data + count; it != end; ++it)
            it->vf_0x20(arg);
    }
};

void fn_7100c043e0(UnkPolyNodeOwner* self, void* arg) {
    self->VisitAll(arg);
}

void fn_710105a0e4(UnkPolyNodeOwner* self, void* arg) {
    self->VisitAll(arg);
}

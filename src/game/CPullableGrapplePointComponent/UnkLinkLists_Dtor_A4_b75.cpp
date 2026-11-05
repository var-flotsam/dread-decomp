// A holder of three arrays (base::UnkVector) whose first array points at objects that keep a back pointer in their
// first word: the destructor clears every back pointer, empties the three arrays and frees them in reverse order.
// Four identical copies, one per unit (tools/tu_map.py): CPullableGrapplePointComponent (fn_710082c928),
// CMultiLockOnBlockComponent (fn_7100882460), CMorphBallLauncherComponent (fn_710088602c) and CItemLifeComponent
// (fn_71008e03dc). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkLinked_A4_b75 {
    void* back;
};

struct UnkLinkLists_A4_b75 {
    base::UnkVector<UnkLinked_A4_b75*> linked;  // 0x0
    base::UnkVector<u64> field_0x28;
    base::UnkVector<u64> field_0x50;

    __attribute__((always_inline)) void Destroy() {
        for (UnkLinked_A4_b75** it = linked.data, **end = linked.data + linked.count; it != end; ++it)
            (*it)->back = nullptr;
        linked.count = 0;
        field_0x28.count = 0;
        field_0x50.count = 0;
        field_0x50.Free();
        field_0x28.Free();
        linked.Free();
    }
};

void fn_710082c928(UnkLinkLists_A4_b75* self) {
    self->Destroy();
}

void fn_7100882460(UnkLinkLists_A4_b75* self) {
    self->Destroy();
}

void fn_710088602c(UnkLinkLists_A4_b75* self) {
    self->Destroy();
}

void fn_71008e03dc(UnkLinkLists_A4_b75* self) {
    self->Destroy();
}

// SCameraRailNode unit (tools/tu_map.py unit 1934): two destructors, copies of fn_71007ce1fc and
// fn_71007ce2ec (CXParasiteStayOnPlaceBehavior_Dtor_b49a3.cpp) with their own vtable. The vtable, then
// the callback set at +0x20; fn_7100c04454 also frees the object. Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "game/SCameraRailNode.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkCameraRailCallbackSet_b52 {
    void** vtable;  // 0x20
    UnkOwnedErasedVector callbacks;  // 0x20
    u32 field_0x50;
    UnkRefObject* owner;  // 0x50
};

struct UnkCameraRailCallbackOwner_b52 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkCameraRailCallbackSet_b52 set;  // 0x20

    __attribute__((always_inline)) ~UnkCameraRailCallbackOwner_b52();
};

// A delete expression lets the compiler drop the last store into the freed object (the capacity), as
// the original's deleting destructor does.
__attribute__((always_inline)) inline UnkCameraRailCallbackOwner_b52::~UnkCameraRailCallbackOwner_b52() {
    UnkCameraRailCallbackOwner_b52* self = this;
    self->vtable = reinterpret_cast<void**>(g_unk_7101aa2888 + 0x10);
    UnkCameraRailCallbackSet_b52* set = &self->set;
    set->vtable = g_unk_7101a20950 + 2;
    if (UnkRefObject* p = set->owner) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
    set->vtable = g_unk_7101a20998 + 2;
    // The array at +0x28 is destroyed as a member, after this.
}

void fn_7100c03ce0(UnkCameraRailCallbackOwner_b52* self) {
    self->~UnkCameraRailCallbackOwner_b52();
}

// By reference: the object is known to exist, so the delete has no null check.
void fn_7100c04454(UnkCameraRailCallbackOwner_b52& self) {
    delete &self;
}

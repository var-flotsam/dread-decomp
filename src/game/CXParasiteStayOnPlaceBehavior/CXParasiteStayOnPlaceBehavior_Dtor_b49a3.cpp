// CXParasiteStayOnPlaceBehavior unit (tools/tu_map.py unit 1229). Its destructors: the vtable, then the
// callback set at +0x20 (its vtable, the counted owner at +0x58 dropped, the base vtable, then the owned
// array of type-erased callbacks at +0x28); fn_71007ce2ec also frees the object. Names and layouts are
// placeholders.
#include "base/UnkErasedVector.h"
#include "game/CXParasiteStayOnPlaceBehavior.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkCallbackSet_b49a3 {
    void** vtable;  // 0x20
    UnkOwnedErasedVector callbacks;  // 0x28
    u32 field_0x50;
    UnkRefObject* owner;  // 0x58
};

struct UnkXParasiteStayOnPlaceBehavior_b49a3 {
    void** vtable;
    u8 pad_0x8[0x18];
    UnkCallbackSet_b49a3 set;  // 0x20

    __attribute__((always_inline)) ~UnkXParasiteStayOnPlaceBehavior_b49a3();
};

// A delete expression lets the compiler drop the last store into the freed object (the capacity), as
// the original's deleting destructor does.
__attribute__((always_inline)) inline UnkXParasiteStayOnPlaceBehavior_b49a3::~UnkXParasiteStayOnPlaceBehavior_b49a3() {
    UnkXParasiteStayOnPlaceBehavior_b49a3* self = this;
    self->vtable = reinterpret_cast<void**>(g_unk_7101a71848 + 0x10);
    UnkCallbackSet_b49a3* set = &self->set;
    set->vtable = g_unk_7101a20950 + 2;
    if (UnkRefObject* p = set->owner) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
    set->vtable = g_unk_7101a20998 + 2;
    // The array at +0x28 is destroyed as a member, after this.
}

void fn_71007ce1fc(UnkXParasiteStayOnPlaceBehavior_b49a3* self) {
    self->~UnkXParasiteStayOnPlaceBehavior_b49a3();
}

// By reference: the object is known to exist, so the delete has no null check.
void fn_71007ce2ec(UnkXParasiteStayOnPlaceBehavior_b49a3& self) {
    delete &self;
}

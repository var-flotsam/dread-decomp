// CXParasiteGoTransformBehavior's unit (tools/tu_map.py unit 1228): a deleting destructor (vtable 0x7101a71818)
// of a class holding a callback set at +0x20 (game/UnkCallbackSetVector.h's set class: its owner is released,
// then its callbacks) and an array of 0x40-byte polymorphic elements at +0x60. Names and layouts are
// placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkVector.h"
#include "game/CXParasiteGoTransformBehavior.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkGoRefCounted_ovb0124 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
};

// A callback set (vtable 0x7101a20950 over its base 0x7101a20998): the callbacks and a refcounted owner.
struct UnkGoCallbackSet_ovb0124 {
    void** vtable;
    UnkOwnedErasedVector callbacks;  // 0x8
    u32 field_0x30;
    UnkGoRefCounted_ovb0124* owner;  // 0x38

    __attribute__((always_inline)) void Destroy() {
        vtable = g_unk_7101a20950 + 2;
        if (UnkGoRefCounted_ovb0124* p = owner) {
            if (--p->ref_count == 0)
                p->Delete();
        }
        vtable = g_unk_7101a20998 + 2;
        callbacks.~UnkOwnedErasedVector();
    }
};

struct UnkGoElem_ovb0124 {
    virtual ~UnkGoElem_ovb0124();
    u8 pad_0x8[0x38];
};

struct UnkGoBehavior_ovb0124 {
    void** vtable;
    u8 pad_0x8[0x18];
    UnkGoCallbackSet_ovb0124 set;                  // 0x20
    base::UnkVector<UnkGoElem_ovb0124> elements;  // 0x60
};
static_assert(sizeof(UnkGoBehavior_ovb0124) == 0x88, "layout");

void fn_71007cd92c(UnkGoBehavior_ovb0124* self) {
    self->vtable = g_unk_7101a71818 + 2;
    self->elements.Free();
    self->set.Destroy();
    __builtin_operator_delete(self);
}

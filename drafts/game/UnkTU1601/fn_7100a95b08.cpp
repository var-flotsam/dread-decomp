// CThermalDeviceComponent's unit (tools/tu_map.py unit 1601): inserts a copy of an element (a callback set
// and a u32, 0x48 bytes) at an index of the element array, moving the later elements up one place after
// growing the array (fn_7100a956b8); at the end it appends (fn_7100a95868). Names and layouts are
// placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "game/CDemolitionBlockComponent.h"
#include "game/UnkTU1601.h"
#include "types.h"

struct UnkSetBase_la95b08 {  // vtable g_unk_7101a20998
    virtual void vf0();
    UnkOwnedErasedVector callbacks;
    u32 field_0x30;

    UnkSetBase_la95b08() : field_0x30(0) {}
};

// A callback set with the refcounted owner it belongs to; its move constructor is out of line
// (fn_71007b1928).
struct UnkSet_la95b08 : UnkSetBase_la95b08 {  // vtable g_unk_7101a33fa0
    virtual void vf8();
    base::UnkRefHandle owner;

    UnkSet_la95b08(const UnkSet_la95b08& other) {
        owner.ptr = nullptr;
        owner.Set(other.owner.ptr);
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callbacks, &other.callbacks);
    }
    ~UnkSet_la95b08() {
        if (base::UnkRefCounted* p = owner.ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkSetElem_la95b08 {
    UnkSet_la95b08 set;
    u32 field_0x40;
};
static_assert(sizeof(UnkSetElem_la95b08) == 0x48, "element size");

struct UnkSetVector_la95b08 {
    UnkSetElem_la95b08* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100a95b08(UnkSetVector_la95b08* self, const UnkSetElem_la95b08* value, u32 index) {
    if (index == self->count) {
        PushCopyOntoCallbackSetArray(reinterpret_cast<UnkCallbackSetVector*>(self),
                      reinterpret_cast<const UnkCallbackSetElem*>(value));
        return;
    }
    u32 n = self->count;
    u32 new_capacity = self->capacity;
    if (new_capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    CCharClassWaterPoolComponent_ReserveCallbackSetArray(reinterpret_cast<UnkCallbackSetVector*>(self), new_capacity);
    for (u32 i = self->count - 1; i >= index; --i) {
        UnkSetElem_la95b08* e = &self->data[i];
        fn_71007b1928(&e[1].set, &e->set);
        e[1].field_0x40 = e->field_0x40;
        self->data[i].~UnkSetElem_la95b08();
        if (i == 0)
            break;
    }
    new (&self->data[index]) UnkSetElem_la95b08(*value);
    ++self->count;
}

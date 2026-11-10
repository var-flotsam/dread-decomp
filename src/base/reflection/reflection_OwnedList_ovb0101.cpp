// The base::reflection unit 1717 (tools/tu_map.py): the destructor of a named list of owned objects
// (deleted through their vtable, slot 2), as the reflected owned-pointer collections hold them.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// An owned object, deleted through its vtable (slot 2).
struct UnkReflOwned_b0101 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

struct UnkReflOwnedList_b0101 {
    CStrId id;
    base::UnkVector<UnkReflOwned_b0101*> objects;  // 0x8
};

// Deletes the objects (clearing their slots), frees the array and releases the name.
void fn_7100b7857c(UnkReflOwnedList_b0101* self) {
    base::UnkVector<UnkReflOwned_b0101*>* objects = &self->objects;
    for (UnkReflOwned_b0101** it = objects->data; it != objects->data + objects->count;) {
        if (!*it) {
            ++it;
            continue;
        }
        (*it)->vf_0x10();
        *it++ = nullptr;
    }
    objects->Free();
    self->id.Reset();
}

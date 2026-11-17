// nn::time's unit (tools/tu_map.py): the destructor of an object holding a list of string-id nodes and
// three arrays whose first one points at objects with a back pointer. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/nn/time.h"
#include "types.h"

struct UnkIdNode_l06c1a4 {
    base::global::CStrId id;  // 0x0
    u8 unk_0x8[0x10];
    UnkIdNode_l06c1a4* next;  // 0x18
};

struct UnkLinked_l06c1a4 {
    void* back;
};

struct UnkTimeObj_l06c1a4 {
    void** vtable;                                              // 0x0
    u32 count;                                                  // 0x8
    UnkIdNode_l06c1a4* head;                                    // 0x10
    UnkIdNode_l06c1a4* tail;                                    // 0x18
    base::UnkAllocator* allocator;                              // 0x20
    u64 field_0x28;
    base::global::CRntVector<UnkLinked_l06c1a4*> linked;  // 0x30
    base::global::CRntVector<u64> field_0x58;
    base::global::CRntVector<u64> field_0x80;
};
static_assert(__builtin_offsetof(UnkTimeObj_l06c1a4, field_0x80) == 0x80, "layout");

// Clears the back pointers, frees the three arrays in reverse order, then (the base's destructor) releases
// each node's id and frees the node through the list's allocator.
void fn_710006c1a4(UnkTimeObj_l06c1a4* self) {
    for (UnkLinked_l06c1a4 **it = self->linked.data, **end = self->linked.data + self->linked.count; it != end; ++it)
        (*it)->back = nullptr;
    self->linked.count = 0;
    self->field_0x58.count = 0;
    self->field_0x80.count = 0;
    self->field_0x80.Free();
    self->field_0x58.Free();
    self->linked.Free();

    self->vtable = &g_unk_7101a1a160[2];
    for (UnkIdNode_l06c1a4* n = self->head; n;) {
        UnkIdNode_l06c1a4* next = n->next;
        n->id.~CStrId();
        self->allocator->vf_0x48(n);
        n = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
}

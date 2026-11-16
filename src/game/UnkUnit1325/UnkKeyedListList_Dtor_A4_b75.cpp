// Unit 1325 of tools/tu_map.py (no class hint): the destructor of a linked list (vtable g_unk_7101a7a548) whose nodes
// hold a string id and an inner linked list (vtable g_unk_7101a46040) of plain nodes: frees every inner node, empties
// the inner list, releases the id and frees the node through the outer list's allocator. Two identical copies
// (fn_710086f394, fn_710086f4a0). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CEmmyProtoAIComponent.h"
#include "game/UnkTU1325.h"
#include "types.h"
#include "base/UnkVtableList.h"

struct UnkInnerNode_A4_b75 {
    u64 value;
    UnkInnerNode_A4_b75* next;
};

struct UnkInnerList_A4_b75 : base::UnkVtableList<UnkInnerNode_A4_b75> {
    __attribute__((always_inline)) void Destroy() {
        vtable = reinterpret_cast<char*>(g_unk_7101a46040) + 0x10;
        Clear();
    }
};

struct UnkOuterNode_A4_b75 {
    base::global::CStrId id;
    u64 field_0x8;
    UnkInnerList_A4_b75 inner;  // 0x10
    u8 pad_0x40[0x8];
    UnkOuterNode_A4_b75* next;  // 0x48
};

struct UnkOuterList_A4_b75 : base::UnkVtableList<UnkOuterNode_A4_b75> {
    __attribute__((always_inline)) void Destroy() {
        vtable = reinterpret_cast<char*>(g_unk_7101a7a548) + 0x10;
        for (UnkOuterNode_A4_b75* n = head; n;) {
            UnkOuterNode_A4_b75* next = n->next;
            n->inner.Destroy();
            n->id.Reset();
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

void fn_710086f394(UnkOuterList_A4_b75* self) {
    self->Destroy();
}

void fn_710086f4a0(UnkOuterList_A4_b75* self) {
    self->Destroy();
}

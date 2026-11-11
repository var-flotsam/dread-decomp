// A fourth set of 8-byte values like UnkAIManagerNodeSet_b43.cpp's (tools/tu_map.py unit 893; its
// constructor fn_71005cce80 builds the list with room for 10 nodes per block): a vtable, then a
// pooled list of {value, next} nodes (base/UnkNodeOwner.h's layout, with the node list vtables
// g_unk_7101a32660/g_unk_7101a32680). The destructors and the add that skips values already
// present. Names and layouts are placeholders.
#include "base/UnkNodeOwner.h"
#include "base/UnkSList.h"
#include "game/UnkTU604.h"
#include "types.h"

extern void* g_unk_7101a522b8[];  // unit 893's set

template <u32 Next>
__attribute__((always_inline)) inline void UnkKeepNode_b44a2(UnkListNode<Next>*) {}

struct UnkNodeSetTag_b44a2;
using UnkNodeSetList_b44a2 = UnkNodeOwner<UnkListNode<8>, UnkKeepNode_b44a2<8>>;

struct UnkNodeSet_b44a2 {
    void** vtable;
    UnkNodeSetList_b44a2 list;

    base::UnkSList<UnkNodeSetTag_b44a2>* Values() {
        return reinterpret_cast<base::UnkSList<UnkNodeSetTag_b44a2>*>(&list);
    }

    void Destroy(void** set_vtable) {
        vtable = set_vtable + 2;
        list.Destroy(g_unk_7101a32660, g_unk_7101a32680);
    }

    __attribute__((always_inline)) s32 IndexOf(u64 v) {
        u32 i = 0;
        for (auto* n = Values()->head; n; n = n->next, ++i) {
            if (n->value == v)
                return (s32)i;
        }
        return -1;
    }

    __attribute__((always_inline)) void Add(u64 v) {
        if (IndexOf(v) < 0)
            Values()->PushBack(v);
    }
};

template <void** SetVtable>
struct UnkNodeSetDeleted_b44a2 : UnkNodeSet_b44a2 {
    ~UnkNodeSetDeleted_b44a2() { Destroy(SetVtable); }
};

void fn_71005ccea4(UnkNodeSet_b44a2* self) { self->Destroy(g_unk_7101a522b8); }
void fn_71005ccfa8(UnkNodeSetDeleted_b44a2<g_unk_7101a522b8>* self) {
    self->~UnkNodeSetDeleted_b44a2();
    __builtin_operator_delete(self);
}
void fn_71005cd094(UnkNodeSet_b44a2* self, u64 v) { self->Add(v); }

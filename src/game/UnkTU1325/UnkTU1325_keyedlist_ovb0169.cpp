// Unit 1325 (no class hint): a keyed list (vtable g_unk_7101a7a548) of timed nodes: each holds a string id, a
// timer, an inner list (vtable g_unk_7101a46040) of plain values and two flags. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CEmmyProtoAIComponent.h"
#include "game/UnkTU1325.h"
#include "types.h"
#include "base/UnkVtableList.h"

using base::global::CStrId;

struct UnkInnerNode_b0169 {
    u32 value;
    UnkInnerNode_b0169* next;
};

struct UnkInnerList_b0169 : base::UnkVtableList<UnkInnerNode_b0169> {
    __attribute__((always_inline)) void Destroy() {
        vtable = reinterpret_cast<char*>(g_unk_7101a46040) + 0x10;
        Clear();
    }
    __attribute__((always_inline)) void PushBack(const UnkInnerNode_b0169* src) {
        UnkInnerNode_b0169* n = static_cast<UnkInnerNode_b0169*>(allocator->vf_0x40(0x10, 8, flags));
        if (n) {
            n->value = src->value;
            n->next = nullptr;
            base::UnkLinkedList<UnkInnerNode_b0169>::PushBack(n);
        }
    }
};

struct UnkTimedEntry_b0169 {
    CStrId id;
    f32 timer;  // 0x8
    UnkInnerList_b0169 inner;  // 0x10
    union {
        u16 both;  // 0x40
        struct {
            u8 flag_a;  // 0x40
            u8 flag_b;  // 0x41
        };
    };
    u8 unk_0x42[6];

    UnkTimedEntry_b0169(CStrId& key) : id(key) {
        inner.vtable = reinterpret_cast<char*>(g_unk_7101a46040) + 0x10;
        inner.count = 0;
        inner.head = inner.tail = nullptr;
        inner.allocator = GetDefaultAllocator();
        inner.flags = 0x13;
        both = 0;
    }
    ~UnkTimedEntry_b0169() { inner.Destroy(); }
};

struct UnkTimedNode_b0169 : UnkTimedEntry_b0169 {
    UnkTimedNode_b0169* next;  // 0x48

    __attribute__((always_inline)) void Destroy(struct UnkTimedList_b0169* list);
};
static_assert(sizeof(UnkTimedNode_b0169) == 0x50, "layout");

struct UnkTimedList_b0169 : base::UnkVtableList<UnkTimedNode_b0169> {
    __attribute__((always_inline)) void Clear() {
        for (UnkTimedNode_b0169* n = head; n;) {
            UnkTimedNode_b0169* next = n->next;
            n->Destroy(this);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

__attribute__((always_inline)) void UnkTimedNode_b0169::Destroy(UnkTimedList_b0169* list) {
    inner.Destroy();
    id.Reset();
    list->allocator->vf_0x48(this);
}

// Whether the id's node has no values and neither flag.
bool fn_71008701b4(UnkTimedList_b0169* self, CStrId* id) {
    UnkTimedNode_b0169* n = self->head;
    for (; n; n = n->next) {
        if (n->id.data == id->data)
            break;
    }
    if (n && n->inner.count == 0 && !n->flag_b && !n->flag_a)
        return true;
    return false;
}

void fn_710086f5ac(UnkTimedList_b0169* self) {
    self->Clear();
}

// The list's deleting destructor.
void fn_71008702a8(UnkTimedList_b0169* self) {
    self->vtable = reinterpret_cast<char*>(g_unk_7101a7a548) + 0x10;
    self->Clear();
    __builtin_operator_delete(self);
}

// Appends the entry's contents to the list as a new node, taking over its id.
void fn_710086f9e0(UnkTimedList_b0169* self, UnkTimedEntry_b0169* other) {
    UnkTimedNode_b0169* node = static_cast<UnkTimedNode_b0169*>(self->allocator->vf_0x40(0x50, 8, self->flags));
    if (!node)
        return;
    node->id.data = other->id.data;
    other->id.data = &g_unk_7101d47108->empty;
    node->timer = other->timer;
    node->inner.vtable = reinterpret_cast<char*>(g_unk_7101a46040) + 0x10;
    node->inner.count = 0;
    node->inner.head = node->inner.tail = nullptr;
    node->inner.allocator = other->inner.allocator;
    node->inner.flags = other->inner.flags;
    for (UnkInnerNode_b0169* v = other->inner.head; v; v = v->next)
        node->inner.PushBack(v);
    node->both = other->both;
    node->next = nullptr;
    // self->PushBack(node), written out: calling the member schedules the empty list's `count = 1`
    // after the head and tail stores, where the original sets it up first.
    if (self->count == 0) {
        self->head = node;
        self->tail = node;
        self->count = 1;
    } else {
        self->tail->next = node;
        node->next = nullptr;
        self->tail = node;
        ++self->count;
    }
}

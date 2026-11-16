// Unit 1325 (no class hint): a keyed list (vtable g_unk_7101a7a548) of timed nodes: each holds a string id, a
// timer, an inner list (vtable g_unk_7101a46040) of plain values and two flags. Names and layouts are placeholders.
#include "base/UnkLinkedList.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a7a548[];
extern void* g_unk_7101a46040[];

struct UnkInnerNode_b0169 {
    u32 value;
    UnkInnerNode_b0169* next;
};

struct UnkInnerList_b0169 {
    void* vtable;
    base::UnkLinkedList<UnkInnerNode_b0169> body;  // 0x8

    __attribute__((always_inline)) void Destroy() {
        vtable = reinterpret_cast<char*>(g_unk_7101a46040) + 0x10;
        body.Clear();
    }
    __attribute__((always_inline)) void PushBack(const UnkInnerNode_b0169* src) {
        UnkInnerNode_b0169* n = static_cast<UnkInnerNode_b0169*>(body.allocator->vf_0x40(0x10, 8, body.flags));
        if (n) {
            n->value = src->value;
            n->next = nullptr;
            body.PushBack(n);
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
        inner.body.count = 0;
        inner.body.head = inner.body.tail = nullptr;
        inner.body.allocator = fn_7100083990();
        inner.body.flags = 0x13;
        both = 0;
    }
    ~UnkTimedEntry_b0169() { inner.Destroy(); }
};

struct UnkTimedNode_b0169 : UnkTimedEntry_b0169 {
    UnkTimedNode_b0169* next;  // 0x48

    __attribute__((always_inline)) void Destroy(struct UnkTimedList_b0169* list);
};
static_assert(sizeof(UnkTimedNode_b0169) == 0x50, "layout");

struct UnkTimedList_b0169 {
    void* vtable;
    base::UnkLinkedList<UnkTimedNode_b0169> body;  // 0x8

    // Clear, destroying each node's own list and id first.
    __attribute__((always_inline)) void Clear() {
        for (UnkTimedNode_b0169* n = body.head; n;) {
            UnkTimedNode_b0169* next = n->next;
            n->Destroy(this);
            n = next;
        }
        body.count = 0;
        body.head = nullptr;
        body.tail = nullptr;
    }
};

__attribute__((always_inline)) void UnkTimedNode_b0169::Destroy(UnkTimedList_b0169* list) {
    inner.Destroy();
    id.Reset();
    list->body.allocator->vf_0x48(this);
}

// Whether the id's node has no values and neither flag.
bool fn_71008701b4(UnkTimedList_b0169* self, CStrId* id) {
    UnkTimedNode_b0169* n = self->body.head;
    for (; n; n = n->next) {
        if (n->id.data == id->data)
            break;
    }
    if (n && n->inner.body.count == 0 && !n->flag_b && !n->flag_a)
        return true;
    return false;
}

void fn_710086f5ac(UnkTimedList_b0169* self) {
    self->Clear();
}

struct TmpEntry {
    u64 unused;
    UnkTimedEntry_b0169 e;
    TmpEntry(CStrId& key) : e(key) {}
};

// Appends the node's contents to the list, taking over its id.
__attribute__((noinline)) void fn_710086f9e0(UnkTimedList_b0169* self, UnkTimedEntry_b0169* other) {
    UnkTimedNode_b0169* node = static_cast<UnkTimedNode_b0169*>(self->body.allocator->vf_0x40(0x50, 8, self->body.flags));
    if (!node)
        return;
    node->id.data = other->id.data;
    other->id.data = &g_unk_7101d47108->empty;
    node->timer = other->timer;
    node->inner.vtable = reinterpret_cast<char*>(g_unk_7101a46040) + 0x10;
    node->inner.body.count = 0;
    node->inner.body.head = node->inner.body.tail = nullptr;
    node->inner.body.allocator = other->inner.body.allocator;
    node->inner.body.flags = other->inner.body.flags;
    for (UnkInnerNode_b0169* v = other->inner.body.head; v; v = v->next)
        node->inner.PushBack(v);
    node->both = other->both;
    node->next = nullptr;
    // self->body.PushBack(node), written out: calling the member schedules the empty list's `count = 1`
    // after the head and tail stores, where the original sets it up first.
    if (self->body.count == 0) {
        self->body.head = node;
        self->body.tail = node;
        self->body.count = 1;
    } else {
        self->body.tail->next = node;
        node->next = nullptr;
        self->body.tail = node;
        ++self->body.count;
    }
}

// The list's deleting destructor.
void fn_71008702a8(UnkTimedList_b0169* self) {
    self->vtable = reinterpret_cast<char*>(g_unk_7101a7a548) + 0x10;
    self->Clear();
    __builtin_operator_delete(self);
}

struct UnkTimedList;
struct UnkTimedNode;

// Unlinks `node`, frees it and returns the one after it.
UnkTimedNode_b0169* fn_710086f718(UnkTimedList* list_, UnkTimedNode* node_) {
    UnkTimedList_b0169* self = reinterpret_cast<UnkTimedList_b0169*>(list_);
    UnkTimedNode_b0169* node = reinterpret_cast<UnkTimedNode_b0169*>(node_);
    UnkTimedNode_b0169* prev;
    UnkTimedNode_b0169* it = nullptr;
    UnkTimedNode_b0169* next = self->body.head;
    while (next) {
        prev = it;
        it = next;
        next = next->next;
        if (it == node) {
            if (prev) {
                prev->next = next;
                if (self->body.tail == node)
                    self->body.tail = prev;
            } else {
                self->body.head = next;
            }
            if (--self->body.count == 0)
                self->body.head = self->body.tail = nullptr;
            if (node)
                node->Destroy(self);
            return next;
        }
    }
    return next;
}

static inline UnkTimedNode_b0169* FindNode(UnkTimedList_b0169* self, CStrId* id) {
    for (UnkTimedNode_b0169* n = self->body.head; n; n = n->next) {
        if (n->id.data == id->data)
            return n;
    }
    return nullptr;
}

void fn_710086f870(void* block, void* arg) {
    UnkTimedList_b0169* self = static_cast<UnkTimedList_b0169*>(block);
    CStrId* id = static_cast<CStrId*>(arg);
    UnkTimedNode_b0169* n = FindNode(self, id);
    if (n) {
        n->timer = -1.0f;
        n->inner.body.Clear();
        n->both = 0;
    } else {
        TmpEntry tmp(*id);
        tmp.e.timer = -1.0f;
        fn_710086f9e0(self, &tmp.e);
    }
}

void fn_710086fb24(UnkTimedList_b0169* self, CStrId* id, f32 timer) {
    UnkTimedNode_b0169* n = FindNode(self, id);
    if (n) {
        n->timer = -1.0f;
        n->inner.body.Clear();
        n->both = 0;
        n->timer = timer;
    } else {
        TmpEntry tmp(*id);
        tmp.e.timer = timer;
        fn_710086f9e0(self, &tmp.e);
    }
}

void fn_710086fca0(UnkTimedList_b0169* self, CStrId* id, UnkInnerList_b0169* src, bool b, bool a) {
    UnkTimedNode_b0169* n = FindNode(self, id);
    if (n) {
        n->timer = -1.0f;
        n->inner.body.Clear();
        n->both = 0;
    } else {
        TmpEntry tmp(*id);
        tmp.e.timer = -1.0f;
        fn_710086f9e0(self, &tmp.e);
        n = self->body.tail;
        if (!n)
            return;
    }
    if (&n->inner != src) {
        n->inner.body.Clear();
        for (UnkInnerNode_b0169* v = src->body.head; v; v = v->next)
            n->inner.PushBack(v);
    }
    n->flag_a = a;
    n->flag_b = b;
}

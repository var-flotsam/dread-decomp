// Unit 1325 (no class hint): a keyed list (vtable g_unk_7101a7a548) of timed nodes: each holds a string id, a
// timer, an inner list (vtable g_unk_7101a46040) of plain values and two flags. Names and layouts are placeholders.
#include "base/UnkVector.h"
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
    u32 count;
    UnkInnerNode_b0169* head;
    UnkInnerNode_b0169* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    __attribute__((always_inline)) void Clear() {
        for (UnkInnerNode_b0169* n = head; n;) {
            UnkInnerNode_b0169* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
    __attribute__((always_inline)) void Destroy() {
        vtable = reinterpret_cast<char*>(g_unk_7101a46040) + 0x10;
        Clear();
    }
    __attribute__((always_inline)) void PushBack(const UnkInnerNode_b0169* src) {
        UnkInnerNode_b0169* n = static_cast<UnkInnerNode_b0169*>(allocator->vf_0x40(0x10, 8, flags));
        if (n) {
            n->value = src->value;
            n->next = nullptr;
            if (count) {
                // Untyped store: the original reloads the count after linking the node in.
                __builtin_memcpy(&tail->next, &n, sizeof(n));
                n->next = nullptr;
                tail = n;
            } else {
                head = tail = n;
            }
            ++count;
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
        inner.allocator = fn_7100083990();
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

struct UnkTimedList_b0169 {
    void* vtable;
    u32 count;
    UnkTimedNode_b0169* head;  // 0x10
    UnkTimedNode_b0169* tail;
    base::UnkAllocator* allocator;
    u32 flags;

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

struct TmpEntry {
    u64 unused;
    UnkTimedEntry_b0169 e;
    TmpEntry(CStrId& key) : e(key) {}
};

// Appends the node's contents to the list, taking over its id.
__attribute__((noinline)) void fn_710086f9e0(UnkTimedList_b0169* self, UnkTimedEntry_b0169* other) {
    UnkTimedNode_b0169* node = static_cast<UnkTimedNode_b0169*>(self->allocator->vf_0x40(0x50, 8, self->flags));
    if (!node)
        return;
    node->id.data = other->id.data;
    other->id.data = &g_unk_7101d47108->empty;
    node->timer = other->timer;
    node->inner.count = 0;
    node->inner.head = node->inner.tail = nullptr;
    node->inner.vtable = reinterpret_cast<char*>(g_unk_7101a46040) + 0x10;
    node->inner.allocator = other->inner.allocator;
    node->inner.flags = other->inner.flags;
    for (UnkInnerNode_b0169* v = other->inner.head; v; v = v->next)
        node->inner.PushBack(v);
    node->both = other->both;
    node->next = nullptr;
    if (self->count) {
        // Untyped store: the original reloads the count after linking the node in.
        __builtin_memcpy(&self->tail->next, &node, sizeof(node));
        node->next = nullptr;
        self->tail = node;
    } else {
        self->head = self->tail = node;
    }
    ++self->count;
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
    UnkTimedNode_b0169* next = self->head;
    while (next) {
        prev = it;
        it = next;
        next = next->next;
        if (it == node) {
            if (prev) {
                prev->next = next;
                if (self->tail == node)
                    self->tail = prev;
            } else {
                self->head = next;
            }
            if (--self->count == 0)
                self->head = self->tail = nullptr;
            if (node)
                node->Destroy(self);
            return next;
        }
    }
    return next;
}

static inline UnkTimedNode_b0169* FindNode(UnkTimedList_b0169* self, CStrId* id) {
    for (UnkTimedNode_b0169* n = self->head; n; n = n->next) {
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
        n->inner.Clear();
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
        n->inner.Clear();
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
        n->inner.Clear();
        n->both = 0;
    } else {
        TmpEntry tmp(*id);
        tmp.e.timer = -1.0f;
        fn_710086f9e0(self, &tmp.e);
        n = self->tail;
        if (!n)
            return;
    }
    if (&n->inner != src) {
        n->inner.Clear();
        for (UnkInnerNode_b0169* v = src->head; v; v = v->next)
            n->inner.PushBack(v);
    }
    n->flag_a = a;
    n->flag_b = b;
}

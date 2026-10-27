// Drops the entries registered under an id from two keyed lists (notifying each entry's object
// first and hiding two items for flagged ids), then forwards the id to fn_7100f00f4c.
#include "base/UnkVector.h"
#include "engine/scene/UnkToggle.h"
#include "types.h"

struct UnkRefObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

static inline void RefAdd(UnkRefObj* o) { __atomic_fetch_add(&o->ref_count, 1, __ATOMIC_SEQ_CST); }
static inline void RefRelease(UnkRefObj* o) {
    __atomic_fetch_sub(&o->ref_count, 1, __ATOMIC_SEQ_CST);
    if (__atomic_load_n(&o->ref_count, __ATOMIC_SEQ_CST) == 0)
        o->vf_0x10();
}

struct UnkRef {
    UnkRefObj* ptr;
    UnkRef(const UnkRef& o) : ptr(o.ptr) {
        if (ptr)
            RefAdd(ptr);
    }
    ~UnkRef() {
        if (ptr)
            RefRelease(ptr);
    }
};

struct UnkListNode {
    u32 key;
    UnkRef ref;
    UnkListNode* next;
};

struct UnkList {
    u32 count;
    UnkListNode* head;
    UnkListNode* tail;
    base::UnkAllocator* allocator;

    UnkListNode* Find(u32 key) {
        for (UnkListNode* n = head; n; n = n->next)
            if (n->key == key)
                return n;
        return nullptr;
    }
    void Remove(UnkListNode* node) {
        UnkListNode* prev = nullptr;
        for (UnkListNode* n = head; n;) {
            UnkListNode* next = n->next;
            if (n == node) {
                if (prev) {
                    prev->next = next;
                    if (tail == node)
                        tail = prev;
                } else {
                    head = next;
                }
                if (--count == 0) {
                    head = nullptr;
                    tail = nullptr;
                }
                node->~UnkListNode();
                allocator->vf_0x48(node);
                return;
            }
            prev = n;
            n = next;
        }
    }
};

struct UnkHashNode {
    u32 key;
    u8 pad_0x4[0x14];
    bool field_0x18;
    u8 pad_0x19[0x17];
    UnkHashNode* next;
};

// An object holding a scene item at +0x18 (hidden for flagged ids).
struct UnkItemHolder {
    u8 pad_0x0[0x18];
    UnkToggle item;
};

struct UnkStruct_0x128 {
    u8 pad_0x0[0x10];
    UnkHashNode** buckets;
    u32 field_0x18;
    u32 bucket_count;
    u8 pad_0x20[0x60];
    UnkList field_0x80;
    u8 pad_0xa0[0x10];
    UnkList field_0xb0;
    u8 pad_0xd0[0x38];
    UnkItemHolder* field_0x108;
    UnkItemHolder* field_0x110;
    u8 pad_0x118[0x9];
    bool field_0x121;

    UnkHashNode* Get(u32 key) {
        u32 i = key % bucket_count;
        UnkHashNode* n = buckets[i];
        while (n->key != key)
            n = n->next;
        return n;
    }
};

void fn_710021fafc(UnkRefObj*);
void fn_7100f00f4c(UnkStruct_0x128*, u32, bool);

void fn_7100f011c4(UnkStruct_0x128* self, u32 id) {
    if (self->field_0x121)
        return;
    if (UnkListNode* n = self->field_0x80.Find(id)) {
        UnkRef ref = n->ref;
        fn_710021fafc(ref.ptr);
        if (self->Get(n->key)->field_0x18) {
            fn_710020ac34(&self->field_0x108->item, false);
            fn_710020ac34(&self->field_0x110->item, false);
        }
        self->field_0x80.Remove(n);
    }
    if (UnkListNode* n = self->field_0xb0.Find(id)) {
        UnkRef ref = n->ref;
        fn_710021fafc(ref.ptr);
        self->field_0xb0.Remove(n);
    }
    fn_7100f00f4c(self, id, true);
}

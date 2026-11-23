// Unregisters an id: unless forced, a locked id is refused; otherwise any running entries for it
// are stopped (fn_7100f011c4) and its registration node is removed. Returns false if not found.
#include "base/global/CRntVector.h"
#include "game/UnkTU2578.h"
#include "game/UnkTU324.h"
#include "game/UnkTU58.h"
#include "types.h"
#include "base/UnkLinkedList.h"

struct UnkRefObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
    u8 pad_0xc[0x20];
    u32 field_0x2c;
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

struct UnkList : base::UnkLinkedList<UnkListNode> {
    UnkListNode* Find(u32 key) {
        for (UnkListNode* n = head; n; n = n->next)
            if (n->key == key)
                return n;
        return nullptr;
    }
};

struct UnkHashNode_f00f4c {
    u32 key;
    u8 pad_0x4[0x17];
    bool field_0x1b;
    u8 pad_0x1c[0x14];
    UnkHashNode_f00f4c* next;
};

struct UnkStruct_0x128 {
    u8 pad_0x0[0x10];
    UnkHashNode_f00f4c** buckets;
    u32 field_0x18;
    u32 bucket_count;
    u8 pad_0x20[0x30];
    UnkList field_0x50;
    u8 pad_0x78[0x8];
    UnkList field_0x80;
    u8 pad_0xa8[0x8];
    UnkList field_0xb0;

    UnkHashNode_f00f4c* Find(u32 key) {
        if (field_0x18 == 0)
            return nullptr;
        u32 i = key % bucket_count;
        UnkHashNode_f00f4c** bucket = &buckets[i];
        if (!bucket)
            return nullptr;
        for (UnkHashNode_f00f4c* n = *bucket; n; n = n->next)
            if (n->key == key)
                return n;
        return nullptr;
    }

    bool IsActive(u32 id) {
        UnkListNode* n = field_0x80.Find(id);
        if (!n)
            return false;
        UnkRef ref = n->ref;
        u32 state = ref.ptr->field_0x2c;
        return state != 0xa && state != 0xc;
    }
};

bool fn_7100f00f4c(UnkStruct_0x128* self, u32 id, bool force) {
    if (!force) {
        UnkHashNode_f00f4c* h = self->Find(id);
        if (h && h->field_0x1b) {
            fn_71000810b0(0x10);
            return true;
        }
    }
    if (!self->field_0x50.Find(id))
        return true;
    if (self->IsActive(id) || self->field_0xb0.Find(id))
        fn_7100f011c4(self, id);
    if (UnkListNode* n = self->field_0x50.Find(id)) {
        fn_710021ea60(n->ref.ptr);
        self->field_0x50.Remove(n);
        return true;
    }
    return false;
}

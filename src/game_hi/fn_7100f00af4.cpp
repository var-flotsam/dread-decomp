// Starts the sound registered under an id unless it is already playing: optionally replaces its
// file path, asks the player for an instance, names it, keeps it in the playing list and marks the
// id as started. Returns false when disabled or when nothing could be started.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkRefObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
    u8 pad_0xc[0x14];
    base::global::CStrId field_0x20;
    u8 pad_0x28[0x4];
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
    UnkRef() : ptr(nullptr) {}
    UnkRef(const UnkRef& o) : ptr(o.ptr) {
        if (ptr)
            RefAdd(ptr);
    }
    ~UnkRef() {
        if (ptr)
            RefRelease(ptr);
    }
    UnkRef& operator=(const UnkRef& o) {
        if (o.ptr)
            RefAdd(o.ptr);
        if (ptr)
            RefRelease(ptr);
        ptr = o.ptr;
        return *this;
    }
};

struct UnkListNode {
    u32 key;
    UnkRef ref;
    // A union member (no type-based alias info): the original reloads the list count after
    // the link stores.
    union {
        UnkListNode* next;
    };
};

struct UnkList {
    u32 count;
    UnkListNode* head;
    UnkListNode* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    UnkListNode* Find(u32 key) {
        for (UnkListNode* n = head; n; n = n->next)
            if (n->key == key)
                return n;
        return nullptr;
    }
    void PushBack(u32 key, const UnkRef& value) {
        UnkListNode* n = static_cast<UnkListNode*>(allocator->vf_0x40(sizeof(UnkListNode), 8, flags));
        if (n) {
            n->key = key;
            n->ref.ptr = nullptr;
            n->ref = value;
            n->next = nullptr;
            if (count == 0) {
                head = n;
                tail = n;
                count = 1;
            } else {
                tail->next = n;
                n->next = nullptr;
                tail = n;
                ++count;
            }
        }
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

// A file-path string id (pool g_unk_7101d47200) with CStrId's copy-assignment.
struct UnkPathId {
    base::global::UnkStringData* data;

    UnkPathId& operator=(const UnkPathId& other) {
        if (data != other.data) {
            base::global::UnkStringData* d = other.data;
            base::global::UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
};

struct UnkHashNode {
    u32 key;
    u8 pad_0x4[0x4];
    UnkPathId field_0x8;
    base::global::CStrId field_0x10;
    u8 pad_0x18[0x1];
    bool field_0x19;
    u8 pad_0x1a[0x1];
    bool field_0x1b;
    u8 pad_0x1c[0x14];
    UnkHashNode* next;
};

struct UnkStruct_0x128 {
    u8 pad_0x0[0x10];
    UnkHashNode** buckets;
    u32 field_0x18;
    u32 bucket_count;
    u8 pad_0x20[0x30];
    UnkList field_0x50;
    u8 pad_0x78[0x8];
    UnkList field_0x80;
    u8 pad_0xa8[0x8];
    UnkList field_0xb0;
    u8 pad_0xd8[0x4a];
    bool field_0x122;

    UnkHashNode* Find(u32 key) {
        if (field_0x18 == 0)
            return nullptr;
        u32 i = key % bucket_count;
        UnkHashNode** bucket = &buckets[i];
        if (!bucket)
            return nullptr;
        for (UnkHashNode* n = *bucket; n; n = n->next)
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

struct UnkPathKey {
    u8 data[0x10];
    UnkPathKey(const UnkPathId* path);  // fn_710008d2ec
    ~UnkPathKey();                      // fn_710008d8e8
};

struct UnkPlayer;
extern UnkPlayer* g_unk_716e554378;

UnkRef fn_710021cab8(UnkPlayer*, const UnkPathKey&, u32, u32);

// NON_MATCHING(minor): when the started instance is null after the name release, the original
// skips re-storing the (still null) node pointer and re-tests the null flag before the second
// release; ours stores it anyway (stp) and threads the second test. Same behaviour.
bool fn_7100f00af4(UnkStruct_0x128* self, u32 id, UnkPathId* path, bool flag) {
    if (!self->field_0x122)
        return false;
    if (self->field_0x50.Find(id))
        return true;
    if (self->IsActive(id))
        return true;
    if (path) {
        if (UnkHashNode* h = self->Find(id))
            h->field_0x8 = *path;
    }
    UnkHashNode* h = self->Find(id);
    if (!h)
        return false;
    UnkRef ref = fn_710021cab8(g_unk_716e554378, UnkPathKey(&h->field_0x8), h->field_0x19 ? 0 : 2, flag);
    if (!ref.ptr)
        return false;
    ref.ptr->field_0x20 = h->field_0x10;
    {
        UnkRef a = ref;
        UnkRef b = a;
        self->field_0x50.PushBack(id, b);
    }
    h->field_0x1b = true;
    return true;
}

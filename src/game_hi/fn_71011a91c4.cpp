// Handles a resource-backed item: unless deferring, looks up its resource's handler and queues
// the handler when it is idle; otherwise (or when nothing is queued) resets the item and
// forwards it to the global manager.
#include "types.h"

struct UnkKey_71011a91c4 {
    u8 unk_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkRes_71011a91c4 {
    virtual void* vf_0x0();
    u8 unk_0x8[0x8];
    UnkKey_71011a91c4* key;
};

struct UnkHandler_71011a91c4 {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18();
    virtual s32 vf_0x20();
    u8 unk_0x8[0x10];
    u32 field_0x18;
};

struct UnkMapNode_71011a91c4 {
    UnkKey_71011a91c4* key;
    UnkHandler_71011a91c4* value;
    u8 unk_0x10[0x10];
    UnkMapNode_71011a91c4* next;
};

struct UnkItemBase_71011a91c4 {
    void* vtable;
};

struct UnkItem_71011a91c4 {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18();
    virtual void vf_0x20(); virtual void vf_0x28(); virtual void vf_0x30(); virtual void vf_0x38();
    virtual void vf_0x40(bool);
    virtual void vf_0x48(bool);
    UnkItemBase_71011a91c4 base;
    UnkRes_71011a91c4* res;
    void vf_0x170_call();
};

struct UnkItemVt_71011a91c4 {
    void* slots[0x170 / 8];
    void (*vf_0x170)(UnkItem_71011a91c4*);
};

struct UnkListNode_71011a91c4 {
    UnkHandler_71011a91c4* value;
    UnkListNode_71011a91c4* next;
};

struct UnkAllocator_71011a91c4 {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18();
    virtual void vf_0x20(); virtual void vf_0x28(); virtual void vf_0x30(); virtual void vf_0x38();
    virtual void* vf_0x40(u64 size, u64 align, u32 flags);
};

struct UnkStruct_71011a91c4 {
    u8 unk_0x0[0x48];
    UnkMapNode_71011a91c4** buckets;
    u32 count;
    u32 bucket_count;
    u8 unk_0x58[0x128];
    union {
        u32 queue_count;
    };
    UnkListNode_71011a91c4* queue_head;
    UnkListNode_71011a91c4* queue_tail;
    UnkAllocator_71011a91c4* allocator;
    u32 alloc_flags;

    UnkMapNode_71011a91c4** BucketFor(UnkKey_71011a91c4* const& key) {
        if (count == 0)
            return nullptr;
        u32 idx = (key->hash_lo ^ key->hash_hi) % bucket_count;
        return buckets + idx;
    }
};

void* fn_71000e8a5c();
bool fn_710009d50c(void* type, void* base);
bool fn_710104ddcc(UnkHandler_71011a91c4* h, UnkItem_71011a91c4* item);
u32 fn_710104de7c(UnkHandler_71011a91c4* h);
bool fn_71002199d8(void* mgr, UnkItemBase_71011a91c4* item, u32 flag);
extern void* g_unk_7101d47300;

static inline UnkRes_71011a91c4* CastRes_71011a91c4(UnkRes_71011a91c4* r) {
    if (!r)
        return nullptr;
    return fn_710009d50c(r->vf_0x0(), fn_71000e8a5c()) ? r : nullptr;
}

// Queues the handler registered for the item's resource when it is idle; false when there is
// none or it doesn't accept the item.
static inline bool TryQueue_71011a91c4(UnkStruct_71011a91c4* self, UnkItem_71011a91c4* item) {
    UnkRes_71011a91c4* res = CastRes_71011a91c4(item->res);
    UnkMapNode_71011a91c4** bucket = self->BucketFor(res->key);
    if (!bucket)
        return false;
    UnkMapNode_71011a91c4* n = *bucket;
    for (; n; n = n->next)
        if (n->key == res->key)
            break;
    if (!n)
        return false;
    if (!fn_710104ddcc(n->value, item))
        return false;
    s32 busy = n->value->vf_0x20();
    u32 state = fn_710104de7c(n->value);
    if ((busy == 0) & (state == n->value->field_0x18)) {
        UnkListNode_71011a91c4* q =
            static_cast<UnkListNode_71011a91c4*>(self->allocator->vf_0x40(0x10, 8, self->alloc_flags));
        if (q) {
            q->value = n->value;
            q->next = nullptr;
            if (self->queue_count == 0) {
                self->queue_head = q;
                self->queue_tail = q;
            } else {
                self->queue_tail->next = q;
                q->next = nullptr;
                self->queue_tail = q;
            }
            self->queue_count++;
        }
    }
    return true;
}

static inline bool Forward_71011a91c4(UnkItem_71011a91c4* item, u32 flag) {
    item->vf_0x40(false);
    item->vf_0x48(false);
    return fn_71002199d8(g_unk_7101d47300, item ? &item->base : nullptr, flag);
}

bool fn_71011a91c4(UnkStruct_71011a91c4* self, UnkItem_71011a91c4* item, u32 flag) {
    reinterpret_cast<UnkItemVt_71011a91c4*>(*reinterpret_cast<void**>(item))->vf_0x170(item);
    if (__builtin_expect(!flag, 0) && TryQueue_71011a91c4(self, item))
        return true;
    return Forward_71011a91c4(item, flag);
}

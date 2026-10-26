// Inserts a node for a newly created key at the head of its bucket in a chained hash table:
// the node (hash, key, next) comes from the allocator at 0x50, the lowest used bucket (0x40)
// and the first node (0x48) follow, and the node count at 0x5c goes up.
#include "base/UnkEmptyDtorElem.h"
#include "types.h"

struct UnkKey {
    u16 field_0x0;
    u8 field_0x2;
    u8 unk_0x3;
};

struct UnkNode {
    u32 hash;
    UnkKey key;
    UnkNode* next;
};

struct UnkNodeAllocator {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void* vf_0x40(u64 size, u64 align, u32 flags);
};

struct UnkHashTable {
    u8 unk_0x0[0x18];
    u8 buckets[8];
    u32 bucket_count;
    u32 modulus;
    u8 unk_0x28[0x18];
    u32 lowest;
    u8 unk_0x44[4];
    UnkNode* first;
    UnkNodeAllocator* allocator;
    u32 alloc_flags;
    u32 count;
    u32 field_0x60_count;
    u32 inserts;
};

void fn_71002874d0(UnkKey* key);
u32 fn_71002874e0(u16 a, u8 b);
UnkNode** fn_71002b288c(void* buckets, u32 index);

void fn_71002b26fc(UnkHashTable* self) {
    UnkKey key;
    fn_71002874d0(&key);
    u32 hash = fn_71002874e0(key.field_0x0, key.field_0x2);
    self->field_0x60_count++;
    UnkNode* node = static_cast<UnkNode*>(self->allocator->vf_0x40(0x10, 8, self->alloc_flags));
    if (node) {
        node->hash = hash;
        node->key = key;
        node->next = nullptr;
    }
    u32 index = self->bucket_count ? hash % self->modulus : 0xffffffffu;
    u32 lowest = self->lowest;
    self->lowest = index < lowest ? index : lowest;
    if (index <= lowest)
        self->first = node;
    UnkNode** head = fn_71002b288c(self->buckets, index);
    node->next = *head;
    *head = node;
    self->count++;
    fn_71002874dc(&key);
}

// Unit 1036: the copy constructor of a pooled list of reference-counted values (16-byte nodes; a source without
// a pool only gives its allocator). Names and layouts are placeholders.
#include "base/UnkBlockFreeList.h"
#include "base/UnkVector.h"
#include "game/UnkTU852.h"
#include "types.h"

struct UnkCounted_b260 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Destroy();  // vf_0x10
    // A union member: the original reloads the node's value after the increment.
    union {
        s32 ref_count;       // 0x8
    };
};

struct UnkCountedNode_b260 {
    UnkCounted_b260* value;
    UnkCountedNode_b260* next;
};

struct UnkCountedPool_b260 {
    char* vtable;
    base::UnkAllocator* parent;
    UnkBlockFreeList blocks;
};

struct UnkCountedList_b260 {
    char* vtable;
    u32 count;
    UnkCountedNode_b260* head;
    UnkCountedNode_b260* tail;
    base::UnkAllocator* allocator;
    u32 flags;
    UnkCountedPool_b260 pool;  // +0x30
    u32 field_0x68;
    bool field_0x6c;

    void PushBack(UnkCountedNode_b260* node) {
        if (count == 0) {
            head = node;
            tail = node;
        } else {
            __builtin_memcpy(&tail->next, &node, sizeof(node));
            node->next = nullptr;
            tail = node;
        }
        ++count;
    }
};

void fn_71006a15c8(UnkCountedList_b260* self, const UnkCountedList_b260* other) {
    self->count = 0;
    self->vtable = reinterpret_cast<char*>(g_unk_7101a4d918) + 0x10;
    self->head = nullptr;
    self->tail = nullptr;
    self->allocator = fn_7100083990();
    self->flags = 0x13;
    self->vtable = reinterpret_cast<char*>(g_unk_7101a4d8f8) + 0x10;
    self->pool.vtable = g_unk_7101a1a4c8 + 0x10;
    self->pool.parent = nullptr;
    fn_710008377c(&self->pool.blocks);
    self->field_0x68 = 0;
    self->field_0x6c = false;
    u32 flags = other->flags;
    self->flags = flags;
    base::UnkAllocator* source = other->pool.parent;
    if (!source) {
        self->allocator = other->allocator;
        return;
    }
    u64 other_size = other->pool.blocks.size;
    if (void* buffer = self->pool.blocks.buffer) {
        fn_71000837f4(&self->pool.blocks);
        self->pool.parent->vf_0x48(buffer);
        self->pool.parent = nullptr;
    }
    u64 size = other_size & 0xffffffff0ull;
    if (size) {
        self->pool.parent = source;
        void* buffer = source->vf_0x40(size, 8, flags);
        fn_710008378c(&self->pool.blocks, buffer, size, 0x10);
    }
    self->allocator = reinterpret_cast<base::UnkAllocator*>(reinterpret_cast<char*>(self) +
                                                            __builtin_offsetof(UnkCountedList_b260, pool));
    if (self == other)
        return;
    for (UnkCountedNode_b260* n = self->head; n;) {
        UnkCountedNode_b260* next = n->next;
        UnkCounted_b260* value = n->value;
        if (value && --value->ref_count == 0)
            value->Destroy();
        self->allocator->vf_0x48(n);
        n = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    for (UnkCountedNode_b260* n = other->head; n; n = n->next) {
        void* mem = self->allocator->vf_0x40(sizeof(UnkCountedNode_b260), 8, self->flags);
        if (!mem)
            continue;
        UnkCountedNode_b260* node = static_cast<UnkCountedNode_b260*>(mem);
        node->value = nullptr;
        UnkCounted_b260* value = n->value;
        if (value) {
            ++value->ref_count;
            UnkCounted_b260* old = node->value;
            if (old && --old->ref_count == 0)
                old->Destroy();
            node->value = value;
        }
        node->next = nullptr;
        self->PushBack(node);
    }
}

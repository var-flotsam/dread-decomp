// Unit 604: the copy constructor of a list of 8-byte values in 0x10-byte nodes over an embedded block
// pool: the pool is sized like the other's, then the other's values are appended in order. Names and
// layouts are placeholders.
#include "base/UnkBlockFreeList.h"
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU604.h"
#include "types.h"

struct UnkValueNode_l3f9d0c {
    u64 value;
    UnkValueNode_l3f9d0c* next;
};

struct UnkValueList_l3f9d0c {
    void** vtable;
    u32 count;
    UnkValueNode_l3f9d0c* head;     // 0x10
    UnkValueNode_l3f9d0c* tail;     // 0x18
    base::IAllocator* allocator;  // 0x20: the pool once it has one
    u32 flags;                      // 0x28
    base::UnkBlockPool pool;        // 0x30
};
static_assert(__builtin_offsetof(UnkValueList_l3f9d0c, pool) == 0x30, "");

void fn_71003f9d0c(UnkValueList_l3f9d0c* self, const UnkValueList_l3f9d0c* other) {
    using Node = UnkValueNode_l3f9d0c;
    self->vtable = g_unk_7101a32680 + 2;
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    base::IAllocator* allocator = GetDefaultAllocator();
    self->flags = 0x13;
    self->allocator = allocator;
    self->vtable = g_unk_7101a32660 + 2;
    self->pool.Init();
    self->pool.field_0x38 = 0;
    self->pool.field_0x3c = false;
    u32 flags = other->flags;
    self->flags = flags;

    base::IAllocator* parent = other->pool.parent;
    if (!parent) {
        self->allocator = other->allocator;
        return;
    }
    u32 n = other->pool.blocks.size / sizeof(Node);
    self->pool.Release();
    if (n) {
        self->pool.parent = parent;
        u64 size = (u64)n * sizeof(Node);
        fn_710008378c(&self->pool.blocks, parent->vf_0x40(size, 8, flags), size, sizeof(Node));
    }
    // The pool's address, formed as an integer (as in base::UnkPoolList::Init).
    self->allocator = reinterpret_cast<base::IAllocator*>(reinterpret_cast<u64>(self) + 0x30);

    if (self == other)
        return;
    for (Node* node = self->head; node;) {
        Node* next = node->next;
        self->allocator->vf_0x48(node);
        node = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    for (const Node* src = other->head; src; src = src->next) {
        Node* node = static_cast<Node*>(self->allocator->vf_0x40(sizeof(Node), 8, self->flags));
        if (!node)
            continue;
        node->value = src->value;
        node->next = nullptr;
        if (self->count == 0) {
            self->head = node;
            self->tail = node;
        } else {
            self->tail->next = node;
            node->next = nullptr;
        }
        self->tail = node;
        ++self->count;
    }
}

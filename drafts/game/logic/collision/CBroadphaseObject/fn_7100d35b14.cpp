// game::logic::collision::CBroadphaseObject: appends a (key, value) pair to an owned singly linked
// list unless the pair is already there. Names and layouts are placeholders.
#include "types.h"

struct UnkAllocator {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void* Alloc(u64 size, u64 align, u32 tag);
};

struct UnkPairNode {
    u64 key;
    s32 value;
    UnkPairNode* next;
};

struct UnkBroadphaseObject {
    u8 pad_0x0[0xf8];
    u32 count;
    u8 pad_0xfc[4];
    UnkPairNode* head;
    UnkPairNode* tail;
    UnkAllocator* allocator;
    u32 allocTag;
    u8 pad_0x11c[4];
    u8 locked;
};

void fn_7100d35b14(UnkBroadphaseObject* self, s32 value, u64 key) {
    if (self->locked)
        return;

    u32 index = 0;
    for (UnkPairNode* n = self->head; n; n = n->next, ++index) {
        if (n->key == key && n->value == value) {
            if (static_cast<s32>(index) >= 0)
                return;
            break;
        }
    }

    UnkPairNode* node = static_cast<UnkPairNode*>(self->allocator->Alloc(sizeof(UnkPairNode), 8, self->allocTag));
    if (!node)
        return;
    node->key = key;
    node->value = value;
    node->next = nullptr;

    u32 newCount;
    if (self->count == 0) {
        newCount = 1;
        UnkPairNode* ends[2] = {node, node};
        // writes head and tail together, as one pair store
        __builtin_memcpy(&self->head, ends, sizeof(ends));
        self->count = newCount;
    } else {
        self->tail->next = node;
        __builtin_memset(&node->next, 0, sizeof(node->next));
        newCount = self->count + 1;
        self->tail = node;
        self->count = newCount;
    }
}

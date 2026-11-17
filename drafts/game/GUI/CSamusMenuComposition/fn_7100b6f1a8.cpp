// GUI::CSamusMenuComposition's unit (1714): splits a C string at a separator into a list of strings whose
// nodes come from a block pool the list keeps, growing the pool first when the pieces outnumber its blocks.
// Names and layouts are placeholders.
#include <string.h>

#include "base/UnkBlockFreeList.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::UnkAllocator;
using base::global::CRntString;

struct UnkStrNode_b6f {
    CRntString str;
    UnkStrNode_b6f* next;  // +0x30
    UnkStrNode_b6f* prev;  // +0x38
};
static_assert(sizeof(UnkStrNode_b6f) == 0x40, "");

struct UnkStrList_b6f {
    void** vtable;
    u32 count;  // +0x8
    UnkStrNode_b6f* head;  // +0x10
    UnkStrNode_b6f* tail;  // +0x18
    u32 flags;  // +0x20
    UnkAllocator* allocator;  // +0x28: the nodes' allocator (the pool below once it has a block)
    UnkAllocator* parent;  // +0x30: where the pool's block comes from
    void* pool_vtable;  // +0x38: the node pool
    UnkAllocator* pool_parent;  // +0x40
    UnkBlockFreeList pool_blocks;  // +0x48

    void Clear() {
        for (UnkStrNode_b6f* node = head; node;) {
            UnkStrNode_b6f* next = node->next;
            fn_710007ecd4(&node->str);
            allocator->vf_0x48(node);
            node = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }

    u32 Capacity() const { return pool_blocks.size / sizeof(UnkStrNode_b6f); }

    // Gives the pool one block for `nodes` nodes.
    void Reserve(u32 nodes) {
        u32 block_flags = flags;
        UnkAllocator* own = allocator;
        UnkAllocator* from = parent;
        if (!from)
            from = own;
        if (void* block = pool_blocks.buffer) {
            fn_71000837f4(&pool_blocks);
            pool_parent->vf_0x48(block);
            pool_parent = nullptr;
        }
        if (!from)
            from = fn_7100083990();
        pool_parent = from;
        u64 size = (u64)nodes * sizeof(UnkStrNode_b6f);
        void* buffer = from->vf_0x40(size, 8, block_flags);
        fn_710008378c(&pool_blocks, buffer, size, sizeof(UnkStrNode_b6f));
        allocator = reinterpret_cast<UnkAllocator*>(&pool_vtable);
    }

    UnkStrNode_b6f* PushBack() {
        UnkStrNode_b6f* node = static_cast<UnkStrNode_b6f*>(allocator->vf_0x40(sizeof(UnkStrNode_b6f), 8, flags));
        if (node) {
            fn_710007e6c4(node);
            node->next = nullptr;
            node->prev = nullptr;
        }
        if (count == 0) {
            head = node;
            tail = node;
            count = 1;
        } else {
            tail->next = node;
            node->next = nullptr;
            node->prev = tail;
            ++count;
            tail = node;
        }
        return node;
    }
};

void fn_7100b6f1a8(const char* str, const char* separator, UnkStrList_b6f* out) {
    out->Clear();
    u32 length = strlen(str);
    u32 separator_length = strlen(separator);
    if (str) {
        u32 pieces = 0;
        const char* p = str;
        while (p) {
            p = strstr(p, separator);
            if (p)
                p += separator_length;
            ++pieces;
        }
        if (pieces > out->Capacity())
            out->Reserve(pieces);
    }
    if (length) {
        const char* p = str;
        u32 pos = 0;
        do {
            const char* end = strstr(p, separator);
            if (!end) {
                size_t rest = strlen(p);
                if (!rest)
                    return;
                end = p + rest;
            }
            UnkStrNode_b6f* node = out->PushBack();
            u32 n = end - p;
            fn_710007e7f8(&node->str, p, n);
            u32 step = n + separator_length;
            pos += step;
            p += step;
        } while (pos < length);
    }
}

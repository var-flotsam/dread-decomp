// CMeleeComponent's unit (tools/tu_map.py): splits a C string at each occurrence of a separator into a
// list of strings. The list is emptied first and its node pool grown to hold every piece. Names and
// layouts are placeholders.
#include <string.h>

#include <new>

#include "../../game_aca57c/UnkBufferString.h"
#include "base/UnkBlockFreeList.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CRntVector.h"
#include "types.h"

extern void* g_unk_7101a1b678[];

struct UnkStringNode_l9319dc {
    UnkBufferString<0x40, g_unk_7101a1b678> text;
    UnkStringNode_l9319dc* next;  // 0x88

    UnkStringNode_l9319dc() : next(nullptr) {}
};
static_assert(sizeof(UnkStringNode_l9319dc) == 0x90, "UnkStringNode_l9319dc layout");

// The list's node pool: an allocator (vtable at +0) over a block free list.
struct UnkStringNodePool_l9319dc {
    void* vtable;
    base::UnkAllocator* backing;  // 0x8
    UnkBlockFreeList blocks;      // 0x10

    u64 Capacity() const { return blocks.size / sizeof(UnkStringNode_l9319dc); }
};

// The nodes: their count and the first and last of them.
struct UnkStringChain_l9319dc {
    u32 count;
    UnkStringNode_l9319dc* head;  // 0x8
    UnkStringNode_l9319dc* tail;  // 0x10

    void PushBack(UnkStringNode_l9319dc* node) {
        if (count == 0) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            node->next = nullptr;
            tail = node;
        }
        ++count;
    }
};

struct UnkStringList_l9319dc {
    u8 pad_0x0[8];
    UnkStringChain_l9319dc nodes;   // 0x8
    base::UnkAllocator* allocator;  // 0x20
    u32 flags;                      // 0x28
    UnkStringNodePool_l9319dc pool;  // 0x30

    void Clear() {
        UnkStringNode_l9319dc* node = nodes.head;
        while (node) {
            UnkStringNode_l9319dc* next = node->next;
            node->~UnkStringNode_l9319dc();
            allocator->vf_0x48(node);
            node = next;
        }
        nodes.count = 0;
        nodes.head = nullptr;
        nodes.tail = nullptr;
    }

    // Gives the pool room for `n` nodes, and makes it the list's allocator.
    void Reserve(u32 n) {
        if (n <= pool.Capacity())
            return;
        base::UnkAllocator* alloc = pool.backing ? pool.backing : allocator;
        u32 alloc_flags = flags;
        if (void* buffer = pool.blocks.buffer) {
            fn_71000837f4(&pool.blocks);
            pool.backing->vf_0x48(buffer);
            pool.backing = nullptr;
        }
        if (n) {
            if (!alloc)
                alloc = fn_7100083990();
            pool.backing = alloc;
            u64 size = (u64)n * sizeof(UnkStringNode_l9319dc);
            fn_710008378c(&pool.blocks, alloc->vf_0x40(size, 8, alloc_flags), size, sizeof(UnkStringNode_l9319dc));
        }
        allocator = reinterpret_cast<base::UnkAllocator*>(&pool);
    }
};

void fn_71009319dc(const char* text, const char* separator, UnkStringList_l9319dc* list) {
    list->Clear();
    u32 text_length = strlen(text);
    u32 separator_length = strlen(separator);

    u32 pieces = 0;
    if (text) {
        const char* p = text;
        do {
            const char* found = strstr(p, separator);
            ++pieces;
            p = found ? found + separator_length : nullptr;
        } while (p);
    }
    list->Reserve(pieces);

    if (text_length == 0)
        return;
    const char* p = text;
    for (u32 pos = 0; pos < text_length;) {
        const char* end = strstr(p, separator);
        if (!end) {
            u64 rest = strlen(p);
            if (rest == 0)
                return;
            end = p + rest;
        }
        void* memory = list->allocator->vf_0x40(sizeof(UnkStringNode_l9319dc), 8, list->flags);
        auto* node = new (memory) UnkStringNode_l9319dc;
        list->nodes.PushBack(node);
        u32 length = end - p;
        fn_710007e7f8(node->text.Str(), p, length);
        u32 step = length + separator_length;
        pos += step;
        p += step;
    }
}

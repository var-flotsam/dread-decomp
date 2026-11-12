// CCharClassFrozenAsPlatformComponent unit: refills a pooled list of strings with the pieces of `text` split at
// `delimiter`. Names and layouts are placeholders.
#include <string.h>

#include "base/UnkBlockFreeList.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "types.h"

extern void* g_unk_7101a1b750[];

struct UnkStringNode_l7fcdf4 {
    base::global::CRntString string;  // 0x0
    void* pool_vtable;                // 0x30
    u8 pad_0x38[0x80];
    u64 field_0xb8;
    u32 field_0xc0;
    UnkStringNode_l7fcdf4* next;  // 0xc8
};
static_assert(sizeof(UnkStringNode_l7fcdf4) == 0xd0, "node size");

struct UnkStringPool_l7fcdf4 {
    char* vtable;
    base::UnkAllocator* parent;
    UnkBlockFreeList blocks;
};

struct UnkStringList_l7fcdf4 {
    void* field_0x0;
    u32 count;
    UnkStringNode_l7fcdf4* head;
    UnkStringNode_l7fcdf4* tail;
    base::UnkAllocator* allocator;
    u32 flags;
    u32 pad_0x2c;
    UnkStringPool_l7fcdf4 pool;  // 0x30
};

// The block list's address is computed where it is used, from the raw bytes.
static inline UnkBlockFreeList* BlocksOf(UnkStringList_l7fcdf4* list) {
    return reinterpret_cast<UnkBlockFreeList*>(reinterpret_cast<char*>(list) + 0x40);
}

void fn_71007fcdf4(const char* text, const char* delimiter, UnkStringList_l7fcdf4* list) {
    UnkStringNode_l7fcdf4* old = list->head;
    while (old) {
        UnkStringNode_l7fcdf4* next = old->next;
        fn_710007ed40(&old->string);
        fn_710007ecd4(&old->string);
        list->allocator->vf_0x48(old);
        old = next;
    }
    list->count = 0;
    list->head = nullptr;
    list->tail = nullptr;

    size_t text_length = strlen(text);
    size_t delimiter_length = strlen(delimiter);

    u32 pieces = 0;
    for (const char* p = text; p;) {
        const char* hit = strstr(p, delimiter);
        ++pieces;
        p = hit ? hit + (delimiter_length & 0xffffffff) : nullptr;
    }

    if (list->pool.blocks.size / sizeof(UnkStringNode_l7fcdf4) < pieces) {
        base::UnkAllocator* current = list->allocator;
        base::UnkAllocator* parent = list->pool.parent;
        u32 flags = list->flags;
        void* buffer = list->pool.blocks.buffer;
        base::UnkAllocator* source = parent ? parent : current;
        if (buffer) {
            fn_71000837f4(BlocksOf(list));
            list->pool.parent->vf_0x48(buffer);
            list->pool.parent = nullptr;
        }
        if (pieces) {
            if (!source)
                source = fn_7100083990();
            list->pool.parent = source;
            u64 size = (u64)pieces * sizeof(UnkStringNode_l7fcdf4);
            void* mem = source->vf_0x40(size, 8, flags);
            fn_710008378c(BlocksOf(list), mem, size, sizeof(UnkStringNode_l7fcdf4));
        }
        list->allocator = reinterpret_cast<base::UnkAllocator*>(reinterpret_cast<char*>(list) +
                                                                __builtin_offsetof(UnkStringList_l7fcdf4, pool));
    }

    if ((u32)text_length == 0)
        return;
    u32 consumed = 0;
    do {
        const char* end = strstr(text, delimiter);
        if (!end) {
            u64 rest = strlen(text);
            if (rest == 0)
                return;
            end = text + rest;
        }
        void* mem = list->allocator->vf_0x40(sizeof(UnkStringNode_l7fcdf4), 8, list->flags);
        UnkStringNode_l7fcdf4* node = static_cast<UnkStringNode_l7fcdf4*>(mem);
        if (node) {
            fn_710007e6e4(&node->string, reinterpret_cast<base::UnkAllocator*>(&node->pool_vtable));
            node->pool_vtable = reinterpret_cast<char*>(g_unk_7101a1b750) + 0x10;
            node->field_0xb8 = 0;
            node->field_0xc0 = 0;
            node->next = nullptr;
        }
        u32 length = (u32)(u64)end - (u32)(u64)text;
        if (list->count == 0) {
            list->head = node;
            list->tail = node;
            list->count = 1;
        } else {
            list->tail->next = node;
            node->next = nullptr;
            list->count = list->count + 1;
            list->tail = node;
        }
        fn_710007e7f8(&node->string, text, length);
        u32 step = length + (u32)delimiter_length;
        consumed += step;
        text += step;
    } while (consumed < (u32)text_length);
}

// CPlaythroughDef's unit (tools/tu_map.py): splits `text` at each `separator` into a list of strings,
// emptying the list first and reserving a block of nodes for every piece. Names and layouts are
// placeholders.
#include <new>
#include <string.h>

#include "base/UnkBlockFreeList.h"
#include "base/UnkNodeOwner.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CRntVector.h"
#include "game/UnkBufferStringPool.h"
#include "types.h"

using base::global::CRntString;

using UnkPlaythroughTokens_ovb0123 = UnkNodeOwner<UnkListNode<0x68>, UnkDestroyStrNode<0x68>>;

// One piece: its string, the string's storage object, and the list link.
struct UnkSplitToken_l2f271c {
    u8 raw[0x30];  // the string
    void** storage_vtable;  // 0x30
    u8 pad_0x38[0x20];
    u64 field_0x58;
    u32 field_0x60;
    u8 pad_0x64[4];
    UnkListNode<0x68>* next;  // 0x68

    CRntString* Str() { return reinterpret_cast<CRntString*>(raw); }

    UnkSplitToken_l2f271c() {
        CRntString_InitWithAllocator(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = g_unk_7101a1b5a0 + 2;
        field_0x58 = 0;
        field_0x60 = 0;
        next = nullptr;
    }
};
static_assert(sizeof(UnkSplitToken_l2f271c) == 0x70, "layout");

// Makes room in the list's own block for `n` pieces: the old block is released and a new one taken from
// the block's allocator (else the list's, else the default one); the list then allocates from the block.
inline void ReserveTokens(UnkPlaythroughTokens_ovb0123* out, u32 n) {
    if (out->field_0x40.size / sizeof(UnkSplitToken_l2f271c) >= n)
        return;
    base::UnkAllocator* own = out->field_0x38;
    base::UnkAllocator* list = out->field_0x20;
    base::UnkAllocator* alloc = own ? own : list;
    u32 flags = *reinterpret_cast<u32*>(out->pad_0x28);
    if (void* block = out->field_0x40.buffer) {
        fn_71000837f4(&out->field_0x40);
        out->field_0x38->vf_0x48(block);
        out->field_0x38 = nullptr;
    }
    if (n) {
        if (!alloc)
            alloc = fn_7100083990();
        out->field_0x38 = alloc;
        u64 size = n * sizeof(UnkSplitToken_l2f271c);
        fn_710008378c(&out->field_0x40, alloc->vf_0x40(size, 8, flags), size, sizeof(UnkSplitToken_l2f271c));
    }
    out->field_0x20 = reinterpret_cast<base::UnkAllocator*>(&out->field_0x30);
}

void fn_71002f271c(const char* text, const char* separator, UnkPlaythroughTokens_ovb0123* out) {
    out->FreeNodes();
    out->field_0x8 = 0;
    out->field_0x10 = nullptr;
    out->field_0x18 = nullptr;

    u32 text_len = strlen(text);
    u32 sep_len = strlen(separator);

    u32 count = 0;
    if (text) {
        const char* p = text;
        do {
            const char* hit = strstr(p, separator);
            ++count;
            p = hit ? hit + sep_len : nullptr;
        } while (p);
    }

    ReserveTokens(out, count);

    if (text_len) {
        u32 pos = 0;
        const char* p = text;
        do {
            const char* end = strstr(p, separator);
            if (!end) {
                size_t rest = strlen(p);
                if (!rest)
                    return;
                end = p + rest;
            }
            void* mem = out->field_0x20->vf_0x40(sizeof(UnkSplitToken_l2f271c), 8,
                                                 *reinterpret_cast<u32*>(out->pad_0x28));
            UnkSplitToken_l2f271c* token = new (mem) UnkSplitToken_l2f271c();
            UnkListNode<0x68>* node = reinterpret_cast<UnkListNode<0x68>*>(token);
            if (out->field_0x8 == 0) {
                out->field_0x10 = node;
                out->field_0x18 = node;
                out->field_0x8 = 1;
            } else {
                out->field_0x18->next = node;
                node->next = nullptr;
                ++out->field_0x8;
                out->field_0x18 = node;
            }
            u32 len = end - p;
            CRntString_AssignPrefix(token->Str(), p, len);
            u32 step = len + sep_len;
            pos += step;
            p += step;
        } while (pos < text_len);
    }
}

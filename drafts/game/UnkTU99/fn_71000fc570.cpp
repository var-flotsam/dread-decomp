// Unit 99 (base::snd::CSoundSystemATK): splits `text` at every `separator` into a list of strings with
// inline storage. The list is emptied first and its node pool is resized to the number of pieces.
// Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "base/UnkBlockFreeList.h"
#include "base/UnkLinkedList.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU99.h"
#include "types.h"

using base::global::CRntString;

namespace {

// A string with 0x100 bytes of inline storage, linked to the next piece.
struct UnkToken_l0fc570 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;
    UnkToken_l0fc570* next;  // +0x148

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }

    UnkToken_l0fc570() {
        CRntString_InitWithAllocator(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
        next = nullptr;
    }
    ~UnkToken_l0fc570() {
        CRntString_Clear(Str());
        CRntString_Destroy(Str());
    }
};
static_assert(sizeof(UnkToken_l0fc570) == 0x150, "");

struct UnkTokens_l0fc570 {
    char* vtable;
    base::UnkLinkedList<UnkToken_l0fc570> body;  // 0x8
    char* pool_vtable;  // +0x30: the node pool
    base::UnkAllocator* pool_parent;
    UnkBlockFreeList pool_blocks;

    // base::UnkLinkedList::Clear, destroying each token first.
    void Clear() {
        for (UnkToken_l0fc570* node = body.head; node;) {
            UnkToken_l0fc570* next = node->next;
            node->~UnkToken_l0fc570();
            body.allocator->vf_0x48(node);
            node = next;
        }
        body.count = 0;
        body.head = nullptr;
        body.tail = nullptr;
    }

    // Gives the node pool room for `n` nodes when it has less, and allocates the nodes from it.
    void Reserve(u32 n) {
        if (pool_blocks.size / sizeof(UnkToken_l0fc570) >= n)
            return;
        base::UnkAllocator* parent = pool_parent ? pool_parent : body.allocator;
        u32 pool_flags = body.flags;
        if (void* buffer = pool_blocks.buffer) {
            fn_71000837f4(&pool_blocks);
            pool_parent->vf_0x48(buffer);
            pool_parent = nullptr;
        }
        if (n) {
            if (!parent)
                parent = GetDefaultAllocator();
            pool_parent = parent;
            u64 size = n * sizeof(UnkToken_l0fc570);
            fn_710008378c(&pool_blocks, parent->vf_0x40(size, 8, pool_flags), size, sizeof(UnkToken_l0fc570));
        }
        body.allocator = reinterpret_cast<base::UnkAllocator*>(&pool_vtable);
    }

    UnkToken_l0fc570* Append() {
        UnkToken_l0fc570* node =
            new (body.allocator->vf_0x40(sizeof(UnkToken_l0fc570), 8, body.flags)) UnkToken_l0fc570;
        body.PushBack(node);
        return node;
    }
};

}  // namespace

void fn_71000fc570(const char* text, const char* separator, UnkTunableTokens* out) {
    UnkTokens_l0fc570* tokens = reinterpret_cast<UnkTokens_l0fc570*>(out);
    tokens->Clear();
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
    tokens->Reserve(pieces);

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
        UnkToken_l0fc570* token = tokens->Append();
        u32 length = end - p;
        CRntString_AssignPrefix(token->Str(), p, length);
        u32 step = length + separator_length;
        pos += step;
        p += step;
    }
}

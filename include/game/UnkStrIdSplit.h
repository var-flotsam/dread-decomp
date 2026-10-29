#pragma once

// Splitting a string id at commas into a vector of string ids, as two of the character class
// tracks' units do (fn_7100d1f764, CCharClassModifyColliderStateBaseTrack's, and fn_7100cfca0c,
// CCharClassFXControlTrack's). The pieces are collected in a list of strings over a block pool
// (fn_71007fcdf4 splits, fn_71007fd1dc destroys the list), then turned into ids. Names and layouts
// are placeholders.

#include "base/UnkBlockFreeList.h"
#include "base/UnkCloneRecords.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

#include <new>

// A piece: a string with inline storage, and the next piece at +0xc8.
struct UnkStrPiece {
    u8 text[0xc8];  // a base::global::CRntString with its storage
    UnkStrPiece* next;
};

// The list the pieces are collected in (base/UnkCloneHelpers.h's UnkPoolList layout, its pool left
// without a buffer).
struct UnkStrPieceList {
    char* vtable;
    u32 count;
    UnkStrPiece* head;
    UnkStrPiece* tail;
    base::UnkAllocator* allocator;
    u32 flags;
    char* pool_vtable;  // 0x30
    base::UnkAllocator* pool_parent;
    UnkBlockFreeList blocks;  // 0x40
    u32 field_0x68;
    bool field_0x6c;

    // The piece at `i` (null past the end), found by walking the links.
    UnkStrPiece* At(u64 i) const {
        if (i >= count)
            return nullptr;
        UnkStrPiece* piece = head;
        u64 k = 0;
        u32 j = 0;
        do {
            if (k == i)
                return piece;
            piece = piece->next;
            ++k;
        } while (++j != count);
        return nullptr;
    }
};
static_assert(__builtin_offsetof(UnkStrPieceList, blocks) == 0x40, "UnkStrPieceList placeholder layout");

extern char g_unk_7101a74458[];  // the list's vtable (reached through the GOT)
void fn_71007fcdf4(const char* text, const char* separators, UnkStrPieceList* out);
void fn_71007fd1dc(UnkStrPieceList* self);
void fn_71007fd04c(base::UnkStrIdVec* self, u32 capacity);  // reserve

// The ids of the comma-separated parts of `name`, replacing `out`'s.
__attribute__((always_inline)) inline void UnkSplitStrIds(const base::global::CStrId* name, base::UnkStrIdVec* out) {
    UnkStrPieceList list;
    list.count = 0;
    list.head = nullptr;
    list.tail = nullptr;
    list.allocator = fn_7100083990();
    list.flags = 0x13;
    list.vtable = g_unk_7101a74458 + 0x10;
    list.pool_vtable = g_unk_7101a1a4c8 + 0x10;
    list.pool_parent = nullptr;
    fn_710008377c(&list.blocks);
    list.field_0x68 = 0;
    list.field_0x6c = false;
    if (name->data != &g_unk_7101d47108->empty)
        fn_71007fcdf4(base::reflection::refl_RntStringOf(*name).data, ",", &list);
    for (u32 i = 0; i < out->count; ++i)
        out->data[i].Reset();
    out->count = 0;
    u32 n = list.count;
    if (n) {
        fn_71007fd04c(out, n);
        for (u64 i = 0; i != n; ++i) {
            UnkStrPiece* piece = list.At(i);
            alignas(8) u8 raw[sizeof(base::global::CStrId)];
            auto* built = reinterpret_cast<base::global::CStrId*>(raw);
            fn_7100000850(built, *reinterpret_cast<const base::global::CRntString*>(piece), true);
            if (out->field_0x14 && out->count <= i) {
                u32 cap = out->capacity;
                if (cap <= i && out->step != 0)
                    cap = ((u32)i / out->step + 1) * out->step;
                out->Reserve(cap);
                u64 n = i + 1;
                out->Reserve(n);
                // A signed counter compared for inequality, as in the original (its slots are
                // addressed through the widened counter).
                for (s32 k = out->count; k != n; ++k) {
                    // An empty id, constructed in place.
                    base::global::CStrId& id = out->data[k];
                    id.data = nullptr;
                    if (!g_unk_7101d47108)
                        base::global::UnkEnsureStringPool();
                    id.data = &g_unk_7101d47108->empty;
                }
                out->count = n;
            }
            base::global::CStrId& slot = out->data[i];
            slot.Reset();
            slot.data = built->data;  // moved in (the built id is never destroyed)
        }
    }
    fn_71007fd1dc(&list);
}

// Copy and move constructors of a record holding three string ids (taken over by the move,
// leaving the pool's empty string), plain values copied as blocks, an object copied or moved out
// of line (fn_710007ea60, fn_710007eab4) and a string with 0x20 bytes of inline storage (always
// copied). Also a word/record pair's constructors.
#include <new>

#include "UnkBufferString.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CStrId.h"
#include "types.h"

extern void* g_unk_7101a1b5a0[];
using UnkString20 = UnkBufferString<0x20, g_unk_7101a1b5a0>;

struct UnkMovedStrId {
    base::global::UnkStringData* data;

    __attribute__((always_inline)) UnkMovedStrId(UnkMovedStrId&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
};

// Plain bytes copied as one block.
template <u32 N, u32 Align>
struct alignas(Align) UnkBlock {
    u8 bytes[N];

    __attribute__((always_inline)) UnkBlock(const UnkBlock& other) { __builtin_memcpy(bytes, other.bytes, N); }
};

struct UnkObject30 {
    u64 field_0x0[6];

    __attribute__((always_inline)) UnkObject30(const UnkObject30& other) {
        fn_710007ea60(reinterpret_cast<base::UnkNamedEntry*>(this), reinterpret_cast<const base::UnkNamedEntry*>(&other));
    }
    __attribute__((always_inline)) UnkObject30(UnkObject30&& other) {
        fn_710007eab4(reinterpret_cast<base::global::CRntString*>(this),
                      reinterpret_cast<base::global::CRntString*>(&other));
    }
};

template <class Id>
struct UnkIdsObjectString {
    u64 field_0x0;
    Id field_0x8;
    Id field_0x10;
    Id field_0x18;
    UnkBlock<4, 4> field_0x20[7];
    UnkBlock<0x14, 8> field_0x40;
    UnkObject30 field_0x58;
    UnkBlock<4, 4> field_0x88;
    UnkBlock<0xc, 8> field_0x90;
    UnkString20 field_0xa0;
    UnkBlock<4, 4> field_0x108;
    UnkBlock<6, 8> field_0x110;
};
using UnkCopiedRecord = UnkIdsObjectString<base::global::CStrId>;
using UnkMovedRecord = UnkIdsObjectString<UnkMovedStrId>;
static_assert(sizeof(UnkCopiedRecord) == 0x118, "UnkIdsObjectString layout");

void fn_7100eac378(UnkCopiedRecord* self, const UnkCopiedRecord* other) { new (self) UnkCopiedRecord(*other); }
void fn_7100eac4c8(UnkMovedRecord* self, UnkMovedRecord* other) {
    new (self) UnkMovedRecord(static_cast<UnkMovedRecord&&>(*other));
}

struct UnkWordRecordPair {
    u32 key;
    UnkCopiedRecord value;

    __attribute__((always_inline)) UnkWordRecordPair(const u32& key, const UnkCopiedRecord& value)
        : key(key), value(value) {}
    __attribute__((always_inline)) UnkWordRecordPair(const UnkWordRecordPair& other)
        : key(other.key), value(other.value) {}
};

// NON_MATCHING(minor): the original copies the 0x14-byte block at 0x48 with single loads and
// stores (ours pairs them), and registers differ.
void fn_7100ead774(UnkWordRecordPair* self, const UnkWordRecordPair* other) { new (self) UnkWordRecordPair(*other); }
// NON_MATCHING(minor): as fn_7100ead774, and the source and destination swap registers.
void fn_7100ead8d4(UnkWordRecordPair* self, const u32* key, const UnkCopiedRecord* value) {
    new (self) UnkWordRecordPair(*key, *value);
}

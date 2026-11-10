// CPlaythroughDef's unit (tools/tu_map.py): the record's id list (+0xa8) as text. fn_71002f25ec sets
// it from a comma-separated string (split by fn_71002f271c into a string list, base/UnkNodeOwner.h,
// destroyed by fn_71002f2974); fn_71002f2478 joins it back with commas. Names and layouts are
// placeholders.
#include "base/UnkBlockFreeList.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkCloneRecords.h"
#include "base/UnkNodeOwner.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

extern void* g_unk_7101a222a8[];  // the string list's vtable

using UnkPlaythroughTokens_ovb0123 = UnkNodeOwner<UnkListNode<0x68>, UnkDestroyStrNode<0x68>>;
void fn_71002f2974(UnkNodeOwner<UnkListNode<0x68>, UnkDestroyStrNode<0x68>>* self);
void fn_71002f271c(const char* text, const char* separator, UnkPlaythroughTokens_ovb0123* out);

struct UnkPlaythroughIdsOwner_ovb0123 {
    u8 pad_0x0[0xa8];
    base::UnkStrIdVec ids;  // 0xa8
};

// A string id built from a run-time string (fn_7100000850).
struct UnkPlaythroughBuiltId_ovb0123 {
    union {
        CStrId id;
    };

    explicit UnkPlaythroughBuiltId_ovb0123(const CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkPlaythroughBuiltId_ovb0123() { id.~CStrId(); }
};

// Appends the id, taking it over (the id is left empty).
static inline void PushBackMove(base::UnkStrIdVec* ids, CStrId& id) {
    u32 n = ids->count;
    u32 c = ids->capacity;
    if (c <= n && ids->step != 0)
        c = (n / ids->step + 1) * ids->step;
    if ((s32)n >= (s32)c)
        for (;;) {
        }
    ids->Reserve(c);
    __builtin_memcpy(&ids->data[ids->count], &id.data, 8);
    id.data = &g_unk_7101d47108->empty;
    ++ids->count;
}

void fn_71002f25ec(UnkPlaythroughIdsOwner_ovb0123* self, const char* const* text) {
    UnkPlaythroughTokens_ovb0123 tokens;
    tokens.field_0x8 = 0;
    tokens.field_0x10 = nullptr;
    tokens.field_0x18 = nullptr;
    tokens.field_0x20 = fn_7100083990();
    *reinterpret_cast<u32*>(tokens.pad_0x28) = 0x13;
    tokens.vtable = g_unk_7101a222a8 + 2;
    tokens.field_0x30 = g_unk_7101a1a4c8 + 0x10;
    tokens.field_0x38 = nullptr;
    fn_710008377c(&tokens.field_0x40);
    *reinterpret_cast<u32*>(tokens.pad_0x68) = 0;
    tokens.field_0x6c = 0;
    fn_71002f271c(*text, ",", &tokens);
    for (UnkListNode<0x68>* node = tokens.field_0x10; node; node = node->next) {
        UnkPlaythroughBuiltId_ovb0123 id(*reinterpret_cast<const CRntString*>(node));
        PushBackMove(&self->ids, id.id);
    }
    fn_71002f2974(&tokens);
}

void fn_710007f564(void*, const char*);
extern void* g_unk_7101a1b750[];  // the joined text's storage vtable

// A string whose storage object follows it.
struct UnkPlaythroughJoinStorage_ovb0123 {
    void** vtable;
    u8 pad_0x8[0x80];
    u64 field_0x88;
    u32 field_0x90;
};

struct UnkPlaythroughJoined_ovb0123 {
    u8 raw[0x30];  // the string
    UnkPlaythroughJoinStorage_ovb0123 storage;  // +0x30

    CRntString* Str() { return reinterpret_cast<CRntString*>(raw); }

    UnkPlaythroughJoined_ovb0123() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage));
        storage.vtable = g_unk_7101a1b750 + 2;
        storage.field_0x88 = 0;
        storage.field_0x90 = 0;
    }
    ~UnkPlaythroughJoined_ovb0123() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};
static_assert(__builtin_offsetof(UnkPlaythroughJoined_ovb0123, storage.field_0x90) == 0xc0, "layout");

// The pooled data behind an id as its text accessor sees it: the text at +0x10 and a count of the
// reads at +0x44.
struct UnkPlaythroughIdData_ovb0123 {
    u8 pad_0x0[0x10];
    CRntString text;  // 0x10
    u8 pad_0x40[4];
    union {
        u32 use_count;  // 0x44
    };
};

// A counted reference to an id's data, released without a null check.
struct UnkPlaythroughIdRef_ovb0123 {
    base::global::UnkStringData* data;

    explicit UnkPlaythroughIdRef_ovb0123(const CStrId& id) : data(id.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkPlaythroughIdRef_ovb0123() {
        base::global::UnkStringPool* pool = g_unk_7101d47108;
        if (pool && data != &pool->empty && --data->ref_count == 0)
            fn_71000803a4(pool, data);
    }
    const CRntString* Text() const {
        auto* d = reinterpret_cast<UnkPlaythroughIdData_ovb0123*>(data);
        ++d->use_count;
        return &d->text;
    }
};

UnkPlaythroughJoined_ovb0123 fn_71002f2478(const UnkPlaythroughIdsOwner_ovb0123* self) {
    UnkPlaythroughJoined_ovb0123 out;
    u32 count = self->ids.count;
    if (count) {
        const CStrId* p = self->ids.data;
        {
            UnkPlaythroughIdRef_ovb0123 id(p[0]);
            fn_710007f3b4(out.Str(), id.Text());
        }
        for (const CStrId *q = p + 1, *e = p + count; q != e; ++q) {
            UnkPlaythroughIdRef_ovb0123 id(*q);
            fn_710007f564(out.Str(), ",");
            fn_710007f3b4(out.Str(), id.Text());
        }
    }
    return out;
}

// Unit 2604: the move assignment of a record of three ids, a block of plain data and a map from string ids
// to (string id, data) values; the other record's map entries are inserted into this one's. Names and
// layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// Five words copied as one.
struct UnkIdData_lf8e8f8 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
};

// A map value: a string id and its data.
struct UnkIdValue_lf8e8f8 {
    CStrId id;
    UnkIdData_lf8e8f8 data;  // 0x8
};

struct UnkIdNode_lf8e8f8 {
    CStrId key;
    UnkIdValue_lf8e8f8 value;  // 0x8
    UnkIdNode_lf8e8f8* prev;   // 0x28
    UnkIdNode_lf8e8f8* next;   // 0x30
    UnkIdNode_lf8e8f8* chain;  // 0x38

    // Builds a node in raw storage: the key copied, the value taken over (the other is left with the
    // empty id), no links.
    static void Construct(UnkIdNode_lf8e8f8* n, const CStrId& key, UnkIdValue_lf8e8f8& value) {
        new (&n->key) CStrId(key);
        n->value.id.data = value.id.data;
        value.id.data = &g_unk_7101d47108->empty;
        n->value.data = value.data;
        n->prev = nullptr;
        n->next = nullptr;
        n->chain = nullptr;
    }

    // The list links, through accessors (their stores may alias each other).
    UnkIdNode_lf8e8f8*& Next() { return next; }
    UnkIdNode_lf8e8f8*& Prev() { return prev; }
};
static_assert(sizeof(UnkIdNode_lf8e8f8) == 0x40, "UnkIdNode_lf8e8f8 layout");

struct UnkIdMap_lf8e8f8 {
    base::IAllocator* allocator;
    u32 flags;                    // 0x8
    u32 count;                    // 0xc
    UnkIdNode_lf8e8f8** buckets;  // 0x10
    u32 field_0x18;
    u32 bucket_count;  // 0x1c
    u8 pad_0x20[0x18];
    UnkIdNode_lf8e8f8* tail;  // 0x38
    UnkIdNode_lf8e8f8* head;  // 0x40

    // Adds a node for the key with no check for one already there.
    void Insert(const CStrId& key, UnkIdValue_lf8e8f8& value) {
        UnkIdNode_lf8e8f8** bucket = nullptr;
        if (field_0x18)
            bucket = &buckets[key.Hash() % bucket_count];
        UnkIdNode_lf8e8f8* chained = *bucket;
        UnkIdNode_lf8e8f8* n =
            static_cast<UnkIdNode_lf8e8f8*>(allocator->vf_0x40(sizeof(UnkIdNode_lf8e8f8), 8, flags));
        if (n)
            UnkIdNode_lf8e8f8::Construct(n, key, value);
        n->chain = chained;
        *bucket = n;
        if (!head) {
            head = n;
        } else if (tail) {
            n->Next() = tail;
            tail->Prev() = n;
        }
        tail = n;
        ++count;
    }
};

struct UnkIdRecord_lf8e8f8 {
    CStrId field_0x0;
    CFilePathStrId path;  // 0x8
    CStrId field_0x10;
    UnkIdData_lf8e8f8 data;  // 0x18
    UnkIdMap_lf8e8f8 map;    // 0x30
};
static_assert(__builtin_offsetof(UnkIdRecord_lf8e8f8, map) == 0x30, "UnkIdRecord_lf8e8f8 layout");

UnkIdRecord_lf8e8f8* fn_7100f8e8f8(UnkIdRecord_lf8e8f8* self, UnkIdRecord_lf8e8f8* other) {
    if (self != other) {
        self->field_0x0.ReleaseAndTake(other->field_0x0);
        self->path = static_cast<CFilePathStrId&&>(other->path);
        self->field_0x10.ReleaseAndTake(other->field_0x10);
        self->data = other->data;
        for (UnkIdNode_lf8e8f8* n = other->map.head; n; n = n->Prev())
            self->map.Insert(n->key, n->value);
    }
    return self;
}

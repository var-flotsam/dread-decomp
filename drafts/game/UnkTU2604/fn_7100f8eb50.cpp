// Unit 2604: finds the record a string id keys in an owner's chained, linked hash map, adding an empty one
// (named after the key) when there is none. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrIdInline;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// The record's name: a string id that can be moved out of (the source keeps the empty id).
struct UnkTU2604FindRecordId {
    UnkStringData* data;

    UnkTU2604FindRecordId() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    UnkTU2604FindRecordId(UnkTU2604FindRecordId&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkTU2604FindRecordId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = &pool->empty;
        }
    }
    UnkTU2604FindRecordId& operator=(const CStrId& other) {
        if (data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47108;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
};

struct UnkTU2604FindRecord {
    UnkTU2604FindRecordId id;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;

    UnkTU2604FindRecord() : field_0x8(0), field_0x10(0), field_0x18(0) {}
    UnkTU2604FindRecord(UnkTU2604FindRecord&& other) = default;
};

struct UnkTU2604FindNode {
    CStrId key;
    UnkTU2604FindRecord value;  // 0x8
    UnkTU2604FindNode* next;    // 0x28
    UnkTU2604FindNode* prev;    // 0x30
    UnkTU2604FindNode* chain;   // 0x38

    UnkTU2604FindNode(const CStrId& k, UnkTU2604FindRecord&& v)
        : key(k), value(static_cast<UnkTU2604FindRecord&&>(v)), next(nullptr), prev(nullptr), chain(nullptr) {}
};

struct UnkTU2604FindMap {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;  // 0xc
    UnkTU2604FindNode** buckets;
    u32 bucket_count;
    u32 divisor;
    u8 pad_0x20[0x18];
    UnkTU2604FindNode* tail;  // 0x38
    UnkTU2604FindNode* head;  // 0x40

    UnkTU2604FindNode** Bucket(const CStrId& key) {
        if (bucket_count == 0)
            return nullptr;
        return &buckets[key.Hash() % divisor];
    }

    bool Contains(const CStrId& key) {
        if (UnkTU2604FindNode** bucket = Bucket(key)) {
            for (UnkTU2604FindNode* node = *bucket; node; node = node->chain) {
                if (node->key == key)
                    return true;
            }
        }
        return false;
    }

    // The record of a key known to be there.
    UnkTU2604FindRecord* Get(const CStrId& key) {
        UnkTU2604FindNode* node = *Bucket(key);
        while (node && !(node->key == key))
            node = node->chain;
        return &node->value;
    }

    UnkTU2604FindRecord* Insert(const CStrId& key, UnkTU2604FindRecord&& record) {
        UnkTU2604FindNode** bucket = nullptr;
        if (bucket_count != 0)
            bucket = &buckets[key.Hash() % divisor];
        UnkTU2604FindNode* old = *bucket;
        auto* node = static_cast<UnkTU2604FindNode*>(allocator->vf_0x40(sizeof(UnkTU2604FindNode), 8, flags));
        if (node)
            new (node) UnkTU2604FindNode(key, static_cast<UnkTU2604FindRecord&&>(record));
        node->chain = old;
        *bucket = node;
        if (!head)
            head = node;
        else if (tail) {
            node->prev = tail;
            tail->next = node;
        }
        tail = node;
        ++count;
        return &node->value;
    }
};

struct UnkTU2604FindOwner {
    u8 pad_0x0[0x30];
    UnkTU2604FindMap records;  // 0x30
};

UnkTU2604FindRecord* fn_7100f8eb50(UnkTU2604FindOwner* self, const CStrId* key) {
    if (self->records.Contains(*key))
        return self->records.Get(*key);
    UnkTU2604FindRecord* record = self->records.Insert(*key, UnkTU2604FindRecord());
    record->id = *key;
    return record;
}

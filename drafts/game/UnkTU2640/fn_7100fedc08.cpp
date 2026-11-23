// Unit 2640: the find-or-add of a map of string ids to (string id, array of 0x78-byte elements) records: a
// missing key gets a node with an empty record, whose id is then set to the key. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/GUI/UnkMissionLogRecords.h"
#include "game/UnkTU2640.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkElem78 {
    u8 unk_0x0[0x78];
};

// The record's string id: empty when built, released in one step.
struct UnkRecordId_lfedc08 {
    UnkStringData* data;

    UnkRecordId_lfedc08() = default;
    UnkRecordId_lfedc08(UnkRecordId_lfedc08&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
    // As CStrId's copy assignment.
    UnkRecordId_lfedc08& operator=(const CStrId& other) {
        if (data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47108;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    StringPool_Release(pool, data);
            }
            data = d;
        }
        return *this;
    }
    ~UnkRecordId_lfedc08() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0)
                    StringPool_Release(pool, data);
            }
            data = &pool->empty;
        }
    }
};

// A string id and an array of elements.
struct UnkIdArray78Record_lfedc08 {
    UnkRecordId_lfedc08 id;
    UnkElemArray<UnkElem78> items;  // 0x8

    UnkIdArray78Record_lfedc08() = default;
    UnkIdArray78Record_lfedc08(UnkIdArray78Record_lfedc08&& other)
        : id(static_cast<UnkRecordId_lfedc08&&>(other.id)), items(other.items) {
        other.items.data = nullptr;
        other.items.count = 0;
        other.items.capacity = 0;
    }
    ~UnkIdArray78Record_lfedc08() { fn_7100ffaa88(&items); }
};
static_assert(sizeof(UnkIdArray78Record_lfedc08) == 0x30, "UnkIdArray78Record_lfedc08 layout");

struct UnkIdArray78Node_lfedc08 {
    CStrId key;
    UnkIdArray78Record_lfedc08 value;  // 0x8
    UnkIdArray78Node_lfedc08* next;    // 0x38, the list's
    UnkIdArray78Node_lfedc08* prev;    // 0x40
    UnkIdArray78Node_lfedc08* chain;   // 0x48, the bucket's

    UnkIdArray78Node_lfedc08(const CStrId& k, UnkIdArray78Record_lfedc08&& v)
        : key(k), value(static_cast<UnkIdArray78Record_lfedc08&&>(v)), next(nullptr), prev(nullptr),
          chain(nullptr) {}
};

// A chained hash map whose nodes are also linked in one list.
struct UnkIdArray78Map_lfedc08 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    base::global::CRntVector<UnkIdArray78Node_lfedc08*> buckets;
    UnkIdArray78Node_lfedc08* tail;  // 0x38
    UnkIdArray78Node_lfedc08* head;  // 0x40

    UnkIdArray78Node_lfedc08** BucketFor(const CStrId& key) {
        if (buckets.count == 0)
            return nullptr;
        return buckets.data + key.Hash() % buckets.capacity;
    }

    UnkIdArray78Node_lfedc08* Find(const CStrId& key) {
        UnkIdArray78Node_lfedc08** bucket = BucketFor(key);
        if (!bucket)
            return nullptr;
        for (UnkIdArray78Node_lfedc08* node = *bucket; node; node = node->chain)
            if (node->key == key)
                return node;
        return nullptr;
    }

    UnkIdArray78Node_lfedc08* Insert(const CStrId& key, UnkIdArray78Record_lfedc08&& value) {
        UnkIdArray78Node_lfedc08** bucket = BucketFor(key);
        UnkIdArray78Node_lfedc08* old = *bucket;
        auto* node = static_cast<UnkIdArray78Node_lfedc08*>(
            allocator->vf_0x40(sizeof(UnkIdArray78Node_lfedc08), 8, flags));
        if (node)
            new (node) UnkIdArray78Node_lfedc08(key, static_cast<UnkIdArray78Record_lfedc08&&>(value));
        node->chain = old;
        *bucket = node;
        if (!head) {
            head = node;
        } else if (UnkIdArray78Node_lfedc08* last = tail) {
            node->prev = last;
            tail->next = node;
        }
        tail = node;
        ++count;
        return node;
    }
};

struct UnkIdArray78Records_lfedc08 {
    u64 field_0x0;
    UnkIdArray78Map_lfedc08 map;  // 0x8
};

// Makes a zeroed record the empty one: the empty id (the pool created first when there is none) and an
// empty array (allocator flags 0xf, growing by 10).
static void InitEmpty_lfedc08(UnkIdArray78Record_lfedc08* record) {
    record->id.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    record->id.data = &g_unk_7101d47108->empty;
    record->items.data = nullptr;
    record->items.count = 0;
    record->items.capacity = 0;
    record->items.step = 10;
    record->items.field_0x14 = true;
    record->items.flags = 0xf;
    record->items.allocator = GetDefaultAllocator();
}

// The record under `key`, added (named for the key) when there is none.
UnkIdArray78Record_lfedc08* fn_7100fedc08(UnkIdArray78Records_lfedc08* self, const CStrId* key) {
    if (self->map.Find(*key))
        return &self->map.Find(*key)->value;
    UnkIdArray78Record_lfedc08* record;
    {
        UnkIdArray78Record_lfedc08 empty{};
        InitEmpty_lfedc08(&empty);
        record = &self->map.Insert(*key, static_cast<UnkIdArray78Record_lfedc08&&>(empty))->value;
    }
    record->id = *key;
    return record;
}

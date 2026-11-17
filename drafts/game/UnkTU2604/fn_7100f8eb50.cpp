// Unit 2604: finds the record a string id keys in an owner's chained, linked hash map, adding an empty one
// (named after the key) when there is none. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntDictionary.h"
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

using UnkTU2604FindMap = base::global::CRntDictionary<CStrId, UnkTU2604FindRecord>;

struct UnkTU2604FindOwner {
    u8 pad_0x0[0x30];
    UnkTU2604FindMap records;  // 0x30
};

UnkTU2604FindRecord* fn_7100f8eb50(UnkTU2604FindOwner* self, const CStrId* key) {
    if (self->records.FindNode(*key))
        return self->records.Find(*key);
    UnkTU2604FindRecord* record = &self->records.Insert(*key, UnkTU2604FindRecord())->value;
    record->id = *key;
    return record;
}

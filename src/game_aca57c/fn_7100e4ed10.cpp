// Clears of containers of two parallel arrays (records and string ids) sharing a count: the ids
// are destroyed, then each record (its arrays freed, its string id released). One container type
// per record size (0xa8 and 0x80 bytes; their copy constructors are fn_7100e509a4 and
// fn_7100e5190c).
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

// A string id whose destructor decrements and tests in two steps (the original reloads the id
// after the decrement).
struct UnkReleasedStrId {
    UnkStringData* data;

    ~UnkReleasedStrId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// The arrays' elements are plain values; only their storage is freed.
struct UnkArray : base::UnkVector<u32> {
    ~UnkArray() { Free(); }
};

struct UnkRecordA8 {
    UnkArray field_0x0;
    u8 unk_0x28[0x20];
    UnkArray field_0x48;
    UnkArray field_0x70;
    u8 unk_0x98[0x10];
};
static_assert(sizeof(UnkRecordA8) == 0xa8, "UnkRecordA8 layout");

struct UnkRecord80 {
    u8 unk_0x0[0x10];
    UnkReleasedStrId field_0x10;
    UnkArray field_0x18;
    UnkArray field_0x40;
    u8 unk_0x68[0x18];
};
static_assert(sizeof(UnkRecord80) == 0x80, "UnkRecord80 layout");

template <class Record>
struct UnkRecordIdArray {
    Record* records;
    UnkReleasedStrId* ids;
    u32 count;
};

template <class Record>
__attribute__((always_inline)) static inline void Clear(UnkRecordIdArray<Record>* self) {
    if (self->ids) {
        for (u64 i = 0; i < self->count; ++i)
            self->ids[i].~UnkReleasedStrId();
    }
    if (self->records) {
        for (u64 i = 0; i < self->count; ++i)
            self->records[i].~Record();
    }
    self->count = 0;
}

// NON_MATCHING(minor): in the records loop the original addresses the record's fields from
// its last array's allocator (record + 0x90); ours from the record itself, which takes one more
// register.
void fn_7100e4ed10(UnkRecordIdArray<UnkRecordA8>* self) { Clear(self); }
// NON_MATCHING(minor): as fn_7100e4ed10 (the original's base is record + 0x60, ours
// record + 0x38).
void fn_7100e4eb8c(UnkRecordIdArray<UnkRecord80>* self) { Clear(self); }

// Destructor of a record of two string ids (0x0, 0x18) and two arrays (0x20, 0x48).
struct UnkIdsArraysRecord {
    UnkReleasedStrId field_0x0;
    u8 unk_0x8[0x10];
    UnkReleasedStrId field_0x18;
    UnkArray field_0x20;
    UnkArray field_0x48;

    __attribute__((always_inline)) ~UnkIdsArraysRecord() {}
};

void fn_7100b47fb0(UnkIdsArraysRecord* self) { self->~UnkIdsArraysRecord(); }
void fn_7100e4d63c(UnkIdsArraysRecord* self) { self->~UnkIdsArraysRecord(); }

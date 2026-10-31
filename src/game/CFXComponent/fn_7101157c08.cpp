// The clear of a container of two parallel arrays (inner arrays and string ids) in CFXComponent's
// unit, as fn_7100eb1824 (src/game_aca57c/fn_7100eb1824.cpp): destroys the ids, then frees each
// inner array.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

// A string id whose destructor decrements and tests in two steps (the original reloads the id
// after the decrement).
struct UnkFXReleasedStrId {
    UnkStringData* data;

    UnkFXReleasedStrId(const UnkFXReleasedStrId& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkFXReleasedStrId(UnkFXReleasedStrId&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }

    ~UnkFXReleasedStrId() {
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

struct UnkFXIdValues {
    u32 field_0x0;
    u32 field_0x4;
    UnkFXReleasedStrId id;
};

struct UnkFXIdValuesArrays {
    base::UnkVector<UnkFXIdValues>* records;
    UnkFXReleasedStrId* ids;
    u32 count;
    u32 capacity;
};

void fn_7101157c08(UnkFXIdValuesArrays* self) {
    if (self->ids) {
        for (u64 i = 0; i < self->count; ++i)
            self->ids[i].~UnkFXReleasedStrId();
    }
    if (self->records) {
        for (u64 i = 0; i < self->count; ++i)
            self->records[i].Free();
    }
    self->count = 0;
}

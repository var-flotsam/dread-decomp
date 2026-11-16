// GUI::CMissionLog's unit (tools/tu_map.py): the "erase" slot of the reflected collection type of the
// array of 0x38-byte mission-log records (a value, a string id and an array of ids): destroys the record
// at the iterator and moves the later ones down by one. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/GUI/UnkMissionLogRecords.h"
#include "types.h"

using base::global::UnkStrIdBase;
using base::global::UnkStringPool;

// A string id that can be moved from; its destructor releases in two steps, as CStrId::Reset does.
struct UnkMovableStrId_b5ba48 : UnkStrIdBase {
    UnkMovableStrId_b5ba48(UnkMovableStrId_b5ba48&& other) : UnkStrIdBase(other) {
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkMovableStrId_b5ba48() {
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

// An array of ids (CRntVector's layout) that takes the other's storage when moved and frees its own.
struct UnkStrIdArray_b5ba48 {
    UnkMovableStrId_b5ba48* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkStrIdArray_b5ba48(UnkStrIdArray_b5ba48&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    ~UnkStrIdArray_b5ba48() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkMovableStrId_b5ba48();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkMissionLogRecord_b5ba48 {
    u32 field_0x0;
    UnkMovableStrId_b5ba48 id;
    UnkStrIdArray_b5ba48 ids;
};
static_assert(sizeof(UnkMissionLogRecord_b5ba48) == sizeof(UnkElem38), "record size");

using UnkMissionLogRecords_b5ba48 = UnkElemArray<UnkMissionLogRecord_b5ba48>;

// Destroys the record at `pos` and moves the later ones down by one.
static inline void Erase_b5ba48(UnkMissionLogRecords_b5ba48* self, UnkMissionLogRecord_b5ba48* pos) {
    u32 index = pos - self->data;
    if (index < self->count) {
        self->data[index].~UnkMissionLogRecord_b5ba48();
        for (u64 i = index; i < self->count - 1; ++i) {
            UnkMissionLogRecord_b5ba48* at = &self->data[i];
            new (at) UnkMissionLogRecord_b5ba48(static_cast<UnkMissionLogRecord_b5ba48&&>(at[1]));
            (&self->data[i])[1].~UnkMissionLogRecord_b5ba48();
        }
        self->count = self->count - 1;
    }
}

// The slot's iterator is the record's index.
// NOTE: the original keeps the index-to-pointer-and-back round trip (an exact division by 0x38); this folds it.
void fn_7100b5ba48(UnkElem38Array* self, const u32* it) {
    Erase_b5ba48(reinterpret_cast<UnkMissionLogRecords_b5ba48*>(self),
                 reinterpret_cast<UnkMissionLogRecord_b5ba48*>(&self->data[*it]));
}

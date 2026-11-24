// GUI::CMissionLog's unit (tools/tu_map.py): inserts a copy of a mission-log record (a value, a string id and
// an array of ids) at an index of the array of 0x38-byte records, moving the later records up one place;
// at the end it appends (fn_7100b593d0). Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/GUI/UnkMissionLogRecords.h"
#include "types.h"

using base::global::UnkStrIdBase;
using base::global::UnkStringPool;

// A string id that can be copied (retaining it) and moved from; its destructor releases in two steps, as CStrId::Reset does.
struct UnkMovableStrId_b5c5a0 : UnkStrIdBase {
    UnkMovableStrId_b5c5a0(const UnkMovableStrId_b5c5a0& other) : UnkStrIdBase(other) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkMovableStrId_b5c5a0(UnkMovableStrId_b5c5a0&& other) : UnkStrIdBase(other) {
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkMovableStrId_b5c5a0() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// An array of ids (base::UnkStrIdVec's layout) that copies element by element, takes the other's storage
// when moved and frees its own.
struct UnkStrIdArray_b5c5a0 {
    UnkMovableStrId_b5c5a0* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::IAllocator* allocator;

    // Reserving and appending are base::UnkStrIdVec's (the same layout and element).
    base::UnkStrIdVec* Vec() { return reinterpret_cast<base::UnkStrIdVec*>(this); }
    void Reserve(u32 n) { fn_710000542c(Vec(), n); }
    void PushBack(const UnkMovableStrId_b5c5a0& v) { Vec()->PushBack(reinterpret_cast<const base::global::CStrId&>(v)); }

    UnkStrIdArray_b5c5a0(const UnkStrIdArray_b5c5a0& other)
        : data(nullptr), count(0), capacity(0), step(other.step), field_0x14(other.field_0x14),
          flags(other.flags), allocator(other.allocator) {
        Reserve(other.capacity);
        for (const UnkMovableStrId_b5c5a0 *p = other.data, *e = p + other.count; p != e; ++p)
            PushBack(*p);
    }
    UnkStrIdArray_b5c5a0(UnkStrIdArray_b5c5a0&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    ~UnkStrIdArray_b5c5a0() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkMovableStrId_b5c5a0();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkMissionLogRecord_b5c5a0 {
    u32 field_0x0;
    UnkMovableStrId_b5c5a0 id;
    UnkStrIdArray_b5c5a0 ids;
};
static_assert(sizeof(UnkMissionLogRecord_b5c5a0) == sizeof(UnkElem38), "record size");

static inline UnkMissionLogRecord_b5c5a0* Records_b5c5a0(UnkElem38Array* self) {
    return reinterpret_cast<UnkMissionLogRecord_b5c5a0*>(self->data);
}

void fn_7100b5c5a0(UnkElem38Array* self, const UnkMissionLogRecord_b5c5a0* value, u32 index) {
    if (index == self->count) {
        CMissionLog_EntriesPushBack(self, reinterpret_cast<const UnkElem38*>(value));
        return;
    }
    u32 n = self->count;
    u32 new_capacity = self->capacity;
    if (new_capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    fn_7100b5c28c(self, new_capacity);
    for (u32 i = self->count - 1; i >= index;) {
        UnkMissionLogRecord_b5c5a0* e = &Records_b5c5a0(self)[i];
        new (&e[1]) UnkMissionLogRecord_b5c5a0(static_cast<UnkMissionLogRecord_b5c5a0&&>(*e));
        Records_b5c5a0(self)[i].~UnkMissionLogRecord_b5c5a0();
        if (i-- == 0)
            break;
    }
    new (&Records_b5c5a0(self)[index]) UnkMissionLogRecord_b5c5a0(*value);
    ++self->count;
}

// GUI::CMissionLog's unit (tools/tu_map.py): moves a mission-log record (a value, a string id and an array
// of ids) in at an index of the array of 0x38-byte records, moving the later records up one place;
// appending grows like PushBack, including its hang. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/GUI/UnkMissionLogRecords.h"
#include "types.h"

using base::global::UnkStrIdBase;
using base::global::UnkStringPool;

// A string id that can be moved from; its destructor releases in two steps, as CStrId::Reset does.
struct UnkMovableStrId_b5c910 : UnkStrIdBase {
    UnkMovableStrId_b5c910(UnkMovableStrId_b5c910&& other) : UnkStrIdBase(other) {
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkMovableStrId_b5c910() {
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

// An array of ids (CRntVector's layout) that takes the other's storage when moved and frees its own.
struct UnkStrIdArray_b5c910 {
    UnkMovableStrId_b5c910* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkStrIdArray_b5c910(UnkStrIdArray_b5c910&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    ~UnkStrIdArray_b5c910() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkMovableStrId_b5c910();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkMissionLogRecord_b5c910 {
    u32 field_0x0;
    UnkMovableStrId_b5c910 id;
    UnkStrIdArray_b5c910 ids;
};
static_assert(sizeof(UnkMissionLogRecord_b5c910) == sizeof(UnkElem38), "record size");

static inline UnkMissionLogRecord_b5c910* Records_b5c910(UnkElem38Array* self) {
    return reinterpret_cast<UnkMissionLogRecord_b5c910*>(self->data);
}

void fn_7100b5c910(UnkElem38Array* self, UnkMissionLogRecord_b5c910* value, u32 index) {
    if (index == self->count) {
        u32 n = self->count;
        u32 new_capacity = self->capacity;
        if (new_capacity <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)new_capacity)  // hangs, as CRntVector::PushBack does
            for (;;) {
            }
        fn_7100b5c28c(self, new_capacity);
        new (&Records_b5c910(self)[self->count])
            UnkMissionLogRecord_b5c910(static_cast<UnkMissionLogRecord_b5c910&&>(*value));
    } else {
        u32 n = self->count;
        u32 new_capacity = self->capacity;
        if (new_capacity <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        fn_7100b5c28c(self, new_capacity);
        for (u32 i = self->count - 1; i >= index;) {
            UnkMissionLogRecord_b5c910* e = &Records_b5c910(self)[i];
            new (&e[1]) UnkMissionLogRecord_b5c910(static_cast<UnkMissionLogRecord_b5c910&&>(*e));
            Records_b5c910(self)[i].~UnkMissionLogRecord_b5c910();
            if (i-- == 0)
                break;
        }
        new (&Records_b5c910(self)[index])
            UnkMissionLogRecord_b5c910(static_cast<UnkMissionLogRecord_b5c910&&>(*value));
    }
    ++self->count;
}

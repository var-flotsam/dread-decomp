// Unit 2640: copies a 0x18-byte record (a file-path id, a word and a half-word) in at an index of an
// engine array, moving the later records up. Names and layouts are placeholders.
#include <new>

#include "../UnkUnit2601/UnkUnit2601_VecCopy_b56.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2640PathData.h"
#include "types.h"

using base::global::UnkStrIdBase;
using base::global::UnkStringPool;

// A file-path id that can be copied (retaining it) or moved from; its destructor releases in two
// steps, as CStrId::Reset does.
struct UnkMovablePathId_lffbc64 : UnkStrIdBase {
    UnkMovablePathId_lffbc64(const UnkMovablePathId_lffbc64& other) : UnkStrIdBase(other) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    UnkMovablePathId_lffbc64(UnkMovablePathId_lffbc64&& other) : UnkStrIdBase(other) {
        other.data = &g_unk_7101d47200->empty;
    }
    ~UnkMovablePathId_lffbc64() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkPathRecord_lffbc64 {
    UnkMovablePathId_lffbc64 id;
    UnkTU2640PathData data;  // 0x8

    UnkPathRecord_lffbc64(const UnkPathRecord_lffbc64& other) : id(other.id), data(other.data) {}
    UnkPathRecord_lffbc64(UnkPathRecord_lffbc64&& other)
        : id(static_cast<UnkMovablePathId_lffbc64&&>(other.id)), data(other.data) {}
};
static_assert(sizeof(UnkPathRecord_lffbc64) == 0x18);

struct UnkPathRecordArray_lffbc64 {
    UnkPathRecord_lffbc64* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool autoGrow;
};

// The array's reserve, out of line (the same code as unit 2601's).
static inline void Reserve_lffbc64(UnkPathRecordArray_lffbc64* self, u32 n) {
    fn_7100f86020(reinterpret_cast<UnkPathRecordVec_b56*>(self), n);
}

void fn_7100ffbc64(UnkPathRecordArray_lffbc64* self, const UnkPathRecord_lffbc64* value, u32 index) {
    u32 n = self->count;
    u32 capacity = self->capacity;
    if (n == index) {
        if (capacity <= n && self->step != 0)
            capacity = (index / self->step + 1) * self->step;
        // A capacity that does not grow past the index hangs, as in the original.
        if ((s32)capacity <= (s32)index)
            for (;;) {
            }
        Reserve_lffbc64(self, capacity);
        new (&self->data[self->count]) UnkPathRecord_lffbc64(*value);
    } else {
        if (capacity <= n && self->step != 0)
            capacity = (n / self->step + 1) * self->step;
        Reserve_lffbc64(self, capacity);
        for (u32 i = self->count - 1; i >= index; --i) {
            UnkPathRecord_lffbc64* q = &self->data[i];
            new (q + 1) UnkPathRecord_lffbc64(static_cast<UnkPathRecord_lffbc64&&>(*q));
            self->data[i].~UnkPathRecord_lffbc64();
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkPathRecord_lffbc64(*value);
    }
    ++self->count;
}

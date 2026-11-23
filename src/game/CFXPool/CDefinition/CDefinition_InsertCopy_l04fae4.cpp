// The CFXPool::CDefinition unit: copies a file-path entry (a file-path id and a word) in at an index
// of the unit's engine array, moving the later entries up. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2200.h"
#include "types.h"

using base::global::UnkStrIdBase;
using base::global::UnkStringPool;

// A file-path id that can be copied (retaining it) or moved from; its destructor releases in two
// steps, as CStrId::Reset does.
struct UnkMovablePathId_l04fae4 : UnkStrIdBase {
    UnkMovablePathId_l04fae4(const UnkMovablePathId_l04fae4& other) : UnkStrIdBase(other) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    UnkMovablePathId_l04fae4(UnkMovablePathId_l04fae4&& other) : UnkStrIdBase(other) {
        other.data = &g_unk_7101d47200->empty;
    }
    ~UnkMovablePathId_l04fae4() {
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

// A file-path entry: the id and a word.
struct UnkFXPathRecord_l04fae4 {
    UnkMovablePathId_l04fae4 id;
    u64 field_0x8;

    UnkFXPathRecord_l04fae4(const UnkFXPathRecord_l04fae4& other) : id(other.id), field_0x8(other.field_0x8) {}
    UnkFXPathRecord_l04fae4(UnkFXPathRecord_l04fae4&& other)
        : id(static_cast<UnkMovablePathId_l04fae4&&>(other.id)), field_0x8(other.field_0x8) {}
};
static_assert(sizeof(UnkFXPathRecord_l04fae4) == 0x10);

struct UnkFXPathRecordArray_l04fae4 {
    UnkFXPathRecord_l04fae4* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool autoGrow;
};

// The array's reserve, out of line.
static inline void Reserve_l04fae4(UnkFXPathRecordArray_l04fae4* self, u32 n) {
    fn_7100c8c210(reinterpret_cast<base::global::CRntVector<UnkFXPathEntry_b0068>*>(self), n);
}

void fn_710104fae4(UnkFXPathRecordArray_l04fae4* self, const UnkFXPathRecord_l04fae4* value, u32 index) {
    u32 n = self->count;
    u32 capacity = self->capacity;
    if (n == index) {
        if (capacity <= n && self->step != 0)
            capacity = (index / self->step + 1) * self->step;
        // A capacity that does not grow past the index hangs, as in the original.
        if ((s32)capacity <= (s32)index)
            for (;;) {
            }
        Reserve_l04fae4(self, capacity);
        new (&self->data[self->count]) UnkFXPathRecord_l04fae4(*value);
    } else {
        if (capacity <= n && self->step != 0)
            capacity = (n / self->step + 1) * self->step;
        Reserve_l04fae4(self, capacity);
        for (u32 i = self->count - 1; i >= index; --i) {
            UnkFXPathRecord_l04fae4* q = &self->data[i];
            new (q + 1) UnkFXPathRecord_l04fae4(static_cast<UnkFXPathRecord_l04fae4&&>(*q));
            self->data[i].~UnkFXPathRecord_l04fae4();
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkFXPathRecord_l04fae4(*value);
    }
    ++self->count;
}

// build-flags: no-strict-aliasing
// The CFXPool::CDefinition unit: moves a file-path entry (a file-path id and a word) in at an index
// of the unit's engine array, moving the later entries up. Names and layouts are placeholders.
#include <new>

#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU2200.h"
#include "types.h"

// A file-path entry as moved: a move takes the other's id.
struct UnkFXPathRecord_l04fc9c {
    base::UnkStrIdMemberOf<g_unk_7101d47200> id;  // CFilePathStrId's pool
    u64 field_0x8;

    UnkFXPathRecord_l04fc9c(UnkFXPathRecord_l04fc9c&& other)
        : id(static_cast<base::UnkStrIdMemberOf<g_unk_7101d47200>&&>(other.id)), field_0x8(other.field_0x8) {}
};
static_assert(sizeof(UnkFXPathRecord_l04fc9c) == 0x10);

struct UnkFXPathRecordArray_l04fc9c {
    UnkFXPathRecord_l04fc9c* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool autoGrow;
};

// The array's reserve, out of line.
static inline void Reserve_l04fc9c(UnkFXPathRecordArray_l04fc9c* self, u32 n) {
    fn_7100c8c210(reinterpret_cast<base::global::CRntVector<UnkFXPathEntry_b0068>*>(self), n);
}

void fn_710104fc9c(UnkFXPathRecordArray_l04fc9c* self, UnkFXPathRecord_l04fc9c* value, u32 index) {
    u32 n = self->count;
    u32 capacity = self->capacity;
    if (n == index) {
        if (capacity <= n && self->step != 0)
            capacity = (index / self->step + 1) * self->step;
        // A capacity that does not grow past the index hangs, as in the original.
        if ((s32)capacity <= (s32)index)
            for (;;) {
            }
        Reserve_l04fc9c(self, capacity);
        UnkFXPathRecord_l04fc9c* end = self->data + self->count;
        new (end) UnkFXPathRecord_l04fc9c(static_cast<UnkFXPathRecord_l04fc9c&&>(*value));
    } else {
        if (capacity <= n && self->step != 0)
            capacity = (n / self->step + 1) * self->step;
        Reserve_l04fc9c(self, capacity);
        for (u32 i = self->count - 1; i >= index; --i) {
            UnkFXPathRecord_l04fc9c* q = &self->data[i];
            new (q + 1) UnkFXPathRecord_l04fc9c(static_cast<UnkFXPathRecord_l04fc9c&&>(*q));
            self->data[i].~UnkFXPathRecord_l04fc9c();
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkFXPathRecord_l04fc9c(static_cast<UnkFXPathRecord_l04fc9c&&>(*value));
    }
    ++self->count;
}

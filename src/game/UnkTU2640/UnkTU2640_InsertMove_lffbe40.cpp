// Unit 2640: moves a 0x18-byte record (a file-path id, a word and a half-word) in at an index of an
// engine array, moving the later records up. Names and layouts are placeholders.
#include <new>

#include "../UnkUnit2601/UnkUnit2601_VecCopy_b56.h"
#include "base/UnkDestructors.h"
#include "types.h"

// A word and a half-word, copied as one 12-byte block (packed: the copy skips the tail padding).
struct __attribute__((packed)) UnkTU2640PathData {
    u64 field_0x0;
    u32 field_0x8;
};

// A file-path id and its data; a move takes the other's id.
struct UnkPathRecord_lffbe40 {
    base::UnkStrIdMemberOf<g_unk_7101d47200> id;  // CFilePathStrId's pool
    UnkTU2640PathData data;                       // 0x8

    UnkPathRecord_lffbe40(UnkPathRecord_lffbe40&& other)
        : id(static_cast<base::UnkStrIdMemberOf<g_unk_7101d47200>&&>(other.id)), data(other.data) {}
};
static_assert(sizeof(UnkPathRecord_lffbe40) == 0x18);

struct UnkPathRecordArray_lffbe40 {
    UnkPathRecord_lffbe40* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool autoGrow;
};

// The array's reserve, out of line (the same code as unit 2601's).
static inline void Reserve_lffbe40(UnkPathRecordArray_lffbe40* self, u32 n) {
    fn_7100f86020(reinterpret_cast<UnkPathRecordVec_b56*>(self), n);
}

void fn_7100ffbe40(UnkPathRecordArray_lffbe40* self, UnkPathRecord_lffbe40* value, u32 index) {
    u32 n = self->count;
    u32 capacity = self->capacity;
    if (n == index) {
        if (capacity <= n && self->step != 0)
            capacity = (index / self->step + 1) * self->step;
        // A capacity that does not grow past the index hangs, as in the original.
        if ((s32)capacity <= (s32)index)
            for (;;) {
            }
        Reserve_lffbe40(self, capacity);
        new (&self->data[self->count]) UnkPathRecord_lffbe40(static_cast<UnkPathRecord_lffbe40&&>(*value));
    } else {
        if (capacity <= n && self->step != 0)
            capacity = (n / self->step + 1) * self->step;
        Reserve_lffbe40(self, capacity);
        for (u32 i = self->count - 1; i >= index; --i) {
            UnkPathRecord_lffbe40* q = &self->data[i];
            new (q + 1) UnkPathRecord_lffbe40(static_cast<UnkPathRecord_lffbe40&&>(*q));
            self->data[i].~UnkPathRecord_lffbe40();
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkPathRecord_lffbe40(static_cast<UnkPathRecord_lffbe40&&>(*value));
    }
    ++self->count;
}

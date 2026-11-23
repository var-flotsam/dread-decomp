// base::tunable::CTunable's unit: erases an entry from the array of 0x160-byte entries (three pooled string
// ids and a string with its own storage; the destructor is out of line, fn_7100133be4): the entry
// is destroyed and each later one is moved down. Returns the position now holding the next entry.
// Names and layouts are placeholders.
#include <new>

#include "../../../../src/game_aca57c/UnkBufferString.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkStr133568 {
    base::global::UnkStringData* data;

    UnkStr133568(UnkStr133568&& o) : data(o.data) { o.data = &g_unk_7101d47108->empty; }
};

struct UnkEntry133568 {
    UnkStr133568 field_0x0;
    UnkStr133568 field_0x8;
    UnkStr133568 field_0x10;
    UnkBufferString<0x100, g_unk_7101a194b8> field_0x18;

    UnkEntry133568(UnkEntry133568&& o)
        : field_0x0(static_cast<UnkStr133568&&>(o.field_0x0)),
          field_0x8(static_cast<UnkStr133568&&>(o.field_0x8)),
          field_0x10(static_cast<UnkStr133568&&>(o.field_0x10)), field_0x18(o.field_0x18) {}
    ~UnkEntry133568();  // fn_7100133be4
};

struct UnkEntryList133568 {
    UnkEntry133568* data;
    u32 count;
};

UnkEntry133568* fn_7100133568(UnkEntryList133568* self, UnkEntry133568* pos) {
    u32 index = pos - self->data;
    if (index < self->count) {
        self->data[index].~UnkEntry133568();
        u32 last = self->count - 1;
        if (index < last) {
            for (u64 i = index; i < last; ++i) {
                new (&self->data[i]) UnkEntry133568(static_cast<UnkEntry133568&&>(self->data[i + 1]));
                self->data[i + 1].~UnkEntry133568();
                last = self->count - 1;
            }
        }
        self->count = last;
    }
    return &self->data[index];
}

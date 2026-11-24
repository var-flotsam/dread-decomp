// Unit 1728 (timeline motion): destroys a part of a timeline (fn_7100b8b7e8 calls it at +0x30): four
// arrays of owned objects, last first, then a table of owned objects with a second block it frees.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

// A table of owned objects with a second block (0x8) allocated next to its array.
struct UnkOwnedTable_b8aa94 {
    UnkOwnedPtr* data;
    void* field_0x8;
    u32 count;
    u8 pad_0x14[0xc];
    base::IAllocator* allocator;

    ~UnkOwnedTable_b8aa94() {
        if (data) {
            for (u32 i = 0; i < count; ++i)
                data[i].~UnkOwnedPtr();
        }
        count = 0;
        if (field_0x8)
            allocator->vf_0x48(field_0x8);
        if (data)
            allocator->vf_0x48(data);
    }
};

struct UnkTimelinePart_b8aa94 {
    UnkOwnedTable_b8aa94 table;                         // 0x0
    base::global::CRntVector<UnkOwnedPtr> field_0x28;  // 0x28
    base::global::CRntVector<UnkOwnedPtr> field_0x50;  // 0x50
    base::global::CRntVector<UnkOwnedPtr> field_0x78;  // 0x78
    base::global::CRntVector<UnkOwnedPtr> field_0xa0;  // 0xa0
};
static_assert(__builtin_offsetof(UnkTimelinePart_b8aa94, field_0xa0) == 0xa0, "placeholder layout");

void fn_7100b8aa94(void* part) {
    auto* self = static_cast<UnkTimelinePart_b8aa94*>(part);
    self->field_0xa0.Free();
    self->field_0x78.Free();
    self->field_0x50.Free();
    self->field_0x28.Free();
    self->table.~UnkOwnedTable_b8aa94();
}

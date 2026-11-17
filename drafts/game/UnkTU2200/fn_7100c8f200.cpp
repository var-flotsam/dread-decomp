// Unit 2200 (CCutSceneDef neighbourhood): fn_7100c8f200 moves a 0xa0-byte entry in at an index of an
// array of them, moving the later entries up one place; appending grows like PushBack, including its
// hang. Names and layouts are placeholders.
#include <new>

#include "base/UnkDestructors.h"
#include "base/UnkKeyedStrIds.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2200.h"
#include "types.h"

using UnkPathIdMember_lc8b9c8 = base::UnkStrIdMemberOf<g_unk_7101d47200>;

// The entry's second table (cleared by fn_7100c88920 when the entry is destroyed): taken over on a move.
struct UnkCutSceneTable_lc8b9c8 {
    void* field_0x0;
    void* field_0x8;
    u32 count;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    base::UnkAllocator* allocator;

    UnkCutSceneTable_lc8b9c8(UnkCutSceneTable_lc8b9c8&& other)
        : field_0x0(other.field_0x0), field_0x8(other.field_0x8), count(other.count),
          field_0x14(other.field_0x14), field_0x18(other.field_0x18), field_0x1c(other.field_0x1c),
          allocator(other.allocator) {
        other.count = 0;
        other.field_0x0 = nullptr;
        other.field_0x8 = nullptr;
    }
};

struct UnkCutSceneEntry_lc8b9c8 {
    base::UnkStrIdMember field_0x0;
    UnkPathIdMember_lc8b9c8 field_0x8;
    base::UnkStrIdMember field_0x10;
    base::UnkStrIdMember field_0x18;
    u16 field_0x20;
    u32 field_0x24;
    UnkKeyedStrIds field_0x28;
    u64 field_0x50;
    base::UnkStrIdMember field_0x58;
    base::UnkStrIdMember field_0x60;
    u32 field_0x68;
    UnkCutSceneTable_lc8b9c8 field_0x70;
    UnkPathIdMember_lc8b9c8 field_0x98;

    UnkCutSceneEntry_lc8b9c8(UnkCutSceneEntry_lc8b9c8&& other)
        : field_0x0(static_cast<base::UnkStrIdMember&&>(other.field_0x0)),
          field_0x8(static_cast<UnkPathIdMember_lc8b9c8&&>(other.field_0x8)),
          field_0x10(static_cast<base::UnkStrIdMember&&>(other.field_0x10)),
          field_0x18(static_cast<base::UnkStrIdMember&&>(other.field_0x18)), field_0x20(other.field_0x20),
          field_0x24(other.field_0x24), field_0x28(static_cast<UnkKeyedStrIds&&>(other.field_0x28)),
          field_0x50(other.field_0x50), field_0x58(static_cast<base::UnkStrIdMember&&>(other.field_0x58)),
          field_0x60(static_cast<base::UnkStrIdMember&&>(other.field_0x60)), field_0x68(other.field_0x68),
          field_0x70(static_cast<UnkCutSceneTable_lc8b9c8&&>(other.field_0x70)),
          field_0x98(static_cast<UnkPathIdMember_lc8b9c8&&>(other.field_0x98)) {}
};
static_assert(__builtin_offsetof(UnkCutSceneEntry_lc8b9c8, field_0x70) == 0x70, "layout");
static_assert(sizeof(UnkCutSceneEntry_lc8b9c8) == 0xa0, "layout");

// The array as the unit's other files declare it; its elements are the entries above.
struct UnkElemA0_b42a3;
struct UnkElemA0Array_b42a3 {
    UnkElemA0_b42a3* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100c8f200(UnkElemA0Array_b42a3* self, UnkElemA0_b42a3* value, u32 index) {
    using Entry = UnkCutSceneEntry_lc8b9c8;
    Entry* entry = reinterpret_cast<Entry*>(value);
    if (index == self->count) {
        u32 n = self->count;
        u32 new_capacity = self->capacity;
        if (new_capacity <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)new_capacity)  // hangs, as CRntVector::PushBack does
            for (;;) {
            }
        fn_7100c8b9c8(self, new_capacity);
        new (reinterpret_cast<Entry*>(self->data) + self->count) Entry(static_cast<Entry&&>(*entry));
    } else {
        u32 n = self->count;
        u32 new_capacity = self->capacity;
        if (new_capacity <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        fn_7100c8b9c8(self, new_capacity);
        for (u32 i = self->count - 1; i >= index;) {
            Entry* e = reinterpret_cast<Entry*>(self->data) + i;
            new (&e[1]) Entry(static_cast<Entry&&>(*e));
            fn_7100c886c0(reinterpret_cast<Entry*>(self->data) + i);
            if (i-- == 0)
                break;
        }
        new (reinterpret_cast<Entry*>(self->data) + index) Entry(static_cast<Entry&&>(*entry));
    }
    ++self->count;
}

// Unit 2200 (CCutSceneDef neighbourhood): fn_7100c8b9c8 grows an array of 0xa0-byte entries to
// `capacity`, moving each entry over and destroying the old one (fn_7100c886c0).
// Names and layouts are placeholders.
#include <new>

#include "base/UnkDestructors.h"
#include "base/UnkKeyedStrIds.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkElemA0Array_b42a3.h"
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

void fn_7100c8b9c8(UnkElemA0Array_b42a3* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    auto* entries = static_cast<UnkCutSceneEntry_lc8b9c8*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkCutSceneEntry_lc8b9c8), 8, self->flags));
    if (self->data) {
        auto* src = reinterpret_cast<UnkCutSceneEntry_lc8b9c8*>(self->data);
        UnkCutSceneEntry_lc8b9c8* dst = entries;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkCutSceneEntry_lc8b9c8(static_cast<UnkCutSceneEntry_lc8b9c8&&>(*src));
            fn_7100c886c0(src);
        }
        self->allocator->vf_0x48(self->data);
    }
    self->data = reinterpret_cast<UnkElemA0_b42a3*>(entries);
    self->capacity = capacity;
}

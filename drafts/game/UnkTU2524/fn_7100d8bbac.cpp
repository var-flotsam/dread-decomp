// Unit 2524 of tools/tu_map.py: hands a callback each enabled entity of the current group (of up to
// five, grown on access) and of a second list, with the entity's factor scaled and offset, capped.
// Names and layouts are placeholders.
#include <algorithm>
#include <functional>

#include "base/global/CRntVector.h"
#include "types.h"

struct UnkFactorEntity;
f32 fn_7100d7ce40(UnkFactorEntity* self);

struct UnkEntity_ld8bbac {
    u8 pad_0x0[0x1b4];
    bool enabled;  // 0x1b4
    u8 pad_0x1b5[0x1f3];
    f32 offset;  // 0x3a8
};
static_assert(__builtin_offsetof(UnkEntity_ld8bbac, offset) == 0x3a8, "layout");

using UnkEntityList_ld8bbac = base::global::CRntVector<UnkEntity_ld8bbac*>;
using UnkEntityCallback_ld8bbac = std::function<void(UnkEntity_ld8bbac*, f32, bool)>;

struct UnkEntityGroups_ld8bbac {
    u32 field_0x0;
    f32 max_value;                  // 0x4
    UnkEntityList_ld8bbac entities;  // 0x8
    u32 group_count;                // 0x30
    u8 groups[5 * sizeof(UnkEntityList_ld8bbac)];  // 0x34: the groups, unaligned after the count
    u32 current_group;              // 0xfc

    // The group at `index`, adding empty groups up to it first.
    UnkEntityList_ld8bbac& Group(u32 index) {
        UnkEntityList_ld8bbac* list = reinterpret_cast<UnkEntityList_ld8bbac*>(groups);
        if (index + 1 > group_count) {
            for (u32 i = group_count; i < index + 1; ++i)
                new (&list[i]) UnkEntityList_ld8bbac(0xf);
            group_count = index + 1;
        }
        return list[index];
    }
};
static_assert(__builtin_offsetof(UnkEntityGroups_ld8bbac, current_group) == 0xfc, "layout");

// Hands an enabled entity to the callback with its factor scaled and offset, capped.
static inline void Notify(const UnkEntityGroups_ld8bbac* self, UnkEntity_ld8bbac* entity,
                          const UnkEntityCallback_ld8bbac& callback, f32 scale) {
    if (entity->enabled) {
        f32 value = fn_7100d7ce40(reinterpret_cast<UnkFactorEntity*>(entity)) * scale + entity->offset;
        callback(entity, std::min(value, self->max_value), false);
    }
}

void fn_7100d8bbac(UnkEntityGroups_ld8bbac* self, const UnkEntityCallback_ld8bbac& callback, f32 scale) {
    for (u32 i = 0; i != self->group_count; ++i) {
        UnkEntityList_ld8bbac& group = self->Group(i);
        const bool current = i == self->current_group;
        for (UnkEntity_ld8bbac** it = group.data; it != group.data + group.count; ++it) {
            if (current)
                Notify(self, *it, callback, scale);
        }
    }
    UnkEntityList_ld8bbac& entities = self->entities;
    for (UnkEntity_ld8bbac** it = entities.data; it != entities.data + entities.count; ++it) {
        Notify(self, *it, callback, scale);
    }
}

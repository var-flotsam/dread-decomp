// Unit 2200 (CCutSceneDef neighbourhood): fn_7100c92508 inserts a copy of a 0x40-byte entry at an index
// of an array of them, moving the later entries up one place (appending is fn_7100c8b894).
// Names and layouts are placeholders.
#include <new>

#include "base/UnkPooledStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2200.h"
#include "types.h"

struct UnkCutSceneEntry_lc92508 {
    base::UnkPooledStrId<g_unk_7101d47108> field_0x0;
    u32 field_0x8;
    base::UnkPooledStrId<g_unk_7101d47200> field_0x10;
    base::UnkPooledStrId<g_unk_7101d47108> field_0x18;
    base::UnkPooledStrId<g_unk_7101d47108> field_0x20;
    base::UnkPooledStrId<g_unk_7101d47108> field_0x28;
    u64 field_0x30;
    u32 field_0x38;

    UnkCutSceneEntry_lc92508(const UnkCutSceneEntry_lc92508& other) = default;
    UnkCutSceneEntry_lc92508(UnkCutSceneEntry_lc92508&& other) = default;
};
static_assert(sizeof(UnkCutSceneEntry_lc92508) == 0x40, "layout");

// The array as the unit's other files declare it; its elements are the entries above.
struct UnkElem40_b42a3;
struct UnkElem40Array_b42a3 {
    UnkElem40_b42a3* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100c92508(UnkElem40Array_b42a3* self, const UnkElem40_b42a3* value, u32 index) {
    if (index == self->count) {
        fn_7100c8b894(self, value);
        return;
    }
    u32 n = self->count;
    u32 new_capacity = self->capacity;
    if (new_capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    fn_7100c8b758(self, new_capacity);
    for (u32 i = self->count - 1; i >= index;) {
        auto* e = reinterpret_cast<UnkCutSceneEntry_lc92508*>(self->data) + i;
        new (&e[1]) UnkCutSceneEntry_lc92508(static_cast<UnkCutSceneEntry_lc92508&&>(*e));
        fn_7100c8a490(reinterpret_cast<UnkCutSceneEntry_lc92508*>(self->data) + i);
        if (i-- == 0)
            break;
    }
    new (reinterpret_cast<UnkCutSceneEntry_lc92508*>(self->data) + index)
        UnkCutSceneEntry_lc92508(*reinterpret_cast<const UnkCutSceneEntry_lc92508*>(value));
    ++self->count;
}

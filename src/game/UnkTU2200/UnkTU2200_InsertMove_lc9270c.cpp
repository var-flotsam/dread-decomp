// Unit 2200 (CCutSceneDef neighbourhood): fn_7100c9270c moves a 0x40-byte entry in at an index of an
// array of them, moving the later entries up one place; appending grows like PushBack, including its
// hang. Names and layouts are placeholders.
#include <new>

#include "base/UnkPooledStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2200.h"
#include "types.h"

struct UnkCutSceneEntry_lc9270c {
    base::UnkPooledStrId<g_unk_7101d47108> field_0x0;
    u32 field_0x8;
    base::UnkPooledStrId<g_unk_7101d47200> field_0x10;
    base::UnkPooledStrId<g_unk_7101d47108> field_0x18;
    base::UnkPooledStrId<g_unk_7101d47108> field_0x20;
    base::UnkPooledStrId<g_unk_7101d47108> field_0x28;
    u64 field_0x30;
    u32 field_0x38;

    UnkCutSceneEntry_lc9270c(UnkCutSceneEntry_lc9270c&& other) = default;
};
static_assert(sizeof(UnkCutSceneEntry_lc9270c) == 0x40, "layout");

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

void fn_7100c9270c(UnkElem40Array_b42a3* self, UnkElem40_b42a3* value, u32 index) {
    using Entry = UnkCutSceneEntry_lc9270c;
    Entry* entry = reinterpret_cast<Entry*>(value);
    if (index == self->count) {
        u32 n = self->count;
        u32 new_capacity = self->capacity;
        if (new_capacity <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)new_capacity)  // hangs, as CRntVector::PushBack does
            for (;;) {
            }
        fn_7100c8b758(self, new_capacity);
        new (reinterpret_cast<Entry*>(self->data) + self->count) Entry(static_cast<Entry&&>(*entry));
    } else {
        u32 n = self->count;
        u32 new_capacity = self->capacity;
        if (new_capacity <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        fn_7100c8b758(self, new_capacity);
        for (u32 i = self->count - 1; i >= index;) {
            Entry* e = reinterpret_cast<Entry*>(self->data) + i;
            new (&e[1]) Entry(static_cast<Entry&&>(*e));
            fn_7100c8a490(reinterpret_cast<Entry*>(self->data) + i);
            if (i-- == 0)
                break;
        }
        new (reinterpret_cast<Entry*>(self->data) + index) Entry(static_cast<Entry&&>(*entry));
    }
    ++self->count;
}

// Unit 604 of tools/tu_map.py (no name hint): walks a group's entries from `start` on and refreshes the data
// of each one that holds a value (fn_71003f6cfc); the first one's slot is pointed at `target` through its weak
// handle (src/game_s2/UnkWeakHandle.h) and flagged. The lists are those of UnkTU604_Lists_ovb0256.cpp.
// Names and layouts are placeholders.
#include "game/UnkListEntry_b256.h"
#include "game/UnkListGroup_b256.h"
#include "game/UnkTU604.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkMemberSlot_l3f9fe0 {
    UnkWeakHandle* handle;  // 0x0
    u8 pad_0x8[5];
    bool flag;  // 0xd
};

struct UnkMemberData_l3f9fe0 {
    u8 pad_0x0[0x146d0];
    UnkMemberSlot_l3f9fe0 slot;  // 0x146d0
};

void fn_71003f6cfc(UnkMemberData_l3f9fe0* data);

static inline UnkListEntry_b256* NthEntry(UnkListEntry_b256* e, u32 i) {
    for (u32 k = 0; k < i; ++k)
        e = e->next;
    return e;
}

void fn_71003f9fe0(void* self, UnkListGroup_b256* list, u32 start, UnkWeakTarget* target) {
    u32 count = list->count;
    for (u32 i = start; i < count; ++i) {
        if (!NthEntry(list->head, i)->value)
            continue;
        auto* data = reinterpret_cast<UnkMemberData_l3f9fe0*>(fn_71003f92ac(NthEntry(list->head, i)->value));
        if (i == start) {
            AssignRef(&data->slot.handle, target ? GetWeakHandle(target) : nullptr);
            data->slot.flag = true;
        }
        fn_71003f6cfc(data);
    }
}

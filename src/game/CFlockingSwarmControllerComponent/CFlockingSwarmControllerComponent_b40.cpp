// CFlockingSwarmControllerComponent unit (tools/tu_map.py unit 1158): fn_7100741008 goes over the
// swarm's groups (the array at +0x1a8, fn_71007410a4 gives group i); for every group with members, it
// sets the flag of each entry of its list in the array at +0x140 (fn_7100738c9c, fn_7100d35a98).
// Names and layouts are placeholders.
#include "game/UnkBroadphaseObject.h"
#include "types.h"

struct UnkGroup_fs {
    u8 pad_0x0[0x8];
    u32 count;
};

struct UnkEntryList_fs {
    UnkEntry** data;
    u32 count;
};

struct UnkArray_fs {
    void* data;
    u32 count;
};

struct UnkFlockingSwarm_fs {
    u8 pad_0x0[0x140];
    UnkArray_fs field_0x140;
    u8 pad_0x150[0x58];
    UnkArray_fs field_0x1a8;
};

UnkGroup_fs* fn_71007410a4(UnkArray_fs* array, u32 index);
UnkEntryList_fs* fn_7100738c9c(UnkArray_fs* array, u32 index);

void fn_7100741008(UnkFlockingSwarm_fs* self) {
    for (u32 i = 0; i < self->field_0x1a8.count; ++i) {
        if (!fn_71007410a4(&self->field_0x1a8, i)->count)
            continue;
        UnkEntryList_fs* list = fn_7100738c9c(&self->field_0x140, i);
        for (UnkEntry **it = list->data, **end = list->data + list->count; it != end; ++it)
            fn_7100d35a98(*it, true);
    }
}

// Sets or clears slot `index` (0-15) of group `group` in a global table. Setting stores the
// pointer and marks the slot; clearing a set slot nulls it and reports the change through a
// global callback with the group's id.
#include "game/UnkGraphicsState.h"
#include "types.h"

void* fn_7100153d50();

void fn_710015f770(u32 index, void* ptr, u32 group) {
    if (index > 15)
        return;
    if (ptr) {
        g_unk_716e2a9730[group][index].ptr = ptr;
        g_unk_716e2a9730[group][index].dirty = true;
        return;
    }
    if (g_unk_716e2a9730[group][index].ptr) {
        g_unk_716e2a9730[group][index].ptr = nullptr;
        void* context = fn_7100153d50();
        u32 id = g_unk_7101a1d818[group];
        g_unk_716e29b858(context, id, index, nullptr);
    }
}

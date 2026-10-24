// Sets the state of one of 16 per-slot entries; on a change, records the kind of transition
// (from/to the {0,1} or {2,3} state groups) and flags the entry as dirty.
#include "game/UnkGraphicsState.h"
#include "types.h"

void fn_7100160990(u32 index, s32 state, u32 slot) {
    if (index >= 16)
        return;
    if (g_unk_716e2a9730[slot][index].state == state)
        return;
    g_unk_716e2a9730[slot][index].state = state;
    switch (state) {
    case 0:
    case 1:
        switch (g_unk_716e2a9730[slot][index].field_0x14) {
        case 0:
        case 1:
            g_unk_716e2a9730[slot][index].transition = 2;
            break;
        case 2:
        case 3:
            g_unk_716e2a9730[slot][index].transition = 4;
            break;
        }
        break;
    case 2:
    case 3:
        switch (g_unk_716e2a9730[slot][index].field_0x14) {
        case 0:
        case 1:
            g_unk_716e2a9730[slot][index].transition = 3;
            break;
        case 2:
        case 3:
            g_unk_716e2a9730[slot][index].transition = 5;
            break;
        }
        break;
    }
    g_unk_716e2a9730[slot][index].dirty = true;
}

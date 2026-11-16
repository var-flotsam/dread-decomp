// A unit with no known name (unit 210): flips the emitter double buffer, restarts the frame's counters and
// returns the slots queued for release to the free table. Names and layouts are placeholders.
#include "game/UnkSlot_b0235.h"
#include "game/UnkTU210.h"
#include "types.h"

extern u32 g_unk_716e34c3c8;
extern void** g_unk_716e34c3c0;
extern u32 g_unk_7101a1dbc8;
extern u32 g_unk_716e34c3d8;
extern void* g_unk_716e34c3d0;

void fn_710016eae4() {
    g_unk_716e34c3c8 = ~g_unk_716e34c3c8 & 1;
    void* buffer = g_unk_716e34c3c0[g_unk_716e34c3c8];
    g_unk_716e34c3d8 = 0;
    g_unk_716e34c3d0 = buffer;
    g_unk_7101a1dbc8 = g_unk_7101a1dbc8 + 1;
    u64 count = g_unk_716e352430;
    if (count != 0) {
        u32 top = g_unk_716e34e428;
        UnkSlot_b0235** queued = g_unk_716e34e430;
        do {
            UnkSlot_b0235* freed = *queued++;
            top = top - 1;
            u32 index = freed->slot;
            UnkSlot_b0235* last = g_unk_716e34c420[top];
            g_unk_716e34c420[index] = last;
            last->slot = index;
            g_unk_716e34c420[top] = freed;
            freed->slot = -1;
        } while (--count != 0);
        g_unk_716e34e428 = top;
    }
    g_unk_716e352430 = 0;
}

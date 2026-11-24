// CSwarmControllerComponent's unit: clears a slot's flags and counts down its group's pending count;
// at zero, removes the group if it is active. Names and layouts are placeholders.
#include "types.h"

struct UnkSwarmController_ovb6;
struct UnkSwarmGroup_ovb6;

struct UnkSwarmSlotGroup_l74dda0 {
    u8 pad_0x0[0x24];
    u8 active;  // 0x24
    u8 pad_0x25[0xbb];
    u32 pending;  // 0xe0

    // True once nothing is pending (also when nothing was).
    bool Release() {
        if (pending == 0)
            return true;
        pending = pending - 1;
        return pending == 0;
    }
};

struct UnkSwarmSlot_l74dda0 {
    u8 pad_0x0[0x10];
    UnkSwarmSlotGroup_l74dda0* group;  // 0x10
    u8 pad_0x18[0xc];
    u16 flags;  // 0x24
};

void CSwarmControllerComponent_DeactivateGroup(UnkSwarmController_ovb6* self, UnkSwarmGroup_ovb6* group);

void fn_710074dda0(UnkSwarmController_ovb6* self, UnkSwarmSlot_l74dda0* slot) {
    slot->flags = 0;
    UnkSwarmSlotGroup_l74dda0* group = slot->group;
    bool done = group->Release();
    if (done) {
        if (group->active)
            CSwarmControllerComponent_DeactivateGroup(self, reinterpret_cast<UnkSwarmGroup_ovb6*>(group));
    }
}

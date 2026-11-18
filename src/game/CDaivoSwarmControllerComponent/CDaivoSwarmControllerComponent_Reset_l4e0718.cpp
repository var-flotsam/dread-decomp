// CDaivoSwarmControllerComponent's unit (tools/tu_map.py unit 768): the reset. After the swarm controller's
// reset (fn_710064f184), the value from vf_0x208 goes to +0x2bc and +0x1a4, and every member of every group
// gets a fresh one at +0x160 with its flag at +0x25 cleared. Names and layouts are placeholders.
#include "types.h"

struct UnkRedenkiFullReset_ovb0084;
void fn_710064f184(UnkRedenkiFullReset_ovb0084* self);

struct UnkDaivoMember_l4e0718 {
    u8 pad_0x0[0x25];
    bool field_0x25;
    u8 pad_0x26[0x160 - 0x26];
    f32 field_0x160;
};

// A group of members (0x148 bytes).
struct UnkDaivoGroup_l4e0718 {
    u8 pad_0x0[0xb8];
    UnkDaivoMember_l4e0718** members;  // 0xb8
    u32 memberCount;  // 0xc0
    u8 pad_0xc4[0x148 - 0xc4];
};

struct UnkDaivoGroupList_l4e0718 {
    u8 pad_0x0[0x50];
    UnkDaivoGroup_l4e0718* groups;  // 0x50
    u8 pad_0x58[0x110 - 0x58];
    s32 count;  // 0x110

    // The group at `i`, or null past the end.
    UnkDaivoGroup_l4e0718* At(s64 i) {
        if (i >= count)
            return nullptr;
        return groups ? &groups[i] : nullptr;
    }
};

struct UnkDaivoData_l4e0718 {
    u8 pad_0x0[0x10];
    UnkDaivoGroupList_l4e0718* groupList;  // 0x10
};

struct UnkDaivoSwarm_l4e0718 {
    void** vtable;
    u8 pad_0x8[0xe8 - 0x8];
    UnkDaivoData_l4e0718* data;  // 0xe8
    u8 pad_0xf0[0x1a4 - 0xf0];
    f32 field_0x1a4;
    u8 pad_0x1a8[0x2bc - 0x1a8];
    f32 field_0x2bc;

    f32 vf_0x208() { return reinterpret_cast<f32 (*)(UnkDaivoSwarm_l4e0718*)>(vtable[0x208 / 8])(this); }
};

void fn_71004e0718(UnkDaivoSwarm_l4e0718* self) {
    fn_710064f184(reinterpret_cast<UnkRedenkiFullReset_ovb0084*>(self));
    f32 value = self->vf_0x208();
    self->field_0x2bc = value;
    self->field_0x1a4 = value;
    s32 count = self->data->groupList->count;
    for (s32 i = 0; i < count; i++) {
        UnkDaivoGroup_l4e0718* group = self->data->groupList->At(i);
        for (u64 j = 0; j < group->memberCount; j++) {
            UnkDaivoMember_l4e0718* member = group->members[j];
            member->field_0x160 = self->vf_0x208();
            group->members[j]->field_0x25 = false;
        }
    }
}

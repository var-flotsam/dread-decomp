// CSwarmControllerComponent's unit: removes a member from group `index` of the swarm's groups (the array at +0x1a8,
// fn_71007410a4 gives the group, as src/game/CFlockingSwarmControllerComponent/CFlockingSwarmControllerComponent_b40.cpp),
// closing the gap with memcpy (as the original does); when the group is left empty, hands it to fn_710074bdd4 with 1.5. Names and layouts
// are placeholders.
#include "game/CFlockingSwarmControllerComponent.h"
#include "game/CSwarmControllerComponent.h"
#include "types.h"

struct UnkGroup_fs;
struct UnkArray_fs;

struct UnkSwarmGroup_A5_b68 {
    void** data;
    u32 count;
};

struct UnkSwarmController_A5_b68 {
    u8 pad_0x0[0x1a8];
    u8 field_0x1a8[0x10];
};

void fn_7100748370(UnkSwarmController_A5_b68* self, void* member, u32 index) {
    UnkSwarmGroup_A5_b68* group = reinterpret_cast<UnkSwarmGroup_A5_b68*>(
        fn_71007410a4(reinterpret_cast<UnkArray_fs*>(self->field_0x1a8), index));
    u32 count = group->count;
    void** data = group->data;
    u64 i = 0;
    for (; i != count; ++i) {
        if (data[i] == member)
            break;
    }
    if ((u32)i < count) {
        __builtin_memcpy(&data[i], &data[i] + 1, (u64)(count - (u32)i - 1) * sizeof(void*));
        count = group->count - 1;
        group->count = count;
    }
    if (count == 0)
        fn_710074bdd4(self, index, 1.5f);
}

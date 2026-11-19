// behaviortree::CGoToSideAction's unit (tools/tu_map.py): the update. Once a side was found, while the
// actor's navigation (+0x298) has a kind-6 path with a goal, the action ends (2) when fn_7100380e48 sends
// it there; otherwise it fails (6). Names and layouts are placeholders.
#include "types.h"

struct UnkGoToSideAction_ovb0043;
bool fn_7100380e48(UnkGoToSideAction_ovb0043* self);

struct UnkGoToSidePath_l380f34 {
    u8 pad_0x0[0x10];
    u32 kind;  // +0x10
};

struct UnkGoToSidePathHandle_l380f34 {
    u8 pad_0x0[0x10];
    UnkGoToSidePath_l380f34* target;  // +0x10
};

struct UnkGoToSideRoute_l380f34 {
    UnkGoToSidePathHandle_l380f34* path;  // +0x0
    u8 pad_0x8[0x28];
    void* goal;  // +0x30
};

struct UnkGoToSideNavigation_l380f34 {
    u8 pad_0x0[0xb0];
    UnkGoToSideRoute_l380f34* route;  // +0xb0
};

struct UnkGoToSideActor_l380f34 {
    u8 pad_0x0[0x298];
    UnkGoToSideNavigation_l380f34* navigation;  // +0x298
};

struct UnkGoToSideAction_l380f34 {
    u8 pad_0x0[0x18];
    UnkGoToSideActor_l380f34* actor;  // +0x18
    u8 pad_0x20[0x19];
    bool found;  // +0x39
};

// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
u32 fn_7100380f34(UnkGoToSideAction_l380f34* self) {
    UnkGoToSideNavigation_l380f34* navigation = self->actor->navigation;
    if (!navigation || !self->found)
        return 6;
    UnkGoToSideRoute_l380f34* route = navigation->route;
    UnkGoToSidePathHandle_l380f34* handle = route->path;
    if (!handle || !handle->target || handle->target->kind != 6 || !route->goal)
        return 6;
    return fn_7100380e48(reinterpret_cast<UnkGoToSideAction_ovb0043*>(self)) ? 2 : 6;
}
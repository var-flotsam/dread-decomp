// CFlockingSwarmControllerComponent unit: the swarm's active members (0x148 bytes each), nearest the
// player first (fn_7100739e4c). Names and layouts are placeholders.
#include <algorithm>
#include "../../game_s7/UnkIter.h"
#include "base/global/CRntVector.h"
#include "game/CFlockingSwarmControllerComponent.h"
#include "types.h"

struct UnkObj_710072a110;

bool fn_7100739e4c(UnkObj_710072a110* a, UnkObj_710072a110* b);

struct UnkSwarmMember_l739f10 {
    u8 pad_0x0[0x24];
    bool active;  // +0x24
    u8 pad_0x25[0x123];
};
static_assert(sizeof(UnkSwarmMember_l739f10) == 0x148, "UnkSwarmMember_l739f10 placeholder layout");

struct UnkSwarmMembers_l739f10 {
    u8 pad_0x0[0x50];
    UnkSwarmMember_l739f10* data;
    u8 pad_0x58[0xb8];
    s32 count;  // +0x110

    UnkSwarmMember_l739f10* At(s64 i) {
        if (i >= count)
            return nullptr;
        if (!data)
            return nullptr;
        return &data[i];
    }
};

struct UnkSwarmMembersOwner_l739f10 {
    u8 pad_0x0[0x10];
    UnkSwarmMembers_l739f10* members;
};

struct UnkSwarm_l739f10 {
    u8 pad_0x0[0xe8];
    UnkSwarmMembersOwner_l739f10* field_0xe8;

    UnkSwarmMembers_l739f10* Members() { return field_0xe8->members; }
};

base::global::CRntVector<UnkObj_710072a110*> fn_7100739f10(UnkSwarmUpdate_b65a4* update) {
    auto* self = reinterpret_cast<UnkSwarm_l739f10*>(update);
    base::global::CRntVector<UnkObj_710072a110*> result(0xf);
    result.Reserve(self->Members()->count);
    for (s32 i = 0; i < self->Members()->count; ++i) {
        UnkSwarmMember_l739f10* member = self->Members()->At(i);
        if (member && member->active)
            result.PushBack(reinterpret_cast<UnkObj_710072a110*>(member));
    }
    using Iter = UnkIter<UnkObj_710072a110*>;
    std::sort(Iter{result.data}, Iter{result.data + result.count}, fn_7100739e4c);
    return result;
}

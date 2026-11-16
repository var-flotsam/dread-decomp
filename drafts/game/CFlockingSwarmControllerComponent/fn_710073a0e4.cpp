// CFlockingSwarmControllerComponent unit: the rank a swarm member takes next, from the leader's (found
// with fn_7100739c64 when none is given). Inside the leader's own group, the lowest rank of the active
// members, raising *order to the highest order among them; in another group, the member rank farthest
// ahead of *order, counting round the group's rank count. Names and layouts are placeholders.
#include "game/CFlockingSwarmControllerComponent.h"
#include "types.h"

struct UnkSwarmMember_l73a0e4 {
    u8 pad_0x0[0x24];
    bool active;  // +0x24
    u8 pad_0x25[0x27];
    u32 rank;   // +0x4c
    u64 place;  // +0x50: the group in the low word, the order in the high one
    u8 pad_0x58[0xf0];

    u32 Group() const { return static_cast<u32>(place); }
    u32 Order() const { return static_cast<u32>(place >> 32); }
};
static_assert(sizeof(UnkSwarmMember_l73a0e4) == 0x148, "UnkSwarmMember_l73a0e4 placeholder layout");

struct UnkSwarmMembers_l73a0e4 {
    u8 pad_0x0[0x50];
    UnkSwarmMember_l73a0e4* data;
    u8 pad_0x58[0xb8];
    s32 count;  // +0x110

    UnkSwarmMember_l73a0e4* At(s32 i) {
        if (i >= count)
            return nullptr;
        return data ? &data[i] : nullptr;
    }
};

struct UnkSwarmMembersOwner_l73a0e4 {
    u8 pad_0x0[0x10];
    UnkSwarmMembers_l73a0e4* members;
};

// A group: its number of ranks at vf_0x28.
struct UnkSwarmGroup_l73a0e4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual u32 RankCount();  // vf_0x28
};

// The groups: the current one's number at vf_0x28, a group by number at vf_0x30.
struct UnkSwarmGroups_l73a0e4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual u32 Current();                       // vf_0x28
    virtual UnkSwarmGroup_l73a0e4* Get(u32 group);  // vf_0x30
};

struct UnkSwarm_l73a0e4 {
    u8 pad_0x0[0x80];
    UnkSwarmGroups_l73a0e4* groups;  // +0x80
    u8 pad_0x88[0x60];
    UnkSwarmMembersOwner_l73a0e4* field_0xe8;
};

void fn_710073a0e4(UnkSwarmUpdate_b65a4* update, u32* out_rank, u32* order, UnkParent_71007455dc* parent) {
    auto* self = reinterpret_cast<UnkSwarm_l73a0e4*>(update);
    if (!parent)
        parent = fn_7100739c64(update);
    auto* leader = reinterpret_cast<UnkSwarmMember_l73a0e4*>(parent);
    u32 group = leader->Group();
    *order = leader->Order();
    u32 rank;
    if (self->groups->Current() == group) {
        rank = leader->rank;
        for (s64 i = 0; i < self->field_0xe8->members->count; ++i) {
            UnkSwarmMember_l73a0e4* member = self->field_0xe8->members->At(i);
            if (!member->active)
                continue;
            if (rank > member->rank)
                rank = member->rank;
            if (*order < member->Order())
                *order = member->Order();
        }
    } else {
        rank = *order;
        u32 ranks = self->groups->Get(group)->RankCount();
        UnkSwarmMembers_l73a0e4* members = self->field_0xe8->members;
        u32 best = 0;
        for (s64 i = 0; i < members->count; ++i) {
            UnkSwarmMember_l73a0e4* member = members->At(i);
            if (!member->active)
                continue;
            u32 r = ranks > member->rank ? member->rank : ranks - 1;
            // How far r is ahead of *order, counting round the ranks.
            u32 ahead = *order >= r ? *order - r : *order - r + ranks;
            if (ahead > best) {
                best = ahead;
                rank = r;
            }
        }
    }
    *out_rank = rank;
}

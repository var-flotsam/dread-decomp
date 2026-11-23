// CFlockingSwarmControllerComponent unit: the swarm's leader, chosen once and cached at +0x2a0: among
// the active members (0x148 bytes each) standing on the navigation mesh, one outside the excluded
// group first, then the highest rank within its group, then the smallest distance. Names and layouts
// are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/CFlockingSwarmControllerComponent.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU1161.h"
#include "game/UnkVec3Result.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkSwarmMember_l739c64 {
    u8 pad_0x0[0x24];
    bool active;  // +0x24
    u8 pad_0x25[0x23];
    u32 group;  // +0x48
    u32 rank;   // +0x4c
    u8 pad_0x50[0x14];
    f32 distance;  // +0x64
    u8 pad_0x68[0xe0];
};
static_assert(sizeof(UnkSwarmMember_l739c64) == 0x148, "UnkSwarmMember_l739c64 placeholder layout");

struct UnkSwarmMembers_l739c64 {
    u8 pad_0x0[0x50];
    UnkSwarmMember_l739c64* data;
    u8 pad_0x58[0xb8];
    s32 count;  // +0x110

    UnkSwarmMember_l739c64* At(s32 i) {
        if (i >= count)
            return nullptr;
        return data ? &data[i] : nullptr;
    }
};

struct UnkSwarmMembersOwner_l739c64 {
    u8 pad_0x0[0x10];
    UnkSwarmMembers_l739c64* members;
};

struct UnkSwarmGroupSource_l739c64;
struct UnkSwarmGroupSourceVtbl_l739c64 {
    u8 pad_0x0[0x28];
    u32 (*vf_0x28)(UnkSwarmGroupSource_l739c64* self);
};
struct UnkSwarmGroupSource_l739c64 {
    UnkSwarmGroupSourceVtbl_l739c64* vtable;
};

struct UnkSwarm_l739c64 {
    u8 pad_0x0[0x80];
    UnkSwarmGroupSource_l739c64* field_0x80;
    u8 pad_0x88[0x60];
    UnkSwarmMembersOwner_l739c64* field_0xe8;
    u8 pad_0xf0[0x1b0];
    UnkSwarmMember_l739c64* leader;  // +0x2a0

    UnkSwarmMembers_l739c64* Members() { return field_0xe8 ? field_0xe8->members : nullptr; }
};
static_assert(__builtin_offsetof(UnkSwarm_l739c64, leader) == 0x2a0, "UnkSwarm_l739c64 placeholder layout");

static inline UnkNavMeshManager* NavMeshManager() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

UnkParent_71007455dc* fn_7100739c64(UnkSwarmUpdate_b65a4* update) {
    auto* self = reinterpret_cast<UnkSwarm_l739c64*>(update);
    if (!self->leader) {
        u32 excluded = self->field_0x80->vtable->vf_0x28(self->field_0x80);
        u32 best_group = 0;
        u32 best_rank = 0;
        f32 best_distance = 3.4028235e38f;
        UnkSwarmMember_l739c64* best = nullptr;
        u16 best_cell = 0xffff;
        s32 n = self->field_0xe8->members->count;
        for (s32 i = 0; i < n; ++i) {
            UnkSwarmMember_l739c64* member = self->Members()->At(i);
            if (!member->active)
                continue;
            u32 group = member->group;
            u32 rank = member->rank;
            bool better;
            if (!best)
                better = true;
            else if (best_group == excluded && excluded != group)
                better = true;
            else if (best_group != group)
                better = false;
            else if (best_rank < rank)
                better = true;
            else
                better = best_rank == rank && member->distance < best_distance;
            if (!better)
                continue;
            UnkNavMesh* mesh = NavMeshManager()->navmesh;
            UnkVec3Result pos = fn_7100745e24(reinterpret_cast<UnkParent_71007455dc*>(member));
            CVector2D point;
            fn_710007b430(&point, reinterpret_cast<const CVector3D*>(&pos));
            u16 cell = fn_710027a41c(mesh, &point, 0);
            if (best_cell != 0xffff && cell == 0xffff)
                continue;
            best_distance = member->distance;
            best = member;
            best_group = group;
            best_cell = cell;
            best_rank = rank;
        }
        self->leader = best;
    }
    return reinterpret_cast<UnkParent_71007455dc*>(self->leader);
}

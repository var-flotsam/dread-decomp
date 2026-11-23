// CRedenkiSwarmControllerComponent's unit (tools/tu_map.py unit 977): the position the swarm heads for.
// The player's position (the owner's without a player), lowered while the player is in state 6, or,
// outside the swarm's area or off a walkable navmesh cell, the position of the player's "damage" collider.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/CRedenkiSwarmControllerComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkSwarmArea_b66a4.h"
#include "game/UnkSwarmHold_b66a4.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU367.h"
#include "game/UnkVec3Result.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

// An actor's translation column (the transform at +0x38).
struct UnkSwarmActor_l64fef8 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    UnkVec3Result Position() const { return UnkVec3Result(x, y, z); }
};

struct UnkSwarmPlayerState_l64fef8 {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkSwarmPlayerLogic_l64fef8 {
    u8 pad_0x0[0x2d08];
    UnkSwarmPlayerState_l64fef8* state;  // 0x2d08
};

struct UnkSwarmPlayer_l64fef8 : UnkSwarmActor_l64fef8 {
    u8 pad_0x68[0x208 - 0x68];
    UnkSwarmPlayerLogic_l64fef8* logic;  // 0x208
    u8 pad_0x210[0x2a0 - 0x210];
    UnkCollisionContainer* collision;  // 0x2a0

    bool InState6() const { return logic && logic->state && logic->state->state == 6; }
};

struct UnkSwarmSettings_l64fef8 {
    u8 pad_0x0[0x58];
    bool field_0x58;
};

// The members of the swarm controller this function reads besides UnkSwarmHold_b66a4's.
struct UnkSwarmController_l64fef8 {
    u8 pad_0x0[0x30];
    UnkSwarmActor_l64fef8* owner;  // 0x30
    u8 pad_0x38[0x560 - 0x38];
    UnkSwarmSettings_l64fef8* settings;  // 0x560
};

struct UnkGameFlags_l64fef8 {
    u8 pad_0x0[0xc4];
    bool field_0xc4;
    bool field_0xc5;
};

static inline UnkNavMeshManager* NavMeshManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

// Whether the navmesh cell under `point` exists and lacks flag 2.
static inline bool OnWalkableCell(const base::math::CVector2D* point) {
    UnkNavMesh* mesh = NavMeshManager()->navmesh;
    u16 cell = fn_710027a41c(mesh, point, 0);
    return cell != 0xffff && !(fn_71002b009c(fn_710027a33c(mesh, cell)) & 2);
}

UnkVec3Result fn_710064fef8(UnkSwarmHold_b66a4* self) {
    UnkSwarmController_l64fef8* controller = reinterpret_cast<UnkSwarmController_l64fef8*>(self);
    auto* player = reinterpret_cast<UnkSwarmPlayer_l64fef8*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return controller->owner->Position();

    UnkVec3Result pos = player->Position();
    if (controller->settings->field_0x58) {
        bool in_state6 = player->InState6();
        auto* flags = reinterpret_cast<UnkGameFlags_l64fef8*>(g_unk_7101d47310);
        if (!flags->field_0xc5 && flags->field_0xc4 && in_state6)
            pos.y += -150.0f;
        return pos;
    }

    UnkSwarmArea_b66a4* area = self->mode ? self->Area(0x1b8) : self->Area(0x1b0);
    if (area) {
        base::math::CVector2D pos2;
        fn_710007b430(&pos2, reinterpret_cast<const base::math::CVector3D*>(&pos));
        if (!area->Contains(&pos2) || !OnWalkableCell(&pos2)) {
            UnkCollisionContainer* collision = player->collision;
            UnkCollisionObject* collider = fn_71008640a4(collision, &GameManagerIdTable().field_0x1650);
            if (collider) {
                UnkMatrix44 m = fn_7100d36a2c(reinterpret_cast<UnkShape*>(collider));
                pos = UnkVec3Result(m.m[3], m.m[7], m.m[11]);
            }
        }
    }
    return pos;
}

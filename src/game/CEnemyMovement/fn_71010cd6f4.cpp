// CEnemyMovement unit: the navigation hit of the segment the owner's second actor stands on in the
// owner's path (its stored position), falling back to the base movement's hit.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkNavMeshHit.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkPath442.h"
#include "game/UnkTU362.h"
#include "types.h"

using base::math::CVector2D;

struct UnkCharacterMovementNav_A5_b66;
UnkNavMeshHit fn_7100979c3c(UnkCharacterMovementNav_A5_b66* self);

struct UnkSegmentOwner_l0cd6f4 {
    u8 pad_0x0[0x10];
    u32 kind;  // 0x10
};

struct UnkOwnerActor_l0cd6f4 {
    u8 pad_0x0[0x848];
    UnkSegmentOwner_l0cd6f4* field_0x848;
    u8 pad_0x850[0x124];
    CVector2D point;  // 0x974
    u8 pad_0x97c[8];
    u32 segment;  // 0x984
};

struct UnkOwner_l0cd6f4 {
    u8 pad_0x0[0x218];
    UnkOwnerActor_l0cd6f4* actor;  // 0x218
    u8 pad_0x220[0x20];
    u8* path;  // 0x240
};

struct UnkEnemyMovement_l0cd6f4 {
    u8 pad_0x0[0x30];
    UnkOwner_l0cd6f4* owner;
};

static inline UnkNavMeshManager* NavManager() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

UnkNavMeshHit fn_71010cd6f4(UnkEnemyMovement_l0cd6f4* self) {
    UnkNavMesh* mesh = NavManager()->navmesh;
    UnkOwner_l0cd6f4* owner = self->owner;
    u8* path = owner->path;
    if (mesh && path) {
        UnkOwnerActor_l0cd6f4* actor = owner->actor;
        if (actor && actor->field_0x848 && actor->field_0x848->kind == 1) {
            UnkPath442* line = reinterpret_cast<UnkPath442*>(path + 0x68);
            u32 index = actor->segment;
            if (index < line->SegmentCount()) {
                const u32* entry = fn_710035e580(line, index);
                if (*reinterpret_cast<const s16*>(entry) != -1)
                    return UnkNavMeshHit(entry, &actor->point);
            }
        }
    }
    return fn_7100979c3c(reinterpret_cast<UnkCharacterMovementNav_A5_b66*>(self));
}

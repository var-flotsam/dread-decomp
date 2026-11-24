// CEnemyMovement unit: the navigation cell of the segment the owner's second actor stands on in the
// owner's path; falls back to the base movement's lookup. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "types.h"

struct UnkCharacterMovementCell_ovb0021;
UnkNavMeshCell* fn_7100979b30(UnkCharacterMovementCell_ovb0021* self);

struct UnkPath442 {
    u8 pad_0x0[0x8];
    u32 count;  // 0x8
    u8 pad_0xc[0x20];
    u8 closed;  // 0x2c
};
const u32* GetPathSegmentEntryOrFallback(const UnkPath442* self, u32 index);

struct UnkSegmentOwner_l0cd610 {
    u8 pad_0x0[0x10];
    u32 kind;  // 0x10
};

struct UnkOwnerActor_l0cd610 {
    u8 pad_0x0[0x848];
    UnkSegmentOwner_l0cd610* field_0x848;
    u8 pad_0x850[0x134];
    u32 segment;  // 0x984
};

struct UnkOwner_l0cd610 {
    u8 pad_0x0[0x218];
    UnkOwnerActor_l0cd610* actor;  // 0x218
    u8 pad_0x220[0x20];
    u8* path;  // 0x240
};

struct UnkEnemyMovement_l0cd610 {
    u8 pad_0x0[0x30];
    UnkOwner_l0cd610* owner;
};

static inline UnkNavMeshManager* NavManager() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

static inline u32 SegmentCount(const UnkPath442* path) {
    return path->count - ((path->closed == 0) & (path->count != 0));
}

void fn_71010cd610(UnkEnemyMovement_l0cd610* self) {
    UnkNavMesh* mesh = NavManager()->navmesh;
    UnkOwner_l0cd610* owner = self->owner;
    u8* path = owner->path;
    if (mesh && path) {
        UnkOwnerActor_l0cd610* actor = owner->actor;
        if (actor && actor->field_0x848 && actor->field_0x848->kind == 1) {
            UnkPath442* line = reinterpret_cast<UnkPath442*>(path + 0x68);
            u32 index = actor->segment;
            if (index < SegmentCount(line)) {
                u16 cell = *reinterpret_cast<const u16*>(GetPathSegmentEntryOrFallback(line, index));
                if (cell != 0xffff)
                    {
                    fn_710027a328(mesh, cell);
                    return;
                }
            }
        }
    }
    fn_7100979b30(reinterpret_cast<UnkCharacterMovementCell_ovb0021*>(self));
}

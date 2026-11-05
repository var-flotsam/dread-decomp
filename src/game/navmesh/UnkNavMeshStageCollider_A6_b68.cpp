// CNavMeshItemStage's unit: the collision object registered under the string id at `index` of a stage's
// id table (0x10-byte entries at +0x50, count at +0x4c), looked up in the collision container of the
// owner's actor (+8, +0x30, +0x2a0). Null when the index is out of range or any link is missing. Two
// identical copies. Names are placeholders.
#include "game/UnkCollisionRecord.h"

namespace {

struct UnkActor_A6_b68 {
    u8 unk_0x0[0x2a0];
    UnkCollisionContainer* collision;
};

struct UnkOwner_A6_b68 {
    u8 unk_0x0[0x30];
    UnkActor_A6_b68* actor;
};

}  // namespace

struct UnkNavMeshStage_A6_b68 {
    u8 unk_0x0[0x8];
    UnkOwner_A6_b68* owner;
    u8 unk_0x10[0x3c];
    u32 count;
    u8 ids[1][0x10];
};

static inline __attribute__((always_inline)) UnkCollisionObject* StageCollider_A6_b68(UnkNavMeshStage_A6_b68* self,
                                                                                       u32 index) {
    if (index < self->count && self->owner && self->owner->actor && self->owner->actor->collision) {
        u8(*ids)[0x10] = self->ids;
        u64 i = index;
        asm("" : "+r"(ids), "+r"(i));
        return fn_71008640a4(self->owner->actor->collision, reinterpret_cast<const base::global::CStrId*>(ids[i]));
    }
    return nullptr;
}

UnkCollisionObject* fn_71009c5bdc(UnkNavMeshStage_A6_b68* self, u32 index) {
    return StageCollider_A6_b68(self, index);
}

UnkCollisionObject* fn_71009c5c1c(UnkNavMeshStage_A6_b68* self, u32 index) {
    return StageCollider_A6_b68(self, index);
}

// UnkTU811 (CEmmyAIComponent's unit): records the entity's transform with the name of its current
// collision shape in a ring of 100 records (+0x27724). Names and layouts are placeholders.
#include <new>

#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/CCharacterMovement.h"
#include "types.h"

using base::global::CStrId;

struct UnkMatrixRecord;
void fn_7100569e58(UnkMatrixRecord* self);

// A transform and a name. An empty one (an identity matrix and the empty id) is built in place out of
// line, by fn_7100569e58.
struct UnkPoseRecord_l5455e8 {
    UnkMatrix44 matrix;
    CStrId name;  // 0x40

    // Builds a record in the storage at `slot`: the matrix word by word, then the name.
    static void Construct(UnkPoseRecord_l5455e8* slot, const UnkMatrix44& m, const CStrId& id) {
        UnkCopyMatrix(&slot->matrix, m);
        new (&slot->name) CStrId(id);
    }
    void Set(const UnkMatrix44& m, const CStrId& id) {
        UnkCopyMatrix(&matrix, m);
        name = id;
    }
};
static_assert(sizeof(UnkPoseRecord_l5455e8) == 0x48, "UnkPoseRecord_l5455e8");

struct UnkShapeView_l5455e8 {
    u8 pad_0x0[0x20];
    CStrId name;  // 0x20
};

struct UnkEntity_l5455e8 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x218 - 0x78];
    UnkFlagged* collision;  // 0x218
};

static constexpr u32 kPoseCount = 100;

struct UnkEmmyAI_l5455e8 {
    u8 pad_0x0[0x30];
    UnkEntity_l5455e8* entity;  // 0x30
    u8 pad_0x38[0x27724 - 0x38];
    u32 pose_count;                               // 0x27724
    UnkPoseRecord_l5455e8 poses[kPoseCount];      // 0x27728
    s32 last_pose;                                // 0x29348
};
static_assert(__builtin_offsetof(UnkEmmyAI_l5455e8, last_pose) == 0x29348, "layout");

static inline void ClearPoses(UnkEmmyAI_l5455e8* self) {
    for (u32 i = 0; i < self->pose_count; ++i)
        self->poses[i].~UnkPoseRecord_l5455e8();
    self->pose_count = 0;
}

static inline UnkPoseRecord_l5455e8& PoseAt(UnkEmmyAI_l5455e8* self, u32 index) {
    if (index >= self->pose_count) {
        for (u32 i = self->pose_count; i <= index; ++i)
            fn_7100569e58(reinterpret_cast<UnkMatrixRecord*>(&self->poses[i]));
        self->pose_count = index + 1;
    }
    return self->poses[index];
}

static inline void AddPose(UnkEmmyAI_l5455e8* self, const UnkMatrix44& matrix, const CStrId& name) {
    if (self->last_pose == -1) {
        ClearPoses(self);
        UnkPoseRecord_l5455e8::Construct(&self->poses[self->pose_count], matrix, name);
        ++self->pose_count;
        self->last_pose = 0;
    } else if (self->pose_count == kPoseCount) {
        u32 next = self->last_pose + 1;
        self->last_pose = next >= kPoseCount ? 0 : next;
        PoseAt(self, self->last_pose).Set(matrix, name);
    } else {
        UnkPoseRecord_l5455e8::Construct(&self->poses[self->pose_count], matrix, name);
        self->last_pose = self->pose_count++;
    }
}

bool fn_71005455e8(UnkEmmyAI_l5455e8* self) {
    UnkFlagged* collision = self->entity->collision;
    if (!collision)
        return false;
    UnkShapeView_l5455e8* shape = reinterpret_cast<UnkShapeView_l5455e8*>(fn_7100976588(collision));
    if (!shape)
        return false;
    UnkMatrix44 matrix;
    UnkCopyMatrix(&matrix, fn_710002614c(&self->entity->transform));
    CStrId name(shape->name);
    AddPose(self, matrix, name);
    return true;
}

// behaviortree::CGroundShockerFollowPathAction's unit (tools/tu_map.py unit 857): the action's start. After
// the base node's (fn_7100394bd4), when the owner's AI component is a CBaseGroundShockerAIComponent and the
// owner's +0x298 part is in state 2, copies the path id and the movement's (+0x1f0) point into the AI;
// then the part is handed a cleared kind-5 parameter block (fn_71006f4b0c). Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkBehaviorTree.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkFollowPathPoint_l5a1f48 {
    f32 x, y, z;
};

// The path the AI follows: its id and the point it starts from.
struct UnkFollowPathTarget_l5a1f48 {
    u32 path_id;                       // 0x0
    UnkFollowPathPoint_l5a1f48 point;  // 0x4
};

struct UnkFollowPathAI_l5a1f48 {
    u8 pad_0x0[0x14678];
    UnkFollowPathTarget_l5a1f48 path;  // 0x14678
};

struct UnkFollowPathMovement_l5a1f48 {
    u8 pad_0x0[0x40];
    UnkFollowPathPoint_l5a1f48 point;  // 0x40
};

struct UnkFollowPathRecord_l5a1f48 {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkFollowPathEntry_l5a1f48 {
    u8 pad_0x0[0x10];
    UnkFollowPathRecord_l5a1f48* record;  // 0x10
};

struct UnkFollowPathInfo_l5a1f48 {
    u8 pad_0x0[0xac];
    u32 path_id;  // 0xac
};

struct UnkFollowPathStatus_l5a1f48 {
    UnkFollowPathEntry_l5a1f48* entry;  // 0x0
    u8 pad_0x8[0x8];
    UnkFollowPathInfo_l5a1f48* info;  // 0x10

    // The path info, while the current entry's record is in state 2.
    UnkFollowPathInfo_l5a1f48* Following() const {
        UnkFollowPathRecord_l5a1f48* record = entry ? entry->record : nullptr;
        if (record && record->state == 2)
            return info;
        return nullptr;
    }
};

struct UnkFollowPathPart_l5a1f48 {
    u8 pad_0x0[0xb0];
    UnkFollowPathStatus_l5a1f48* status;  // 0xb0
};

struct UnkFollowPathOwner_l5a1f48 {
    u8 pad_0x0[0x1f0];
    UnkFollowPathMovement_l5a1f48* movement;  // 0x1f0
    u8 pad_0x1f8[0x48];
    UnkReflObject* ai;  // 0x240
    u8 pad_0x248[0x50];
    UnkFollowPathPart_l5a1f48* part;  // 0x298
};

struct UnkFollowPathAction_l5a1f48 {
    u8 pad_0x0[0x18];
    UnkFollowPathOwner_l5a1f48* owner;  // 0x18
};

void fn_71005a1f48(UnkFollowPathAction_l5a1f48* self) {
    fn_7100394bd4(self);
    UnkReflObject* obj = self->owner->ai;
    bool ground_shocker = obj && fn_710009d50c(obj->vf_0x0(), fn_71005a2224());
    UnkReflObject* ai = ground_shocker ? obj : nullptr;
    UnkFollowPathPart_l5a1f48* part = self->owner->part;
    if (ai && part) {
        if (UnkFollowPathMovement_l5a1f48* movement = self->owner->movement) {
            if (UnkFollowPathInfo_l5a1f48* info = part->status->Following()) {
                UnkFollowPathTarget_l5a1f48& path = reinterpret_cast<UnkFollowPathAI_l5a1f48*>(ai)->path;
                path.path_id = info->path_id;
                path.point = movement->point;
            }
        }
    }
    if (part) {
        UnkParams params;
        params.field_0x1c = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        params.vtable = g_unk_7101a25b10 + 2;
        __builtin_memset(params.field_0x8, 0, 13);
        params.field_0x30 = 0;
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.field_0x34 = 5;
        fn_71006f4b0c(part, &params, 1);
    }
}

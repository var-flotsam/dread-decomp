// CRedenkiSwarmControllerComponent's unit (tools/tu_map.py unit 977): the approach step. With no target the
// state is reset (vf_0x2e8); otherwise the swarm is sent towards its goal (or the kept goal at +0x478 when the
// area does not contain it) along the path (vf_0x2f8). When that fails and no point near the target is found,
// the billboard effect plays, every active member's +0x15c is cleared and the state becomes 2 at the target's
// position. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CDizzeanSwarmControllerComponent.h"
#include "game/CFlockingSwarmControllerComponent.h"
#include "game/CRedenkiSwarmControllerComponent.h"
#include "game/CSwarmControllerComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkSwarmPoint_b65a4.h"
#include "game/UnkSwarmRecord_b65a4.h"
#include "game/UnkTU1159.h"
#include "game/UnkTU1161.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2560.h"
#include "game/UnkVec3Result.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::CClass;

extern u32 g_unk_7101a5aef0;
extern char g_unk_710164a418[];

struct UnkRedenkiApproachMember_l655e80 {
    u8 pad_0x0[0x15c];
    u32 field_0x15c;
    u32 field_0x160;
};

// One group: its members and their count.
struct UnkRedenkiApproachGroup_l655e80 {
    u8 pad_0x0[0xb8];
    UnkRedenkiApproachMember_l655e80** members;  // 0xb8
    u32 count;  // 0xc0
    u8 pad_0xc4[0x148 - 0xc4];
};

struct UnkRedenkiKeyHolder_l655e80 {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkRedenkiBillboard_l655e80 {
    void** vtable;
    u8 pad_0x8[0xc8];
    UnkRedenkiKeyHolder_l655e80* field_0xd0;
};

struct UnkRedenkiGroups_l655e80 {
    u8 pad_0x0[0x10];
    UnkRedenkiBillboard_l655e80* field_0x10;
    u8 pad_0x18[0x50 - 0x18];
    UnkRedenkiApproachGroup_l655e80* groups;  // 0x50
    u8 pad_0x58[0x110 - 0x58];
    s32 count;  // 0x110

    UnkRedenkiApproachGroup_l655e80* Group(s64 index) {
        if (index >= count)
            return nullptr;
        return groups ? &groups[index] : nullptr;
    }
};

struct UnkRedenkiChain_l655e80 {
    u8 pad_0x0[0x10];
    UnkRedenkiGroups_l655e80* field_0x10;
};

struct UnkRedenkiStateSub_l655e80 {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

// What fn_7100df57f4 answers: its state at +0xb4, or (in state 7) the one of its +0x20.
struct UnkRedenkiPlayerState_l655e80 {
    u8 pad_0x0[0x20];
    UnkRedenkiStateSub_l655e80* sub;  // 0x20
    u8 pad_0x28[0xb4 - 0x28];
    s32 state;  // 0xb4
};

struct UnkRedenkiPlayerMode_l655e80 {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkRedenkiPlayerPart_l655e80 {
    u8 pad_0x0[0x2d08];
    UnkRedenkiPlayerMode_l655e80* mode;  // 0x2d08
};

struct UnkRedenkiPlayer_l655e80 {
    u8 pad_0x0[0x208];
    UnkRedenkiPlayerPart_l655e80* part;  // 0x208
};

struct UnkRedenkiManagerFlags_l655e80 {
    u8 pad_0x0[0xc4];
    bool field_0xc4;
    bool field_0xc5;
};

static inline bool PlayerFlag() {
    UnkRedenkiPlayer_l655e80* player =
        reinterpret_cast<UnkRedenkiPlayer_l655e80*>(fn_7100dec788(g_unk_7101d47310));
    if (!player || !player->part || !player->part->mode || player->part->mode->state != 6)
        return false;
    UnkRedenkiManagerFlags_l655e80* flags = reinterpret_cast<UnkRedenkiManagerFlags_l655e80*>(g_unk_7101d47310);
    if (flags->field_0xc5)
        return false;
    return flags->field_0xc4;
}

struct UnkRedenkiArea_l655e80 {
    void** vtable;
    bool Contains(CVector2D* point) {
        return reinterpret_cast<bool (*)(UnkRedenkiArea_l655e80*, CVector2D*)>(vtable[0x50 / 8])(this, point);
    }
};

struct UnkRedenkiTuning_l655e80 {
    u8 pad_0x0[0x58];
    bool field_0x58;
};

struct UnkRedenkiApproach_l655e80 {
    void** vtable;
    u8 pad_0x8[0xe8 - 0x8];
    UnkRedenkiChain_l655e80* field_0xe8;
    u8 pad_0xf0[0x2c8 - 0xf0];
    u8 path[0x54];  // 0x2c8
    UnkSwarmRecord_b65a4 record;  // 0x31c
    u8 pad_0x31d[0x470 - 0x31d];
    UnkParent_71007455dc* target;  // 0x470
    f32 keptGoal[3];  // 0x478
    u8 pad_0x484[0x544 - 0x484];
    u32 state;  // 0x544
    f32 statePos[3];  // 0x548
    u8 pad_0x554[0x560 - 0x554];
    UnkRedenkiTuning_l655e80* tuning;  // 0x560

    UnkRedenkiArea_l655e80* Area() {
        return reinterpret_cast<UnkRedenkiArea_l655e80* (*)(UnkRedenkiApproach_l655e80*)>(vtable[0x1b8 / 8])(this);
    }
    void SetState(u32 a, u32 b) {
        reinterpret_cast<void (*)(UnkRedenkiApproach_l655e80*, u32, u32)>(vtable[0x2e8 / 8])(this, a, b);
    }
    bool Go(const UnkVec3Result* goal, UnkParent_71007455dc* target, u32 mode, void* path, bool flag, f32 speed) {
        return reinterpret_cast<bool (*)(UnkRedenkiApproach_l655e80*, const UnkVec3Result*, UnkParent_71007455dc*, u32,
                                         void*, bool, f32)>(vtable[0x2f8 / 8])(this, goal, target, mode, path, flag,
                                                                               speed);
    }
};

void fn_7100655e80(UnkRedenkiApproach_l655e80* self) {
    UnkParent_71007455dc* target = self->target;
    if (!target) {
        self->SetState(0, 0);
        return;
    }
    UnkSwarmPoint_b65a4 point;
    UnkVec3Result goal = fn_710064fef8(reinterpret_cast<UnkSwarmHold_b66a4*>(self));
    UnkRedenkiArea_l655e80* area = self->Area();
    UnkRedenkiPlayerState_l655e80* player =
        static_cast<UnkRedenkiPlayerState_l655e80*>(GetPlayerOpticCamouflageAbility(g_unk_7101d47310));
    s32 state = player->state;
    if (state == 7 && player->sub)
        state = player->sub->state;
    bool inArea = false;
    if (area && state != 6) {
        CVector2D goal2;
        fn_710007b430(&goal2, reinterpret_cast<const CVector3D*>(&goal));
        inArea = area->Contains(&goal2);
    }
    if (!inArea) {
        goal.x = self->keptGoal[0];
        goal.y = self->keptGoal[1];
        goal.z = self->keptGoal[2];
    }

    f32 speed = g_unk_71726fc5b0 ? g_unk_7101a5aef8 : -1.0f;
    bool flag = !self->tuning->field_0x58 && PlayerFlag();
    if (!self->Go(&goal, target, g_unk_7101a5aef0, self->path, flag, speed)) {
        UnkSwarmUpdate_b65a4* update = reinterpret_cast<UnkSwarmUpdate_b65a4*>(self);
        UnkVec3Result pos = fn_7100745e24(target);
        point.value = 0;
        if (fn_710073fea8(update, &pos, &point, 50.0f)) {
            fn_7100737d4c(&self->record, &point);
            self->record.active = true;
            fn_710073f7bc(reinterpret_cast<UnkRedenkiSwarmController*>(self), 1);
            return;
        }
    }

    UnkRedenkiChain_l655e80* chain = self->field_0xe8;
    UnkRedenkiBillboard_l655e80* billboard = nullptr;
    if (chain && chain->field_0x10) {
        UnkRedenkiBillboard_l655e80* obj = chain->field_0x10->field_0x10;
        billboard =
            obj && fn_710009d50c(
                       reinterpret_cast<const CClass* (*)(UnkRedenkiBillboard_l655e80*)>(obj->vtable[0])(obj),
                       fn_710072be34())
                ? obj
                : nullptr;
    }
    void* manager = g_unk_71726da048;
    if (!manager) {
        manager = fn_71000839a0(0xe0, 0x3f);
        fn_7100e1bca4(manager);
        g_unk_71726da048 = manager;
    }
    {
        u64 key = billboard->field_0xd0->field_0x30;
        fn_710074e190(self, fn_7100e1cb50(manager, &key, g_unk_710164a418), 1.0f);
    }

    for (s32 i = 0; i < self->field_0xe8->field_0x10->count; ++i) {
        UnkRedenkiApproachGroup_l655e80* group = self->field_0xe8->field_0x10->Group(i);
        for (u32 j = 0; j < group->count; ++j) {
            UnkRedenkiApproachMember_l655e80* member = group->members[j];
            if (member && fn_7100744e1c(reinterpret_cast<UnkSwarmMember_ovb6*>(member))) {
                member->field_0x15c = 0;
                member->field_0x160 = 0;
            }
        }
    }

    UnkVec3Result pos = fn_7100745e24(target);
    self->state = 2;
    self->statePos[0] = pos.x;
    self->statePos[1] = pos.y;
    self->statePos[2] = pos.z;
}

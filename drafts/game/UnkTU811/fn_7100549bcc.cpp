// UnkTU811 (CEmmyAIComponent's unit): whether the E.M.M.I. may act against its target. A pending request
// the manager refuses answers yes; otherwise the target's movement must be busy and not in the state of
// the kind fn_71009a60fc names, not hanging on a breakable tile group (state 6), and not reached by any
// of the component's links. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CSamusMagnetGloveMovementState/CParams.h"
#include "game/UnkActor.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU365.h"
#include "game/UnkTU811.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

bool fn_7100daaae0(UnkManager_71726af468* self, bool flag);

struct UnkGroupLink_l549bcc {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* object;  // 0x10
};

struct UnkGroupSlot_l549bcc {
    u8 pad_0x0[0x10];
    UnkGroupLink_l549bcc* link;  // 0x10
};

struct UnkMoveState_l549bcc {
    void* vtable;
    u8 pad_0x8[0x8];
    u32 kind;  // 0x10
    u8 pad_0x14[0x1b8 - 0x14];
    UnkGroupSlot_l549bcc* group;  // 0x1b8
};

struct UnkMoveVtable_l549bcc {
    u8 pad_0x0[0x3b8];
    bool (*vf_0x3b8)(struct UnkMove_l549bcc* self);
};

#pragma pack(push, 4)
struct UnkMove_l549bcc {
    UnkMoveVtable_l549bcc* vtable;
    u8 pad_0x8[0x2d08 - 0x8];
    UnkMoveState_l549bcc* current;  // 0x2d08
    u8 pad_0x2d10[0x2d18 - 0x2d10];
    u32 stateCount;  // 0x2d18
    UnkMoveState_l549bcc* states[1];  // 0x2d1c, 4-byte aligned
};
#pragma pack(pop)

struct UnkReachVtable_l549bcc {
    u8 pad_0x0[0x50];
    bool (*vf_0x50)(struct UnkReach_l549bcc* self, const CVector2D* point);
};

struct UnkReach_l549bcc {
    UnkReachVtable_l549bcc* vtable;
};

struct UnkTrackerView_l549bcc {
    u8 pad_0x0[0x30];
    UnkReach_l549bcc* reach;  // 0x30
};

struct UnkLinkHolder_l549bcc : base::UnkRefCounted {
    void* source;  // 0x10
};

struct UnkLink_l549bcc {
    u8 pad_0x0[0x8];
    u8 field_0x8[0x28];
    u32 state;  // 0x30
    u8 pad_0x34[4];
    base::UnkRefHandle handle;  // 0x38
};
static_assert(sizeof(UnkLink_l549bcc) == 0x40, "UnkLink_l549bcc layout");

struct UnkTargetHolder_l549bcc {
    u8 pad_0x0[0x10];
    UnkActor* actor;  // 0x10
};

struct UnkRequest_l549bcc {
    UnkTargetHolder_l549bcc* target;  // 0x0
    u32 active;  // 0x8
    u8 pad_0xc[0x80 - 0xc];
    s16 id;  // 0x80
    u8 slot;  // 0x82
};

struct UnkEmmyAI_l549bcc {
    u8 pad_0x0[0x14b40];
    UnkRequest_l549bcc request;  // 0x14b40
    u8 pad_0x14bc4[0x2a6c8 - 0x14bc4];
    UnkLink_l549bcc* links;  // 0x2a6c8
    u32 linkCount;  // 0x2a6d0
};

static inline UnkManager_71726af468* GetManager() {
    if (!g_unk_71726af468) {
        UnkManager_71726af468* m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return g_unk_71726af468;
}

static inline UnkMoveState_l549bcc* FindState(UnkMove_l549bcc* move, u32 kind) {
    if (kind > 14)
        return nullptr;
    UnkMoveState_l549bcc** it = move->states;
    for (u64 size = (u64)move->stateCount * sizeof(*it); size != 0; ++it, size -= sizeof(*it)) {
        if ((*it)->kind == kind)
            return *it;
    }
    return nullptr;
}

static inline UnkMove_l549bcc* Movement(UnkActor* actor) {
    return static_cast<UnkMove_l549bcc*>(actor->field_0x208);
}

bool fn_7100549bcc(UnkEmmyAI_l549bcc* self) {
    UnkRequest_l549bcc* request = &self->request;
    if (request->active && request->id != -1 && request->slot != 0xff) {
        if (!fn_7100daaae0(GetManager(), false))
            return true;
    }

    UnkActor* target = request->target ? request->target->actor : nullptr;
    bool able = fn_71005436e8(reinterpret_cast<UnkEmmyAIComponent_b46a1*>(self));
    if (!target || !able)
        return false;
    if (!target->field_0x208)
        return false;
    if (!fn_71009e12d0(target->field_0x208) && !Movement(target)->vtable->vf_0x3b8(Movement(target)))
        return false;

    UnkMove_l549bcc* move = Movement(target);
    UnkMoveState_l549bcc* state = FindState(move, fn_71009a60fc());
    bool free = !state || state != move->current;
    if (!free)
        return false;

    move = Movement(target);
    UnkMoveState_l549bcc* hang = FindState(move, 6);
    if (hang && move->current == hang && hang->group && hang->group->link) {
        base::reflection::UnkReflObject* group = hang->group->link->object;
        if (group)
            free = !fn_710009d50c(group->vf_0x0(), fn_7100ca0484());
    }
    if (!free)
        return false;

    UnkLink_l549bcc* link = self->links;
    for (u64 size = (u64)self->linkCount * sizeof(*link); size != 0; ++link, size -= sizeof(*link)) {
        if (link->state == 1) {
            {
                UnkRefPtr reference = fn_710023b580(link->field_0x8);
                fn_7100234f78(&link->handle, reinterpret_cast<const base::UnkRefHandle*>(&reference));
            }
            link->state = 2;
        }
        auto* holder = static_cast<UnkLinkHolder_l549bcc*>(link->handle.ptr);
        if (!holder || !holder->source)
            continue;
        auto* tracker = reinterpret_cast<UnkTrackerView_l549bcc*>(fn_710028aca8(holder->source));
        if (!tracker)
            continue;
        UnkReach_l549bcc* reach = tracker->reach;
        if (!reach)
            continue;
        CVector3D position = {target->field_0x44, target->field_0x54, target->field_0x64};
        CVector2D point;
        fn_710007b430(&point, &position);
        if (reach->vtable->vf_0x50(reach, &point)) {
            free = false;
            break;
        }
    }
    return free;
}

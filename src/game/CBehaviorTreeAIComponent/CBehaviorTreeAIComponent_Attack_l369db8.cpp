// CBehaviorTreeAIComponent's unit (tools/tu_map.py, unit 447): runs the current attack with a query built
// from the actor and the attack parameters, filling in the distance to the target when the settings ask
// for it; without the settings' flag it falls back to fn_7101105950. Names and layouts are placeholders.
#include "base/UnkVtableList.h"
#include "base/math/CVector.h"
#include "game/CAttack.h"
#include "game/UnkActor.h"
#include "game/UnkTU394.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

#include <math.h>

// A list of nodes (link at +8) freed through the allocator.
struct UnkAttackNodeList_l369db8 : base::UnkVtableList<void> {
    ~UnkAttackNodeList_l369db8() {
        vtable = g_unk_7101a21540 + 0x10;
        void* n = head;
        while (n) {
            void* next = *reinterpret_cast<void**>(static_cast<char*>(n) + 8);
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

// A counted reference released on destruction.
struct UnkAttackRef_l369db8 {
    UnkRefObject* p;
    ~UnkAttackRef_l369db8() {
        if (p) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkAttackQuery_l369db8 {
    UnkAttackRef_l369db8 ref;
    f32 distance;    // 0x8
    f32 distance2;   // 0xc
    u8 pad_0x10[0x18];
    UnkAttackNodeList_l369db8 nodes;  // 0x28

    UnkAttackQuery_l369db8(UnkActor* actor, void* params) {
        fn_71002c6f80(reinterpret_cast<UnkAttackQuery_ovb0115*>(this), actor, params);
    }
};

struct UnkAttackSettings_l369db8 {
    u8 pad_0x0[0x140];
    bool use_distance;  // 0x140
    u8 pad_0x141[0x3bb];
    bool enabled;  // 0x4fc
};

struct UnkMoveMode_l369db8 {
    u8 pad_0x0[0xec];
    s32 mode;  // 0xec
};

struct UnkActorAI_l369db8 {
    u8 pad_0x0[0x1610];
    UnkMoveMode_l369db8* movement;  // 0x1610
};

struct UnkBTAIComponent_l369db8 {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0x1130];
    u8 params[8];  // 0x1168, the attack parameters
    u8 pad_0x1170[0x13548];
    UnkAttackSettings_l369db8* settings;  // 0x146b8
};
static_assert(__builtin_offsetof(UnkBTAIComponent_l369db8, settings) == 0x146b8, "settings");

struct UnkAICall_b0143;
struct UnkAIResult_b0143;
void* fn_71010ffe3c(UnkAICall_b0143* self, UnkAIResult_b0143* arg, bool flag);
void* fn_7101105950(UnkBTAIComponent_l369db8* self);

// Whether the actor's AI moves in mode 1 (then only the x distance counts).
static inline bool MovesInMode1(UnkActor* actor) {
    if (!actor)
        return false;
    UnkActorAI_l369db8* ai = static_cast<UnkActorAI_l369db8*>(actor->field_0x240);
    if (!ai || !ai->movement)
        return false;
    return ai->movement->mode == 1;
}

void* fn_7100369db8(UnkBTAIComponent_l369db8* self) {
    if (!self->settings->enabled)
        return fn_7101105950(self);
    UnkAttackQuery_l369db8 query(self->actor, self->params);
    if (self->settings->use_distance && fn_7100d7c990(self->actor)) {
        UnkActor* actor = self->actor;
        f32 distance;
        if (!MovesInMode1(actor)) {
            base::math::CVector2D flat;
            base::math::CVector3D diff;
            f32 x = actor->field_0x44;
            f32 y = actor->field_0x54;
            f32 z = actor->field_0x64;
            UnkActor* target = fn_7100d7c990(actor);
            f32 dx = x - target->field_0x44;
            f32 dy = y - target->field_0x54;
            f32 dz = z - target->field_0x64;
            diff.x = dx;
            diff.y = dy;
            diff.z = dz;
            fn_710007b430(&flat, &diff);
            distance = sqrtf(flat.x * flat.x + flat.y * flat.y);
        } else {
            f32 x = actor->field_0x44;
            distance = fabsf(x - fn_7100d7c990(actor)->field_0x44);
        }
        query.distance = distance;
        query.distance2 = distance;
    }
    return fn_71010ffe3c(reinterpret_cast<UnkAICall_b0143*>(self),
                         reinterpret_cast<UnkAIResult_b0143*>(&query), false);
}

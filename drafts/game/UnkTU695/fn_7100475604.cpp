// Unit 695 (tools/tu_map.py): the update of the set of actors CAIManager keeps for the Chozo robot soldiers
// (src/game/CAIManager/UnkAIManagerNodeSet_b43.cpp): which of them is the one to follow. Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU362.h"
#include "game/navmesh/SNavMeshPath.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkNavPathOwner_l475604;
bool fn_7101103080(UnkNavPathOwner_l475604* ai, void* path, void* a, void* b);

// Where an actor stands on the navigation mesh: a cell and a side, either -1 when it stands nowhere.
struct UnkNavSpot_l475604 {
    s16 cell;
    u8 side;
    u8 pad_0x3[0xd];

    ~UnkNavSpot_l475604() { fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(this)); }
};

struct UnkNavigator_l475604 {
    void** vtable;

    // vtable slot 0x2d8
    UnkNavSpot_l475604 Spot() { return reinterpret_cast<UnkNavSpot_l475604 (*)(UnkNavigator_l475604*)>(vtable[0x5b])(this); }
};

struct UnkNavPoint_l475604 {
    s16 cell;
    u8 side;
    u8 pad_0x3[0x11d];
};
static_assert(sizeof(UnkNavPoint_l475604) == 0x120, "layout");

struct UnkNavPoints_l475604 {
    void** vtable;
    u8 pad_0x8[0xc];
    UnkNavPoint_l475604 points[264];

    // vtable slot 0x28
    u32 Count() { return reinterpret_cast<u32 (*)(UnkNavPoints_l475604*)>(vtable[5])(this); }
};

// A path over the navigation mesh, built by fn_71002acfe4 and destroyed by fn_71002ad0dc.
struct UnkNavPath_l475604 {
    u8 pad_0x0[0x10];
    UnkNavPoints_l475604 points;
    u8 pad_0x12928[0x160];

    UnkNavPath_l475604() { fn_71002acfe4(this); }
    ~UnkNavPath_l475604() { fn_71002ad0dc(reinterpret_cast<UnkObject_71002ad0dc*>(this)); }
};
static_assert(sizeof(UnkNavPath_l475604) == 0x12a88, "layout");

// A Chozo robot soldier's AI component.
struct UnkRobotSoldierAI_l475604 {
    void** vtable;
    u8 pad_0x8[0x37];
    bool field_0x3f;
    u8 pad_0x40[0x8];
    bool field_0x48;
};

struct UnkActor_l475604 : UnkWeakTarget {
    u8 pad_0x8[0x3c];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x190];
    UnkNavigator_l475604* navigator;      // 0x1f8
    u8 pad_0x200[0x40];
    base::reflection::UnkReflObject* ai;  // 0x240
};
static_assert(__builtin_offsetof(UnkActor_l475604, ai) == 0x240, "layout");

struct UnkActorNode_l475604 {
    UnkActor_l475604* value;
    UnkActorNode_l475604* next;
};

struct UnkActorSet_l475604 {
    void** vtable;
    u8 pad_0x8[0x8];
    u32 count;                    // 0x10
    UnkActorNode_l475604* head;   // 0x18
    u8 pad_0x20[0x58];
    UnkActor_l475604* target;     // 0x78
    f32 time;                     // 0x80
    UnkWeakHandle* handle;        // 0x88
};
static_assert(__builtin_offsetof(UnkActorSet_l475604, handle) == 0x88, "layout");

static inline bool IsRobotSoldier(base::reflection::UnkReflObject* ai) {
    return fn_710009d50c(ai->vf_0x0(), fn_710046ea2c());
}

void fn_7100475604(UnkActorSet_l475604* self, f32 dt) {
    if (self->target)
        self->time += dt;
    switch (self->count) {
    case 1:
        self->target = self->head->value;
        ResetRef(&self->handle);
        return;
    case 2:
        break;
    default:
        return;
    }

    // Keep a weak handle to the actor that isn't the one the handle follows now.
    UnkActor_l475604* other;
    if (self->handle && self->handle->target) {
        other = self->head->value;
        if (self->handle->target == other)
            other = self->head->next->value;
    } else {
        other = self->head->value;
    }
    AssignRef(&self->handle, GetWeakHandle(other));

    // The first actor whose AI is busy, and the last one that isn't.
    bool several = false;
    UnkActor_l475604* busy = nullptr;
    UnkActor_l475604* idle = nullptr;
    for (UnkActorNode_l475604* n = self->head; n; n = n->next) {
        UnkActor_l475604* actor = n->value;
        if (actor && actor->ai && fn_71011057fc(reinterpret_cast<UnkRobotSoldierAI_A2_b69*>(actor->ai))) {
            if (busy)
                several = true;
            else
                busy = actor;
        } else {
            idle = actor;
        }
    }
    if (several) {
        self->target = nullptr;
        return;
    }

    UnkActor_l475604* current = self->target;
    if (idle && busy) {
        if (current == idle)
            return;
        // Follow the idle one, unless its path reaches where the busy one stands.
        base::reflection::UnkReflObject* ai = idle->ai;
        if (ai && IsRobotSoldier(ai)) {
            bool reaches = false;
            UnkNavSpot_l475604 spot = busy->navigator->Spot();
            if (spot.cell != -1 && spot.side != 0xff) {
                UnkNavPath_l475604 path;
                if (fn_7101103080(reinterpret_cast<UnkNavPathOwner_l475604*>(ai), &path, nullptr, nullptr)) {
                    for (u64 i = 0; i < path.points.Count(); ++i) {
                        UnkNavPoint_l475604& point = path.points.points[i];
                        if (point.cell == spot.cell && point.side == spot.side) {
                            reaches = true;
                            break;
                        }
                    }
                }
            }
            if (reaches) {
                self->target = nullptr;
                self->time = 0.0f;
                return;
            }
        }
        self->target = idle;
        self->time = 0.0f;
        return;
    }

    if (current) {
        if (self->time <= 3.0f)
            return;
        base::reflection::UnkReflObject* ai = current->ai;
        if (!ai || !IsRobotSoldier(ai))
            return;
        auto* soldier = reinterpret_cast<UnkRobotSoldierAI_l475604*>(ai);
        if (soldier->field_0x3f || soldier->field_0x48)
            return;
    }

    // The actor nearest to the player.
    auto* player = reinterpret_cast<UnkActor_l475604*>(fn_7100dec788(g_unk_7101d47310));
    UnkActor_l475604* nearest = nullptr;
    f32 best = 3.4028235e+38f;
    for (UnkActorNode_l475604* n = self->head; n; n = n->next) {
        UnkActor_l475604* actor = n->value;
        f32 dx = actor->x - player->x;
        f32 dy = actor->y - player->y;
        f32 dz = actor->z - player->z;
        f32 d = dx * dx + dy * dy + dz * dz;
        if (!nearest || d < best) {
            nearest = actor;
            best = d;
        }
    }
    if (self->target == nearest)
        return;
    self->target = nearest;
    self->time = 0.0f;
}

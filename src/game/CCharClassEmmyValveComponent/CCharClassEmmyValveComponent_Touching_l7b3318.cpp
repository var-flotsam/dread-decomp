// CCharClassEmmyValveComponent unit (tools/tu_map.py, unit 1212): fn_71007b3318 tells whether any registered
// E.M.M.I. that is on walkable ground (or whose path crosses a walkable boundary) has one of its two valve
// collision objects overlapping the valve's shape. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CActorComponent.h"
#include "game/CCharClassEmmyValveComponent.h"
#include "game/CCollisionComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU376.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

extern const base::global::CStrId g_unk_717271a2c8 __attribute__((visibility("hidden")));
extern const base::global::CStrId g_unk_717271a2d0 __attribute__((visibility("hidden")));

// A navigation path: the cells it starts and ends in (0xffff for none).
struct UnkValvePath_l7b3318 {
    u8 pad_0x0[2];
    u16 from;  // 0x2
    u8 pad_0x4[0x1e - 0x4];
    u16 to;  // 0x1e
    u8 pad_0x20[0x100 - 0x20];

    UnkValvePath_l7b3318() { fn_71002b57bc(Object()); }
    ~UnkValvePath_l7b3318() { fn_71002b6094(Object()); }
    UnkValvePath_l7b3318& operator=(const UnkValvePath_l7b3318& other) {
        fn_71002b5c90(Object(), other.Object());
        return *this;
    }
    UnkObject_0x100* Object() { return reinterpret_cast<UnkObject_0x100*>(this); }
    const UnkObject_0x100* Object() const { return reinterpret_cast<const UnkObject_0x100*>(this); }
};
static_assert(sizeof(UnkValvePath_l7b3318) == 0x100, "layout");

struct UnkValveMovement_l7b3318 {
    void** vtable;
    u8 pad_0x8[0x1870 - 0x8];
    UnkValvePath_l7b3318 path;  // 0x1870
    bool has_path;              // 0x1970

    UnkNavMeshCell* vf_0x2d0() {
        return reinterpret_cast<UnkNavMeshCell* (*)(UnkValveMovement_l7b3318*)>(vtable[0x2d0 / 8])(this);
    }
};
static_assert(__builtin_offsetof(UnkValveMovement_l7b3318, has_path) == 0x1970, "layout");

struct UnkValveEmmyOwner_l7b3318 {
    u8 pad_0x0[0x218];
    UnkValveMovement_l7b3318* movement;  // 0x218
    u8 pad_0x220[0x2a0 - 0x220];
    UnkPlayerMovementSource* collision;  // 0x2a0
};

struct UnkValveEmmyActor_l7b3318 {
    u8 pad_0x0[0x30];
    UnkValveEmmyOwner_l7b3318* owner;  // 0x30
};

struct UnkValveEmmyEntry_l7b3318 {
    u8 pad_0x0[0x10];
    UnkValveEmmyActor_l7b3318* actor;  // 0x10
};

struct UnkValveEmmyList_l7b3318 {
    u8 pad_0x0[8];
    u32 count;
    u32 entries[1];  // UnkValveEmmyEntry_l7b3318* entries, 4-byte aligned
    UnkValveEmmyEntry_l7b3318** Entries() { return reinterpret_cast<UnkValveEmmyEntry_l7b3318**>(entries); }
};

struct UnkValveMode_l7b3318 {
    u8 pad_0x0[0x10];
    u32 mode;  // 0x10
};

struct UnkValveShape_l7b3318 {
    u8 pad_0x0[0x158];
    void* body;  // 0x158
};

struct UnkValve_l7b3318 {
    u8 pad_0x0[0x50];
    UnkValveMode_l7b3318* field_0x50;  // 0x50
    u8 pad_0x58[0x250 - 0x58];
    UnkValveShape_l7b3318* shape;  // 0x250
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

static inline bool Walkable(UnkNavMesh* mesh, u16 cell) {
    return cell != 0xffff && fn_71002b005c(fn_710027a33c(mesh, cell));
}

// Whether the movement's path runs between a walkable cell and one that is not.
static inline bool PathCrossesWalkable(UnkNavMesh* mesh, UnkValveMovement_l7b3318* movement) {
    UnkValvePath_l7b3318 path;
    if (movement->has_path)
        path = movement->path;
    if (path.from == 0xffff || path.to == 0xffff)
        return false;
    return Walkable(mesh, path.from) ^ Walkable(mesh, path.to);
}

// Whether the owner's collision object `name` overlaps the valve's shape.
static inline bool Overlaps(UnkValve_l7b3318* self, UnkPlayerMovementSource* collision,
                            const base::global::CStrId* name) {
    void* object = fn_710086406c(collision, name);
    if (!object)
        return false;
    UnkValveShape_l7b3318* shape = self->shape;
    void* body = shape->body;
    UnkMatrix44 transform = fn_7100d36a2c(reinterpret_cast<UnkShape*>(shape));
    return fn_7100d57ca0(static_cast<UnkLavaDropsShape_A2_b71*>(body), &transform, object, nullptr, 0);
}

bool CCharClassEmmyValveComponent_IsAnyActorOverlappingShape(UnkSmBase_b60* component) {
    UnkValve_l7b3318* self = reinterpret_cast<UnkValve_l7b3318*>(component);
    UnkValveEmmyList_l7b3318* list = reinterpret_cast<UnkValveEmmyList_l7b3318*>(g_unk_71726af0a8);
    UnkValveEmmyEntry_l7b3318** begin = list->Entries();
    UnkValveEmmyEntry_l7b3318** end = begin + list->count;
    for (UnkValveEmmyEntry_l7b3318** p = begin; p != end; ++p) {
        UnkValveEmmyEntry_l7b3318* entry = *p;
        if (!entry)
            continue;
        UnkValveEmmyActor_l7b3318* actor = entry->actor;
        if (!actor || !fn_710024fbc8(actor))
            continue;
        UnkValveEmmyOwner_l7b3318* owner = actor->owner;
        if (!owner)
            continue;
        UnkValveMovement_l7b3318* movement = owner->movement;
        if (!movement)
            continue;
        if (fn_71010cab0c(movement) && !(self->field_0x50 && self->field_0x50->mode == 1))
            continue;
        UnkNavMeshCell* cell = movement->vf_0x2d0();
        if (!cell || !fn_71002b005c(cell)) {
            if (fn_71010cc3a4(movement) != 4)
                continue;
            UnkNavMesh* mesh = NavMeshManager()->navmesh;
            if (!mesh || !PathCrossesWalkable(mesh, movement))
                continue;
        }
        UnkPlayerMovementSource* collision = owner->collision;
        if (!collision)
            continue;
        if (Overlaps(self, collision, &g_unk_717271a2c8) || Overlaps(self, collision, &g_unk_717271a2d0))
            return true;
    }
    return false;
}

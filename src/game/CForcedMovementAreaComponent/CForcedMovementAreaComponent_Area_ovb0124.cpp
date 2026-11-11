// CForcedMovementAreaComponent's unit (tools/tu_map.py unit 1256): the component's destructor and the clear of
// its map of string ids to reference arrays (+0x48); its update (the colliders it touched, then its own
// collider and the cooldown at +0xe8); the probe that places its offset (+0xec) by testing the central unit's
// shape; the emmy door logic (the turn it gives the player through fn_710116c128, the registration with the
// manager g_unk_7101d47360, the check of the side the player walked through); a hit query of a named collider
// and an overlap test of its bounds. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkDeferredSet.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CActorComponent.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CChozoCommanderSentenceSphereAttack.h"
#include "game/CForcedMovementAreaComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1176.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU2547.h"
#include "game/UnkTU365.h"
#include "game/UnkTU811.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// An array of counted references.
struct UnkAreaRefs_ovb0124 {
    UnkRefObject** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    void Free() {
        if (data) {
            for (u64 i = 0; i < count; ++i) {
                if (UnkRefObject* p = data[i]) {
                    if (--p->ref_count == 0)
                        p->vf_0x10();
                }
            }
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

// A map from string ids to arrays of references.
struct UnkAreaMap_ovb0124 {
    UnkAreaRefs_ovb0124* values;
    base::UnkStrIdMember* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;  // 0x20
};

__attribute__((noinline)) void fn_71007e9580(void* p) {
    UnkAreaMap_ovb0124* self = static_cast<UnkAreaMap_ovb0124*>(p);
    if (self->keys) {
        for (u64 i = 0; i < self->count; ++i)
            self->keys[i].~UnkStrIdMemberOf();
    }
    if (self->values) {
        for (u64 i = 0; i < self->count; ++i)
            self->values[i].Free();
    }
    self->count = 0;
}

struct UnkBaseComponent;
void fn_710024f6d4(UnkBaseComponent*);
extern void* g_unk_7101a73c28[];

// The map with its arrays (sharing one allocator).
struct UnkAreaMapOwned_ovb0124 {
    UnkAreaMap_ovb0124 map;

    ~UnkAreaMapOwned_ovb0124() {
        fn_71007e9580(&map);
        if (map.keys)
            map.allocator->vf_0x48(map.keys);
        if (map.values)
            map.allocator->vf_0x48(map.values);
    }
};
static_assert(sizeof(UnkAreaMapOwned_ovb0124) == 0x28, "layout");

// A registration in a deferred set, removed on destruction.
struct UnkAreaRegistration_ovb0124 {
    base::UnkDeferredSet<void>* set;
    u64 field_0x8;
    u64 field_0x10;

    ~UnkAreaRegistration_ovb0124() {
        if (set) {
            fn_71004f3ea4(set, this);
            set = nullptr;
        }
        field_0x8 = 0;
        field_0x10 = 0;
    }
};

struct UnkForcedMovementArea_ovb0124 {
    void** vtable;
    u8 pad_0x8[0x28];
    void** vtable_0x30;
    void** vtable_0x38;
    u8 pad_0x40[0x8];
    UnkAreaMapOwned_ovb0124 map;  // 0x48
    UnkAreaRefs_ovb0124 refs;     // 0x70
    u8 pad_0x98[0x20];
    UnkAreaRegistration_ovb0124 field_0xb8;
    UnkAreaRegistration_ovb0124 field_0xd0;
};
static_assert(__builtin_offsetof(UnkForcedMovementArea_ovb0124, field_0xd0) == 0xd0, "layout");

void fn_71007e9378(UnkForcedMovementArea_ovb0124* self) {
    self->vtable = g_unk_7101a73c28 + 2;
    self->vtable_0x30 = g_unk_7101a73c28 + 0x2d;
    self->vtable_0x38 = g_unk_7101a73c28 + 0x32;
    self->field_0xd0.~UnkAreaRegistration_ovb0124();
    self->field_0xb8.~UnkAreaRegistration_ovb0124();
    self->refs.Free();
    self->map.~UnkAreaMapOwned_ovb0124();
    fn_710024f6d4(reinterpret_cast<UnkBaseComponent*>(self));
}

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

struct UnkDoorRegistry_b61;
struct UnkUnit2547;
struct UnkCentralUnit;
struct UnkBreakablePart;
struct UnkBreakableTarget;
struct UnkTracker;
struct UnkShapeTester_A5_b68;

// Returned in memory (not trivially copyable); only bit 0 of the first word is read.
struct UnkShapeHit_A5_b68 {
    u32 flags;
    u8 pad_0x4[0xc];
    UnkShapeHit_A5_b68(const UnkShapeHit_A5_b68& other);
};
UnkShapeHit_A5_b68 fn_7100d572f8(UnkShapeTester_A5_b68* tester, void* shape, const base::math::CVector2D* pos,
                                 const base::math::CVector3D* offset, u64 flags);

extern CStrId g_unk_717271fdf8 __attribute__((visibility("hidden")));

struct UnkAreaTarget_ovb0124 {
    u8 pad_0x0[0x10];
    void* actor;
};

struct UnkAreaTracker_ovb0124 {
    u8 pad_0x0[0x30];
    void* shape;
};

struct UnkAreaOwner_ovb0124 {
    u8 pad_0x0[0x44];
    u32 x;
    u8 pad_0x48[0xc];
    u32 y;
    u8 pad_0x58[0xc];
    u32 z;
};

struct UnkForcedArea_ovb0124 {
    u8 pad_0x0[0x18];
    UnkAreaOwner_ovb0124* owner;  // 0x18
    u8 pad_0x20[0xcc];
    u32 offset_x;   // 0xec: the probe offset (x, y, z), written as raw words
    u64 offset_yz;  // 0xf0
};

void fn_71007e98c8(UnkForcedArea_ovb0124* self) {
    UnkDoorRegistry_b61* reg = g_unk_71726bee10;
    if (!reg) {
        reg = static_cast<UnkDoorRegistry_b61*>(fn_71000839a0(4, 0x3f));
        fn_7100ddb6dc(reg);
        g_unk_71726bee10 = reg;
    }
    if (char* unit = reinterpret_cast<char*>(fn_7100ddb768(reinterpret_cast<UnkUnit2547*>(reg)))) {
        auto* target = reinterpret_cast<UnkAreaTarget_ovb0124*>(
            *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(unit + 0x5a8)));
        if (target && target->actor) {
            void* shape = reinterpret_cast<UnkAreaTracker_ovb0124*>(fn_710028aca8(target->actor))->shape;
            if (shape) {
                UnkShapeTester_A5_b68* tester = g_unk_71726a47a0;
                if (!tester) {
                    tester = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
                    fn_7100d5134c(tester);
                    g_unk_71726a47a0 = tester;
                }
                CVector2D probe;
                CVector2D pos;
                {
                    UnkAreaOwner_ovb0124* owner = self->owner;
                    u32 x = owner->x;
                    u32 y = owner->y;
                    u32 z = owner->z;
                    u32 v[3];
                    v[0] = x;
                    v[1] = y;
                    v[2] = z;
                    fn_710007b430(&pos, reinterpret_cast<const CVector3D*>(v));
                }
                probe.x = pos.x + 200.0f;
                probe.y = pos.y + 300.0f;
                bool hit = fn_7100d572f8(tester, shape, &probe, reinterpret_cast<const CVector3D*>(&self->offset_x), 0)
                               .flags &
                           1;
                self->offset_yz = 0x43960000;  // (300.0f, 0.0f)
                self->offset_x = hit ? 0xc3480000 : 0x43480000;  // -200.0f : 200.0f
            }
        }
    }
    if (void* object = fn_710043acb4(self->owner)) {
        if (UnkCollisionObject* c = fn_71008640a4(static_cast<UnkCollisionContainer*>(object), &g_unk_717271fdf8))
            fn_7100d35a98(reinterpret_cast<UnkEntry*>(c), false);
    }
}

using base::global::CStrId;
using base::reflection::UnkReflObject;

extern CStrId g_unk_717271fde8[2] __attribute__((visibility("hidden")));

struct UnkAreaHit_ovb0124 {
    u8 pad_0x0[0x10];
    UnkReflObject* component;  // 0x10
};

struct UnkAreaHitRef_ovb0124 {
    u8 pad_0x0[0x10];
    UnkAreaHit_ovb0124* hit;  // 0x10
};

struct UnkAreaCollision_ovb0124 {
    u8 pad_0x0[0x30];
    void* field_0x30;
};

struct UnkForcedAreaUpdate_ovb0124 {
    u8 pad_0x0[0x18];
    void* owner;  // 0x18
    u8 pad_0x20[0x38];
    u32 field_0x58;
    u8 pad_0x5c[0x14];
    UnkRefObject** refs;  // 0x70
    u32 ref_count;        // 0x78
    u8 pad_0x7c[0x1c];
    bool field_0x98;
    bool field_0x99;
    u8 pad_0x9a[0x4e];
    f32 timer;  // 0xe8
};
static_assert(__builtin_offsetof(UnkForcedAreaUpdate_ovb0124, timer) == 0xe8, "layout");

void fn_71007e9ba4(UnkForcedAreaUpdate_ovb0124* self, UnkAreaHit_ovb0124* hit, void* collision);

void fn_71007e9a60(UnkForcedAreaUpdate_ovb0124* self, f32 dt) {
    if (self->ref_count) {
        for (UnkRefObject** it = self->refs; it != self->refs + self->ref_count; ++it) {
            UnkAreaHit_ovb0124* hit = reinterpret_cast<UnkAreaHitRef_ovb0124*>(*it)->hit;
            UnkReflObject* component = hit->component;
            auto* collision = reinterpret_cast<UnkAreaCollision_ovb0124*>(
                fn_710009d50c(component->vf_0x0(), fn_710085fc44()) ? component : nullptr);
            fn_71007e9ba4(self, hit, collision->field_0x30);
        }
        for (u64 i = 0; i < self->ref_count; ++i) {
            if (UnkRefObject* ref = self->refs[i]) {
                if (--ref->ref_count == 0)
                    ref->vf_0x10();
            }
        }
        self->ref_count = 0;
    }
    if (self->field_0x99 && self->field_0x98 && self->field_0x58 == 0) {
        self->field_0x99 = false;
        if (void* object = fn_710043acb4(self->owner)) {
            if (UnkCollisionObject* c = fn_71008640a4(static_cast<UnkCollisionContainer*>(object), &g_unk_717271fde8[0]))
                fn_7100d35a98(reinterpret_cast<UnkEntry*>(c), false);
        }
    }
    if (self->timer > 0.0f)
        self->timer -= dt;
}

struct UnkDoorRegistry_b61;
struct UnkScenarioObject_ovb0032;
struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
struct UnkGeneratorTarget_ovb0120;
void fn_710116c128(UnkGeneratorTarget_ovb0120*, const UnkVec3f*, const UnkVec3f*, bool);

struct UnkAreaActor_ovb0124 {
    u8 pad_0x0[0x38];
    UnkXform transform;  // 0x38
};

struct UnkForcedMovementArea_A5_b68e;

struct UnkForcedAreaPush_ovb0124 {
    u8 pad_0x0[0x18];
    UnkAreaActor_ovb0124* owner;  // 0x18
    u8 pad_0x20[0xc8];
    f32 timer;     // 0xe8
    UnkVec3f push;  // 0xec
};

__attribute__((noinline)) void fn_71007ea408(UnkForcedMovementArea_A5_b68e* area) {
    auto* self = reinterpret_cast<UnkForcedAreaPush_ovb0124*>(area);
    UnkDoorRegistry_b61* reg = g_unk_71726bee10;
    if (!reg) {
        reg = static_cast<UnkDoorRegistry_b61*>(fn_71000839a0(4, 0x3f));
        fn_7100ddb6dc(reg);
        g_unk_71726bee10 = reg;
    }
    bool blocked = fn_7100ddb938(reg);
    void* target = fn_7100774014(reinterpret_cast<UnkScenarioObject_ovb0032*>(self->owner));
    if (blocked || !target)
        return;
    if (!(self->timer <= 0.0f))
        return;
    auto* player = reinterpret_cast<UnkAreaActor_ovb0124*>(fn_7100dec788(g_unk_7101d47310));
    f32 player_yaw = player->transform.GetAngle().y;
    f32 yaw = self->owner->transform.GetAngle().y;
    UnkVec3f turn(0.0f, player_yaw - yaw, 0.0f);
    fn_710116c128(static_cast<UnkGeneratorTarget_ovb0120*>(target), &self->push, &turn, true);
    self->timer = 0.1f;
}

struct UnkDoorRegistry_b61;
struct UnkUnit2547;
struct UnkCentralUnit;
struct UnkWeakTarget;
struct UnkThermalComponent_ovb0097;
void* fn_71007ea32c(void* actor);
struct UnkEmmyAIComponent_b46a1;
extern void* g_unk_7101d47360;
void fn_7100ee1f34(void*);
bool fn_7100eea7b0(void* manager);
struct UnkForcedMovementArea_A5_b68e;

struct UnkAreaUnitLink_ovb0124 {
    u8 pad_0x0[0x10];
    void* actor;  // 0x10
};

struct UnkAreaReg_ovb0124 {
    base::UnkDeferredSet<void>* set;
    u64 field_0x8;
    u64 field_0x10;
};

struct UnkForcedAreaEmmy_ovb0124 {
    u8 pad_0x0[0x18];
    void* owner;  // 0x18
    u8 pad_0x20[0x98];
    UnkAreaReg_ovb0124 registration;  // 0xb8
};

static inline void* Manager_ovb0124() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = manager;
    }
    return manager;
}

void fn_71007ec21c(UnkForcedAreaEmmy_ovb0124* self) {
    if (!fn_71007ea250(static_cast<UnkWeakTarget*>(self->owner)))
        return;
    UnkDoorRegistry_b61* reg = g_unk_71726bee10;
    if (!reg) {
        reg = static_cast<UnkDoorRegistry_b61*>(fn_71000839a0(4, 0x3f));
        fn_7100ddb6dc(reg);
        g_unk_71726bee10 = reg;
    }
    char* unit = reinterpret_cast<char*>(fn_7100ddb768(reinterpret_cast<UnkUnit2547*>(reg)));
    if (!unit)
        return;
    auto* link = *reinterpret_cast<UnkAreaUnitLink_ovb0124**>(unit + 0x6d8);
    if (!link || !link->actor)
        return;
    void* emmy = fn_71007ea32c(link->actor);
    if (!emmy)
        return;
    if (fn_710024fbc8(emmy) && fn_71005436e8(static_cast<UnkEmmyAIComponent_b46a1*>(emmy)))
        return;
    UnkAreaReg_ovb0124* r = &self->registration;
    void* manager = Manager_ovb0124();
    if (r->set) {
        fn_71004f3ea4(r->set, r);
        r->set = nullptr;
    }
    r->set = reinterpret_cast<base::UnkDeferredSet<void>*>(static_cast<char*>(manager) + 8);
    fn_71004f3cf0(r->set, r);
    if (fn_7100eea7b0(Manager_ovb0124()))
        return;
    fn_71007ea408(reinterpret_cast<UnkForcedMovementArea_A5_b68e*>(self));
}

using base::global::CStrId;

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};
UnkMatrix44_7100078c9c fn_7100078cd8(const UnkVec3f& t);

struct UnkTree;
struct UnkShape;
struct UnkMtx44;

struct UnkAreaHitObj_ovb0124 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        s32 ref_count;
    };
};

struct UnkAreaHitRef_ovb0124b {
    UnkAreaHitObj_ovb0124* ptr;

    UnkAreaHitRef_ovb0124b() : ptr(nullptr) {}
    ~UnkAreaHitRef_ovb0124b() {
        if (UnkAreaHitObj_ovb0124* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkAreaHitVec_ovb0124 {
    f32 x, y, z;
    UnkAreaHitVec_ovb0124(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkAreaCollisionHit_ovb0124 {
    UnkAreaHitRef_ovb0124b object;
    UnkAreaHitRef_ovb0124b field_0x8;
    s32 id;
    UnkAreaHitVec_ovb0124 point;
    UnkAreaHitVec_ovb0124 field_0x20;
    UnkAreaHitVec_ovb0124 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkAreaCollisionHit_ovb0124()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};

// A hit query from a point (as in src/game/CAutsharpAIComponent/CAutsharpAIComponent_Patrol_ovb0117.cpp).
struct UnkAreaHitQuery_ovb0124 {
    void** vtable;
    UnkAreaCollisionHit_ovb0124 hit;
    bool field_0x50;
    u32 field_0x54;
    UnkVec3f origin;
    u64 field_0x68;

    explicit UnkAreaHitQuery_ovb0124(const UnkVec3f& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin = o;
        field_0x68 = 0;
    }
    ~UnkAreaHitQuery_ovb0124() { vtable = g_unk_7101a20ed8 + 2; }
};

struct UnkAreaPosOwner_ovb0124 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;

    UnkVec3f Position() const { return UnkVec3f(x, y, z); }
};

struct UnkForcedAreaQuery_ovb0124 {
    u8 pad_0x0[0x18];
    UnkAreaPosOwner_ovb0124* owner;  // 0x18
};

struct UnkAreaShaped_ovb0124 {
    u8 pad_0x0[0x158];
    UnkShape* shape;
};

bool fn_71007ec35c(UnkForcedAreaQuery_ovb0124* self, const CStrId* name) {
    UnkGameManagerSub_0x608* scenario = g_unk_7101d47310->field_0x608;
    UnkTree* tree = scenario ? static_cast<UnkTree*>(scenario->field_0x820) : nullptr;
    UnkMatrix44_7100078c9c xf = fn_7100078cd8(self->owner->Position());
    UnkAreaHitQuery_ovb0124 query(self->owner->Position());
    void* object = fn_710043acb4(self->owner);
    if (!object)
        return false;
    UnkCollisionObject* c = fn_71008640a4(static_cast<UnkCollisionContainer*>(object), name);
    if (!c)
        return false;
    return fn_7100d5b484(tree, 0x10, reinterpret_cast<UnkAreaShaped_ovb0124*>(c)->shape,
                         reinterpret_cast<const UnkMtx44*>(&xf), &query, 0);
}

using base::global::CStrId;

struct UnkAreaBox_ovb0124 {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
};

// A collider whose bounds (+0x40) are refreshed through slot 0x98 while its flag at +0x60 is set.
struct UnkAreaCollider_ovb0124 {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkAreaBox_ovb0124 bounds;  // 0x40
    u8 pad_0x50[0x10];
    bool dirty;  // 0x60

    const UnkAreaBox_ovb0124& Bounds() {
        if (dirty)
            reinterpret_cast<void (*)(UnkAreaCollider_ovb0124*)>(vtable[0x98 / 8])(this);
        return bounds;
    }
};

struct UnkForcedAreaBounds_ovb0124 {
    u8 pad_0x0[0x18];
    void* owner;  // 0x18
};

// Whether `v` is at least `lo` / at most `hi` (the bound is read first).
static inline bool AtLeast_ovb0124(f32 lo, f32 v) {
    return v >= lo;
}
static inline bool AtMost_ovb0124(f32 hi, f32 v) {
    return v <= hi;
}

static inline bool Overlaps_ovb0124(UnkAreaCollider_ovb0124* collider, const UnkAreaBox_ovb0124* box) {
    const UnkAreaBox_ovb0124& b = collider->Bounds();
    return AtLeast_ovb0124(b.min_x, box->max_x) && AtMost_ovb0124(b.max_x, box->min_x) &&
           AtLeast_ovb0124(b.min_y, box->max_y) && AtMost_ovb0124(b.max_y, box->min_y);
}

bool fn_71007ecc08(UnkForcedAreaBounds_ovb0124* self, const UnkAreaBox_ovb0124* box) {
    void* object = fn_710043acb4(self->owner);
    if (!object)
        return false;
    auto* container = static_cast<UnkCollisionContainer*>(object);
    auto* first = reinterpret_cast<UnkAreaCollider_ovb0124*>(fn_71008640a4(container, &g_unk_717271fde8[1]));
    auto* second = reinterpret_cast<UnkAreaCollider_ovb0124*>(fn_71008640a4(container, &g_unk_717271fde8[0]));
    if (first)
        return Overlaps_ovb0124(first, box);
    if (second)
        return Overlaps_ovb0124(second, box);
    return false;
}

using base::math::CVector3D;

struct UnkDoorRegistry_b61;
struct UnkUnit2547;
struct UnkCentralUnit;
struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
struct UnkEntityState;
void fn_7100232864(UnkEntityState*, bool);

struct UnkAreaPosition_ovb0124 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
};

struct UnkAreaUnitLink2_ovb0124 {
    u8 pad_0x0[0x10];
    UnkEntityState* state;  // 0x10
};

struct UnkAreaUnit_ovb0124 {
    u8 pad_0x0[0x18];
    UnkAreaPosition_ovb0124* owner;  // 0x18
    u8 pad_0x20[0x6b8];
    UnkAreaUnitLink2_ovb0124* link;  // 0x6d8
};

struct UnkForcedAreaDoor_ovb0124 {
    u8 pad_0x0[0x18];
    void* owner;  // 0x18
    u8 pad_0x20[0x88];
    CVector3D facing;  // 0xa8
};

bool fn_71007eca08(UnkForcedAreaDoor_ovb0124* self) {
    UnkDoorRegistry_b61* reg = g_unk_71726bee10;
    if (!reg) {
        reg = static_cast<UnkDoorRegistry_b61*>(fn_71000839a0(4, 0x3f));
        fn_7100ddb6dc(reg);
        g_unk_71726bee10 = reg;
    }
    auto* unit = reinterpret_cast<UnkAreaUnit_ovb0124*>(fn_7100ddb768(reinterpret_cast<UnkUnit2547*>(reg)));
    CVector3D pos = unit ? CVector3D{unit->owner->x, unit->owner->y, unit->owner->z} : g_unk_710164779c;
    f32 ux = pos.x;
    f32 uy = pos.y;
    f32 uz = pos.z;
    auto* player = reinterpret_cast<UnkAreaPosition_ovb0124*>(fn_7100dec788(g_unk_7101d47310));
    f32 px = player->x;
    f32 py = player->y;
    f32 pz = player->z;
    f32 dx = ux - px;
    f32 dy = uy - py;
    f32 dz = uz - pz;
    f32 inv = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
    dx *= inv;
    dy *= inv;
    dz *= inv;
    f32 fx = self->facing.x;
    f32 fy = self->facing.y;
    f32 fz = self->facing.z;
    f32 finv = 1.0f / (sqrtf(fx * fx + fy * fy + fz * fz) + 0.0001f);
    fx *= finv;
    fy *= finv;
    fz *= finv;
    if (dx * fx + dy * fy + dz * fz > 0.0f) {
        fn_7100000790("CurrentScenario.OnWalkThroughEmmyDoor", "ob", self->owner, false);
    } else if (unit && unit->link && unit->link->state) {
        fn_7100232864(unit->link->state, true);
    }
    const u32* zero = reinterpret_cast<const u32*>(&g_unk_710164779c);
    u32* facing = reinterpret_cast<u32*>(&self->facing);
    facing[0] = zero[0];
    facing[1] = zero[1];
    facing[2] = zero[2];
    return true;
}

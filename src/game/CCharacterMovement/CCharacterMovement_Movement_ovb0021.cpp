// CCharacterMovement (tools/tu_map.py unit 1445; the name from reflection): the movement's
// navigation cell lookup (fn_7100979b30) and its removal from the collision manager's owner lists
// (fn_710097973c); the held-by-track check (fn_71009765d8); the list of weak references with a
// countdown at +0x3f8 (erase fn_7100979360, reserve fn_710097a850, add fn_7100979144); the track
// requests (fn_7100976ab8 and the three that build one, fn_71009766b8, fn_71009767f4,
// fn_7100976944); the ground probe (fn_7100977cec); the destructor (fn_7100975f10); keeping the hit
// against the id table's source (fn_7100977974); placing the owner (fn_7100978f58); and taking a
// transform's translation and angles (fn_7100979854). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharacterMovement.h"
#include "game/CMovementComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU365.h"
#include "game/UnkTU39.h"
#include "game/UnkTU58.h"
#include "game/UnkXformNode.h"
#include "game/navmesh/CNavMesh.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

#include <new>

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkMovement_71010d0e38;

// ----
// A weak reference with a countdown (16 bytes): the list at CCharacterMovement's +0x3f8.
struct UnkTimedRef_ovb0021 {
    UnkWeakHandle* handle;
    f32 time;

    // The handle is cleared through memset: the original reloads the list's array after it.
    UnkTimedRef_ovb0021() {
        __builtin_memset(&handle, 0, sizeof(handle));
        time = -1.0f;
    }
    UnkTimedRef_ovb0021(const UnkTimedRef_ovb0021& other) : handle(nullptr) {
        if (UnkWeakHandle* p = other.handle) {
            ++p->ref_count;
            if (UnkWeakHandle* old = handle) {
                if (--old->ref_count == 0)
                    old->vf_0x10();
            }
            handle = p;
        }
        __builtin_memcpy(&time, &other.time, sizeof(time));
    }
    ~UnkTimedRef_ovb0021() {
        if (UnkWeakHandle* p = handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

using UnkTimedRefList_ovb0021 = base::UnkVector<UnkTimedRef_ovb0021>;

// Removes `it` by moving the last entry into its place; returns the entry now at its position.
UnkTimedRef_ovb0021* fn_7100979360(UnkTimedRefList_ovb0021* list, UnkTimedRef_ovb0021* it) {
    u32 index = static_cast<u32>(it - list->data);
    if (index < list->count) {
        u32 last = list->count - 1;
        if (index < last) {
            list->data[index].~UnkTimedRef_ovb0021();
            new (&list->data[index]) UnkTimedRef_ovb0021(list->data[last]);
        }
        for (u32 i = last; list->count > i; ++i)
            list->data[i].~UnkTimedRef_ovb0021();
        list->count = last;
    }
    return list->data + index;
}

struct UnkMovementEntry_ovb0021 {
    u8 unk_0x0[0x20];
    base::global::CStrId name;  // 0x20
};

struct UnkMovementOwner_ovb0021 {
    u8 unk_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 unk_0x228[0x78];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkCharacterMovement_ovb0021 {
    u8 unk_0x0[0x30];
    UnkMovementOwner_ovb0021* owner;  // 0x30
    u8 unk_0x38[0x380];
    UnkMovementEntry_ovb0021* field_0x3b8;
    UnkMovementEntry_ovb0021* field_0x3c0;
    u8 unk_0x3c8[0x30];
    UnkTimedRefList_ovb0021 timed;  // 0x3f8
};

// Grows the list's storage to `capacity` entries (base::UnkVector's reserve, written out).
void fn_710097a850(UnkTimedRefList_ovb0021* self, u32 capacity) {
    if (self->capacity < capacity) {
        auto* p = static_cast<UnkTimedRef_ovb0021*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkTimedRef_ovb0021), 8, self->flags));
        if (self->data) {
            UnkTimedRef_ovb0021* src = self->data;
            UnkTimedRef_ovb0021* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkTimedRef_ovb0021(*src);
                src->~UnkTimedRef_ovb0021();
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = capacity;
    }
}

// The entry that refers to `target`, or null.
static inline UnkTimedRef_ovb0021* FindTimed(UnkTimedRefList_ovb0021* list, UnkWeakTarget* target) {
    for (UnkTimedRef_ovb0021* entry = list->data; entry != list->data + list->count; ++entry) {
        UnkWeakTarget* current = entry->handle ? entry->handle->target : nullptr;
        if (current && current == target)
            return entry;
    }
    return nullptr;
}

// Keeps a weak reference to `target` for at least `time` (extending the entry it already has).
void fn_7100979144(UnkCharacterMovement_ovb0021* self, UnkWeakTarget* target, f32 time) {
    UnkTimedRefList_ovb0021* list = &self->timed;
    if (UnkTimedRef_ovb0021* entry = FindTimed(list, target)) {
        entry->time = entry->time > time ? entry->time : time;
        return;
    }
    u32 n = list->count + 1;
    fn_710097a850(list, n);
    for (u32 i = list->count; i < n; ++i)
        new (&list->data[i]) UnkTimedRef_ovb0021();
    list->count = n;
    UnkTimedRef_ovb0021* end = list->data + n;
    UnkTimedRef_ovb0021* entry = n ? end - 1 : end;
    AssignRef(&entry->handle, GetWeakHandle(target));
    entry->time = time;
}

// ----
struct UnkOwner_710027a41c;
u32 fn_710027a320(UnkNavMesh* mesh);

struct UnkCharacterMovementCell_ovb0021 {
    u8 unk_0x0[0x420];
    u16 cell;        // 0x420, the navigation cell the owner was last found in
    u8 unk_0x422[2];
    u32 cell_frame;  // 0x424, the frame `cell` was looked up on
};

// A position as three words (as src/game_s2/fn_71005416bc.cpp has it).
struct UnkVec3Bits {
    u32 x, y, z;
    UnkVec3Bits(const UnkVec3Bits& other) : x(other.x), y(other.y), z(other.z) {}
};
struct UnkPlace;

static inline UnkNavMeshManager* NavManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

UnkNavMeshCell* fn_7100979b30(UnkCharacterMovementCell_ovb0021* self) {
    UnkNavMesh* mesh = NavManager()->navmesh;
    if (!mesh)
        return nullptr;
    u32 frame = g_unk_7101d58e48;
    if (self->cell_frame != frame) {
        self->cell_frame = frame;
        CVector2D point;
        {
            UnkVec3Bits pos = fn_7100979a60(reinterpret_cast<UnkPlace*>(self));
            fn_710007b430(&point, reinterpret_cast<CVector3D*>(&pos));
        }
        bool still_inside = static_cast<s16>(self->cell) != -1 && fn_710027a320(mesh) >= self->cell &&
                            fn_710027a5f0(reinterpret_cast<UnkOwner_710027a41c*>(mesh), &point, self->cell);
        if (!still_inside) {
            CVector2D query;
            __builtin_memcpy(&query.x, &point.x, 4);
            __builtin_memcpy(&query.y, &point.y, 4);
            self->cell = fn_710027a41c(mesh, &query, 0);
        }
    }
    if (self->cell == 0xffff)
        return nullptr;
    return fn_710027a328(mesh, self->cell);
}

// ----
struct UnkManager_7100863a8c;
void fn_7100d46804(UnkManager_7100863a8c* self);

// The collision manager's two parallel lists that share one count: entries at +0x118, the owners they
// belong to at +0x120.
struct UnkCollisionOwnerLists_ovb0021 {
    u8 unk_0x0[0x118];
    void** entries;  // 0x118
    void** owners;   // 0x120
    u32 count;       // 0x128
};

struct UnkCharacterMovementOwner_ovb0021 {
    u8 unk_0x0[0x30];
    void* owner;  // 0x30
    u8 unk_0x38[0x3e8];
    u16 cell;  // 0x420
};

static inline UnkManager_7100863a8c* CollisionManager() {
    UnkManager_7100863a8c* m = g_unk_71726a4788;
    if (!m) {
        m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return m;
}

static inline void** Find(UnkCollisionOwnerLists_ovb0021* lists, void* owner) {
    for (u32 i = 0; i < lists->count; ++i) {
        if (lists->owners[i] == owner)
            return lists->entries + i;
    }
    return lists->entries + lists->count;
}

// Swaps the last entry into `it`'s place in both lists (the stores go through memcpy: the original
// reloads the count and the owner list after each).
static inline void EraseAt(UnkCollisionOwnerLists_ovb0021* lists, void** it) {
    {
        u32 index = static_cast<u32>(it - lists->entries);
        u32 last = lists->count - 1;
        if (index != last) {
            __builtin_memcpy(&lists->entries[index], &lists->entries[last], sizeof(void*));
            __builtin_memcpy(&lists->owners[index], &lists->owners[lists->count - 1], sizeof(void*));
        }
        lists->count = lists->count - 1;
    }
}

void fn_710097973c(UnkCharacterMovementOwner_ovb0021* self) {
    self->cell = 0xffff;
    UnkCollisionOwnerLists_ovb0021* lists = reinterpret_cast<UnkCollisionOwnerLists_ovb0021*>(CollisionManager());
    void** it = Find(lists, self->owner);
    void** end = lists->entries + lists->count;
    if (it != end)
        EraseAt(lists, it);
}

// ----
struct UnkMovement_71010d0e38;

struct UnkHoldState_ovb0021 {
    u8 pad_0x0[0x70];
    u32 kind;
};

struct UnkHeldOwner_ovb0021 {
    u8 pad_0x0[0x1c8];
    UnkHoldState_ovb0021* hold;
    u8 pad_0x1d0[0x50];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkHeldMovement_ovb0021 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkHeldOwner_ovb0021* owner;  // 0x30
    u8 pad_0x38[0x1dc - 0x38];
    f32 fall_speed;  // 0x1dc
    u8 pad_0x1e0[0x3aa - 0x1e0];
    bool held;  // 0x3aa

    void vf_0x320() { reinterpret_cast<void (*)(UnkHeldMovement_ovb0021*)>(vtable[0x320 / 8])(this); }
};

static inline bool NearZero(f32 v) {
    return v - 0.001f <= 0.0f && v + 0.001f >= 0.0f;
}

void fn_71009765d8(UnkHeldMovement_ovb0021* self) {
    f32 speed = self->fall_speed;
    if (!NearZero(speed) && speed < 0.0f)
        return;
    if (!self->held)
        return;
    if (fn_71010d0e38(reinterpret_cast<UnkMovement_71010d0e38*>(self)))
        return;
    UnkHeldOwner_ovb0021* owner = self->owner;
    if (owner->hold && owner->hold->kind == 2) {
        self->held = true;
        return;
    }
    if (UnkAnimationComponent* animation = owner->animation) {
        bool held = fn_710117b348(animation, fn_7100d09928());
        self->held = held;
        if (held)
            return;
    } else {
        self->held = false;
    }
    self->vf_0x320();
}

// ----
struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b958;

struct UnkTrackTime_ovb0021 {
    u64 key;
    u64 time;
};
struct UnkTrackBlend_ovb0021 {
    u32 a, b, c;
};

// What a track request is built from (0x30 bytes): no id, the default time and blend descriptors,
// no flags and an all-bits mask.
struct UnkTrackParams_ovb0021 {
    s64 id;
    UnkTrackTime_ovb0021 time;
    UnkTrackBlend_ovb0021 blend;
    u32 flags;
    u32 mask;

    __attribute__((always_inline)) UnkTrackParams_ovb0021(UnkTrackTime_ovb0021 time_, UnkTrackBlend_ovb0021 blend_, u32 mask_)
        : id(-1), time(time_), blend(blend_), flags(0), mask(mask_) {}
    __attribute__((always_inline)) UnkTrackParams_ovb0021()
        : UnkTrackParams_ovb0021(reinterpret_cast<const UnkTrackTime_ovb0021&>(g_unk_710164b958),
                                 reinterpret_cast<const UnkTrackBlend_ovb0021&>(g_unk_7172769740),
                                 base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>()) {}
};

// The track record a query resolves, as these functions use it.
struct UnkTrackParamRecord_ovb0021 {
    virtual const base::reflection::CType* vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkTrackParams_ovb0021* params);  // fills in the request
    base::global::CStrId name;  // 0x8
    u8 unk_0x10[8];
    base::global::CStrId state;  // 0x18
};

struct UnkTrackRunState_ovb0021 {
    u8 unk_0x0[0x360];
    bool running;  // 0x360
};

struct UnkTrackOwner_ovb0021 {
    u8 unk_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 unk_0x228[0x88];
    UnkTrackRunState_ovb0021* run_state;  // 0x2b0
};

struct UnkTrackMovement_ovb0021 {
    void** vtable;
    u8 unk_0x8[0x28];
    UnkTrackOwner_ovb0021* owner;  // 0x30

    void vf_0x330(UnkTrackParams_ovb0021* params) {
        reinterpret_cast<void (*)(UnkTrackMovement_ovb0021*, UnkTrackParams_ovb0021*)>(vtable[0x330 / 8])(this, params);
    }
    void vf_0x328(UnkTrackParams_ovb0021* params) {
        reinterpret_cast<void (*)(UnkTrackMovement_ovb0021*, UnkTrackParams_ovb0021*)>(vtable[0x328 / 8])(this, params);
    }
};

// Builds the request of the owner's current track of class `cls` when it is a state track that names
// a state; `forced` marks the request (flag bit 0).
bool fn_7100976ab8(UnkTrackMovement_ovb0021* self, const base::reflection::CClass* cls, UnkTrackParams_ovb0021* params,
                   bool forced) {
    *params = UnkTrackParams_ovb0021();
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return false;
    UnkTrackQuery query = {cls, nullptr, -1.0f};
    if (!fn_710117b08c(animation, &query))
        return false;
    auto* record = reinterpret_cast<UnkTrackParamRecord_ovb0021*>(query.result);
    if (!record || !fn_710009d50c(record->vf_0x0(), fn_7100d2a190()))
        return false;
    if (record->state.data != &g_unk_7101d47108->empty) {
        record->vf_0x18(params);
        if (forced)
            params->flags |= 1;
    }
    return true;
}

UnkTrackParams_ovb0021 fn_71009766b8(UnkTrackMovement_ovb0021* self, bool forced) {
    UnkTrackParams_ovb0021 params;
    fn_7100976ab8(self, fn_7100c440e8(), &params, forced);
    return params;
}

UnkTrackParams_ovb0021 fn_71009767f4(UnkTrackMovement_ovb0021* self, bool forced) {
    UnkTrackParams_ovb0021 params;
    fn_7100976ab8(self, fn_7100d28cc8(), &params, forced);
    self->vf_0x328(&params);
    return params;
}

UnkTrackParams_ovb0021 fn_7100976944(UnkTrackMovement_ovb0021* self, bool forced) {
    UnkTrackParams_ovb0021 params;
    UnkTrackRunState_ovb0021* state = self->owner->run_state;
    if (state && state->running)
        return params;
    fn_7100976ab8(self, fn_7100d29268(), &params, forced);
    self->vf_0x330(&params);
    return params;
}

// ----
struct UnkCharacterMovement;
struct UnkCollisionHit;
bool fn_7100977e50(UnkCharacterMovement* self, f32 max_dot, f32 extra, f32 step, f32 back, u32 mask,
                   const CVector3D* pos, CVector3D* out_point, UnkCollisionHit* out_hit, const CVector3D* dir);
struct UnkMovementComponent_c4;
struct UnkHeightOut_c4;
void fn_71010d1ca4(UnkMovementComponent_c4* self, UnkHeightOut_c4* out);

// A counted reference released through the object's virtual 0x10.
struct UnkProbeRef_ovb0021 {
    UnkRefObject* ptr;

    UnkProbeRef_ovb0021() : ptr(nullptr) {}
    ~UnkProbeRef_ovb0021() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

// The collision hit fn_7100977e50 fills in (0x48 bytes, UnkCollisionHit of src/game/CCharacterMovement.cpp).
struct UnkProbeHit_ovb0021 {
    UnkProbeRef_ovb0021 object;
    UnkProbeRef_ovb0021 field_0x8;
    s32 id;
    f32 values[12];

    UnkProbeHit_ovb0021() : id(-1), values() {}
};
static_assert(sizeof(UnkProbeHit_ovb0021) == 0x48, "UnkProbeHit_ovb0021 layout");

struct UnkProbeLink_ovb0021 {
    u8 unk_0x0[0x10];
    void* target;  // 0x10
};

struct UnkProbeOwner_ovb0021 {
    u8 unk_0x0[0x44];
    f32 x;  // 0x44
    u8 unk_0x48[0xc];
    f32 y;  // 0x54
    u8 unk_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkProbeMovement_ovb0021 {
    void** vtable;
    u8 unk_0x8[0x28];
    UnkProbeOwner_ovb0021* owner;  // 0x30
    u8 unk_0x38[0x140];
    UnkProbeLink_ovb0021* link;  // 0x178
    u8 unk_0x180[0x250];
    u32 mask;  // 0x3d0

    UnkVec3f vf_0x1a8() { return reinterpret_cast<UnkVec3f (*)(UnkProbeMovement_ovb0021*)>(vtable[0x1a8 / 8])(this); }
    void vf_0x218(UnkProbeHit_ovb0021* hit) {
        reinterpret_cast<void (*)(UnkProbeMovement_ovb0021*, UnkProbeHit_ovb0021*)>(vtable[0x218 / 8])(this, hit);
    }
    UnkCharacterMovement* vf_0x2a8() {
        return reinterpret_cast<UnkCharacterMovement* (*)(UnkProbeMovement_ovb0021*)>(vtable[0x2a8 / 8])(this);
    }
};

static inline UnkVec3f OwnerPosition(UnkProbeOwner_ovb0021* a) {
    return UnkVec3f(a->x, a->y, a->z);
}

void fn_7100977cec(UnkProbeMovement_ovb0021* self, UnkHeightOut_c4* out) {
    if (!(self->link && self->link->target) && fn_71010d0e38(reinterpret_cast<UnkMovement_71010d0e38*>(self))) {
        if (UnkCharacterMovement* movement = self->vf_0x2a8()) {
            UnkProbeHit_ovb0021 hit;
            u32 mask = self->mask;
            UnkVec3f pos = OwnerPosition(self->owner);
            UnkVec3f dir = self->vf_0x1a8();
            if (fn_7100977e50(movement, -0.3f, 3000.0f, 3.4028235e38f, 0.0f, mask, reinterpret_cast<CVector3D*>(&pos),
                              nullptr, reinterpret_cast<UnkCollisionHit*>(&hit), reinterpret_cast<CVector3D*>(&dir)))
                self->vf_0x218(&hit);
        }
    }
    fn_71010d1ca4(reinterpret_cast<UnkMovementComponent_c4*>(self), out);
}

// ----

extern void* g_unk_7101a854c8[];  // CCharacterMovement's vtable (reached through the GOT)

struct UnkMoveSlot_0xb0 {
    u8 data[0xb0];
};
void fn_71000309ac(UnkMoveSlot_0xb0* self);

struct UnkMoveTracker_ovb0021 {
    u8 data[0x8];
};
void fn_7100d4c17c(UnkMoveTracker_ovb0021* self);
void fn_7100d4c190(UnkMoveTracker_ovb0021* self);

struct UnkCharacterMovementFull_ovb0021;

static inline void Release(UnkRefObject* p) {
    if (p) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
}

struct UnkCharacterMovementFull_ovb0021 {
    void** vtable;
    u8 unk_0x8[0x228];
    UnkRefObject* field_0x230;
    UnkRefObject* field_0x238;
    u8 unk_0x240[0x38];
    UnkRefObject* field_0x278;
    u8 unk_0x280[0x118];
    UnkWeakHandle* target;  // 0x398
    u8 unk_0x3a0[0x10];
    UnkMoveTracker_ovb0021 tracker;  // 0x3b0
    u8 unk_0x3b8[0x40];
    base::UnkVector<UnkTimedRef_ovb0021> timed;  // 0x3f8
    u16 cell;  // 0x420
    u32 cell_frame;  // 0x424
    UnkMoveSlot_0xb0 slot_0x428;
    UnkMoveSlot_0xb0 slot_0x4d8;
    UnkMoveSlot_0xb0 slot_0x588;
    UnkMoveSlot_0xb0 slot_0x638;
};
static_assert(__builtin_offsetof(UnkCharacterMovementFull_ovb0021, timed) == 0x3f8, "layout");
static_assert(__builtin_offsetof(UnkCharacterMovementFull_ovb0021, slot_0x428) == 0x428, "layout");

void fn_7100975f10(UnkCharacterMovementFull_ovb0021* self) {
    self->vtable = &g_unk_7101a854c8[2];
    fn_7100030a50(&self->slot_0x638);
    fn_7100030a50(&self->slot_0x588);
    fn_7100030a50(&self->slot_0x4d8);
    fn_7100030a50(&self->slot_0x428);
    self->timed.Free();
    fn_7100d4c190(&self->tracker);
    Release(self->target);
    Release(self->field_0x278);
    Release(self->field_0x238);
    Release(self->field_0x230);
    fn_71010d092c(self);
}

// ----
// The out-of-line assignment both hit references share (as src/game/CCharacterMovement.cpp declares it).
struct UnkAssignedSlot {
    void* ptr;
};

struct UnkHitSource_ovb0021 {
    u8 unk_0x0[0x20];
    void* id;  // 0x20
};

struct UnkHitObject_ovb0021 {
    u8 unk_0x0[0x10];
    UnkHitSource_ovb0021* source;  // 0x10
};

struct UnkHitRef_ovb0021 {
    UnkHitObject_ovb0021* ptr;
    UnkHitRef_ovb0021& operator=(const UnkHitRef_ovb0021& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
};

struct UnkHitVec_ovb0021 {
    f32 x, y, z;
    UnkHitVec_ovb0021& operator=(const UnkHitVec_ovb0021& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

// A collision hit (0x48 bytes, UnkCollisionHit of src/game/CCharacterMovement.cpp).
struct UnkMoveHit_ovb0021 {
    UnkHitRef_ovb0021 object;
    UnkHitRef_ovb0021 field_0x8;
    s32 id;
    UnkHitVec_ovb0021 point;
    UnkHitVec_ovb0021 field_0x20;
    UnkHitVec_ovb0021 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkMoveHit_ovb0021& operator=(const UnkMoveHit_ovb0021& o) {
        if (this == &o)
            return *this;
        object = o.object;
        const UnkAssignedSlot* second = reinterpret_cast<const UnkAssignedSlot*>(&o.field_0x8);
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(&field_0x8), second);
        id = o.id;
        point = o.point;
        field_0x20 = o.field_0x20;
        normal = o.normal;
        field_0x38 = o.field_0x38;
        flags = o.flags;
        field_0x40 = o.field_0x40;
        return *this;
    }
};

struct UnkMoveHitList_ovb0021 {
    UnkMoveHit_ovb0021* data;
    u64 count;
};

struct UnkMoveContactState_ovb0021 {
    u8 unk_0x0[0x10];
    s32 contacts;  // 0x10
};

struct UnkCharacterMovementHit_ovb0021 {
    u8 unk_0x0[0x230];
    UnkMoveHit_ovb0021 hit;  // 0x230
};

static inline bool IsTableSource(UnkHitSource_ovb0021* source) {
    if (!source)
        return false;
    UnkGameManagerIdTable& table = GameManagerIdTable();
    return source->id == table.field_0x20f0.data;
}

// Keeps the first hit against an object whose source has the table's id.
void fn_7100977974(UnkCharacterMovementHit_ovb0021* self, UnkMoveHitList_ovb0021* hits, void*, void*, void*,
                   UnkMoveContactState_ovb0021* state) {
    if (state->contacts != 0 || hits->count == 0)
        return;
    for (UnkMoveHit_ovb0021* it = hits->data; it != hits->data + hits->count; ++it) {
        UnkHitObject_ovb0021* object = it->object.ptr;
        if (object && IsTableSource(object->source)) {
            self->hit = *it;
            return;
        }
    }
}

// ----
struct UnkProbeResult;

struct UnkMoveBounds_ovb0021 {
    CVector3D min;
    CVector3D max;
};

// The shape at +0x158 of the movement object: its vf_0x28 gives the bounding box of its +0x8.
struct UnkMoveShape_ovb0021 {
    void** vtable;
    u8 field_0x8[1];

    UnkMoveBounds_ovb0021 GetBounds() {
        return reinterpret_cast<UnkMoveBounds_ovb0021 (*)(UnkMoveShape_ovb0021*, void*)>(vtable[0x28 / 8])(this,
                                                                                                         field_0x8);
    }
};

struct UnkMoveBody_ovb0021 {
    u8 unk_0x0[0x158];
    UnkMoveShape_ovb0021* shape;  // 0x158
};

struct UnkMoveActor_ovb0021 {
    void** vtable;
    u8 unk_0x8[0x218];
    UnkAnimationComponent* animation;  // 0x220

    void vf_0xd0(UnkVec3f* pos, bool flag) {
        reinterpret_cast<void (*)(UnkMoveActor_ovb0021*, UnkVec3f*, bool)>(vtable[0xd0 / 8])(this, pos, flag);
    }
};

struct UnkCharacterMovementPlace_ovb0021 {
    void** vtable;
    u8 unk_0x8[0x28];
    UnkMoveActor_ovb0021* owner;  // 0x30
    u8 unk_0x38[0xc1];
    bool field_0xf9;
    u8 unk_0xfa[0x196];
    f32 pos_x;  // 0x290
    u8 unk_0x294[0xc];
    f32 pos_y;  // 0x2a0
    u8 unk_0x2a4[0xc];
    f32 pos_z;  // 0x2b0
    u8 unk_0x2b4[0x90];
    f32 prev_x;  // 0x344
    u8 unk_0x348[0xc];
    f32 prev_y;  // 0x354
    u8 unk_0x358[0xc];
    f32 prev_z;  // 0x364

    void vf_0x190(UnkVec3f* pos) {
        reinterpret_cast<void (*)(UnkCharacterMovementPlace_ovb0021*, UnkVec3f*)>(vtable[0x190 / 8])(this, pos);
    }
    void vf_0x260() { reinterpret_cast<void (*)(UnkCharacterMovementPlace_ovb0021*)>(vtable[0x260 / 8])(this); }
    void vf_0x288() { reinterpret_cast<void (*)(UnkCharacterMovementPlace_ovb0021*)>(vtable[0x288 / 8])(this); }
    UnkMoveBody_ovb0021* vf_0x2a8() {
        return reinterpret_cast<UnkMoveBody_ovb0021* (*)(UnkCharacterMovementPlace_ovb0021*)>(vtable[0x2a8 / 8])(this);
    }
};
static_assert(__builtin_offsetof(UnkCharacterMovementPlace_ovb0021, prev_z) == 0x364, "layout");

static inline bool InTrack_ovb0021(UnkMoveActor_ovb0021* owner, const base::reflection::CClass* (*cls)()) {
    UnkAnimationComponent* animation = owner->animation;
    return animation && fn_710117b348(animation, cls());
}

// Places the owner at `arg` (three words), dropped onto the ground under it unless a track keeps it
// from falling.
void fn_7100978f58(char* self_, const void* arg) {
    auto* self = reinterpret_cast<UnkCharacterMovementPlace_ovb0021*>(self_);
    const UnkVec3f* target = static_cast<const UnkVec3f*>(arg);
    UnkVec3f pos = *target;
    UnkVec3f ground(0.0f, 0.0f, 0.0f);
    UnkAnimationComponent* animation = self->owner->animation;
    bool falls = animation ? !fn_710117b348(animation, fn_7100d0c7a8()) : true;
    self->vf_0x288();
    if (UnkMoveBody_ovb0021* body = self->vf_0x2a8()) {
        if (UnkMoveShape_ovb0021* shape = body->shape) {
            UnkMoveBounds_ovb0021 bounds = shape->GetBounds();
            if (falls) {
                f32 bottom = bounds.min.y;
                if (!fn_71010d0e38(reinterpret_cast<UnkMovement_71010d0e38*>(self)) &&
                    fn_71010d15d0(self_, reinterpret_cast<const CVector3D*>(target),
                                  reinterpret_cast<UnkProbeResult*>(&ground), -bottom)) {
                    pos.x = ground.x;
                    pos.y = ground.y - bottom;
                    pos.z = ground.z;
                    if (self->field_0xf9) {
                        self->field_0xf9 = false;
                        self->vf_0x260();
                    }
                }
            }
        }
    }
    if (!InTrack_ovb0021(self->owner, fn_7100d11078))
        self->vf_0x190(&pos);
    self->owner->vf_0xd0(&pos, true);
    f32 x = pos.x;
    f32 y = pos.y;
    f32 z = pos.z;
    self->pos_x = x;
    self->pos_y = y;
    self->pos_z = z;
    self->prev_x = x;
    self->prev_y = y;
    self->prev_z = z;
}

// ----
struct UnkCharacterMovementXform_ovb0021 {
    u8 unk_0x0[0x284];
    UnkXform current;  // 0x284
    u8 unk_0x2e0[0x58];
    UnkXform previous;  // 0x338
};
static_assert(__builtin_offsetof(UnkCharacterMovementXform_ovb0021, previous) == 0x338, "layout");

void fn_71010d45b8(UnkCharacterMovementXform_ovb0021* self, UnkXform* xform, const u32* changed);

// Read untyped: the original reloads the mask after the translation is stored.
static inline u32 Changed(const u32* changed) {
    u32 value;
    __builtin_memcpy(&value, changed, sizeof(value));
    return value;
}

// Takes the translation (bit 0 of `changed`) and the angles (bit 1) of `xform` into both poses.
void fn_7100979854(UnkCharacterMovementXform_ovb0021* self, UnkXform* xform, const u32* changed) {
    fn_71010d45b8(self, xform, changed);
    if (Changed(changed) & 1) {
        self->current.SetTranslation(xform->GetTranslation());
        self->previous.SetTranslation(xform->GetTranslation());
    }
    if (Changed(changed) & 2) {
        self->current.SetAngle(xform->GetAngle());
        self->previous.SetAngle(xform->GetAngle());
    }
}

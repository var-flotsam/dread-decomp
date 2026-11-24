// CPowerBombMovement's unit (tools/tu_map.py unit 1612): the power bomb's update. Before it goes off it
// runs the bomb movement's update; while the blast grows it eases its radius in, hurts what it reaches
// and flashes the screen once it is full; then it pulls the X parasites outside it in, shrinks, and
// lets them go when it is gone; once done it waits to remove its owner. Names and layouts are
// placeholders.
#include <math.h>
#include <stdlib.h>

#include "base/UnkDestructors.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CHydrogigaAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2562.h"
#include "game/UnkTU2656.h"
#include "game/UnkTU341.h"
#include "game/UnkTU53.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::UnkReflObject;

// A counted object (count at +0x8, released through slot 0x10); the listener's link has its owner
// at +0x10.
struct UnkPbCounted_ovb0015 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;           // 0x8
    void* owner;             // 0x10
};

struct UnkPbNode_ovb0015 {
    u8 pad_0x0[0x8];
    UnkPbCounted_ovb0015* object;  // 0x8
    UnkPbNode_ovb0015* next;       // 0x10
};

// The chained hash map (the layout of base/UnkHashMap.h's): 11 buckets to start with.
struct UnkPbMap_ovb0015 {
    base::IAllocator* allocator;  // 0x0: the nodes'
    u32 field_0x8;
    u32 count;                     // 0xc
    base::global::CRntVector<void*> buckets;  // 0x10
    UnkPbNode_ovb0015* first;      // 0x38
    UnkPbNode_ovb0015* last;       // 0x40: where the walk starts

    UnkPbMap_ovb0015()
        : allocator(GetDefaultAllocator()), field_0x8(5), count(0), buckets(5), first(nullptr), last(nullptr) {
        buckets.Resize(11);
    }

    // Drops every node's object, frees the nodes and empties the buckets (keeping them).
    void Clear() {
        if (count) {
            UnkPbNode_ovb0015* node = last;
            while (node) {
                UnkPbNode_ovb0015* next = node->next;
                if (UnkPbCounted_ovb0015* o = node->object) {
                    if (--o->ref_count == 0)
                        o->Release();
                }
                allocator->vf_0x48(node);
                node = next;
            }
            __builtin_memset(buckets.data, 0, buckets.capacity * u64(8));
            count = 0;
            first = nullptr;
            last = nullptr;
        }
    }
};
static_assert(sizeof(UnkPbMap_ovb0015) == 0x48, "UnkPbMap_ovb0015 layout");

// The X parasites (fn_71006e9fb4's list, as src/game/UnkTU2553/UnkTU2553_Manager_ovb0205.cpp names it).
struct UnkTgmHolder_b205;
struct UnkTgmRef_b205 : base::UnkRefCounted {
    u8 pad_0xc[4];
    UnkTgmHolder_b205* target;
};
struct UnkTgmRefVector_b205 {
    UnkTgmRef_b205** data;
    u32 count;
};
UnkTgmRefVector_b205* fn_71006e9fb4();

struct UnkXParasiteAI;
struct UnkVector3_b55;
bool CXParasiteAIComponent_ReadIsBeingAttractedAndCheckAvailable(UnkXParasiteAI* self);
void XParasiteAI_AttractToPowerBomb(UnkXParasiteAI* self, UnkVector3_b55* position);
void fn_71006eb6fc(UnkXParasiteAI* self);  // lets the parasite go

void fn_7100e2f030(void* manager, void* actor);
void fn_7100e2eef8(void* manager, UnkVec3f center, f32 radius);

struct UnkPbMovement_lab13c8;
void fn_7100ab13c8(UnkPbMovement_lab13c8* self, const UnkVec3f& position, f32 size);

// The power bomb's tunables.
struct UnkPbTunables_lab0ab0 {
    u8 pad_0x0[0x24];
    f32 field_0x24;  // how long the owner stays after the blast
    f32 field_0x28;  // the full radius
    f32 field_0x2c;  // the growth speed
    f32 field_0x30;  // the shrink speed
};
extern UnkPbTunables_lab0ab0 g_unk_7172754138;

struct UnkPbPos_lab0ab0 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

// The view of an X parasite's AI component this file needs.
struct UnkPbParasite_lab0ab0 {
    u8 pad_0x0[0x18];
    UnkPbPos_lab0ab0* actor;  // 0x18
    u8 pad_0x20[0x146d8 - 0x20];
    f32 spread_x;  // 0x146d8
    f32 spread_y;  // 0x146dc
};

struct UnkPbMovement_lab0ab0 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkPbPos_lab0ab0* owner;  // 0x30
    u8 pad_0x38[0x494 - 0x38];
    f32 field_0x494;
    u8 pad_0x498[0x4b8 - 0x498];
    bool field_0x4b8;  // not gone off yet
    bool field_0x4b9;  // the blast still grows
    u8 pad_0x4ba[0x2];
    f32 radius;  // 0x4bc
    f32 field_0x4c0;
    f32 field_0x4c4;  // the eased radius
    bool field_0x4c8;  // done
    u8 pad_0x4c9[0x3];
    f32 field_0x4cc;  // the time since it was done
    u8 pad_0x4d0[0x20];
    UnkPbMap_ovb0015 map;  // 0x4f0

    // vtable slot 0x2f8: hurts what is in `size` around `position`.
    void vf_0x2f8(u32 kind, const UnkVec3f* position, void* source, u32 a, bool b, u64 c, u64 d, f32 size) {
        typedef void (*Fn)(UnkPbMovement_lab0ab0*, u32, const UnkVec3f*, void*, u32, bool, u64, u64, f32);
        reinterpret_cast<Fn>(vtable[0x2f8 / 8])(this, kind, position, source, a, b, c, d, size);
    }
};

void fn_7100ab0fdc(UnkPbMovement_lab0ab0* self, f32 radius);
void fn_7100aaa1a0(UnkPbMovement_lab0ab0* self, f32 dt);

static inline void* HitManager() {
    void* instance = g_unk_71726eb3e8;
    if (!instance) {
        instance = fn_71000839a0(0xa0f8, 0x3f);
        fn_7100e211cc(instance);
        g_unk_71726eb3e8 = instance;
    }
    return instance;
}

static inline f32 RandomRange(f32 min, f32 max) {
    f32 f = (f32)((f64)(rand() & 0x7fff) * (1.0 / 32767.0));
    return (max - min) * f + min;
}

struct UnkPbVec2_lab0ab0 {
    f32 x;
    f32 y;
};

static inline UnkPbVec2_lab0ab0 operator+(const UnkPbVec2_lab0ab0& a, const UnkPbVec2_lab0ab0& b) {
    return {a.x + b.x, a.y + b.y};
}

static inline UnkPbParasite_lab0ab0* Parasite(UnkTgmRef_b205* ref) {
    return ref ? reinterpret_cast<UnkPbParasite_lab0ab0*>(ref->target) : nullptr;
}

void fn_7100ab0ab0(UnkPbMovement_lab0ab0* self, f32 dt) {
    const UnkPbTunables_lab0ab0& tunables = g_unk_7172754138;
    if (self->field_0x4c8) {
        fn_7100e2f030(HitManager(), self->owner);
        if (self->field_0x4cc >= tunables.field_0x24)
            fn_7100232c68(self->owner);
        self->field_0x4cc += dt;
        return;
    }
    if (self->field_0x4b8) {
        if (self->field_0x494 == 0.0f) {
            self->map.Clear();
            fn_7100ab0fdc(self, tunables.field_0x28);
        }
        fn_7100aaa1a0(self, dt);
        return;
    }
    bool growing = self->field_0x4b9;
    self->field_0x494 += dt;
    if (growing) {
        self->radius += tunables.field_0x2c * dt;
        if (self->radius > tunables.field_0x28) {
            self->field_0x4b9 = false;
            void* fx = g_unk_7101d47310->field_0x1040 + 0x38;
            base::math::CVector3D white;
            white.x = 1.0f;
            white.y = 1.0f;
            white.z = 1.0f;
            fn_7101053c14(fx, &white, 0.0f, 0.5f, 0.15f);
        }
        f32 t = self->radius / tunables.field_0x28;
        {
            base::global::CRntString curve("aaaaqq");
            t = fn_7100076020(t, curve.data ? curve.data : g_unk_710162e2ed);
        }
        self->field_0x4c4 = t * self->radius;
        fn_7100ab0fdc(self, self->field_0x4c4);
        UnkPbPos_lab0ab0* owner = self->owner;
        UnkVec3f pos(owner->x, owner->y, owner->z);
        self->vf_0x2f8(0x1480, &pos, nullptr, 0x2c0, true, 0, 0, self->field_0x4c4);
        owner = self->owner;
        pos = UnkVec3f(owner->x, owner->y, owner->z);
        fn_7100ab13c8(reinterpret_cast<UnkPbMovement_lab13c8*>(self), pos, self->field_0x4c4);
        return;
    }

    // Pull in the parasites outside the blast, to a random point above the bomb.
    UnkTgmRefVector_b205* parasites = fn_71006e9fb4();
    for (UnkTgmRef_b205** it = parasites->data, **end = it + parasites->count; it != end; ++it) {
        UnkPbParasite_lab0ab0* parasite = Parasite(*it);
        if (!CXParasiteAIComponent_ReadIsBeingAttractedAndCheckAvailable(reinterpret_cast<UnkXParasiteAI*>(parasite)))
            continue;
        UnkPbPos_lab0ab0* owner = self->owner;
        UnkPbPos_lab0ab0* actor = parasite->actor;
        f32 ox = owner->x, oy = owner->y, oz = owner->z;
        f32 tx = actor->x, ty = actor->y, tz = actor->z;
        f32 dx = ox - tx;
        f32 dy = oy - ty;
        f32 dz = oz - tz;
        if (dx * dx + dy * dy + dz * dz > self->radius * self->radius) {
            UnkPbVec2_lab0ab0 point = UnkPbVec2_lab0ab0{ox, oy} + UnkPbVec2_lab0ab0{0.0f, parasite->spread_y};
            point = point + UnkPbVec2_lab0ab0{RandomRange(-parasite->spread_x, parasite->spread_x), 0.0f};
            point = point + UnkPbVec2_lab0ab0{0.0f, RandomRange(0.0f, parasite->spread_x)};
            UnkVec3f target(point.x, point.y, 0.0f);
            XParasiteAI_AttractToPowerBomb(reinterpret_cast<UnkXParasiteAI*>(parasite),
                                           reinterpret_cast<UnkVector3_b55*>(&target));
        }
    }
    fn_7100e2eef8(HitManager(), UnkVec3f(self->owner->x, self->owner->y, 0.0f), fabsf(self->radius));
    self->radius -= tunables.field_0x30 * dt;
    if (self->radius <= 0.0f)
        self->radius = 0.0f;
    else
        self->radius = self->radius >= tunables.field_0x28 ? tunables.field_0x28 : self->radius;
    if (self->radius < 0.0f) {
        parasites = fn_71006e9fb4();
        for (UnkTgmRef_b205** it = parasites->data, **end = it + parasites->count; it != end; ++it) {
            UnkPbParasite_lab0ab0* parasite = Parasite(*it);
            if (!parasite)
                continue;
            UnkReflObject* component = reinterpret_cast<UnkReflObject*>(parasite);
            if (fn_710009d50c(component->vf_0x0(), fn_71006e5f28()))
                fn_71006eb6fc(reinterpret_cast<UnkXParasiteAI*>(parasite));
        }
    }
}

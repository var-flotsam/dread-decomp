// CPowerBombMovement's unit (tools/tu_map.py): a bomb movement (vtable g_unk_7101a95348) built on the
// bomb movement's base (fn_7100aa9ee4, src/game/CBombMovement/CBombMovement_ctor.cpp) with a listener at
// +0x4d0 (vtable g_unk_7101a77000) and a chained hash map at +0x4f0 whose nodes hold counted objects:
// its constructor, destructor and the reset that empties the map and flashes the screen. Also the
// reset of another movement of the unit that turns its owner's colliders off but one. Names and
// layouts are placeholders.
#include <new>

#include "base/UnkHashMap.h"
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "game/CBombMovement.h"
#include "game/CComponent.h"
#include "game/CPullOffGrapplePointComponent.h"
#include "game/CTimelineComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2656.h"
#include "types.h"

extern void* g_unk_7101a95348[];  // the movement's vtable

struct UnkBombMovement;
void fn_7100aa9ee4(UnkBombMovement* self);  // the base constructor
struct UnkPbMovementBase_ovb0015;
struct UnkHandleOwner_71010c4230;

// A counted object (count at +0x8, released through slot 0x10); the listener's link has its owner
// at +0x10.
struct UnkPbCounted_ovb0015 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;           // 0x8
    void* owner;             // 0x10
};

// What the map's nodes hold at +0x8.
struct UnkPbCountedRef_ovb0015 {
    UnkPbCounted_ovb0015* ptr;
    ~UnkPbCountedRef_ovb0015() {
        if (ptr && --ptr->ref_count == 0)
            ptr->Release();
    }
};

struct UnkPbNode_ovb0015 {
    u8 pad_0x0[0x8];
    UnkPbCounted_ovb0015* object;  // 0x8
    UnkPbNode_ovb0015* next;       // 0x10
};

struct UnkPbListener_ovb0015 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    UnkPbCounted_ovb0015* link;  // 0x18
};

// The map's bucket array (base/UnkVector.h's layout).
struct UnkPbBuckets_ovb0015 {
    void** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkPbBuckets_ovb0015(u32 flags_)
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(flags_),
          allocator(fn_7100083990()) {}

    // Resizes to `n` buckets, the new ones empty.
    void Resize(u32 n) {
        u32 old = count;
        if (old < n) {
            void** p;
            if (capacity >= n) {
                p = data;
            } else {
                p = static_cast<void**>(allocator->vf_0x40((u64)n * sizeof(void*), 8, flags));
                if (data) {
                    __builtin_memcpy(p, data, (u64)count * sizeof(void*));
                    allocator->vf_0x48(data);
                }
                capacity = n;
                old = count;
                data = p;
            }
            __builtin_memset(p + old, 0, (u64)(n - old) * sizeof(void*));
        } else if (old == n) {
            return;
        }
        count = n;
    }
};

// The chained hash map (the layout of base/UnkHashMap.h's): 11 buckets to start with.
struct UnkPbMap_ovb0015 {
    base::UnkAllocator* allocator;  // 0x0: the nodes'
    u32 field_0x8;
    u32 count;                     // 0xc
    UnkPbBuckets_ovb0015 buckets;  // 0x10
    UnkPbNode_ovb0015* first;      // 0x38
    UnkPbNode_ovb0015* last;       // 0x40: where the walk starts

    UnkPbMap_ovb0015()
        : allocator(fn_7100083990()), field_0x8(5), count(0), buckets(5), first(nullptr), last(nullptr) {
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

struct UnkPbMovement_ovb0015 {
    void** vtable;
    u8 pad_0x8[0x4b0];
    bool field_0x4b8;
    bool field_0x4b9;
    u8 pad_0x4ba[0x2];
    f32 field_0x4bc;
    f32 field_0x4c0;
    u32 field_0x4c4;
    bool field_0x4c8;
    u8 pad_0x4c9[0x3];
    u32 field_0x4cc;
    UnkPbListener_ovb0015 listener;  // 0x4d0
    UnkPbMap_ovb0015 map;            // 0x4f0
};
static_assert(__builtin_offsetof(UnkPbMovement_ovb0015, map) == 0x4f0, "UnkPbMovement_ovb0015 layout");

// The constructor.
__attribute__((flatten)) void fn_7100ab0670(UnkPbMovement_ovb0015* self) {
    fn_7100aa9ee4(reinterpret_cast<UnkBombMovement*>(self));
    self->field_0x4b8 = true;
    self->field_0x4b9 = true;
    self->vtable = g_unk_7101a95348 + 2;
    self->field_0x4bc = 0.0f;
    self->field_0x4c0 = 75.0f;
    self->field_0x4c4 = 0;
    self->field_0x4c8 = false;
    self->field_0x4cc = 0;
    self->listener.vtable = g_unk_7101a77000 + 2;
    __builtin_memset(&self->listener.field_0x8, 0, 0x18);
    new (&self->map) UnkPbMap_ovb0015();
}

// The destructor: the map is emptied and its buckets freed, the listener detaches itself from its
// link's owner (fn_7100a2d4d0) and drops the link, then the base destructor runs.
void fn_7100ab07c8(UnkPbMovement_ovb0015* self) {
    self->vtable = g_unk_7101a95348 + 2;
    base::refl_HashMapClear<UnkPbCountedRef_ovb0015, 8, 0x10>(reinterpret_cast<char*>(&self->map));
    UnkPbListener_ovb0015* l = &self->listener;
    l->vtable = g_unk_7101a77000 + 2;
    if (UnkPbCounted_ovb0015* link = l->link) {
        bool held = true;
        if (void* owner = link->owner) {
            fn_7100a2d4d0(owner, l);
            link = l->link;
            held = link != nullptr;
        }
        if (held && --link->ref_count == 0)
            link->Release();
    }
    fn_7100aa9f3c(reinterpret_cast<UnkPbMovementBase_ovb0015*>(self));
}

// A reset: the handles (fn_71010c4230), the map emptied, then a white flash of 0.15.
void fn_7100ab0944(UnkPbMovement_ovb0015* self) {
    fn_71010c4230(reinterpret_cast<UnkHandleOwner_71010c4230*>(self));
    self->map.Clear();
    void* fx = g_unk_7101d47310->field_0x1040 + 0x38;
    base::math::CVector3D white;
    white.x = 1.0f;
    white.y = 1.0f;
    white.z = 1.0f;
    fn_7101053c14(fx, &white, 0.0f, 0.0f, 0.15f);
}

// ---- Another movement's reset ----

// As src/game/CCollisionComponent/fn_71008651c8.cpp declares it.
struct UnkStruct_71008651c8_Entry;
struct UnkStruct_71008651c8;
using UnkOut_71008651c8 = base::UnkVector<UnkStruct_71008651c8_Entry*>;
void fn_71008651c8(UnkStruct_71008651c8* self, void* const* key, UnkOut_71008651c8* out);

extern void* const g_unk_7172753e28 __attribute__((visibility("hidden")));            // the colliders' key
extern base::global::CStrId g_unk_7172753e30 __attribute__((visibility("hidden")));  // the one kept on

struct UnkPbMovementBase3_ovb0015;
void fn_7100aaa0c4(UnkPbMovementBase3_ovb0015* self);  // the base reset

struct UnkPbOwner_ovb0015 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* field_0x2a0;
};

struct UnkPbMovement3_ovb0015 {
    u8 pad_0x0[0x30];
    UnkPbOwner_ovb0015* owner;
    u8 pad_0x38[0x4b8];
    u16 field_0x4f0;
};
static_assert(__builtin_offsetof(UnkPbMovement3_ovb0015, field_0x4f0) == 0x4f0, "UnkPbMovement3_ovb0015 layout");

// The base reset, then, when the owner has its collision container, every collider found under
// g_unk_7172753e28 turned off and the one named g_unk_7172753e30 turned on (the same steps as
// fn_71005c2500 in src/game/CHydrogigaAIComponent/CHydrogigaAIComponent_Send_b59.cpp); +0x4f0 cleared.
void fn_7100aae6a8(UnkPbMovement3_ovb0015* self) {
    fn_7100aaa0c4(reinterpret_cast<UnkPbMovementBase3_ovb0015*>(self));
    if (UnkCollisionContainer* container = self->owner->field_0x2a0) {
        UnkOut_71008651c8 found(0xf);
        fn_71008651c8(reinterpret_cast<UnkStruct_71008651c8*>(container), &g_unk_7172753e28, &found);
        for (UnkStruct_71008651c8_Entry **it = found.data, **end = found.data + found.count; it != end; ++it)
            fn_7100d35a98(*reinterpret_cast<UnkEntry**>(reinterpret_cast<char*>(*it) + 0x28), false);
        fn_7100d35a98(reinterpret_cast<UnkEntry*>(fn_71008640a4(container, &g_unk_7172753e30)), true);
        found.Free();
    }
    self->field_0x4f0 = 0;
}

// Unit 270 (tools/tu_map.py: 0x71001ea860-0x71001ec8bc, initialiser fn_71001ec830): a pool of 25000
// placed entries (0xe0 bytes each: a transform, a sphere (game/UnkSphere.h) and a box), with a slot
// table that points at them; fn_71001eaf3c (src/game_s1/fn_71001eaf3c.cpp) places one entry.
// Holds fn_71001eaa28 (constructor), fn_71001eadd8 (reset) and fn_71001eb7f0 (a box around a list
// of nodes' points). Names and layouts are placeholders.
#include <new>

#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "game/UnkSphere.h"
#include "types.h"

void fn_710021685c(void* self);  // UnkSphere's constructor

// A box with virtual accessors (0x20 bytes): its minimum and maximum corners.
struct UnkPoolBox {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void SetMin(const UnkVec3f* v);  // vf_0x28
    virtual void SetMax(const UnkVec3f* v);  // vf_0x30
    virtual UnkVec3f GetMin();               // vf_0x38
    virtual UnkVec3f GetMax();               // vf_0x40
    UnkVec3f min;
    UnkVec3f max;
};

// The box's base constructor, and the vtable of the box class the pool uses (GOT 0x7101cc7410).
void fn_7100215dcc(UnkPoolBox* self, const UnkVec3f* min, const UnkVec3f* max);
extern const char g_unk_7101a20090[];
// The pool's vtable (GOT 0x7101cc75c0).
extern char g_unk_7101a1eeb0[];

// One pool entry (0xe0 bytes): a transform, a sphere, a box and the box quantised to tenths.
struct UnkSpherePoolEntry {
    u64 field_0x0;
    UnkMatrix44 matrix;
    UnkSphere sphere;  // 0x48
    UnkPoolBox box;    // 0x98
    s32 cellMin[3];    // 0xb8
    s32 cellMax[3];    // 0xc4
    s64 field_0xd0;
    u32 field_0xd8;
    u32 field_0xdc;
};
static_assert(sizeof(UnkSpherePoolEntry) == 0xe0, "UnkSpherePoolEntry layout");

constexpr u32 kSpherePoolSize = 25000;

static inline void SetIdentity(UnkMatrix44* m) {
    *m = UnkMatrix44{};
    m->m[15] = 1.0f;
    m->m[10] = 1.0f;
    m->m[5] = 1.0f;
    m->m[0] = 1.0f;
}

struct UnkSpherePool {
    void* vtable;
    UnkSpherePoolEntry* slots[kSpherePoolSize];     // 0x8
    UnkSpherePoolEntry entries[kSpherePoolSize];    // 0x30d48
    u8 unk_0x588048[0x5e9ac8 - 0x588048];
    void* field_0x5e9ac8;  // 25000 words
    u8 unk_0x5e9ad0[0x5e9b70 - 0x5e9ad0];
    u32 field_0x5e9b70;
    u32 field_0x5e9b74;
    u32 field_0x5e9b78;
    u32 field_0x5e9b7c;
    u32 field_0x5e9b80;
    u16 field_0x5e9b84;
    bool field_0x5e9b86;
    bool field_0x5e9b87;
    base::UnkVector<void*> field_0x5e9b88;
};
static_assert(sizeof(UnkSpherePool) == 0x5e9bb0, "UnkSpherePool layout");

// An entry's constructor.
static inline void InitEntry(UnkSpherePoolEntry* e) {
    SetIdentity(&e->matrix);
    fn_710021685c(&e->sphere);
    UnkPoolBox* box = &e->box;
    {
        UnkVec3f min(0.0f, 0.0f, 0.0f);
        UnkVec3f max(0.0f, 0.0f, 0.0f);
        fn_7100215dcc(box, &min, &max);
    }
    *reinterpret_cast<const char**>(box) = g_unk_7101a20090 + 0x10;
    UnkVec3f min = box->GetMin();
    UnkVec3f max = box->GetMax();
    e->cellMin[0] = (s32)(min.x * 10.0f);
    e->cellMin[1] = (s32)(min.y * 10.0f);
    e->cellMin[2] = (s32)(min.z * 10.0f);
    e->cellMax[0] = (s32)(max.x * 10.0f);
    e->cellMax[1] = (s32)(max.y * 10.0f);
    e->cellMax[2] = (s32)(max.z * 10.0f);
    e->field_0xdc = 0;
    e->field_0x0 = 0;
    e->field_0xd0 = -1;
}

// The constructor: every entry gets an identity transform, a new sphere and an empty box (its
// quantised corners from the box's own accessors), then the bookkeeping fields and the word buffer.
// NON_MATCHING(minor): register allocation and scheduling of the loop preheader differ: the
// loop-invariant field addresses are computed in another order and kept in other registers (the
// original loads the box vtable's GetMin slot before storing the pool's vtable), and the stores of 0
// at +0x0 and -1 at +0xd0 both use the entry base (one add) instead of a register-offset store and
// the +0xdc address. Otherwise the loop body differs only in register numbers.
void fn_71001eaa28(UnkSpherePool* self) {
    self->vtable = g_unk_7101a1eeb0 + 0x10;
    for (u32 i = 0; i < kSpherePoolSize; ++i)
        InitEntry(&self->entries[i]);
    self->field_0x5e9ac8 = nullptr;
    self->field_0x5e9b84 = 0;
    self->field_0x5e9b70 = 0;
    self->field_0x5e9b74 = 0;
    self->field_0x5e9b78 = 0;
    self->field_0x5e9b7c = 0;
    self->field_0x5e9b86 = true;
    new (&self->field_0x5e9b88) base::UnkVector<void*>(7);
    self->field_0x5e9ac8 = fn_7100083990()->vf_0x40(kSpherePoolSize * sizeof(void*), 8, 0x42);
}

// Clears the counters and points every slot at its own entry again.
void fn_71001eadd8(UnkSpherePool* self) {
    self->field_0x5e9b87 = false;
    self->field_0x5e9b74 = 0;
    self->field_0x5e9b78 = 0;
    self->field_0x5e9b7c = 0;
    self->field_0x5e9b80 = 0;
    for (u32 i = 0; i < kSpherePoolSize; ++i)
        self->slots[i] = &self->entries[i];
}

// Extends `box` to hold `p`.
static inline void Extend(UnkPoolBox* box, const UnkVec3f& p) {
    {
        UnkVec3f m;
        UnkVec3f cur = box->GetMin();
        m.x = cur.x < p.x ? cur.x : p.x;
        m.y = cur.y < p.y ? cur.y : p.y;
        m.z = cur.z < p.z ? cur.z : p.z;
        box->SetMin(&m);
    }
    {
        UnkVec3f m;
        UnkVec3f cur = box->GetMax();
        m.x = cur.x > p.x ? cur.x : p.x;
        m.y = cur.y > p.y ? cur.y : p.y;
        m.z = cur.z > p.z ? cur.z : p.z;
        box->SetMax(&m);
    }
}

// A node whose bounds are its origin and seven more points.
struct UnkBoundsNode {
    u8 unk_0x0[0xa4];
    UnkVec3f origin;     // 0xa4
    UnkVec3f points[7];  // 0xb0
};

struct UnkBoundsItem {
    UnkBoundsNode* node;
    u8 unk_0x8[0x18];
};

struct UnkBoundsList {
    u8 unk_0x0[0x34];
    u32 count;
    UnkBoundsItem* items;
};

// Sets `box` to the bounds of every point of every node in `list` (left as it is when the list is
// empty). The first parameter is unused.
void fn_71001eb7f0(void*, UnkBoundsList* list, UnkPoolBox* box) {
    u32 count = list->count;
    u64 i = 0;
    bool extend = false;
    for (; i != count; ++i) {
        UnkBoundsNode* node = list->items[i].node;
        if (extend) {
            Extend(box, node->origin);
        } else {
            box->SetMin(&node->origin);
            box->SetMax(&node->origin);
        }
        for (u32 k = 0; k < 7; ++k)
            Extend(box, node->points[k]);
        extend = true;
    }
}

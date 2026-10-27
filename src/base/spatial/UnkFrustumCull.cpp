// From unit 244 of tools/tu_map.py (0x71001b76ec-0x71001bb60c, no reflected class): a view
// frustum's six planes (at +0x178 of the view object) tested against a bounding sphere. The sphere
// remembers the plane that last rejected it and the test starts there, walking the planes in the
// order a next-plane table gives. Names and layouts are placeholders.
// Holds fn_71001bab30.
#include "types.h"

// A plane: its normal and its distance term.
struct UnkPlane {
    f32 x;
    f32 y;
    f32 z;
    f32 d;
};

struct UnkFrustumView {
    u8 pad_0x0[0x178];
    UnkPlane planes[6];
};

// A plane's index (0..5). An enum type: its stores don't alias the sphere's floats.
enum class UnkPlaneIndex : u8 {};

struct UnkBoundSphere {
    u8 pad_0x0[0x8];
    f32 radius;
    u8 pad_0xc[0xc];
    f32 center_x;
    f32 center_y;
    f32 center_z;
    u8 pad_0x24[0x29];
    UnkPlaneIndex plane;  // the plane the test starts with
};
static_assert(__builtin_offsetof(UnkBoundSphere, plane) == 0x4d, "UnkBoundSphere placeholder layout");

// The plane tested after each plane.
extern const u8 g_unk_71015e9322[6] __attribute__((visibility("hidden")));

// 1 when the sphere is outside a plane, -1 when it is inside all six (or too small to test),
// 0 when it crosses one. The sphere's start plane moves on with every plane tested.
// NON_MATCHING(minor): the original computes the fifth plane's inside test (cset) first; the same
// six compares and ands, scheduled in another order.
s32 fn_71001bab30(const UnkFrustumView* self, UnkBoundSphere* sphere) {
    f32 r = sphere->radius;
    if (r < 0.0001f)
        return -1;
    u32 index = (u32)sphere->plane;
    bool inside = true;
    for (u32 i = 0; i < 6; ++i) {
        const UnkPlane& p = self->planes[index];
        f32 dist = p.x * sphere->center_x + p.y * sphere->center_y + p.z * sphere->center_z + p.d;
        if (dist > sphere->radius)
            return 1;
        if (dist > -r)
            inside = false;
        index = g_unk_71015e9322[index];
        sphere->plane = (UnkPlaneIndex)index;
    }
    return inside ? -1 : 0;
}

// ---- The view's cull object (fn_71001b7a60) ----

#include "base/UnkVector.h"
#include "game/UnkSphere.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

// The registry every view's cull object is entered in: a chained hash set of pointers at +8 (the
// layout of base/UnkHashMap.h: buckets indexed by the key's low 32 bits modulo the capacity, and a
// list of all nodes). src/game_lo/fn_710019e100.cpp removes a key from it.
struct UnkKeyOwner;
extern void* g_unk_716e34c2e0;
bool fn_710019e100(UnkKeyOwner* self, u64 key);

struct UnkCullNode {
    u64 key;
    u64 value;
    UnkCullNode* next;   // the list of all nodes
    UnkCullNode* prev;
    UnkCullNode* chain;  // the next node in the bucket

    explicit UnkCullNode(u64 k) : key(k), value(k), next(nullptr), prev(nullptr), chain(nullptr) {}

    // The list links, through accessors (their stores may alias the registry's list ends).
    UnkCullNode*& Next() { return next; }
    UnkCullNode*& Prev() { return prev; }
};

struct UnkCullRegistry {
    u8 pad_0x0[0x8];
    base::UnkAllocator* allocator;  // +0x8
    u32 flags;                      // +0x10
    u32 count;                      // +0x14
    UnkCullNode** buckets;          // +0x18 (base::global::CRntVector)
    u32 bucket_count;               // +0x20
    u32 bucket_capacity;            // +0x24
    u8 pad_0x28[0x18];
    UnkCullNode* tail;  // +0x40
    UnkCullNode* head;  // +0x48

    // Adds a node for `key` at the front of its bucket and at the end of the list. An empty
    // bucket array gives a null bucket, which the original dereferences all the same.
    void Insert(u64 key) {
        UnkCullNode** bucket = nullptr;
        if (bucket_count != 0) {
            u32 i = (u32)key % bucket_capacity;
            if (bucket_count <= i)
                __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
            bucket = &buckets[i];
        }
        UnkCullNode* old = *bucket;
        auto* node = static_cast<UnkCullNode*>(allocator->vf_0x40(sizeof(UnkCullNode), 8, flags));
        if (node)
            new (node) UnkCullNode(key);
        node->chain = old;
        *bucket = node;
        if (!head)
            head = node;
        else if (tail) {
            node->Prev() = tail;
            tail->Next() = node;
        }
        tail = node;
        ++count;
    }
};

// The 0xb8-byte cull object.
struct UnkCullObject;
void fn_71001691ac(UnkCullObject* self);  // its constructor

// ---- The view's constructor (fn_71001b76ec) ----

// A box with virtual accessors (0x20 bytes): its minimum and maximum corners. The same class as
// src/game/UnkSpherePool.cpp's (its base constructor and vtable are shared).
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

// The box's base constructor, and the vtable of the box class (GOT 0x7101cc7410).
void fn_7100215dcc(UnkPoolBox* self, const UnkVec3f* min, const UnkVec3f* max);
extern const char g_unk_7101a20090[];

// A box and its corners quantised to tenths (0x38 bytes).
struct UnkViewBox {
    UnkPoolBox box;
    s32 cellMin[3];  // 0x20
    s32 cellMax[3];  // 0x2c
};
static_assert(sizeof(UnkViewBox) == 0x38, "UnkViewBox layout");

void fn_710021685c(void* self);  // UnkSphere's constructor
// Constructors of three small members (other units).
void fn_710016931c(void* self);  // +0x128 (0x24 bytes)
void fn_71001695ec(void* self);  // +0x14c (0x10 bytes)
void fn_710016947c(void* self);  // +0x15c (0x14 bytes)

extern "C" f32 sinf(f32);
extern "C" f32 cosf(f32);

// The view object (UnkFrustumView above: the frustum planes at +0x178). The defaults suggest a
// camera: a 45-degree field of view at +0x80, near/far-like distances (100, 100000, 1000) and a
// 1280x720 viewport at +0xa0.
struct UnkCullView {
    u32 field_0x0;
    u32 field_0x4;
    s32 field_0x8;
    u32 field_0xc;
    UnkCullView* field_0x10;
    u32 field_0x18;
    u32 field_0x1c;
    UnkSphere sphere;  // 0x20
    bool field_0x70;
    f32 field_0x74;
    f32 field_0x78;
    f32 field_0x7c;
    f32 fov;  // 0x80, degrees
    f32 field_0x84;
    f32 field_0x88;
    f32 field_0x8c;
    f32 field_0x90;
    f32 field_0x94;
    f32 field_0x98;
    f32 field_0x9c;
    u32 width;   // 0xa0
    u32 height;  // 0xa4
    u64 field_0xa8;
    u64 field_0xb0;
    u32 field_0xb8;
    f32 field_0xbc;
    f32 field_0xc0;
    f32 field_0xc4;
    f32 field_0xc8;
    f32 field_0xcc;
    f32 field_0xd0;
    f32 field_0xd4;
    f32 field_0xd8;
    f32 field_0xdc;
    f32 field_0xe0;
    f32 field_0xe4[14];  // 0xe4
    u32 field_0x11c;
    f32 field_0x120;
    f32 field_0x124;
    u8 field_0x128[0x24];
    u8 field_0x14c[0x10];
    u8 field_0x15c[0x14];
    UnkCullObject* cull;  // 0x170, entered in g_unk_716e34c2e0
    UnkPlane planes[6];   // 0x178
    u8 pad_0x1d8[0x90];
    f32 sinHalfFov;     // 0x268
    f32 cosHalfFov;     // 0x26c
    f32 invCosHalfFov;  // 0x270
    f32 cosHalfFovSq;   // 0x274
    f32 tanHalfFov;     // 0x278
    u16 field_0x27c;
    u8 field_0x27e;
    void* field_0x280[12];
    bool field_0x2e0;
    UnkViewBox boxes[9];  // 0x2e8
};
static_assert(__builtin_offsetof(UnkCullView, cull) == 0x170, "UnkCullView layout");
static_assert(__builtin_offsetof(UnkCullView, sinHalfFov) == 0x268, "UnkCullView layout");
static_assert(sizeof(UnkCullView) == 0x4e0, "UnkCullView layout");

// The constructor: defaults, the sphere and three members, zeroed planes, nine empty boxes with
// their quantised corners, then the half field of view's sine, cosine and tangent.
// NON_MATCHING(minor): the fourteen 8-byte zero stores to +0x268..+0x2d8 (before the first box
// is built) come out in descending address order where the original has them mostly ascending,
// with their address registers (x8/x9/x10) assigned differently; same stores, nothing between them.
void fn_71001b76ec(UnkCullView* self) {
    self->field_0x0 = 0;
    self->field_0x4 = 0;
    self->field_0x8 = -1;
    self->field_0x18 = 0;
    self->field_0x10 = nullptr;
    fn_710021685c(&self->sphere);
    self->field_0x70 = false;
    self->field_0x74 = 1000.0f;
    self->field_0x78 = 0.0f;
    self->field_0x7c = 0.0f;
    self->fov = 45.0f;
    self->field_0x84 = 45.0f;
    self->field_0x88 = 100000.0f;
    self->field_0x8c = 100.0f;
    self->field_0x90 = 0.0f;
    self->field_0x94 = 1.0f;
    self->field_0x98 = 1.0f;
    self->field_0x9c = 1000.0f;
    self->field_0xa8 = 0;
    self->field_0xb0 = 0;
    self->field_0xb8 = 0;
    self->field_0xbc = 1.0f;
    self->field_0xc0 = 0.0f;
    self->field_0xc4 = 1.0f;
    self->field_0xc8 = 0.0f;
    self->field_0xcc = 1.0f;
    self->field_0xd0 = 0.0f;
    self->field_0xd4 = 0.0f;
    self->field_0xd8 = 0.0f;
    self->field_0xdc = 0.0f;
    self->field_0xe0 = 1.0f;
    for (f32& f : self->field_0xe4)
        f = 0.0f;
    self->field_0x11c = 0;
    self->field_0x120 = 1.0f;
    self->field_0x124 = 0.0f;
    fn_710016931c(self->field_0x128);
    fn_71001695ec(self->field_0x14c);
    fn_710016947c(self->field_0x15c);
    self->cull = nullptr;
    for (UnkPlane& p : self->planes) {
        p.x = 0.0f;
        p.y = 0.0f;
        p.z = 0.0f;
    }
    self->field_0x27c = 0;
    self->field_0x27e = 0;
    self->sinHalfFov = 0.0f;
    self->cosHalfFov = 0.0f;
    self->invCosHalfFov = 0.0f;
    self->cosHalfFovSq = 0.0f;
    for (void*& v : self->field_0x280)
        v = nullptr;
    self->field_0x2e0 = true;
    for (UnkViewBox& b : self->boxes) {
        UnkPoolBox* box = &b.box;
        {
            UnkVec3f min(0.0f, 0.0f, 0.0f);
            UnkVec3f max(0.0f, 0.0f, 0.0f);
            fn_7100215dcc(box, &min, &max);
        }
        *reinterpret_cast<const char**>(box) = g_unk_7101a20090 + 0x10;
        UnkVec3f min = box->GetMin();
        UnkVec3f max = box->GetMax();
        b.cellMin[0] = (s32)(min.x * 10.0f);
        b.cellMin[1] = (s32)(min.y * 10.0f);
        b.cellMin[2] = (s32)(min.z * 10.0f);
        b.cellMax[0] = (s32)(max.x * 10.0f);
        b.cellMax[1] = (s32)(max.y * 10.0f);
        b.cellMax[2] = (s32)(max.z * 10.0f);
    }
    self->field_0x0 = 8;
    self->field_0x4 = 1;
    f32 half = (f32)(self->fov * 3.141592653589793 / 180.0) * 0.5f;
    self->field_0x10 = self;
    self->sinHalfFov = sinf(half);
    f32 c = cosf(half);
    self->cosHalfFov = c;
    self->cosHalfFovSq = c * c;
    self->invCosHalfFov = 1.0f / c;
    self->width = 1280;
    self->height = 720;
    self->tanHalfFov = self->invCosHalfFov * self->sinHalfFov;
}

// Replaces the view's cull object with a new one, entered in the registry.
void fn_71001b7a60(UnkCullView* self) {
    if (self->cull) {
        fn_710019e100(static_cast<UnkKeyOwner*>(g_unk_716e34c2e0), (u64)self->cull);
        self->cull = nullptr;
    }
    auto* registry = static_cast<UnkCullRegistry*>(g_unk_716e34c2e0);
    auto* cull = static_cast<UnkCullObject*>(fn_71000839a0(0xb8, 0x34));
    fn_71001691ac(cull);
    if (cull)
        registry->Insert((u64)cull);
    self->cull = cull;
}

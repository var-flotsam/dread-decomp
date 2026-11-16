// CCollisionComponent unit (tools/tu_map.py unit 1319): tests a shape placed by a transform against the
// component's records whose object bounds overlap the shape's, stopping when the visitor asks; returns
// whether anything was hit. Names and layouts are placeholders.
#include "game/UnkTU365.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "types.h"

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

struct UnkVisitResult_b0040 {
    s32 value;
    UnkVisitResult_b0040(s32 v) : value(v) {}
    UnkVisitResult_b0040(const UnkVisitResult_b0040& other) = default;
    ~UnkVisitResult_b0040() {}
};

// The shared "skip" result, set up on first use.
static inline const UnkVisitResult_b0040& Skip() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

struct UnkCollisionOverlapQuery_ovb0107;
struct UnkCollisionShapeNode_ovb0107;
bool fn_7100865010(UnkCollisionOverlapQuery_ovb0107* self, UnkCollisionShapeNode_ovb0107* node);

// The query fn_7100865010 reads, with the shape's placement as a matrix.
struct UnkOverlapQuery_l864e34 {
    u32 mask;                     // 0x0
    void* filter;                 // 0x8
    UnkVisitResult_b0040 result;  // 0x10
    UnkShape* owner;              // 0x18
    UnkMtx44 transform;           // 0x20
    u16 flags;                    // 0x60
};

struct UnkCollisionRecord_l864e34 {
    u8 pad_0x0[0x28];
    UnkBvhCollider* object;  // 0x28
};
static_assert(sizeof(UnkCollisionRecord_l864e34) == 0x30, "record size");

struct UnkCollisionContainer_l864e34 {
    u8 pad_0x0[0x48];
    UnkCollisionRecord_l864e34* records;  // 0x48
    u32 recordCount;                      // 0x50

    UnkCollisionRecord_l864e34* begin() const { return records; }
    UnkCollisionRecord_l864e34* end() const { return records + recordCount; }
};

// Whether two boxes overlap: the x extents first, then the rest tested together.
static inline bool Overlaps(const UnkBox& a, const UnkBox& b) {
    return a.max.x >= b.min.x && (a.min.x <= b.max.x & a.max.y >= b.min.y & a.min.y <= b.max.y);
}

bool fn_7100864e34(UnkCollisionContainer_l864e34* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, void* filter,
                   u32 flags) {
    UnkMtx44 m = *xf * shape->transform;
    UnkBox bounds = shape->vf_0x28(m);
    UnkOverlapQuery_l864e34 query = {mask, filter, Skip(), shape, m, static_cast<u16>(flags)};
    for (UnkCollisionRecord_l864e34& record : *self) {
        UnkBvhCollider* object = record.object;
        if (object->boundsDirty)
            object->UpdateBounds();
        if (Overlaps(object->bounds, bounds)) {
            if (!fn_7100865010(reinterpret_cast<UnkCollisionOverlapQuery_ovb0107*>(&query),
                               reinterpret_cast<UnkCollisionShapeNode_ovb0107*>(record.object)))
                break;
        }
    }
    return query.result.value & 1;
}

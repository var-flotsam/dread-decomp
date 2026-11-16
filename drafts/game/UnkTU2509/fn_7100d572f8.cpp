// UnkTU2509 (the shared shape tester's unit): tests a point against a shape (polygon collections, type
// 0x10, through their tree at +0xa8; others through vf_0x50) and reports a hit to the visitor, or
// returns the "skip" result with bits 0 and 1 set when there is none. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

struct UnkVisitResult_b0040 {
    s32 value;
    UnkVisitResult_b0040(s32 v) : value(v) {}
    UnkVisitResult_b0040(const UnkVisitResult_b0040& other) = default;
    ~UnkVisitResult_b0040() {}
};

static inline UnkVisitResult_b0040 Skip() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

// What the visitor is told of a hit: the point and empty contact data.
struct UnkPointHit_ld572f8 {
    s32 id;
    CVector3D point;  // 0x4
    void* field_0x10;
    void* field_0x18;
    void* field_0x20;
    u32 field_0x28;
    u32 field_0x2c;

    explicit UnkPointHit_ld572f8(const CVector3D& p)
        : id(-1), point(p), field_0x10(nullptr), field_0x18(nullptr), field_0x20(nullptr), field_0x28(0),
          field_0x2c(0x38) {}
};

struct UnkPointVisitor_ld572f8;
struct UnkPointVisitorVtbl_ld572f8 {
    u8 pad_0x0[0x10];
    UnkVisitResult_b0040 (*vf_0x10)(UnkPointVisitor_ld572f8* self, const UnkPointHit_ld572f8* hit);
};
struct UnkPointVisitor_ld572f8 {
    UnkPointVisitorVtbl_ld572f8* vtbl;
};

struct UnkTestShape_ld572f8;
struct UnkTestShapeVtbl_ld572f8 {
    u8 pad_0x0[0x18];
    s32 (*vf_0x18)(UnkTestShape_ld572f8* self);  // the shape's type
    u8 pad_0x20[0x30];
    bool (*vf_0x50)(UnkTestShape_ld572f8* self, const CVector2D* pos);  // contains the point
};
struct UnkTestShape_ld572f8 {
    UnkTestShapeVtbl_ld572f8* vtbl;
    u8 pad_0x8[0xa0];
    u8 tree[0x8];  // 0xa8 (type 0x10)

    s32 Type() { return vtbl->vf_0x18(this); }
};

bool fn_71001247c8(void* tree, const CVector2D* pos);

// The shape as a polygon collection (type 0x10), else null.
static inline UnkTestShape_ld572f8* AsCollection(UnkTestShape_ld572f8* shape) {
    return shape && shape->Type() == 0x10 ? shape : nullptr;
}

static inline UnkVisitResult_b0040 Report(UnkVisitResult_b0040 result, const CVector2D* pos,
                                          const CVector3D* offset, UnkPointVisitor_ld572f8* visitor) {
    if (!visitor) {
        result.value |= 3;
        return result;
    }
    CVector3D point;
    point.x = pos->x + offset->x;
    point.y = pos->y + offset->y;
    point.z = 0.0f + offset->z;
    UnkPointHit_ld572f8 hit(point);
    return visitor->vtbl->vf_0x10(visitor, &hit);
}

// The unit's header declares the result UnkShapeHit_A5_b68; the original returns the visitor's result
// type (the "skip" global's). The last parameter is the visitor (callers pass none).
UnkVisitResult_b0040 fn_7100d572f8(UnkShapeTester_A5_b68* tester, void* shape_arg, const CVector2D* pos,
                                   const CVector3D* offset, u64 visitor_arg) {
    UnkPointVisitor_ld572f8* visitor = reinterpret_cast<UnkPointVisitor_ld572f8*>(visitor_arg);
    UnkTestShape_ld572f8* shape = static_cast<UnkTestShape_ld572f8*>(shape_arg);
    if (!shape)
        return Skip();
    UnkVisitResult_b0040 result = Skip();
    bool hit;
    if (shape->Type() == 0x10) {
        // Polygon collections test the point against their tree, from the skip result again.
        UnkTestShape_ld572f8* collection = AsCollection(shape);
        result = Skip();
        hit = fn_71001247c8(collection->tree, pos);
    } else {
        hit = shape->vtbl->vf_0x50(shape, pos);
    }
    if (hit)
        result = Report(result, pos, offset, visitor);
    return result;
}

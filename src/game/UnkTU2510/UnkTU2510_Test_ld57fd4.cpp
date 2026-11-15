// Unit 2510 (collision world): one collider against a placed shape. Nothing when the collider's bounds miss
// the shape's; otherwise the shape tester runs the test, through the caller's filter when one is given, and a
// hit on a breakable tile group's collider also reports the touched tiles. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"
#include "game/UnkTU2510.h"

using base::reflection::UnkReflObject;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

// The result of a query (as in UnkCollisionWorld.cpp): bit 0 a hit, bit 1 stop. Returned in memory: its copy is
// user-written. Also the result of the shape tester (fn_7100d51368).
struct UnkShapeTestResult_ovb0107 {
    union {
        u32 flags;
        u32 bits;
    };
    u32 field_0x4;
    UnkShapeTestResult_ovb0107() : bits(0) {}
    UnkShapeTestResult_ovb0107(u32 b) : bits(b) {}
    UnkShapeTestResult_ovb0107(const UnkShapeTestResult_ovb0107& o) : bits(o.bits) {}
};
using UnkColliderResult = UnkShapeTestResult_ovb0107;

// The "skip" result and its guard (shared with the other visitors).
struct UnkVisitResult_b0040 {
    s32 value;
    UnkVisitResult_b0040(s32 v) : value(v) {}
    UnkVisitResult_b0040(const UnkVisitResult_b0040& other) = default;
    ~UnkVisitResult_b0040() {}
};

static inline void InitSkip() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
}

struct UnkShape;

struct UnkShapeTester_A5_b68;

static inline UnkShapeTester_A5_b68* Tester() {
    UnkShapeTester_A5_b68* t = g_unk_71726a47a0;
    if (!t) {
        t = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(t);
        g_unk_71726a47a0 = t;
    }
    return t;
}

struct UnkCollider;

// The collider as this test reads it: its bounds, the object it belongs to (+0x10) and the owner the tester
// takes (+0x158).
struct UnkTestedCollider_d57fd4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void UpdateBounds();  // vf_0x98
    u8 pad_0x8[0x8];
    UnkReflObject* object;  // 0x10
    u8 pad_0x18[0x28];
    f32 minX;  // 0x40
    f32 minY;  // 0x44
    f32 maxX;  // 0x48
    f32 maxY;  // 0x4c
    u8 pad_0x50[0x10];
    bool dirty;  // 0x60
    u8 pad_0x61[0xf7];
    void* owner;  // 0x158
};


// The filter the tester calls back for each candidate (vtable g_unk_7101aa8898).
struct UnkShapeFilter_d57fd4 {
    void** vtable;
    UnkTestedCollider_d57fd4* node;
    u64 field_0x10;
    void* filter;
    bool field_0x20;
    UnkShapeFilter_d57fd4(UnkTestedCollider_d57fd4* n, void* f)
        : vtable(g_unk_7101aa8898 + 2), node(n), field_0x10(0), filter(f), field_0x20(false) {}
};

struct UnkTileGroup;
struct UnkTileQueryShape;
struct UnkTileVisitor;
bool fn_7100ca2978(UnkTileGroup* self, UnkTileQueryShape* shape, void* unused, UnkTileVisitor* visitor);

struct UnkTestBox_d57fd4 {
    f32 minX, minY, maxX, maxY;
};

UnkColliderResult fn_7100d57fd4(void* a, void* b, UnkCollider* collider, void* c, void* d, u32 e) {
    UnkTestedCollider_d57fd4* self = reinterpret_cast<UnkTestedCollider_d57fd4*>(collider);
    const UnkTestBox_d57fd4* box = static_cast<const UnkTestBox_d57fd4*>(c);
    if (self->dirty)
        self->UpdateBounds();
    if (self->maxX >= box->minX && self->minX <= box->maxX && self->maxY >= box->minY && self->minY <= box->maxY) {
        if (d) {
            UnkShapeFilter_d57fd4 filter(self, d);
            InitSkip();
            UnkColliderResult result(g_unk_71726a4778.value);
            UnkShapeTester_A5_b68* tester = Tester();
            result.bits = fn_7100d51368(tester, a, b, self->owner, fn_7100d36a2c(reinterpret_cast<UnkShape*>(self)),
                                        reinterpret_cast<UnkShapeFilter_ovb0107*>(&filter), e)
                              .bits;
            if (result.bits & 1) {
                // A breakable tile group also reports which of its tiles were touched.
                UnkReflObject* object = self->object;
                if (object && fn_710009d50c(object->vf_0x0(), fn_7100ca0484()))
                    fn_7100ca2978(reinterpret_cast<UnkTileGroup*>(object), static_cast<UnkTileQueryShape*>(a), b,
                                  reinterpret_cast<UnkTileVisitor*>(&filter));
            }
            return result;
        }
        UnkShapeTester_A5_b68* tester = Tester();
        return fn_7100d51368(tester, a, b, self->owner, fn_7100d36a2c(reinterpret_cast<UnkShape*>(self)), nullptr, e);
    }
    // Missed: nothing to report.
    InitSkip();
    return UnkColliderResult(g_unk_71726a4778.value);
}

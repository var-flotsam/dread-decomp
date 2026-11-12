// Collision world unit (unit 2510): walks a set of leaf entries, and for each enabled leaf whose layers match the
// mask and whose bounds overlap the query box, runs the dispatcher and merges its result; stops once the stop bit
// is set. Names and layouts are placeholders.
#include "game/UnkTU365.h"
#include "types.h"

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

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

struct UnkVisitResult_b0040 {
    s32 value;
    UnkVisitResult_b0040(s32 v) : value(v) {}
    UnkVisitResult_b0040(const UnkVisitResult_b0040& other) = default;
    ~UnkVisitResult_b0040() {}
};

namespace base::reflection {
struct UnkReflObject;
}
using base::reflection::UnkReflObject;
UnkColliderResult fn_7100d58ee4(UnkReflObject* obj, void* a, void* b, u32 layers, void* c, u32 d);

static inline void InitSkip() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
}

struct UnkLeafCollider_d5c6dc {
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
    virtual u32 GetLayers();  // vf_0x58
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void Refresh();  // vf_0x98
    u8 pad_0x8[0x38];
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    u8 pad_0x50[0x10];
    u8 dirty;  // 0x60
    u8 pad_0x61[9];
    u8 enabled;  // 0x6a
};

struct UnkLeaf_d5c6dc {
    u8 pad_0x0[0x10];
    UnkLeafCollider_d5c6dc* collider;
};

struct UnkLeafSet_d5c6dc {
    u8 pad_0x0[0x10];
    UnkLeaf_d5c6dc** entries;
    u32 count;
};

struct UnkQueryBox_d5c6dc {
    f32 minX, minY, maxX, maxY;
};

UnkColliderResult fn_7100d5c6dc(UnkLeafSet_d5c6dc* set, const UnkColliderResult* in, void* a, u32 mask,
                                UnkQueryBox_d5c6dc* box, void* c, u32 d) {
    UnkColliderResult result;
    result.bits = in->bits;
    u32 stopBit = result.bits & 2;
    u32 count = set->count;
    if ((count == 0) | (stopBit != 0)) return result;
    {
        UnkLeaf_d5c6dc** it = set->entries;
        u64 remaining = (u64)count * 8 - 8;
        do {
            UnkLeaf_d5c6dc* leaf = *it;
            UnkLeafCollider_d5c6dc* collider = leaf ? leaf->collider : nullptr;
            if (collider && collider->enabled && (collider->GetLayers() & mask)) {
                if (collider->dirty) collider->Refresh();
                if (collider->field_0x48 >= box->minX && collider->field_0x40 <= box->maxX &&
                    collider->field_0x4c >= box->minY && collider->field_0x44 <= box->maxY) {
                    InitSkip();
                    UnkColliderResult sub = fn_7100d58ee4(reinterpret_cast<UnkReflObject*>(collider), a, box, mask, c, d);
                    result.bits |= sub.bits;
                }
            }
            if (remaining == 0) break;
            ++it;
            remaining -= 8;
        } while (!(result.bits & 2));
    }
    return result;
}

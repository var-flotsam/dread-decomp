// Collision world unit (unit 2510): runs the shape sweep against every enabled collider of a set, stopping once
// the running result asks to. Names and layouts are placeholders.
#include "game/UnkTU365.h"
#include "types.h"

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

struct UnkSweepResult {
    u32 bits;
    u32 field_0x4;
    UnkSweepResult() : bits(0) {}
    UnkSweepResult(u32 b) : bits(b) {}
    UnkSweepResult(const UnkSweepResult& o) : bits(o.bits) {}
};

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

struct UnkSweepCollider {
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
    virtual bool IsSame(void* other);  // vf_0x68
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void Refresh();  // vf_0x98
    u8 unk_0x8[0x58];
    u8 dirty;  // 0x60
    u8 unk_0x61[9];
    bool enabled;  // 0x6a
};
static_assert(__builtin_offsetof(UnkSweepCollider, enabled) == 0x6a, "");

struct UnkSweepEntry {
    u8 unk_0x0[0x10];
    UnkSweepCollider* collider;
};

struct UnkSweepSet {
    u8 unk_0x0[0x10];
    UnkSweepEntry** entries;  // 0x10
    u32 count;                // 0x18
};

struct UnkSweepBox {
    f32 minX, minY, maxX, maxY;
};
// Box of a collider: min at 0x48/0x4c, max at 0x40/0x44.
struct UnkSweepBounds {
    u8 unk_0x0[0x40];
    f32 maxX, maxY, minX, minY;
};

UnkSweepResult fn_7100d58ab4(void* a, void* b, UnkSweepCollider* obj, void* c, const UnkSweepBox* d, u32 layers,
                             void* e, bool flag);

UnkSweepResult fn_7100d5be04(const UnkSweepSet& set, const UnkSweepResult* init, void* b, void* c, void* shape,
                             u32 layers, const UnkSweepBox* box, void* e, bool flag) {
    UnkSweepResult result(init->bits);
    if ((set.count != 0) & ((result.bits & 2) == 0)) {
        UnkSweepEntry* const* it = set.entries;
        u64 left = u64(set.count) * 8;
        do {
            left -= 8;
            UnkSweepEntry* entry = *it;
            UnkSweepCollider* collider = entry ? entry->collider : nullptr;
            if (collider && collider->enabled && (collider->GetLayers() & layers) && !collider->IsSame(shape)) {
                if (collider->dirty)
                    collider->Refresh();
                const UnkSweepBounds* bounds = reinterpret_cast<const UnkSweepBounds*>(collider);
                if (box->minX <= bounds->minX && bounds->maxX <= box->maxX && box->minY <= bounds->minY &&
                    bounds->maxY <= box->maxY) {
                    InitSkip();
                    result.bits |= fn_7100d58ab4(shape, b, collider, c, box, layers, e, flag).bits;
                }
            }
            if (left == 0)
                break;
            ++it;
        } while (!(result.bits & 2));
    }
    return result;
}

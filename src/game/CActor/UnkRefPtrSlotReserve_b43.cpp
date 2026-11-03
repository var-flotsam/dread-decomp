// Copies of fn_7100336b54 (src/game/CActor/UnkRefPtrSlotReserve_b42.cpp), one per unit
// (CXParasiteAIComponent, CForcedMovementAreaComponent twice, CCollisionComponent, CMorphBallMovement):
// reserve of a growable array of refcounted pointers. When the capacity is below the request, a new
// array is allocated, each pointer moved over (the new slot cleared, then assigned: the object's count
// raised, the slot's previous one released; then the old slot released) and the old array freed.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"


// The object type of fn_710095025c's array: virtual destructor first, count at +0x30.
struct UnkRefCounted30_b43a2 {
    virtual ~UnkRefCounted30_b43a2();
    u8 pad_0x8[0x28];
    s32 ref_count;
};

template <class Obj>
struct RefSlot_b43a2 {
    union {
        Obj* ptr;
    };

    __attribute__((always_inline)) void Assign(Obj* p) {
        if (p) {
            ++p->ref_count;
            if (Obj* old = ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            ptr = p;
        }
    }
    __attribute__((always_inline)) void Release() {
        if (Obj* old = ptr) {
            if (--old->ref_count == 0)
                delete old;
        }
    }
};

template <class Obj>
struct RefSlotArray_b43a2 {
    RefSlot_b43a2<Obj>* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Reserve(u32 n) {
        if (capacity < n) {
            RefSlot_b43a2<Obj>* p =
                static_cast<RefSlot_b43a2<Obj>*>(allocator->vf_0x40((u64)n * sizeof(RefSlot_b43a2<Obj>), 8, flags));
            if (RefSlot_b43a2<Obj>* old = data) {
                u64 j = 0;
                for (u32 i = 0; i < count; ++i, ++j) {
                    p[j].ptr = nullptr;
                    p[j].Assign(old[j].ptr);
                    old[j].Release();
                }
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
    }
};

// fn_71007ed25c's signature as src/game/CScenario.cpp declares it.
template <class Tag>
struct UnkRefPtrArray;
template <class Tag>
struct UnkSlotArray;
struct UnkTag_71007ed25c;
struct UnkTag_71006ebe54;
struct UnkTag_71007ed3a4;
struct UnkTag_7100864cec;
struct UnkTag_710095025c;

#define UNK_REF_SLOT_RESERVE_B43A2(Array, Obj, addr)                                \
    void fn_##addr(Array<UnkTag_##addr>* self, u32 capacity) {                   \
        reinterpret_cast<RefSlotArray_b43a2<Obj>*>(self)->Reserve(capacity);          \
    }

UNK_REF_SLOT_RESERVE_B43A2(UnkSlotArray, base::UnkRefCounted, 71006ebe54)
UNK_REF_SLOT_RESERVE_B43A2(UnkRefPtrArray, base::UnkRefCounted, 71007ed25c)
UNK_REF_SLOT_RESERVE_B43A2(UnkSlotArray, base::UnkRefCounted, 71007ed3a4)
UNK_REF_SLOT_RESERVE_B43A2(UnkSlotArray, base::UnkRefCounted, 7100864cec)
UNK_REF_SLOT_RESERVE_B43A2(UnkSlotArray, UnkRefCounted30_b43a2, 710095025c)

// Reserve of a growable array of refcounted pointers (base::UnkVector's layout), one copy per unit
// (CActor's, CAimComponent's and one more): when the capacity is below the request, a new array is
// allocated, each pointer moved over (the new slot cleared, then assigned: the object's count
// raised, the slot's previous one released; then the old slot released) and the old array freed.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"


struct RefSlot_b42a1 {
    union {
        base::UnkRefCounted* ptr;
    };

    __attribute__((always_inline)) void Assign(base::UnkRefCounted* p) {
        if (p) {
            ++p->ref_count;
            if (base::UnkRefCounted* old = ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            ptr = p;
        }
    }
    __attribute__((always_inline)) void Release() {
        if (base::UnkRefCounted* old = ptr) {
            if (--old->ref_count == 0)
                delete old;
        }
    }
};


struct RefSlotArray_b42a1 {
    RefSlot_b42a1* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Reserve(u32 n) {
        if (capacity < n) {
            RefSlot_b42a1* p =
                static_cast<RefSlot_b42a1*>(allocator->vf_0x40((u64)n * sizeof(RefSlot_b42a1), 8, flags));
            if (RefSlot_b42a1* old = data) {
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

// Signatures as src/game_s1/fn_710024158c.cpp and src/game/logic/collision/CCollider/fn_7100d400b8.cpp
// declare them.
template <class Tag>
struct UnkSlotArray;
struct UnkTag_7100241b30;
struct UnkTag_710031da18;
struct UnkTag_7100336b54;

#define UNK_REF_SLOT_RESERVE_B42A1(addr)                                         \
    void fn_##addr(UnkSlotArray<UnkTag_##addr>* self, u32 capacity) {            \
        reinterpret_cast<RefSlotArray_b42a1*>(self)->Reserve(capacity);          \
    }

UNK_REF_SLOT_RESERVE_B42A1(7100241b30)
UNK_REF_SLOT_RESERVE_B42A1(710031da18)
UNK_REF_SLOT_RESERVE_B42A1(7100336b54)

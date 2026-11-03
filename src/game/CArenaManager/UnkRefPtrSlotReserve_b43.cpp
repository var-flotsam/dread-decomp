// One more copy of fn_7100336b54 (src/game/CActor/UnkRefPtrSlotReserve_b42.cpp), the reserve of a
// growable array of refcounted pointers, in CArenaManager's unit. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"


struct RefSlot_b43a3 {
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


struct RefSlotArray_b43a3 {
    RefSlot_b43a3* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Reserve(u32 n) {
        if (capacity < n) {
            RefSlot_b43a3* p =
                static_cast<RefSlot_b43a3*>(allocator->vf_0x40((u64)n * sizeof(RefSlot_b43a3), 8, flags));
            if (RefSlot_b43a3* old = data) {
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

void fn_71011bb844(void* self, u32 capacity) {
    static_cast<RefSlotArray_b43a3*>(self)->Reserve(capacity);
}

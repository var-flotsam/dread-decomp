// Constructor of an object holding an 11-slot pointer array (five identical copies): value-
// initialises it, then sizes the array to 11 zeroed slots.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkSlots_0x48 {
    base::UnkAllocator* allocator = fn_7100083990();
    u32 flags = 0xe;
    u32 field_0xc = 0;
    base::UnkVector<void*> slots = base::UnkVector<void*>(0xe);
    void* field_0x38 = nullptr;
    void* field_0x40 = nullptr;
};

template <class T>
inline void Resize(base::UnkVector<T>& v, u32 n) {
    if (v.count < n) {
        if (v.capacity < n) {
            T* p = static_cast<T*>(v.allocator->vf_0x40((u64)n * sizeof(T), 8, v.flags));
            if (v.data) {
                __builtin_memcpy(p, v.data, (u64)v.count * sizeof(T));
                v.allocator->vf_0x48(v.data);
            }
            v.data = p;
            v.capacity = n;
        }
        __builtin_memset(&v.data[v.count], 0, (u64)(n - v.count) * sizeof(T));
        v.count = n;
    } else if (v.count != n) {
        v.count = n;
    }
}

inline void Construct(UnkSlots_0x48* self) {
    new (self) UnkSlots_0x48();
    Resize(self->slots, 11);
}

void fn_7100f962b0(UnkSlots_0x48* self) {
    Construct(self);
}

void fn_7100f9c734(UnkSlots_0x48* self) {
    Construct(self);
}

void fn_710104170c(UnkSlots_0x48* self) {
    Construct(self);
}

void fn_71011bb088(UnkSlots_0x48* self) {
    Construct(self);
}

void fn_71011bb594(UnkSlots_0x48* self) {
    Construct(self);
}

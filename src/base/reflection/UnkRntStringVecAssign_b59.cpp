// Unit 83 of tools/tu_map.py (base::reflection): the copy assignment (reflection slot 0x48 of
// class_descriptors_00.cpp) of a reflected array of strings (base::UnkVector's layout, 0x30-byte
// base::global::CRntString elements destroyed by fn_710007ecd4): copy and swap, through the
// array's out-of-line copy constructor fn_71000e0320. Names are placeholders.
#include <utility>

#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

void fn_71000e0320(void* self, const void* other);  // the array's copy constructor

struct UnkRntString_b59 {
    u64 field_0x0[6];

    ~UnkRntString_b59() { fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(this)); }
};

struct UnkRntStringVec_b59 {
    UnkRntString_b59* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkRntStringVec_b59(const UnkRntStringVec_b59& other) { fn_71000e0320(this, &other); }
    ~UnkRntStringVec_b59() { reinterpret_cast<base::UnkVector<UnkRntString_b59>*>(this)->Free(); }

    UnkRntStringVec_b59& operator=(const UnkRntStringVec_b59& other) {
        if (this != &other) {
            UnkRntStringVec_b59 tmp(other);
            std::swap(data, tmp.data);
            std::swap(count, tmp.count);
            std::swap(capacity, tmp.capacity);
            std::swap(step, tmp.step);
            std::swap(field_0x14, tmp.field_0x14);
            std::swap(flags, tmp.flags);
            std::swap(allocator, tmp.allocator);
        }
        return *this;
    }
};

__attribute__((flatten)) void* fn_71000e21e4(void* dst, const void* src) {
    return &(*static_cast<UnkRntStringVec_b59*>(dst) = *static_cast<const UnkRntStringVec_b59*>(src));
}

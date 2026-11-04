// Copy of fn_71000e21e4 (src/base/reflection/UnkRntStringVecAssign_b59.cpp): the copy assignment of a
// second reflected array of 0x30-byte strings (fn_71000e24b0, passed to the conversion set up in
// fn_71000e2800), copy and swap through that array's out-of-line copy constructor fn_71000e25b8.
// Names are placeholders.
#include <utility>

#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

void fn_71000e25b8(void* self, const void* other);  // the array's copy constructor

struct UnkRntString_b61 {
    u64 field_0x0[6];

    ~UnkRntString_b61() { fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(this)); }
};

struct UnkRntStringVec_b61 {
    UnkRntString_b61* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkRntStringVec_b61(const UnkRntStringVec_b61& other) { fn_71000e25b8(this, &other); }
    ~UnkRntStringVec_b61() { reinterpret_cast<base::UnkVector<UnkRntString_b61>*>(this)->Free(); }

    UnkRntStringVec_b61& operator=(const UnkRntStringVec_b61& other) {
        if (this != &other) {
            UnkRntStringVec_b61 tmp(other);
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

__attribute__((flatten)) void* fn_71000e24b0(void* dst, const void* src) {
    return &(*static_cast<UnkRntStringVec_b61*>(dst) = *static_cast<const UnkRntStringVec_b61*>(src));
}

// Destructors of a string id followed by a vector of string ids (freed through its allocator),
// at offset `Offset` of the object: the ids in the vector are released and the vector freed,
// then the leading id is released (fn_7100b5a8f0 and its copies).
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

using base::UnkAllocator;
using base::UnkStrIdMember;

struct UnkNamedStrIdVec {
    UnkStrIdMember name;
    UnkStrIdMember* data;
    u32 count;
    u32 capacity;
    u8 unk_0x18[0x10];
    UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkNamedStrIdVec() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkStrIdMember();
            UnkStrIdMember* p = data;
            count = 0;
            allocator->vf_0x48(p);
            capacity = 0;
        }
    }
};

template <u32 Offset>
__attribute__((always_inline)) static inline void Destroy(char* self) {
    reinterpret_cast<UnkNamedStrIdVec*>(self + Offset)->~UnkNamedStrIdVec();
}

void fn_7100b5a8f0(char* self) { Destroy<0x10>(self); }
void fn_7100b5ade8(char* self) { Destroy<0x10>(self); }
void fn_7100e07620(char* self) { Destroy<0x8>(self); }
void fn_7100e4c0a8(char* self) { Destroy<0x0>(self); }

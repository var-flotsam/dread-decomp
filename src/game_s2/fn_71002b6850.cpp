// Grows a growable array of words to the requested element count and zero-fills the new elements;
// a smaller request only shortens the count.
#include "base/UnkVector.h"
#include "types.h"

struct UnkSource {
    u8 pad_0x0[0x38];
    u32 field_0x38;
};

struct UnkHolder {
    u8 pad_0x0[0x10];
    base::UnkVector<u32> field_0x10;
};

void fn_71002b6850(UnkHolder* self, const UnkSource* source) {
    u32 n = source->field_0x38;
    if (n == 0)
        return;
    base::UnkVector<u32>& v = self->field_0x10;
    u32 count = v.count;
    if (count >= n) {
        if (count > n)
            v.count = n;
        return;
    }
    u32* p;
    if (v.capacity < n) {
        p = static_cast<u32*>(v.allocator->vf_0x40((u64)n * sizeof(u32), 8, v.flags));
        if (v.data) {
            __builtin_memcpy(p, v.data, (u64)v.count * sizeof(u32));
            v.allocator->vf_0x48(v.data);
        }
        count = v.count;
        v.data = p;
        v.capacity = n;
    } else {
        p = v.data;
    }
    __builtin_memset(&p[count], 0, (u64)(n - count) * sizeof(u32));
    v.count = n;
}

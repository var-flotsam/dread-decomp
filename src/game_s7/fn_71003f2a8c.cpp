// Two functions that fill a list of 64-bit name hashes by the height difference between two
// positions (fn_71003f2a8c, and fn_71006c26fc with one more hash). Names and layouts are
// placeholders; the hashes are kept as numbers.
#include "base/UnkVector.h"
#include "types.h"

namespace unk_hash_list {

struct UnkHash {
    u64 value;
};

struct UnkPos {
    float x;
    float y;
    float z;
};

// The layout of base::UnkVector, with another PushBack: the old elements are copied one by one
// through a pointer (the original reloads the count after each), and the new one bitwise.
struct UnkHashList {
    UnkHash* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    void PushBack(const UnkHash& value) {
        u32 n = count;
        u32 old_capacity = capacity;
        u32 new_capacity = old_capacity;
        if (old_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        UnkHash* p;
        if (old_capacity < new_capacity) {
            p = static_cast<UnkHash*>(allocator->vf_0x40((u64)new_capacity * sizeof(UnkHash), 8, flags));
            if (UnkHash* old = data) {
                UnkHash* dst = p;
                for (u32 i = 0; i < count; ++i, ++dst, ++old)
                    __builtin_memcpy(dst, old, sizeof(UnkHash));
                allocator->vf_0x48(data);
            }
            n = count;
            data = p;
            capacity = new_capacity;
        } else {
            p = data;
        }
        __builtin_memcpy(&p[n], &value, sizeof(UnkHash));
        ++count;
    }

    void PushBack(u64 hash) { PushBack(UnkHash{hash}); }
};

}  // namespace unk_hash_list

using unk_hash_list::UnkHashList;
using unk_hash_list::UnkPos;

// The list is taken by reference: only then may the step be loaded before the first push's
// capacity test (the original selects the step when the capacity is 0).
// NON_MATCHING(minor): the first path's first store indexes with a 64-bit register (the original
// zero-extends a 32-bit one).
void fn_71003f2a8c(void* self, UnkHashList& out, const UnkPos& from, const UnkPos& to) {
    out.count = 0;
    float dy = to.y - from.y;
    if (dy > 300.0f) {
        out.PushBack(0x40de2a8e03ac9d5bull);
        out.PushBack(0xe82f1dbb74c4a315ull);
        out.PushBack(0xc0aa43d5214c1213ull);
        return;
    }
    if (dy < 300.0f) {
        out.PushBack(0x6595c9ab2f29a628ull);
        out.PushBack(0xe82f1dbb74c4a315ull);
        out.PushBack(0x40de2a8e03ac9d5bull);
        out.PushBack(0xc0aa43d5214c1213ull);
        return;
    }
    out.PushBack(0xe82f1dbb74c4a315ull);
    out.PushBack(0x40de2a8e03ac9d5bull);
    out.PushBack(0x6595c9ab2f29a628ull);
    out.PushBack(0xc0aa43d5214c1213ull);
}

void fn_71006c26fc(void* self, UnkHashList& out, const UnkPos& from, const UnkPos& to) {
    out.count = 0;
    float dy = to.y - from.y;
    if (dy > 300.0f) {
        out.PushBack(0x40de2a8e03ac9d5bull);
        out.PushBack(0xe82f1dbb74c4a315ull);
        out.PushBack(0x171a1c3aacbbb314ull);
        out.PushBack(0xc0aa43d5214c1213ull);
        return;
    }
    if (dy < 300.0f) {
        out.PushBack(0x6595c9ab2f29a628ull);
        out.PushBack(0xe82f1dbb74c4a315ull);
        out.PushBack(0x40de2a8e03ac9d5bull);
        out.PushBack(0x171a1c3aacbbb314ull);
        out.PushBack(0xc0aa43d5214c1213ull);
        return;
    }
    out.PushBack(0xe82f1dbb74c4a315ull);
    out.PushBack(0x40de2a8e03ac9d5bull);
    out.PushBack(0x6595c9ab2f29a628ull);
    out.PushBack(0x171a1c3aacbbb314ull);
    out.PushBack(0xc0aa43d5214c1213ull);
}

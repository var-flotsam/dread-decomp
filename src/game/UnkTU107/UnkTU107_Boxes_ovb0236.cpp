// Unit 107 (0x710010d604-0x7100110c54, no reliable name; the 2D shapes of src/engine/SystemStartup.cpp): the boxes of a
// kd-tree builder (the box is in src/game_s7/fn_710010e470.cpp). fn_710010e364 sets the number of boxes in their array
// (constructing the new ones with fn_710010d604) and fn_710010ddc4 reads the array from a file: a count, then for each
// box a word and four floats. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CRntFile.h"
#include "types.h"

typedef u32 __attribute__((may_alias)) a_u32_b0236;

// A 2D box and a word, copied word by word through an aliasing type (not trivially copyable).
struct UnkBox_b0236 {
    u32 words[5];

    UnkBox_b0236(const UnkBox_b0236& o) {
        for (u32 i = 0; i < 5; ++i)
            reinterpret_cast<a_u32_b0236*>(words)[i] = reinterpret_cast<const a_u32_b0236*>(o.words)[i];
    }
    f32 Float(u32 i) const { return reinterpret_cast<const f32*>(words)[i]; }
};

struct UnkRect_b0236 {
    f32 min_x, min_y, max_x, max_y;
    UnkRect_b0236(f32 a, f32 b, f32 c, f32 d) : min_x(a), min_y(b), max_x(c), max_y(d) {}
};

void fn_710010d604(UnkBox_b0236* box);
void fn_710010d640(UnkBox_b0236* box, u32 word, const UnkRect_b0236& rect);

// The boxes' array, with the layout of base::UnkVector.
struct UnkBoxVector_b0236 {
    UnkBox_b0236* data;  // 0x0
    u32 count;           // 0x8
    u32 capacity;        // 0xc
    u32 step;            // 0x10
    bool grows;          // 0x14
    u32 flags;           // 0x18
    base::UnkAllocator* allocator;  // 0x20

    void Reserve(u32 n) {
        if (capacity < n)
            Reallocate(n);
    }
    void Reallocate(u32 new_capacity) {
        UnkBox_b0236* p = static_cast<UnkBox_b0236*>(allocator->vf_0x40((u64)new_capacity * sizeof(UnkBox_b0236), 8, flags));
        if (data) {
            UnkBox_b0236* src = data;
            UnkBox_b0236* dst = p;
            for (u32 i = 0; i < count; ++i, ++dst, ++src)
                new (dst) UnkBox_b0236(*src);
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = new_capacity;
    }
};

// Sets the number of boxes: grows the array if needed and constructs the new ones.
__attribute__((noinline)) void fn_710010e364(UnkBoxVector_b0236* self, u32 n) {
    self->Reserve(n);
    for (u32 i = self->count; i < n; ++i)
        fn_710010d604(&self->data[i]);
    self->count = n;
}

// Reads the boxes from a file.
bool fn_710010ddc4(UnkBoxVector_b0236* self, base::global::CRntFile* file) {
    s32 n;
    fn_710007c714(file, &n, 4, 1);
    self->Reserve(n);
    if (n >= 1) {
        for (s32 i = 0; i < n; ++i) {
            u32 word;
            f32 a, b, c, d;
            fn_710007c714(file, &word, 4, 1);
            fn_710007c714(file, &a, 4, 1);
            fn_710007c714(file, &b, 4, 1);
            fn_710007c714(file, &c, 4, 1);
            fn_710007c714(file, &d, 4, 1);
            fn_710010e364(self, self->count + 1);
            u32 count = self->count;
            UnkBox_b0236* end = &self->data[count];
            UnkBox_b0236* last = count ? end - 1 : end;
            fn_710010d640(last, word, UnkRect_b0236(a, b, c, d));
        }
    }
    return true;
}

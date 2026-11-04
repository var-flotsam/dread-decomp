// Unit 250 of tools/tu_map.py (no class hint): the i-th entry (below the count at +0x98) of two
// parallel base::global::CRntVector arrays: the 2D position at +0xa8 divided by the size at +0x90 (two
// u32), and the pair of words at +0xd0. Names and layouts are placeholders.
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkPos2_A1b71 {
    f32 x;
    f32 y;
};

struct UnkWordPair_A1b71 {
    u32 a;
    u32 b;
};

template <class T>
struct UnkRntVector_A1b71 {
    T* data;
    u32 size;
    u32 capacity;

    T& operator[](u32 i) {
        if (size <= i)
            __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
        return data[i];
    }
};

struct UnkUnit250_A1b71 {
    u8 pad_0x0[0x90];
    u32 width;   // 0x90
    u32 height;  // 0x94
    u32 count;   // 0x98
    u8 pad_0x9c[0xc];
    UnkRntVector_A1b71<UnkPos2_A1b71> positions;  // 0xa8
    u8 pad_0xb8[0x18];
    UnkRntVector_A1b71<UnkWordPair_A1b71> pairs;  // 0xd0
};

void fn_71001bd454(UnkUnit250_A1b71* self, u32 i, UnkWordPair_A1b71* pair, UnkPos2_A1b71* pos) {
    if (i >= self->count)
        return;
    const UnkPos2_A1b71& p = self->positions[i];
    f32 w = (f32)self->width;
    f32 h = (f32)self->height;
    *pos = UnkPos2_A1b71{p.x / w, p.y / h};
    const UnkWordPair_A1b71& q = self->pairs[i];
    pair->a = q.a;
    pair->b = q.b;
}

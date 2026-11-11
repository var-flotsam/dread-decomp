// Unit 250 of tools/tu_map.py (0x71001bcbec-0x71001bd6ec, no reflected class): two getters of an entry
// by index, valid below the count at 0x98: a pair of words from the array at 0xf8 and a word from the
// array at 0x120 (base::global::CRntVector's checked operator[]). Names and layouts are placeholders.
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

template <class T>
struct UnkRntVector250 {
    T* data;
    u32 size;

    T& operator[](u32 i) {
        if (size <= i)
            __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
        return data[i];
    }
};

struct UnkPair250 {
    u32 a;
    u32 b;
};

struct UnkObject250 {
    u8 pad_0x0[0x98];
    u32 count;
    u8 pad_0x9c[0x5c];
    UnkRntVector250<UnkPair250> pairs;  // 0xf8
    u8 pad_0x108[0x18];
    UnkRntVector250<u32> words;  // 0x120
};

void fn_71001bd4e8(UnkObject250* self, u32 index, UnkPair250* out) {
    if (index < self->count) {
        const UnkPair250& pair = self->pairs[index];
        out->a = pair.a;
        out->b = pair.b;
    }
}

void fn_71001bd548(UnkObject250* self, u32 index, u32* out) {
    if (index < self->count)
        *out = self->words[index];
}

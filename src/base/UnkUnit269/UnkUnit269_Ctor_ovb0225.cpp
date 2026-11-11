// Unit 269 of tools/tu_map.py (0x71001ea42c-0x71001ea85c, no reflected class): the constructor of the object
// src/base/UnkUnit269/UnkUnit269_Dtor_b40.cpp destroys: the vtable and a flag, then 256 entries of 0xa0
// bytes (an identity matrix at 0x818 and a sphere at 0x858 each, built by fn_710021685c, and a -1 index after it), and the
// words after them. Names and layouts are placeholders; fields are written by offset in the order the
// original stores them.
#include "game/UnkTU269.h"
#include "game/UnkTU308.h"
#include "types.h"

void fn_71001ea42c(char* self) {
    *(char**)(self + 0x0) = g_unk_7101a1ee90 + 0x10;
    *(u8*)(self + 0x8) = 1;
    for (u64 off = 0; off != 0xa000; off += 0xa0) {
        char* entry = self + off;
        f32* m = (f32*)(entry + 0x818);
        __builtin_memset(m, 0, 0x40);
        m[15] = 1.0f;
        m[10] = 1.0f;
        m[5] = 1.0f;
        m[0] = 1.0f;
        fn_710021685c(entry + 0x858);
        *(u64*)(entry + 0x810) = 0;
        *(s64*)(entry + 0x8a8) = -1;
    }
    *(u32*)(self + 0xbb40) = 0;
    __builtin_memset(self + 0xb810, 0, 13);
}

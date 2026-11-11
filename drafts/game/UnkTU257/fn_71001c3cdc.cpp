// Unit 257 of tools/tu_map.py (no class hint): the constructor of its object, the header words of unit
// 245's (src/base/UnkUnit245/UnkUnit245_Ctor_b40.cpp) with a sphere (+0x20, fn_710021685c), zeroed
// words and scales of 1, a text (+0xb0, fn_7100066a14) and 1-scales after it. Names and layouts are
// placeholders; fields are written by offset in the order the original stores them.
#include "types.h"

void fn_710021685c(void*);
void fn_7100066a14(void*);

void fn_71001c3cdc(char* self) {
    *(u64*)(self + 0x0) = 0;
    *(u32*)(self + 0x8) = 0xffffffff;
    *(u32*)(self + 0x18) = 0;
    *(u64*)(self + 0x10) = 0;
    fn_710021685c(self + 0x20);
    *(u8*)(self + 0x70) = 0;
    __builtin_memset(self + 0x78, 0, 0x10);
    *(u32*)(self + 0x98) = 0;
    *(f32*)(self + 0x9c) = 1.0f;
    *(f32*)(self + 0xa0) = 1.0f;
    *(f32*)(self + 0xa4) = 1.0f;
    *(f32*)(self + 0xa8) = 1.0f;
    *(u64*)(self + 0x88) = 0;
    *(u64*)(self + 0x90) = 0;
    fn_7100066a14(self + 0xb0);
    *(char**)(self + 0x10) = self;
    *(f32*)(self + 0xd0) = 1.0f;
    *(f32*)(self + 0xd4) = 1.0f;
    *(u64*)(self + 0xd8) = 0;
    asm volatile("" ::: "memory");
    *(u64*)(self + 0xe0) = 0x3000000001;
    *(u64*)(self + 0x0) = 0x100000200;
}

// Unit 245 of tools/tu_map.py (0x71001bb60c-0x71001bb7cc, no reflected class): the constructor of its
// object, the header words of unit 248's (src/base/UnkUnit248/UnkUnit248.cpp) with a sphere (+0x20,
// fn_710021685c), two scales of 1 and zeroed pointers after it. Names and layouts are placeholders;
// fields are written by offset in the order the original stores them.
#include "types.h"

void fn_710021685c(void*);

void fn_71001bb60c(char* self) {
    *(u64*)(self + 0x0) = 0;
    *(u32*)(self + 0x8) = 0xffffffff;
    *(u32*)(self + 0x18) = 0;
    *(u64*)(self + 0x10) = 0;
    fn_710021685c(self + 0x20);
    *(u8*)(self + 0x70) = 0;
    *(f32*)(self + 0x74) = 1.0f;
    *(u32*)(self + 0x78) = 0;
    *(f32*)(self + 0x7c) = 1.0f;
    *(u64*)(self + 0x80) = 0;
    *(u64*)(self + 0x88) = 0;
    *(u32*)(self + 0x90) = 2;
    *(u64*)(self + 0x98) = 0;
    *(u64*)(self + 0xa0) = 0;
    *(u64*)(self + 0xa8) = 0;
    *(char**)(self + 0x10) = self;
    *(u64*)(self + 0x0) = 0x100004000;
}

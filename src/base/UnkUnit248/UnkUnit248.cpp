// Unit 248 of tools/tu_map.py (0x71001bc2dc-0x71001bcb3c): the constructor of its object, a 0xe8-byte
// record holding a sphere (+0x20, fn_710021685c) and a 0x5c-byte init record (+0x84), with an
// owner link to itself at +0x10. Names and layouts are placeholders; fields are written by offset
// in the order the original stores them.
#include "base/UnkInitRecord_0x5c.h"
#include "game/UnkTU308.h"
#include "types.h"

void fn_71001bc2dc(char* self) {
    *(u64*)(self + 0x0) = 0;
    *(u32*)(self + 0x8) = 0xffffffff;
    *(u32*)(self + 0x18) = 0;
    *(u64*)(self + 0x10) = 0;
    fn_710021685c(self + 0x20);
    *(u16*)(self + 0x80) = 0x100;
    *(u8*)(self + 0x70) = 0;
    *(u64*)(self + 0x78) = 0;
    *(u8*)(self + 0x82) = 1;
    fn_7100025624(self + 0x84);
    *(u8*)(self + 0xe0) = 0;
    *(char**)(self + 0x10) = self;
    *(u64*)(self + 0x0) = 0x80000000020;
}

// Unit 123 of tools/tu_map.py: the constructor of an object tagged for debugging, as in
// src/base/UnkTaggedObject124.cpp: the flags word (0xff09f000), the live tag 0x600d600d, no counted
// resource, the sub-object at +0x18 (fn_71001b3ebc), the self pointer, then the flags' upper half
// set to 10. Names and layouts are placeholders.
#include "types.h"

struct UnkTaggedObject123 {
    u16 flags_lo;
    u16 flags_hi;
    u32 tag;                    // 0x600d600d, 0xdeaddead once destroyed
    void* resource;             // 0x8
    UnkTaggedObject123* self;   // 0x10
    u8 sub[0x8];                // 0x18, size unknown
};

void fn_71001b3ebc(void* sub);  // the sub-object's constructor

void fn_710014297c(UnkTaggedObject123* self) {
    self->flags_lo = 0xf000;
    self->flags_hi = 0xff09;
    self->tag = 0x600d600d;
    self->resource = nullptr;
    fn_71001b3ebc(self->sub);
    self->self = self;
    self->flags_hi = 10;
}

// Unit 124 of tools/tu_map.py (0x7100142aac-0x7100142b8c): the destructor of another object tagged
// for debugging, as in src/base/UnkTaggedObject137.cpp (0x600d600d while alive, 0xdeaddead once
// destroyed, a counted resource at +0x8), whose sub-object at +0x18 has a destructor
// (fn_71001b49dc) and no reset. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "types.h"

struct UnkTaggedObject124 {
    u32 flags;
    u32 tag;                       // 0x600d600d, 0xdeaddead once destroyed
    UnkCountedResource* resource;  // 0x8
    UnkTaggedObject124* self;      // 0x10
    u8 sub[0x8];                   // 0x18, size unknown
};

void fn_71001b49dc(void* sub);  // the sub-object's destructor

void fn_7100142aac(UnkTaggedObject124* self) {
    fn_71001b49dc(self->sub);
    self->tag = 0xdeaddead;
    if (UnkCountedResource* r = self->resource) {
        if (--r->ref_count == 0)
            fn_710001737c(r);
    }
}

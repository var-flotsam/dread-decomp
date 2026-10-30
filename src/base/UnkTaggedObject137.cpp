// Unit 137 of tools/tu_map.py (0x7100147c4c-0x7100147d9c, three functions): an object tagged for
// debugging (0x600d600d while alive, 0xdeaddead once destroyed) that holds a counted resource (+0x8)
// and a sub-object at +0x18. Holds its destructor (fn_7100147ca4). Names and layouts are
// placeholders.
#include "base/UnkCountedResource.h"
#include "types.h"

struct UnkTaggedObject {
    u32 flags;
    u32 tag;                      // 0x600d600d, 0xdeaddead once destroyed
    UnkCountedResource* resource;  // 0x8
    UnkTaggedObject* self;         // 0x10
    u8 sub[0x168];                 // 0x18
};

void fn_710020a444(void* sub);  // the sub-object's reset
void fn_710020a2cc(void* sub);  // its destructor (a generated clone, src/clones/)

void fn_7100147ca4(UnkTaggedObject* self) {
    fn_710020a444(self->sub);
    fn_710020a2cc(self->sub);
    self->tag = 0xdeaddead;
    if (UnkCountedResource* r = self->resource) {
        if (--r->ref_count == 0)
            fn_710001737c(r);
    }
}

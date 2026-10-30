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

// The same destructor in six neighbouring units (0x71001419b4-0x7100147900), each with its own sub-object's reset and destructor.
void fn_71001f37b8(void* sub);
void fn_71001f3658(void* sub);
void fn_71001f4268(void* sub);
void fn_71001f4108(void* sub);
void fn_71001f864c(void* sub);
void fn_71001f84ec(void* sub);
void fn_71001fabb8(void* sub);
void fn_71001faa40(void* sub);
void fn_71001b3fb0(void* sub);
void fn_71001b3f44(void* sub);
void fn_71001bcd98(void* sub);
void fn_71001bccc8(void* sub);

template <void (*Reset)(void*), void (*Destroy)(void*)>
static inline void DestroyTagged(UnkTaggedObject* self) {
    Reset(self->sub);
    Destroy(self->sub);
    self->tag = 0xdeaddead;
    if (UnkCountedResource* r = self->resource) {
        if (--r->ref_count == 0)
            fn_710001737c(r);
    }
}

void fn_7100147ca4(UnkTaggedObject* self) { DestroyTagged<fn_710020a444, fn_710020a2cc>(self); }
void fn_71001419b4(UnkTaggedObject* self) { DestroyTagged<fn_71001f37b8, fn_71001f3658>(self); }
void fn_7100141b04(UnkTaggedObject* self) { DestroyTagged<fn_71001f4268, fn_71001f4108>(self); }
void fn_7100141c8c(UnkTaggedObject* self) { DestroyTagged<fn_71001f864c, fn_71001f84ec>(self); }
void fn_7100141d7c(UnkTaggedObject* self) { DestroyTagged<fn_71001fabb8, fn_71001faa40>(self); }
void fn_71001429bc(UnkTaggedObject* self) { DestroyTagged<fn_71001b3fb0, fn_71001b3f44>(self); }
void fn_7100147900(UnkTaggedObject* self) { DestroyTagged<fn_71001bcd98, fn_71001bccc8>(self); }

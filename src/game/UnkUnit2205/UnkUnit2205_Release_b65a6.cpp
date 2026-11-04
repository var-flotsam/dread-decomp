// Unit 2205: drops the reference to the counted resource at +0x18 (base/UnkCountedResource.h).
// Names are placeholders.
#include "base/UnkCountedResource.h"

struct UnkUnit2205_b65a6 {
    u8 pad_0x0[0x18];
    UnkCountedResource* resource;
};

void fn_7100c95a68(UnkUnit2205_b65a6* self) {
    if (UnkCountedResource* r = self->resource)
        if (--r->ref_count == 0)
            fn_710001737c(r);
}

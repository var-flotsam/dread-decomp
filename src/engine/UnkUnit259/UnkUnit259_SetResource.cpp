// Unit 259 of tools/tu_map.py (0x71001c6960-0x71001d948c): setting the counted resource held at
// +0x317ac98 of a large engine object, and caching it at +0x317aca0 converted by fn_71001b363c when
// it is of the class fn_71001b122c describes (null otherwise). Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkCountedObject259 : UnkReflObject {
    u8 pad_0x8[0x4c];
    union {
        s32 ref_count;  // 0x54, as UnkCountedResource's
    };
};

struct UnkResourceSlot259 {
    UnkCountedObject259* resource;
    void* converted;
};

void* fn_71001b363c(UnkCountedObject259* resource);

void fn_71001d5434(char* self, UnkCountedObject259* const* source) {
    UnkResourceSlot259* slot = reinterpret_cast<UnkResourceSlot259*>(self + 0x317ac98);
    UnkCountedObject259* p = *source;
    if (slot->resource != p) {
        if (p)
            ++p->ref_count;
        UnkCountedObject259* old = slot->resource;
        if (old && --old->ref_count == 0)
            fn_710001737c(reinterpret_cast<UnkCountedResource*>(old));
        slot->resource = p;
    }
    void* converted;
    if (p)
        converted = fn_71001b363c(fn_710009d50c(p->vf_0x0(), fn_71001b122c()) ? p : nullptr);
    else
        converted = nullptr;
    slot->converted = converted;
}

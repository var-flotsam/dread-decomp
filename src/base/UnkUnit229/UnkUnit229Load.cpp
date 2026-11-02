// From unit 229 of tools/tu_map.py (0x71001a12f4-0x71001a9f3c, hint msapi::material): takes a counted
// reference (base/UnkCountedResource.h) to a resource at +0x8, then reads it through an in-memory
// file (base/global/CRntFile.h) over the resource's data at +0x58 (flags 0x16), with fn_71001a6644.
// Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "types.h"

// The file's constructor over existing data (the unit of base/global/CRntFile.h).
void fn_710007c080(base::global::CRntFile* file, void* data, u32 flags, u64 size);

struct UnkUnit229Resource : UnkCountedResource {
    void* data;  // 0x58
};

struct UnkUnit229Object {
    void* vtable;
    UnkCountedResource* resource;  // 0x8
};

void fn_71001a6644(UnkUnit229Object* self, base::global::CRntFile* file);

void fn_71001a65b8(UnkUnit229Object* self, UnkUnit229Resource* resource) {
    if (self->resource != resource) {
        if (resource)
            ++resource->ref_count;
        UnkCountedResource* old = self->resource;
        if (old && --old->ref_count == 0)
            fn_710001737c(old);
        self->resource = resource;
    }
    base::global::CRntFile file;
    fn_710007c080(&file, resource->data, 0x16, 0);
    fn_71001a6644(self, &file);
    fn_710007c0e4(&file);
}

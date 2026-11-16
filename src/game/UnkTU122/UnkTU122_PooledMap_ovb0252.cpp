// Unit 122 (data/tu_map.csv, no reliable name): the constructor of a pooled chained map with a pointer list, and
// the factory that builds an object (0x218 bytes) holding a counted resource and files it in the map. Names and
// layouts are placeholders.
#include "base/global/CRntDictionary.h"
#include "base/UnkChainedMap.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkCountedResource.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU127.h"
#include "game/UnkTU291.h"
#include "types.h"
#include <new>
struct UnkPooledChainedMap_b0252 {
    UnkChainedMap map;
    base::UnkBlockPool pool;  // 0x48
    base::global::CRntVector<void*> entries;  // 0x88
};
static_assert(__builtin_offsetof(UnkPooledChainedMap_b0252, entries) == 0x88, "layout");

void fn_7100142798(UnkPooledChainedMap_b0252* self, u32 buckets, u32 nodes);

void fn_71001420d8(UnkPooledChainedMap_b0252* self) {
    new (&self->map) UnkChainedMap();
    self->pool.Init();
    self->pool.field_0x38 = 0;
    self->pool.field_0x3c = false;
    fn_7100142798(self, 0x400, 0x4000);
    new (&self->entries) base::global::CRntVector<void*>(0xf);
    self->entries.Reserve(0x1000);
}

struct UnkTaggedSubObject127;
struct UnkTaggedSubArg;

// The render pool's objects, by their base.
using UnkPoolMap_b0252 = base::global::CRntDictionary<void*, void*>;

struct UnkDeletable_b0252 {
    void** vtable;
    void Refresh() { reinterpret_cast<void (*)(UnkDeletable_b0252*)>(vtable[0x90 / 8])(this); }
};

struct UnkResourceHolder_b0252 {
    u8 pad_0x0[0x10];
    UnkCountedResource* resource;  // 0x10
    u8 pad_0x18[8];
    UnkDeletable_b0252 sub;  // 0x20
    u8 pad_0x28[0x98 - 0x28];
    UnkCountedResource* second;  // 0x98
};

static inline void Assign(UnkCountedResource** slot, UnkCountedResource* r) {
    if (*slot != r) {
        if (r)
            ++r->ref_count;
        if (UnkCountedResource* old = *slot) {
            if (--old->ref_count == 0)
                fn_710001737c(old);
        }
        *slot = r;
    }
}

void* fn_7100141f1c(UnkPoolMap_b0252* map, UnkCountedResource* resource, UnkResourceHolder_b0252** out, UnkTaggedSubArg* name) {
    UnkResourceHolder_b0252* object = static_cast<UnkResourceHolder_b0252*>(fn_71000839a0(0x218, 0x32));
    fn_710014333c(reinterpret_cast<UnkTaggedSubObject127*>(object), name);
    void* node = nullptr;
    if (object) {
        fn_710020d32c(reinterpret_cast<char*>(object) + 0x20);
        Assign(&object->resource, resource);
        node = reinterpret_cast<u8*>(object) + 8;
        if (resource) {
            Assign(&object->second, resource);
            object->sub.Refresh();
        }
        map->InsertChecked(node, object);
    }
    if (out)
        *out = object;
    return node;
}

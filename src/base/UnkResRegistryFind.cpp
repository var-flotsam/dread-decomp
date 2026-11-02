// From the resource registry's unit (src/base/UnkResRegistry.cpp, which keeps the registry's layout
// to itself; this file reads it through its own views): the resource already loaded under a key,
// counted, or null. The registry keeps, per type name, a chained map of paths to resources (+0x60);
// both levels hash an id by the two words of its key (+0x30 and +0x34). When the type is known but
// the path isn't, the path map at +0x10 is searched too, and its answer isn't used, as in the
// original. Names and layouts are placeholders.
#include "base/UnkResKey.h"
#include "types.h"

using base::global::UnkStringData;

// A pooled id's data as the maps hash it.
struct UnkResIdHashView {
    u8 pad_0x0[0x30];
    u32 key_lo;
    u32 key_hi;
};

static inline u32 IdHash(const UnkStringData* data) {
    const UnkResIdHashView* v = reinterpret_cast<const UnkResIdHashView*>(data);
    return v->key_lo ^ v->key_hi;
}

struct UnkResPathNode {
    UnkStringData* path;
    UnkCountedResource* resource;
    UnkResPathNode* next;
};

struct UnkResPathMapView {
    void* field_0x0;
    u32 count;
    u32 modulus;
};

// The bucket head of a path map.
UnkResPathNode** fn_71000ec350(UnkResPathMapView* map, u32 bucket);

struct UnkResTypeNode {
    UnkStringData* type;
    UnkResPathMapView paths;  // +0x8
    u8 pad_0x18[0x40];
    UnkResTypeNode* next;  // +0x58
};

struct UnkResPlainPathNode {
    UnkStringData* path;
    void* field_0x8;
    UnkResPlainPathNode* next;
};

struct UnkResRegistryFindView {
    u8 pad_0x0[0x10];
    UnkResPlainPathNode** path_buckets;  // +0x10
    u32 path_count;                      // +0x18
    u32 path_modulus;                    // +0x1c
    u8 pad_0x20[0x40];
    UnkResTypeNode** type_buckets;  // +0x60
    u32 type_count;                 // +0x68
    u32 type_modulus;               // +0x6c
};

UnkResRef fn_71000eac38(UnkResRegistry* registry, const UnkResKey* key) {
    UnkResRegistryFindView* self = reinterpret_cast<UnkResRegistryFindView*>(registry);
    if (self->type_count == 0)
        return UnkResRef();
    UnkStringData* type = key->type.data;
    u32 bucket = IdHash(type) % self->type_modulus;
    if (bucket >= self->type_count)
        return UnkResRef();
    UnkResTypeNode* node = self->type_buckets[bucket];
    for (; node; node = node->next)
        if (node->type == type)
            break;
    if (!node)
        return UnkResRef();
    if (node->paths.count != 0) {
        u32 b = IdHash(key->path.data) % node->paths.modulus;
        if (b < node->paths.count) {
            for (UnkResPathNode* p = *fn_71000ec350(&node->paths, b); p; p = p->next) {
                if (p->path == key->path.data) {
                    if (UnkCountedResource* res = p->resource) {
                        ++res->ref_count;
                        UnkResRef r;
                        r.ptr = res;
                        asm("");  // keeps this return's store apart from the null ones
                        return r;
                    }
                    break;
                }
            }
        }
    }
    if (self->path_count != 0) {
        UnkStringData* path = key->path.data;
        u32 b = IdHash(path) % self->path_modulus;
        if (b < self->path_count) {
            UnkResPlainPathNode* p = self->path_buckets[b];
            while (p && p->path != path)
                p = p->next;
        }
    }
    return UnkResRef();
}

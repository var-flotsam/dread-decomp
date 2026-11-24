// Unit 88 of tools/tu_map.py: the resource registry loads the resource a key names. The type's record
// gives the class to build and the load's flags; the new object is loaded and, when that works, put in
// the registry's path map and in its type's own path map. Names and layouts are placeholders.
#include "base/UnkCache.h"
#include "base/UnkResKey.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkBucketSet.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::UnkHashedString;
using base::global::UnkStringData;
using base::reflection::CType;

// A path id and the resource loaded from it, chained in its bucket.
struct UnkResLoadPathNode {
    UnkStringData* path;
    UnkCountedResource* resource;
    UnkResLoadPathNode* next;
};

// A chained map of paths to resources (the registry's at +0x10, and each type node's at +0x8).
using UnkResLoadPathMap = base::global::UnkBucketSet<UnkResLoadPathNode>;

// A type's node: its name, its paths and the next node in the bucket.
struct UnkResLoadTypeNode {
    UnkStringData* type;
    UnkResLoadPathMap paths;  // 0x8
    UnkResLoadTypeNode* next;  // 0x58
};

// The registry's type map, with its node pool.
using UnkResLoadTypeMap = base::global::UnkPooledBucketSet<UnkResLoadTypeNode>;

// An iterator into the type map.
struct UnkResLoadTypeIter {
    UnkResLoadTypeNode* node;
    UnkResLoadTypeNode* previous;
    UnkResLoadTypeMap* map;
    u32 bucket;
    u32 version;
};

// The record a type is registered with (0x90 bytes).
struct UnkResLoadDesc {
    u8 pad_0x0[0x48];
    const CType* type;  // 0x48, the class of what is loaded
    u32 flags;  // 0x50
    void* field_0x58;
    u8 pad_0x60[0x30];
};
static_assert(sizeof(UnkResLoadDesc) == 0x90, "record layout");

struct UnkResLoadRegistry {
    u8 pad_0x0[0x10];
    UnkResLoadPathMap paths;  // 0x10
    UnkResLoadTypeMap types;  // 0x60
    UnkResLoadDesc* descs;  // 0xf0
    base::global::CStrId* names;  // 0xf8
    u32 desc_count;  // 0x100
    u8 pad_0x104[0x14];
    u8 field_0x118[0x30];
};
static_assert(__builtin_offsetof(UnkResLoadRegistry, field_0x118) == 0x118, "registry layout");

struct UnkResPathNode;
struct UnkResPathMapView;
UnkResPathNode** fn_71000ec350(UnkResPathMapView* map, u32 bucket);
struct UnkSetNode_b257j;
struct UnkNamedSet_b257j;
UnkSetNode_b257j** fn_71000ec52c(UnkNamedSet_b257j* set, u32 index);
UnkResLoadTypeIter fn_71000eb300(UnkResLoadTypeMap* map, const UnkResKey* key, const void* tag, u32&& flags);
__attribute__((visibility("hidden"))) extern const char g_unk_71016478c8[];

static inline u32 IdHash(const UnkStringData* data) {
    const UnkHashedString* s = static_cast<const UnkHashedString*>(data);
    return s->hash[0] ^ s->hash[1];
}

// Links a node for `key`'s path and `resource` into `map`.
inline void AddPath(UnkResLoadPathMap* map, const UnkResKey* key, UnkCountedResource* resource) {
    ++map->version;
    UnkResLoadPathNode* node =
        static_cast<UnkResLoadPathNode*>(map->node_allocator->vf_0x40(sizeof(UnkResLoadPathNode), 8, map->node_flags));
    if (node) {
        UnkStringData* path = key->path.data;
        node->path = path;
        if (&g_unk_7101d47200->empty != path)
            ++path->ref_count;
        node->resource = resource;
        node->next = nullptr;
    }
    u32 i;
    if (map->buckets.count)
        i = IdHash(key->path.data) % map->buckets.capacity;
    else
        i = ~0u;
    u32 first = map->first_bucket;
    map->first_bucket = i < first ? i : first;
    if (i <= first)
        map->first_node = node;
    UnkResLoadPathNode** bucket =
        reinterpret_cast<UnkResLoadPathNode**>(fn_71000ec350(reinterpret_cast<UnkResPathMapView*>(map), i));
    node->next = *bucket;
    *bucket = node;
    ++map->node_count;
}

// The record of `key`'s type, or the slot past the last one.
static inline UnkResLoadDesc* FindDesc(UnkResLoadRegistry* self, const UnkResKey* key) {
    for (u64 i = 0; i < self->desc_count; ++i)
        if (self->names[i].data == key->type.data)
            return &self->descs[i];
    return &self->descs[self->desc_count];
}

UnkCountedResource* fn_71000eaf20(UnkResRegistry* registry, const UnkResKey* key, u32 flag, void* a) {
    UnkResLoadRegistry* self = reinterpret_cast<UnkResLoadRegistry*>(registry);
    if (key->type.data == &g_unk_7101d47108->empty || key->path.data == &g_unk_7101d47200->empty)
        return nullptr;

    base::UnkLoadRequest request;
    request.vtable = g_unk_7101a19610 + 0x10;
    request.field_0x8 = 0;
    request.field_0x10 = 2;
    request.field_0x18 = nullptr;

    u32 flags = flag;
    UnkResLoadDesc* desc = FindDesc(self, key);
    if (desc != &self->descs[self->desc_count]) {
        if (flags == 1)
            flags = desc->flags;
        if (!a)
            a = desc->field_0x58;
        request.field_0x8 = reinterpret_cast<u64>(self->field_0x118);
        request.field_0x10 = flags;
        request.field_0x18 = a;
    } else {
        flags = 2;
    }

    u64 index = 0;
    for (; index < self->desc_count; ++index)
        if (self->names[index].data == key->type.data)
            break;
    const CType* type = self->descs[index].type;

    UnkCountedResource* resource = nullptr;
    if (fn_710009d50c(type, reinterpret_cast<const CType*>(fn_71000e8a5c())) && type->m_construct) {
        resource = static_cast<UnkCountedResource*>(GetDefaultAllocator()->vf_0x40(type->m_size, 8, flags));
        if (resource) {
            reinterpret_cast<void (*)(void*)>(type->m_construct)(resource);
            if (fn_71000171a0(resource, reinterpret_cast<const char*>(key), &request)) {
                AddPath(&self->paths, key, resource);

                UnkResLoadTypeMap* types = &self->types;
                UnkResLoadTypeNode* node = nullptr;
                if (types->buckets.count) {
                    u32 h = IdHash(key->type.data) % types->buckets.capacity;
                    if (h < types->buckets.count) {
                        node = reinterpret_cast<UnkResLoadTypeNode*>(
                            *fn_71000ec52c(reinterpret_cast<UnkNamedSet_b257j*>(types), h));
                        UnkStringData* name = key->type.data;
                        while (node && node->type != name)
                            node = node->next;
                    }
                }
                if (!node)
                    node = fn_71000eb300(types, key, g_unk_71016478c8, 0xe).node;
                AddPath(&node->paths, key, resource);
                return resource;
            }
        }
    }
    fn_7100019fc0(resource, true);
    return nullptr;
}

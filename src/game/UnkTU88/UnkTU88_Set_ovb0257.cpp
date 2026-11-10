// Unit 88 of tools/tu_map.py (no name hint): the resource registry's chained sets (base/UnkResKey.h; the
// registry is built in src/base/UnkResRegistry.cpp). fn_71000ea16c empties a set of named nodes (each node
// destroys its list and name and is freed), fn_71000eb7c4 erases the node an iterator points at (keeping
// the set's first bucket and node right), and fn_71000eb628 takes a resource out of the registry: its path
// leaves the path set, then the type node's, and the type node goes when it has no paths left. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "base/UnkCache.h"
#include "base/UnkResKey.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/global/CStrId.h"
#include "types.h"

// ---- the registry's set of named nodes (the type map at +0x60 of the registry) ----

struct UnkSetNode_b257j {
    base::global::CStrId name;  // 0x0
    base::UnkVector<void*> list;  // 0x8
    u8 pad_0x30[0x28];
    UnkSetNode_b257j* next;  // 0x58
};
static_assert(sizeof(UnkSetNode_b257j) == 0x60, "node layout");

struct UnkNamedSet_b257j {
    UnkSetNode_b257j** data;
    u32 count;  // 0x8
    u32 capacity;  // 0xc
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
    u32 first_bucket;  // 0x28
    UnkSetNode_b257j* first_node;  // 0x30
    base::UnkAllocator* node_allocator;  // 0x38
    u32 node_flags;  // 0x40
    u32 node_count;  // 0x44
    u32 version;  // 0x48
};

UnkSetNode_b257j** fn_71000ec52c(UnkNamedSet_b257j* set, u32 index);
void fn_71000ea064(void* list);

// Destroys a node: its list, its name, then the node itself.
static inline __attribute__((always_inline)) void DestroyNode_b257j(UnkNamedSet_b257j* set, UnkSetNode_b257j* node) {
    fn_71000ea064(&node->list);
    if (node->list.data) {
        node->list.count = 0;
        node->list.allocator->vf_0x48(node->list.data);
        node->list.capacity = 0;
    }
    node->name.Reset();
    set->node_allocator->vf_0x48(node);
}

void fn_71000ea16c(UnkNamedSet_b257j* self) {
    if (!self->node_count)
        return;
    u32 n = self->count;
    for (u32 i = 0; i != n; ++i) {
        UnkSetNode_b257j* node = *fn_71000ec52c(self, i);
        while (node) {
            UnkSetNode_b257j* next = node->next;
            DestroyNode_b257j(self, node);
            ++self->version;
            node = next;
        }
    }
    __builtin_memset(self->data, 0, (u64)self->capacity * 8);
    self->node_count = 0;
    self->first_node = nullptr;
    self->first_bucket = -1;
}

// An iterator over the set: the node, its predecessor in the bucket, the set, the bucket and the set's
// version when it was made.
struct UnkSetIter_b257j {
    UnkSetNode_b257j* node;  // 0x0
    UnkSetNode_b257j* previous;  // 0x8
    UnkNamedSet_b257j* set;  // 0x10
    u32 bucket;  // 0x18
    u32 version;  // 0x1c
};

void fn_71000eb7c4(UnkNamedSet_b257j* self, UnkSetIter_b257j* it) {
    u32 index;
    if (it->set && it->set->version == it->version) {
        index = it->bucket;
    } else {
        index = -1;
        it->previous = nullptr;
        it->set = nullptr;
        it->node = nullptr;
        it->bucket = index;
    }
    UnkSetNode_b257j** bucket = fn_71000ec52c(self, index);
    UnkSetNode_b257j* node = it->node;
    if (!node)
        return;
    UnkSetNode_b257j* previous = it->previous;
    if (*bucket == node) {
        *bucket = node->next;
        if (index == self->first_bucket) {
            if (index < self->count) {
                while (!*fn_71000ec52c(self, index)) {
                    index = self->first_bucket + 1;
                    self->first_bucket = index;
                    if (!(index < self->count))
                        break;
                }
            }
            if (node == self->first_node) {
                u32 first = self->first_bucket;
                self->first_node = first < self->count ? *fn_71000ec52c(self, first) : nullptr;
            }
        }
    }
    if (previous)
        previous->next = node->next;
    --self->node_count;
    DestroyNode_b257j(self, node);
    ++self->version;
}

// ---- the resource a key names (compare src/base/UnkResRegistry.cpp, fn_71000ead80) ----

using base::global::UnkStringData;

// What a pooled id's data hashes by: the two words of its key.
struct UnkIdHashView_b257j {
    u8 pad_0x0[0x30];
    u32 key_lo;
    u32 key_hi;
};

static inline u32 IdHash_b257j(const UnkStringData* data) {
    const UnkIdHashView_b257j* v = reinterpret_cast<const UnkIdHashView_b257j*>(data);
    return v->key_lo ^ v->key_hi;
}

struct UnkResRegistryFull_b257j {
    u8 pad_0x0[0x60];
    UnkSetNode_b257j** type_buckets;  // 0x60
    u32 type_count;  // 0x68
    u32 type_modulus;  // 0x6c
};

// ---- a resource leaving the registry ----

struct UnkResObject_b257j {
    virtual const base::reflection::CClass* vf_0x0();
    UnkStringData* type;  // 0x8
    base::global::CFilePathStrId path;  // 0x10
};

void fn_71000eca70(void* set, UnkResPathId* path);

void fn_71000eb628(UnkResRegistry* registry, UnkResObject_b257j* object) {
    UnkResRegistryFull_b257j* self = reinterpret_cast<UnkResRegistryFull_b257j*>(registry);
    if (!object)
        return;
    if (!fn_710009d50c(object->vf_0x0(), fn_71000e8a5c()))
        return;
    UnkResPathId path(object->path);
    fn_71000eca70(reinterpret_cast<char*>(self) + 0x10, &path);
    UnkNamedSet_b257j* types = reinterpret_cast<UnkNamedSet_b257j*>(reinterpret_cast<char*>(self) + 0x60);
    UnkSetNode_b257j* previous = nullptr;
    UnkSetNode_b257j* node = nullptr;
    u32 bucket = -1;
    if (self->type_count != 0) {
        u32 h = IdHash_b257j(object->type) % self->type_modulus;
        if (h < self->type_count) {
            bucket = h;
            node = *fn_71000ec52c(types, bucket);
            if (node) {
                UnkStringData* type = object->type;
                if (node->name.data != type) {
                    for (;;) {
                        previous = node;
                        node = node->next;
                        if (!node || node->name.data == type)
                            break;
                    }
                }
            }
        }
    }
    u32 version = types->version;
    fn_71000eca70(&node->list, &path);
    if (*reinterpret_cast<u32*>(reinterpret_cast<char*>(node) + 0x4c) == 0) {
        UnkSetIter_b257j it;
        it.node = node;
        it.previous = previous;
        it.set = types;
        it.bucket = bucket;
        it.version = version;
        fn_71000eb7c4(types, &it);
    }
    fn_7100019fc0(object, true);
}

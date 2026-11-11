// Unit 2572 of tools/tu_map.py (no reliable name): a cache of counted resources keyed by a resource
// pointer, in a chained hash map hashed by the two halves of the 64-bit id at +0x30 of the key.
// fn_7100ecaed8 adds the resource the actor-definition factory (g_unk_71726a0220) holds for a key
// unless the map has one; fn_7100eca4ec constructs the cache (two tables, a vector and the empty
// file path); fn_7100ecb4ac clears the map, releasing each node's resource and file-path id.
// Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU344.h"
#include "types.h"

// ---- Adding the factory's resource for a key ----

struct UnkKey_caed8 {
    u8 pad_0x0[0x30];
    u32 id_lo;
    u32 id_hi;
};

struct UnkNode_caed8 {
    UnkKey_caed8* key;
    UnkCountedResource* value;
    UnkNode_caed8* next;
};

struct UnkMap_caed8 {
    u8 pad_0x0[8];
    u32 count;         // 0x8
    u32 bucket_count;  // 0xc
};

struct UnkEntry_caed8 {
    u8 pad_0x0[0x10];
    UnkKey_caed8* key;  // 0x10
};

struct UnkActorDefFactory;

struct UnkOwner_caed8 {
    u8 pad_0x0[0x58];
    UnkMap_caed8 map;  // 0x58
};

// A counted reference (returned in memory: it has a destructor).
struct UnkRef_caed8 {
    UnkCountedResource* ptr;
    ~UnkRef_caed8() {
        if (ptr && --ptr->ref_count == 0)
            fn_710001737c(ptr);
    }
};

// The result of the insertion (returned in memory; its size is a guess).
struct UnkInserted_caed8 {
    void* node;
    void* field_0x8;
    void* field_0x10;
    void* field_0x18;
    UnkInserted_caed8(const UnkInserted_caed8&);
};

UnkNode_caed8** fn_7100ecba2c(UnkMap_caed8* self, u32 index);
UnkRef_caed8 fn_71011934b0(UnkActorDefFactory* self, UnkKey_caed8* const* key);
UnkInserted_caed8 fn_7100ecbc08(UnkMap_caed8* self, UnkKey_caed8* const* key, const UnkRef_caed8* value);

void fn_7100ecaed8(UnkOwner_caed8* self, UnkEntry_caed8* entry) {
    UnkMap_caed8* map = &self->map;
    UnkKey_caed8* const* key = &entry->key;
    if (map->count) {
        u32 b = ((*key)->id_lo ^ (*key)->id_hi) % map->bucket_count;
        if (b < map->count) {
            for (UnkNode_caed8* node = *fn_7100ecba2c(map, b); node; node = node->next) {
                if (node->key == *key)
                    return;
            }
        }
    }
    UnkRef_caed8 ref = fn_71011934b0(g_unk_71726a0220, key);
    fn_7100ecbc08(map, key, &ref);
}

// ---- The constructor ----

struct UnkTableA_ca4ec;
struct UnkTableB_ca4ec;

// A cache of resources by file path: two tables, a vector and the (empty) default path.
struct UnkResourceCache_ca4ec {
    u8 field_0x0;
    u8 pad_0x1[7];
    u8 table_a[0x50];                   // 0x8
    u8 table_b[0x50];                   // 0x58
    base::UnkVector<void*> pending;     // 0xa8
    base::global::CFilePathStrId path;  // 0xd0
};
static_assert(__builtin_offsetof(UnkResourceCache_ca4ec, pending) == 0xa8, "layout");
static_assert(__builtin_offsetof(UnkResourceCache_ca4ec, path) == 0xd0, "layout");

void fn_7100eca5dc(UnkTableA_ca4ec* self, base::UnkAllocator* allocator, u32 flags);
void fn_7100eca778(UnkTableB_ca4ec* self, base::UnkAllocator* allocator, u32 flags);

void fn_7100eca4ec(UnkResourceCache_ca4ec* self) {
    self->field_0x0 = 0;
    fn_7100eca5dc(reinterpret_cast<UnkTableA_ca4ec*>(self->table_a), nullptr, 0xe);
    fn_7100eca778(reinterpret_cast<UnkTableB_ca4ec*>(self->table_b), nullptr, 0xe);
    new (&self->pending) base::UnkVector<void*>(0xf);
    new (&self->path) base::global::CFilePathStrId();
}

// ---- Clearing the map ----

using UnkPathId_ecb4ac = base::UnkStrIdMemberOf<g_unk_7101d47200>;

struct UnkNode_ecb4ac {
    UnkPathId_ecb4ac key;       // 0x0
    UnkCountedResource* value;  // 0x8
    UnkNode_ecb4ac* next;       // 0x10
};

struct UnkMap_ecb4ac {
    void* buckets;        // 0x0
    u32 bucket_count;     // 0x8
    u32 bucket_capacity;  // 0xc
    u8 pad_0x10[0x18];
    s32 field_0x28;
    u8 pad_0x2c[4];
    UnkNode_ecb4ac* field_0x30;
    base::UnkAllocator* allocator;  // 0x38
    u8 pad_0x40[4];
    u32 count;  // 0x44
    u32 freed;  // 0x48
};
static_assert(__builtin_offsetof(UnkMap_ecb4ac, allocator) == 0x38, "layout");

// The bucket accessor is the one declared above (the nodes of this map are UnkNode_ecb4ac).

void fn_7100ecb4ac(UnkMap_ecb4ac* self) {
    if (self->count) {
        u32 n = self->bucket_count;
        for (u32 i = 0; i < n; ++i) {
            UnkNode_ecb4ac* node = *reinterpret_cast<UnkNode_ecb4ac**>(fn_7100ecba2c(reinterpret_cast<UnkMap_caed8*>(self), i));
            while (node) {
                UnkCountedResource* value = node->value;
                UnkNode_ecb4ac* next = node->next;
                if (value && --value->ref_count == 0)
                    fn_710001737c(value);
                node->key.~UnkPathId_ecb4ac();
                self->allocator->vf_0x48(node);
                ++self->freed;
                node = next;
            }
        }
        memset(self->buckets, 0, self->bucket_capacity * u64(8));
        self->count = 0;
        self->field_0x30 = nullptr;
        self->field_0x28 = -1;
    }
}

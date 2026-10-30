// The factory of the "actordef" resources (CActorDef, .bmsad; the category's record is
// g_unk_71727b9530, src/game_s7/fn_7100d96670.cpp), unit 2729: a 0x1c8-byte object created on
// first use by fn_710119307c into g_unk_71726a0220. Its interface for the resource registry is a
// second base at +8 (the vtable's offset-to-top there is -8); three chained maps with node pools
// follow. fn_71011930c8 registers it with the registry for the category, and fn_7101193370 sets a
// flag on every item of its first map; fn_7101193ee0 looks a CCharClass up in that map. Names and
// layouts are placeholders inferred from the code.
#include <new>

#include "base/UnkBlockFreeList.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkBlockPool;
using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// What the first map holds (only the flag fn_7101193370 sets is known).
struct UnkActorDefFactoryItem {
    u8 unk_0x0[0xc8];
    bool field_0xc8;

    void SetField_0xc8(bool value) { field_0xc8 = value; }
};

struct UnkActorDefMapNode {
    u64 key;
    UnkActorDefFactoryItem* value;
    UnkActorDefMapNode* next;
};

// A chained hash map with its own node pool (0x90 bytes): the buckets, the first used bucket and
// its first node, the nodes' allocator, the flags and count, and a version that every change bumps.
struct UnkActorDefMap {
    base::UnkVector<UnkActorDefMapNode*> buckets;
    u32 first_bucket;
    UnkActorDefMapNode* first_node;
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    // A union member: the original rereads it after storing to an item while walking the map.
    union {
        u32 version;
    };
    UnkBlockPool pool;

    UnkActorDefMap()
        : buckets(0xe), first_bucket(-1), first_node(nullptr), allocator(fn_7100083990()), flags(0xe), count(0),
          version(1) {
        pool.Init();
        pool.field_0x38 = 0;
        pool.field_0x3c = false;
    }
};
static_assert(sizeof(UnkActorDefMap) == 0x90, "UnkActorDefMap layout");

// The interface the resource registry takes the factory as.
struct UnkActorDefResFactory {
    char* vtable;
};

struct UnkActorDefFactory {
    char* vtable;
    UnkActorDefResFactory res_factory;  // 0x8
    UnkActorDefMap map_0x10;
    UnkActorDefMap map_0xa0;
    UnkActorDefMap map_0x130;
    bool field_0x1c0;
};
static_assert(sizeof(UnkActorDefFactory) == 0x1c8, "UnkActorDefFactory layout");

extern char g_unk_7101cb5270[];  // the factory's vtable (its second base's at +0x60)
// The maps' initialisers (each called with 0x80 and 0x200).
void fn_7101194c84(UnkActorDefMap* map, u32 a, u32 b);
void fn_7101194e50(UnkActorDefMap* map, u32 a, u32 b);
void fn_710119501c(UnkActorDefMap* map, u32 a, u32 b);

// The constructor.
void fn_7101192c5c(UnkActorDefFactory* self) {
    self->vtable = g_unk_7101cb5270 + 0x10;
    self->res_factory.vtable = g_unk_7101cb5270 + 0x60;
    new (&self->map_0x10) UnkActorDefMap();
    fn_7101194c84(&self->map_0x10, 0x80, 0x200);
    new (&self->map_0xa0) UnkActorDefMap();
    fn_7101194e50(&self->map_0xa0, 0x80, 0x200);
    new (&self->map_0x130) UnkActorDefMap();
    fn_710119501c(&self->map_0x130, 0x80, 0x200);
    self->field_0x1c0 = false;
}

// The resource registry's entry (0x48 bytes), as src/base/UnkResFactoryRegistry.cpp lays it out: the
// category's record and the factory, three ids and two callbacks. Its constructor is inlined here
// (fn_710002003c out of line).
template <UnkStringPool** Pool>
struct UnkActorDefEntryId {
    UnkStringData* data;

    // Empty: the pointer is cleared before the pool is made sure of.
    __attribute__((always_inline)) UnkActorDefEntryId() : data(nullptr) {
        if (!*Pool) {
            if (Pool == &g_unk_7101d47200)
                base::global::UnkEnsureFilePathPool();
            else
                base::global::UnkEnsureStringPool();
        }
        data = &(*Pool)->empty;
    }
    __attribute__((always_inline)) explicit UnkActorDefEntryId(const CStrId& other) : data(other.data) {
        if (&(*Pool)->empty != data)
            ++data->ref_count;
    }
    __attribute__((always_inline)) explicit UnkActorDefEntryId(const CFilePathStrId& other)
        : data(other.data) {
        if (&(*Pool)->empty != data)
            ++data->ref_count;
    }
    // Released in two steps (the original reloads the id and its count after the decrement).
    __attribute__((always_inline)) ~UnkActorDefEntryId() {
        if (UnkStringPool* pool = *Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = *Pool;
                }
            }
            data = &pool->empty;
        }
    }
};

void fn_71000204c4(void* context, void* arg);
void fn_71000204cc(void* context, void* arg);

struct UnkActorDefEntryCallback {
    void* context;
    void (*function)();
};

struct UnkActorDefEntry {
    void* desc;
    void* factory;
    UnkActorDefEntryId<&g_unk_7101d47108> id;
    UnkActorDefEntryId<&g_unk_7101d47200> path_0x18;
    UnkActorDefEntryId<&g_unk_7101d47200> path_0x20;
    UnkActorDefEntryCallback field_0x28;
    UnkActorDefEntryCallback field_0x38;

    __attribute__((always_inline)) UnkActorDefEntry(void* desc, void* factory)
        : desc(desc), factory(factory), id(g_unk_7101d470e0), path_0x18(g_unk_7101d470e8),
          field_0x28{nullptr, reinterpret_cast<void (*)()>(fn_71000204c4)},
          field_0x38{nullptr, reinterpret_cast<void (*)()>(fn_71000204cc)} {}
    // Inlined into each registration, as the implicit destructor is when there is only one.
    __attribute__((always_inline)) ~UnkActorDefEntry() {}
};
static_assert(sizeof(UnkActorDefEntry) == 0x48, "UnkActorDefEntry layout");

// The registry and its insertion, as src/game_s7/fn_7100d92158.cpp declares them.
struct UnkRegistry_7100d92158;
struct UnkEntry_7100d92158;
extern UnkRegistry_7100d92158* g_unk_7101d4bbc0;
void fn_710002002c(UnkRegistry_7100d92158* registry, const UnkEntry_7100d92158& entry);

static inline const UnkEntry_7100d92158& AsRegistryEntry(const UnkActorDefEntry& entry) {
    return *reinterpret_cast<const UnkEntry_7100d92158*>(&entry);
}

extern char g_unk_71727b9530[];               // the "actordef" category
extern UnkActorDefFactory* g_unk_71726a0220;  // the factory (created by fn_710119307c)

// The factory's registry interface, if the factory exists.
static inline UnkActorDefResFactory* ResFactory() {
    return g_unk_71726a0220 ? &g_unk_71726a0220->res_factory : nullptr;
}

// Registers the factory for the "actordef" category.
void fn_71011930c8() {
    fn_710002002c(g_unk_7101d4bbc0, AsRegistryEntry(UnkActorDefEntry(g_unk_71727b9530, ResFactory())));
}

// The same registration in units 2542 and 2624 of tools/tu_map.py, for factories of the same shape (the
// registry interface at +8) held in their own singletons, each with its category's record. The
// startup (src/engine/UnkStartupManagers.cpp) passes fn_7100db256c its singleton, which it ignores.
extern void* g_unk_7172758b18;     // unit 2542's factory
extern void* g_unk_71729f0148;     // unit 2624's factory
extern char g_unk_71726aad48[];    // the "animtree" category (src/game/animtree/CAnimTreeRes/fn_71002e0370.cpp)
extern CStrId g_unk_71727b9d30;    // unit 2624's category

static inline UnkActorDefResFactory* ResFactoryOf(void* factory) {
    return factory ? &static_cast<UnkActorDefFactory*>(factory)->res_factory : nullptr;
}

void fn_7100db256c(void* /*unused*/) {
    fn_710002002c(g_unk_7101d4bbc0,
                  AsRegistryEntry(UnkActorDefEntry(g_unk_71726aad48, ResFactoryOf(g_unk_7172758b18))));
}

void fn_7100fc8024() {
    fn_710002002c(g_unk_7101d4bbc0,
                  AsRegistryEntry(UnkActorDefEntry(&g_unk_71727b9d30, ResFactoryOf(g_unk_71729f0148))));
}

// An iterator over a map: the node, the bucket it is in and the map's version when it started; it
// ends early if the map changes.
struct UnkActorDefMapIter {
    UnkActorDefMapNode* node;
    UnkActorDefMap* map;
    u32 version;
    u32 bucket;

    explicit UnkActorDefMapIter(UnkActorDefMap* m)
        : node(m->first_node), map(m), version(m->version), bucket(m->first_bucket) {}

    void Next() {
        if (map->version != version) {
            node = nullptr;
            return;
        }
        node = node->next;
        if (node)
            return;
        while (++bucket < map->buckets.count) {
            node = map->buckets.data[bucket];
            if (node)
                return;
        }
        node = nullptr;
    }
};

// Sets the flag of every item in the first map.
void fn_7101193370(UnkActorDefFactory* self, bool flag) {
    for (UnkActorDefMapIter it(&self->map_0x10); it.node; it.Next())
        it.node->value->SetField_0xc8(flag);
}

// The item the first map holds under the string id `*id` when it is a CCharClass, else null. The
// bucket is the id's 64-bit hash (+0x30 of its pool entry), its halves xor-ed, modulo the bucket
// array's capacity; a bucket past the count is empty.
base::reflection::UnkReflObject* fn_7101193ee0(UnkActorDefFactory* self, const CStrId* id) {
    UnkActorDefMap& map = self->map_0x10;
    if (map.buckets.count == 0)
        return nullptr;
    const UnkStringData* data = id->data;
    const u32* hash = reinterpret_cast<const u32*>(reinterpret_cast<const char*>(data) + 0x30);
    u32 bucket = (hash[0] ^ hash[1]) % map.buckets.capacity;
    if (bucket >= map.buckets.count)
        return nullptr;
    for (UnkActorDefMapNode* node = map.buckets.data[bucket]; node; node = node->next) {
        if (node->key == reinterpret_cast<u64>(data)) {
            auto* item = reinterpret_cast<base::reflection::UnkReflObject*>(node->value);
            if (!item)
                return nullptr;
            return fn_710009d50c(item->vf_0x0(), fn_71011abbd4()) ? item : nullptr;
        }
    }
    return nullptr;
}

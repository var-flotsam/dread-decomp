// The constructor of the object of unit 51, whose reflected class base::input::CRumbleManager::
// CTunableRumbleManager names the file (the object's own type is unverified; base/input/UnkRumbleManager.h).
#include "base/UnkBlockFreeList.h"
#include "base/input/UnkRumbleManager.h"
#include "base/math/UnkZeroVector3.h"

using namespace base::input;

extern char g_unk_7101a1a330[];  // its base vtable (GOT 0x7101cbbf68)
extern char g_unk_7101a1a3d0[];  // its vtable (GOT 0x7101cbc0d8)

void fn_710007533c(void* self);  // the 0x1d00-byte member's constructor
// Sets up the hash map's storage (base::refl_HashMapInit's copy: buckets, nodes).
void fn_7100075958(void* map, u32 buckets, u32 nodes);

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

// A function-local static word (zero-initialised under its guard) that this constructor reads.
extern u32 g_unk_7101d58738;
extern u64 g_unk_7101d58740;

void fn_71000733e4(UnkRumbleManager* self) {
    typedef u64 __attribute__((may_alias)) word;
    char* base = reinterpret_cast<char*>(self);
    self->vtable = g_unk_7101a1a330 + 0x10;
    *reinterpret_cast<word*>(base + 0x1ec) = 0;
    *reinterpret_cast<word*>(base + 0x1f4) = 0;
    self->field_0x1fc = 0;
    new (&self->mixers[0]) nn::hid::VibrationMixer();
    new (&self->mixers[1]) nn::hid::VibrationMixer();
    new (&self->targets[0]) nn::hid::VibrationTarget();
    new (&self->targets[1]) nn::hid::VibrationTarget();
    new (&self->connections[0]) nn::hid::VibrationNodeConnection();
    new (&self->connections[1]) nn::hid::VibrationNodeConnection();
    fn_710007533c(self->sub_0x3a0);
    self->field_0x20b0 = 0;
    self->field_0x20a8 = 0;
    self->field_0x20a0 = 0;
    *reinterpret_cast<word*>(base + 0x20b8) = 0xe0000000a;
    self->alloc_0x20c0 = fn_7100083990();
    self->vtable = g_unk_7101a1a3d0 + 0x10;
    UnkPooledChainedMap* m = &self->map;
    m->alloc = fn_7100083990();
    m->flags = 0xe;
    char* mb = reinterpret_cast<char*>(m);
    *reinterpret_cast<word*>(mb + 0xc) = 0;
    *reinterpret_cast<word*>(mb + 0x14) = 0;
    *reinterpret_cast<u32*>(mb + 0x1c) = 0;
    *reinterpret_cast<u32*>(mb + 0x20) = 10;  // the growth step
    m->field_0x24 = true;
    m->field_0x28 = 0xe;
    base::UnkAllocator* a = fn_7100083990();
    m->field_0x38 = 0;
    m->field_0x40 = 0;
    m->pool_vtable = g_unk_7101a1a4c8 + 0x10;
    m->alloc_0x30 = a;
    m->field_0x50 = 0;
    fn_710008377c(&m->pool);
    m->field_0x80 = 0;
    m->field_0x84 = false;
    fn_7100075958(m, 0xb, 0x200);
    self->field_0x2160 = 0;
    self->field_0x2158 = 0;
    self->field_0x2168 = 10;
    self->field_0x216c = true;
    self->field_0x2170 = 0xf;
    self->alloc_0x2178 = fn_7100083990();
    self->field_0x2180 = 0;
    fn_710008377c(&self->pool_0x2190);
    self->field_0x21c0 = 0;
    self->field_0x21b8 = 0;
    self->field_0x21c8 = 10;
    self->field_0x21cc = true;
    self->field_0x21d0 = 0xf;
    self->alloc_0x21d8 = fn_7100083990();
    // The zero vector's three words (base/math/UnkZeroVector3.h), copied as words.
    const u32* defaults = reinterpret_cast<const u32*>(&g_unk_710164779c);
    self->field_0x21e8 = defaults[0];
    u32 z = defaults[2];
    self->field_0x21ec = defaults[1];
    self->field_0x21f0 = z;
    self->field_0x21f8 = 0;
    self->field_0x2200 = 0;
    u32* zero = &g_unk_7101d58738;
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_7101d58740), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_7101d58740)) {
            *zero = 0;
            __cxa_guard_release(&g_unk_7101d58740);
        }
    }
    self->field_0x2204 = *zero;
    self->field_0x2208 = 0x100;
}

// The rumble files the manager has loaded, cached by path in its map at +0x20d0.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// The resource registry's key and reference types (as src/game/CAbilitySonar.cpp defines them).
struct UnkResType {
    CStrId name;
};

struct UnkResRegistry;
extern UnkResRegistry* g_unk_7101d472f8;

struct UnkResPathId {
    UnkStringData* data;

    UnkResPathId(const CFilePathStrId& other) : data(other.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    ~UnkResPathId() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkResKey {
    CStrId type;
    UnkResPathId path;

    UnkResKey(const CStrId& t, const CFilePathStrId& p) : type(t), path(p) {}
};

struct UnkResRef {
    UnkCountedResource* ptr;

    UnkResRef() : ptr(nullptr) {}
    UnkResRef& operator=(const UnkResRef& other) {
        if (ptr != other.ptr) {
            UnkCountedResource* p = other.ptr;
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
    ~UnkResRef() {
        if (ptr && --ptr->ref_count == 0)
            fn_710001737c(ptr);
    }
};

UnkResKey fn_71000ea448(UnkResRegistry* registry, const UnkResKey* key, void* a);
UnkResRef fn_71000ead80(UnkResRegistry* registry, const UnkResKey* key, u8 flag, void* a);

// The rumble files' resource type.
extern const UnkResType g_unk_7101d676d8;

// A loaded base::res::CRes as the cache reads it.
struct UnkRumbleSource {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual u64 vf_0x28();
};

struct UnkRumbleRes : base::reflection::UnkReflObject {
    u8 unk_0x8[0x50];
    UnkRumbleSource* source;  // 0x58
    u8 unk_0x60[8];
    u64 field_0x68;
};

// A file-path id that starts empty (making the pool first if needed), assigned as CStrId is.
struct UnkRumblePath : base::global::UnkFilePathStrIdBase {
    UnkRumblePath() {
        data = nullptr;
        if (!g_unk_7101d47200)
            base::global::UnkEnsureFilePathPool();
        data = &g_unk_7101d47200->empty;
    }
    ~UnkRumblePath() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
    UnkRumblePath& operator=(const UnkRumblePath& other) {
        if (data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
    CFilePathStrId* Id() { return reinterpret_cast<CFilePathStrId*>(this); }
};

// A cached rumble file (0x20 bytes): the resource, two values read from it and its path.
struct UnkRumbleFile {
    UnkResRef res;
    u64 field_0x8;
    u32 field_0x10;
    UnkRumblePath path;
};

// The cache's map nodes (0x28 bytes): the path, the file, the node list links and the bucket chain.
// The list links are union members: the original reloads them after each link store.
struct UnkRumbleFileNode {
    UnkStringData* key;
    UnkRumbleFile* value;
    union {
        UnkRumbleFileNode* next;
    };
    union {
        UnkRumbleFileNode* prev;
    };
    UnkRumbleFileNode* chain;
};

struct UnkRumbleFileMap {
    base::UnkAllocator* allocator;  // 0x20d0
    u32 flags;
    u32 count;
    UnkRumbleFileNode** buckets;  // 0x20e0
    u32 bucketCount;
    u32 bucketCapacity;
    u8 unk_0x20f0[0x18];
    UnkRumbleFileNode* last;   // 0x2108
    UnkRumbleFileNode* first;  // 0x2110
};

// The halves of the id's 64-bit hash (at +0x30) combined.
static inline u32 PathHash(const UnkStringData* data) {
    const char* raw = reinterpret_cast<const char*>(data);
    return *reinterpret_cast<const u32*>(raw + 0x30) ^ *reinterpret_cast<const u32*>(raw + 0x34);
}

// The bucket a key hashes to; null while the map has no buckets.
static inline UnkRumbleFileNode** Bucket(UnkRumbleFileMap* map, const UnkStringData* key) {
    UnkRumbleFileNode** bucket = nullptr;
    if (map->bucketCount) {
        u32 i = PathHash(key) % map->bucketCapacity;
        bucket = map->buckets + i;
    }
    return bucket;
}

// Builds the rumble file's path for `name` (defined in this unit, which lets the compiler see that
// it keeps no pointer to `out`).
void fn_7100072c00(const void* name, CFilePathStrId* out __attribute__((noescape)));

// The cached rumble file for `name`, loaded and cached on first use (a reference to its resource is
// taken for a cached one); null when it can't be loaded.
UnkRumbleFile* fn_710007454c(UnkRumbleManager* self, const void* name) {
    UnkRumblePath path;
    fn_7100072c00(name, path.Id());
    UnkRumbleFileMap* map = reinterpret_cast<UnkRumbleFileMap*>(reinterpret_cast<char*>(self) + 0x20d0);
    if (map->bucketCount) {
        if (UnkRumbleFileNode** bucket = Bucket(map, path.data)) {
            for (UnkRumbleFileNode* n = *bucket; n; n = n->chain) {
                if (n->key == path.data) {
                    UnkRumbleFileNode* found = *bucket;
                    while (found->key != path.data)
                        found = found->chain;
                    UnkRumbleFile* file = found->value;
                    ++file->res.ptr->ref_count;
                    return file;
                }
            }
        }
    }
    UnkRumbleFile* file = nullptr;
    UnkResKey key(g_unk_7101d676d8.name, *path.Id());
    UnkResKey found = fn_71000ea448(g_unk_7101d472f8, &key, nullptr);
    if (found.type.data != &g_unk_7101d47108->empty && found.path.data != &g_unk_7101d47200->empty) {
        UnkResRef res = fn_71000ead80(g_unk_7101d472f8, &found, 1, nullptr);
        if (res.ptr) {
            UnkRumbleRes* r = reinterpret_cast<UnkRumbleRes*>(res.ptr);
            r = fn_710009d50c(r->vf_0x0(), fn_71000e8a5c()) ? r : nullptr;
            file = new (fn_7100083990()->vf_0x40(sizeof(UnkRumbleFile), 8, 1)) UnkRumbleFile();
            file->res = res;
            file->field_0x10 = (u32)r->field_0x68;
            file->field_0x8 = r->source ? r->source->vf_0x28() : 0;
            file->path = path;
            UnkRumbleFileNode** bucket = nullptr;
            if (map->bucketCount) {
                u32 i = PathHash(path.data) % map->bucketCapacity;
                bucket = map->buckets + i;
            }
            UnkRumbleFileNode* chain = *bucket;
            UnkRumbleFileNode* node =
                static_cast<UnkRumbleFileNode*>(map->allocator->vf_0x40(sizeof(UnkRumbleFileNode), 8, map->flags));
            if (node) {
                node->key = path.data;
                if (&g_unk_7101d47200->empty != path.data)
                    ++path.data->ref_count;
                node->value = file;
                node->next = nullptr;
                node->prev = nullptr;
                node->chain = nullptr;
            }
            node->chain = chain;
            *bucket = node;
            if (!map->first) {
                map->first = node;
            } else if (map->last) {
                node->prev = map->last;
                map->last->next = node;
            }
            map->last = node;
            ++map->count;
        }
    }
    return file;
}

// A pool of 0xa8-byte blocks in one allocation (the manager's at +0x2180): the free list over the
// buffer, and an array with an entry per block.
struct UnkRumbleBlockEntry {
    void* block;
    u64 field_0x8;
};

struct UnkRumbleBlockPool {
    base::UnkAllocator* allocator;  // owns the buffer
    u32 flags;
    UnkBlockFreeList list;                          // +0x10
    base::UnkVector<UnkRumbleBlockEntry> entries;  // +0x38
    u64 count;                                      // +0x60
};

// (Re)creates the pool with `n` blocks from `allocator` (the default one when null), freeing the
// previous buffer.
void fn_7100073900(UnkRumbleBlockPool* self, u64 n, u32 flags, base::UnkAllocator* allocator) {
    // The list's buffer, read as a word at +0x30: the original recomputes the list's address for
    // each call instead of keeping it from this read.
    if (void* buffer = *reinterpret_cast<void**>(reinterpret_cast<char*>(self) + 0x30)) {
        fn_71000837f4(&self->list);
        self->entries.Clear();
        self->allocator->vf_0x48(buffer);
        self->allocator = nullptr;
    }
    self->flags = flags;
    if (!allocator)
        allocator = fn_7100083990();
    self->allocator = allocator;
    u8* mem = static_cast<u8*>(allocator->vf_0x40(n * 0xa8, 8, flags));
    fn_710008378c(&self->list, mem, n * 0xa8, 0xa8);
    self->entries.Reserve(n);
    for (u64 i = 0; i < n; ++i) {
        UnkRumbleBlockEntry e = {mem + i * 0xa8, 0};
        self->entries.PushBack(e);
    }
    self->count = n;
}

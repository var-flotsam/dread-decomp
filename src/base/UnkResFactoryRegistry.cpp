// A registry of factories for resource categories (unit 27): the object (vtable unk_7101a19590,
// its destructor fn_710001fa30) holds a keyed array of 0x48-byte entries at +8, keyed by the
// category's id. An entry names its category (one of the lists the resource units' static
// initialisers add their types to: "curve", "anim" and "blendspace", src/base/animation/CAnimationResource/fn_7100005700.cpp
// and fn_71000113b0.cpp) and a factory object, three string ids and two callbacks; fn_710001fc00
// registers the three, each with a factory created on first use (their deleting destructors are in
// src/game_s1/fn_7100022518.cpp). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// An id released in two steps (the original reloads the id and its count after the decrement).
template <UnkStringPool** Pool>
struct UnkRegistryEntryId {
    UnkStringData* data;

    // Empty: the pointer is cleared before the pool is made sure of.
    UnkRegistryEntryId() : data(nullptr) {
        if (!*Pool) {
            if (Pool == &g_unk_7101d47200)
                base::global::UnkEnsureFilePathPool();
            else
                base::global::UnkEnsureStringPool();
        }
        data = &(*Pool)->empty;
    }
    UnkRegistryEntryId(const UnkRegistryEntryId& other) : data(other.data) {
        if (&(*Pool)->empty != data)
            ++data->ref_count;
    }
    explicit UnkRegistryEntryId(const CStrId& other) : data(other.data) {
        if (&(*Pool)->empty != data)
            ++data->ref_count;
    }
    explicit UnkRegistryEntryId(const CFilePathStrId& other) : data(other.data) {
        if (&(*Pool)->empty != data)
            ++data->ref_count;
    }
    ~UnkRegistryEntryId() {
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

// A resource category: its id comes first.
struct UnkRegistryDesc {
    CStrId id;
};

// The entry's default callbacks, as base::res's type record's (src/base/UnkResRegistry.cpp).
void fn_71000204c4(void* context, void* arg);
void fn_71000204cc(void* context, void* arg);

// Copied member by member (the original reloads the source after each store).
struct UnkRegistryCallback {
    void* context;
    void (*function)();

    UnkRegistryCallback(void* context, void (*function)()) : context(context), function(function) {}
    UnkRegistryCallback(const UnkRegistryCallback& other) : context(other.context), function(other.function) {}
};

// The descriptor and its factory, set together (and copied as one block).
struct UnkRegistryEntryHead {
    UnkRegistryDesc* desc;
    void* factory;
};

// The entry (0x48 bytes): the head of base::res's type record (UnkResDesc) with a factory in place
// of the registry.
struct UnkRegistryEntry : UnkRegistryEntryHead {
    UnkRegistryEntryId<&g_unk_7101d47108> id;
    UnkRegistryEntryId<&g_unk_7101d47200> path_0x18;
    UnkRegistryEntryId<&g_unk_7101d47200> path_0x20;
    UnkRegistryCallback field_0x28;
    UnkRegistryCallback field_0x38;

    UnkRegistryEntry(UnkRegistryDesc* desc, void* factory) asm("fn_710002003c");  // fn_710002003c
    UnkRegistryEntry(const UnkRegistryEntry& other) = default;
};

// The constructor and fn_7100022cd8 are inline functions the compiler kept out of line (weak
// copies): nothing is inferred from their bodies, so fn_710001fc00 reloads its entry's ids after
// each release.
__attribute__((noinline)) inline UnkRegistryEntry::UnkRegistryEntry(UnkRegistryDesc* desc, void* factory)
    : UnkRegistryEntryHead{desc, factory}, id(g_unk_7101d470e0), path_0x18(g_unk_7101d470e8),
      field_0x28(nullptr, reinterpret_cast<void (*)()>(fn_71000204c4)),
      field_0x38(nullptr, reinterpret_cast<void (*)()>(fn_71000204cc)) {}

// The constructor under its address's name: for Clang 9, the matching compiler, an alias of its
// complete-object symbol; for a newer Clang (the port's, which hangs on an asm label on this inline
// constructor) a function that runs it.
#if defined(__clang__) && __clang_major__ <= 9
void fn_710002003c(UnkRegistryEntry*, UnkRegistryDesc*, void*)
    __attribute__((alias("_ZN16UnkRegistryEntryC1EP15UnkRegistryDescPv")));
#else
void fn_710002003c(UnkRegistryEntry* self, UnkRegistryDesc* desc, void* factory) {
    new (self) UnkRegistryEntry(desc, factory);
}
#endif

__attribute__((flatten)) void fn_7100020164(char* self) {
    reinterpret_cast<UnkRegistryEntry*>(self)->~UnkRegistryEntry();
}

// The registry's entries keyed by their descriptor's id: two parallel arrays sharing a count, a
// capacity and a growth step.
struct UnkRegistryEntries {
    UnkRegistryEntry* values;
    UnkRegistryEntryId<&g_unk_7101d47108>* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};
// Grows both arrays to `capacity`.
void fn_7100022e2c(UnkRegistryEntries* self, u32 capacity);

// Appends an entry under `key` and returns it.
__attribute__((noinline)) inline UnkRegistryEntry* fn_7100022cd8(UnkRegistryEntries* self, const CStrId& key,
                                                                 const UnkRegistryEntry& entry) {
    u32 n = self->count;
    if (n == self->capacity) {
        fn_7100022e2c(self, self->step + n);
        n = self->count;
    }
    // The key is copied as raw bytes, then retained: the original reloads the count after the copy.
    UnkRegistryEntryId<&g_unk_7101d47108>* k = &self->keys[n];
    __builtin_memcpy(k, &key, sizeof(*k));
    if (&g_unk_7101d47108->empty != k->data)
        ++k->data->ref_count;
    new (&self->values[self->count]) UnkRegistryEntry(entry);
    u32 count = ++self->count;
    return count ? &self->values[count] - 1 : self->values;
}

// The registry: its entries follow the vtable.
struct UnkResFactoryRegistry {
    void* vtable;
    UnkRegistryEntries entries;
};

// A factory: a vtable and a 0x50-byte member, zeroed and then set up by its own function.
struct UnkRegistryFactory {
    void* vtable;
    u64 field_0x8[10];
};
using UnkRegistryFactoryInit = void (*)(u64* self, void* unk, u32 n);

static inline UnkRegistryFactory* Factory(UnkRegistryFactory*& instance, char* vtable,
                                          UnkRegistryFactoryInit init) {
    if (!instance) {
        auto* f = static_cast<UnkRegistryFactory*>(fn_71000839a0(0x58, 0x3f));
        for (u64& w : f->field_0x8)
            w = 0;
        f->vtable = vtable + 0x10;
        init(f->field_0x8, nullptr, 0xe);
        instance = f;
    }
    return instance;
}

// The factories (created on first use) and their vtables.
extern UnkRegistryFactory* g_unk_7101d4e598;
extern UnkRegistryFactory* g_unk_7101d4e5a0;
extern UnkRegistryFactory* g_unk_7101d4e5a8;
extern char unk_7101a195b0[];
extern char unk_7101a19630[];
extern char unk_7101a19690[];
// The categories: "curve" (src/base/animation/CAnimationResource/fn_7100005700.cpp), "anim" (the same file) and "blendspace"
// (src/base/animation/CBlendSpaceResource/fn_71000113b0.cpp).
extern char g_unk_7101d4f338[];
extern char g_unk_7101d4b418[];
extern char g_unk_7101d4c138[];
// The factories' set-up functions.
void fn_71000204d4(u64* self, void* unk, u32 n);
void fn_7100021230(u64* self, void* unk, u32 n);
void fn_7100021f84(u64* self, void* unk, u32 n);

__attribute__((always_inline)) static inline void Register(UnkResFactoryRegistry* self, char* category,
                                                           UnkRegistryFactory* factory) {
    UnkRegistryEntry e(reinterpret_cast<UnkRegistryDesc*>(category), factory);
    fn_7100022cd8(&self->entries, e.desc->id, e);
}

// Registers the three categories' factories.
void fn_710001fc00(UnkResFactoryRegistry* self) {
    Register(self, g_unk_7101d4f338, Factory(g_unk_7101d4e598, unk_7101a195b0, fn_71000204d4));
    Register(self, g_unk_7101d4b418, Factory(g_unk_7101d4e5a0, unk_7101a19630, fn_7100021230));
    Register(self, g_unk_7101d4c138, Factory(g_unk_7101d4e5a8, unk_7101a19690, fn_7100021f84));
}

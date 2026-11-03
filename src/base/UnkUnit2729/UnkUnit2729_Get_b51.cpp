// From unit 2729 of tools/tu_map.py: a cache of counted resources keyed by a resource pointer, in a
// chained hash map hashed by the two halves of the 64-bit id at +0x30 of the key (see
// UnkUnit2729_Find_b50.cpp for a sibling). A miss falls back to fn_71011939dc, which builds and adds.
// Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

struct UnkKey_71011938bc {
    u8 pad_0x0[0x30];
    u32 id_lo;
    u32 id_hi;
};

struct UnkNode_71011938bc {
    UnkKey_71011938bc* key;
    UnkCountedResource* value;
    UnkNode_71011938bc* next;
};

struct UnkCache_71011938bc {
    u8 pad_0x0[0x10];
    UnkNode_71011938bc** buckets;  // +0x10
    u32 count;                     // +0x18
    u32 bucket_count;              // +0x1c
};

// A counted reference (returned in memory: it has a destructor).
struct UnkRef_71011938bc {
    UnkCountedResource* ptr;
    ~UnkRef_71011938bc();
};

// Declared as in UnkUnit2729_Find_b50.cpp.
struct UnkKey_71011934b0;
struct UnkCounted_71011934b0;
struct UnkCache_71011934b0;
UnkCounted_71011934b0* fn_7100255ce8(const base::global::CStrId& name, UnkKey_71011934b0* const* key, u64 flags);
void fn_7101193cb4(UnkCache_71011934b0* self, UnkCounted_71011934b0* value, UnkKey_71011934b0* const* key);
// The file path a key's resource is loaded from.
base::global::CFilePathStrId fn_710025539c(UnkKey_71011938bc* const* key);

// The miss path: builds the resource from the key and the path (or the key's default path when the
// path is empty) and adds it to the cache.
UnkCountedResource* fn_71011939dc(UnkCache_71011938bc* self, UnkKey_71011938bc* const* key,
                                  const base::global::CFilePathStrId* path) {
    base::global::CFilePathStrId tmp = path->data != &g_unk_7101d47200->empty
                                           ? base::global::CFilePathStrId(*path)
                                           : fn_710025539c(key);
    UnkCounted_71011934b0* v =
        fn_7100255ce8(*reinterpret_cast<const base::global::CStrId*>(key),
                      reinterpret_cast<UnkKey_71011934b0* const*>(&tmp), 0);
    if (v)
        fn_7101193cb4(reinterpret_cast<UnkCache_71011934b0*>(self), v,
                      reinterpret_cast<UnkKey_71011934b0* const*>(&tmp));
    return reinterpret_cast<UnkCountedResource*>(v);
}

UnkRef_71011938bc fn_71011938bc(UnkCache_71011938bc* self, UnkKey_71011938bc* const* key,
                                         const base::global::CFilePathStrId* path) {
    UnkRef_71011938bc ref;
    if (self->count) {
        UnkKey_71011938bc* k = *key;
        u32 b = (k->id_lo ^ k->id_hi) % self->bucket_count;
        if (b < self->count) {
            for (UnkNode_71011938bc* node = self->buckets[b]; node; node = node->next) {
                if (node->key == k) {
                    UnkCountedResource* v = node->value;
                    if (!v)
                        break;
                    s32 n = v->ref_count + 1;
                    ref.ptr = v;
                    v->ref_count = n;
                    if (n == 0)
                        fn_710001737c(v);
                    return ref;
                }
            }
        }
    }
    UnkCountedResource* v = fn_71011939dc(self, key, path);
    ref.ptr = nullptr;
    if (v) {
        ++v->ref_count;
        ref.ptr = v;
    }
    return ref;
}

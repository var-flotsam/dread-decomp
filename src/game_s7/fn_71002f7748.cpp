// Copies of fn_7100d94068 (src/game_aca57c/fn_7100d94118.cpp), which the binary repeats at
// other addresses: fn_71002f7748, fn_71000206e0, fn_710002143c, fn_7100022190, fn_7101025c1c.
// Get-or-create in a hash map of refcounted resources keyed by an object (hashed from two words
// at 0x30): a found resource is sent an event (vtable unk_7101a19610, with the two extra
// arguments) through fn_7100017390, a missing one is created through the map's vf_0x48. Either
// way a new reference is returned. One map type per bucket accessor (fn_7100d94654 and others).
#include "base/UnkCountedResource.h"
#include "game/UnkBuckets.h"
#include "game/UnkKeyObject.h"
#include "game/UnkResourceEvent.h"
#include "game/UnkResourceKey.h"
#include "game/UnkResourceMap.h"
#include "game/UnkResourceNode.h"
#include "game/UnkShared54Ref.h"
#include "types.h"

extern char unk_7101a19610[];

struct UnkRefCounted_2f7324;
namespace base {
struct UnkLoadRequest;
}
void fn_7100017390(UnkRefCounted_2f7324* object, base::UnkLoadRequest* visitor);

template <class Tag, UnkResourceNode** (*Bucket)(UnkBuckets*, u32)>
__attribute__((always_inline)) static inline UnkShared54Ref GetOrCreate(UnkResourceMap<Tag>* self,
                                                                         const UnkResourceKey& key, u32 arg,
                                                                         void* data) {
    UnkResourceNode* found = nullptr;
    if (self->buckets.count) {
        u32 hash = key.object->hash_a ^ key.object->hash_b;
        u32 i = hash % self->buckets.capacity;
        if (i < self->buckets.count) {
            for (UnkResourceNode* n = *Bucket(&self->buckets, i); n; n = n->next) {
                if (n->key == key.object) {
                    found = n;
                    break;
                }
            }
        }
    }
    UnkCountedResource* resource;
    if (found) {
        resource = found->value;
        UnkResourceEvent event;
        event.vtable = unk_7101a19610 + 0x10;
        event.field_0x8 = 0;
        event.arg = arg;
        event.data = data;
        fn_7100017390(reinterpret_cast<UnkRefCounted_2f7324*>(resource), reinterpret_cast<base::UnkLoadRequest*>(&event));
    } else {
        resource = self->vf_0x48(key, arg, data);
    }
    return UnkShared54Ref(resource);
}

struct UnkTag_7100d94118;

// Finds the resource through vf_0x0, creating it through vf_0x48 when there is none.
template <class Tag>
__attribute__((always_inline)) static inline UnkShared54Ref FindOrCreate(UnkResourceMap<Tag>* self,
                                                                          const UnkResourceKey& key, u32 arg,
                                                                          void* data) {
    UnkShared54Ref ref = self->vf_0x0(key, arg, data);
    if (!ref.ptr)
        ref = self->vf_0x48(key, arg, data);
    return ref;
}

#define FIND_OR_CREATE(addr, tag)                                                                  \
    UnkShared54Ref fn_##addr(UnkResourceMap<UnkTag_##tag>* self, const UnkResourceKey& key, u32 arg, \
                             void* data) {                                                         \
        return FindOrCreate<UnkTag_##tag>(self, key, arg, data);                                   \
    }

FIND_OR_CREATE(71002f7748, 7100d94118)
FIND_OR_CREATE(71000206e0, 7100d94118)
FIND_OR_CREATE(710002143c, 7100d94118)
FIND_OR_CREATE(7100022190, 7100d94118)
FIND_OR_CREATE(7101025c1c, 7100d94118)


// Get-or-create in a hash map of refcounted resources keyed by an object (hashed from two words
// at 0x30): a found resource is sent an event (vtable unk_7101a19610, with the two extra
// arguments) through fn_7100017390, a missing one is created through the map's vf_0x48. Either
// way a new reference is returned. One map type per bucket accessor (fn_7100d94654 and others).
#include "types.h"

struct UnkShared54 {
    u8 unk_0x0[0x54];
    s32 ref_count;
};

struct UnkShared54Ref {
    UnkShared54* ptr;

    explicit UnkShared54Ref(UnkShared54* p) : ptr(nullptr) {
        if (p) {
            ++p->ref_count;
            ptr = p;
        }
    }
    ~UnkShared54Ref();
};

struct UnkKeyObject {
    u8 unk_0x0[0x30];
    u32 hash_a;
    u32 hash_b;
};

struct UnkResourceKey {
    u64 field_0x0;
    UnkKeyObject* object;
};

struct UnkResourceNode {
    UnkKeyObject* key;
    UnkShared54* value;
    UnkResourceNode* next;
};

extern char unk_7101a19610[];

struct UnkResourceEvent {
    char* vtable;
    u64 field_0x8;
    u32 arg;
    void* data;
};
void fn_7100017390(UnkShared54* resource, UnkResourceEvent* event);

struct UnkBuckets {
    UnkResourceNode** data;
    u32 count;
    u32 capacity;
};

template <class Tag>
struct UnkResourceMap {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual UnkShared54* vf_0x48(const UnkResourceKey& key, u32 arg, void* data);

    UnkBuckets buckets;
};

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
    UnkShared54* resource;
    if (found) {
        resource = found->value;
        UnkResourceEvent event;
        event.vtable = unk_7101a19610 + 0x10;
        event.field_0x8 = 0;
        event.arg = arg;
        event.data = data;
        fn_7100017390(resource, &event);
    } else {
        resource = self->vf_0x48(key, arg, data);
    }
    return UnkShared54Ref(resource);
}

#define GET_OR_CREATE(addr, bucket)                                                                \
    struct UnkTag_##addr;                                                                          \
    UnkResourceNode** bucket(UnkBuckets* self, u32 index);                                         \
    UnkShared54Ref fn_##addr(UnkResourceMap<UnkTag_##addr>* self, const UnkResourceKey& key, u32 arg, \
                             void* data) {                                                         \
        return GetOrCreate<UnkTag_##addr, bucket>(self, key, arg, data);                           \
    }

GET_OR_CREATE(7100d94118, fn_7100d94654)
GET_OR_CREATE(7100d94e6c, fn_7100d953a8)
GET_OR_CREATE(7100d95bc0, fn_7100d960fc)
GET_OR_CREATE(7100db2b20, fn_7100db3308)

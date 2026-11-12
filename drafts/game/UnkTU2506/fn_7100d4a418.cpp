// Unit 2506: drops every reference held by a two-part container (a flat pointer list and
// a list of fixed-size buckets) and empties it. Names and layouts are placeholders.
#include "types.h"

struct UnkRefCounted {
    struct VTable {
        void* slots[2];
        void (*release)(UnkRefCounted*);
    };
    VTable* vtable;
    u32 refCount;
};

struct __attribute__((packed)) UnkRefBucket {
    u32 count;
    UnkRefCounted* items[128];
};

struct UnkRefContainer {
    UnkRefBucket* buckets;
    UnkRefCounted** flat;
    u32 count;
};

static inline void DropRef(UnkRefCounted* p) {
    if (p && --p->refCount == 0)
        p->vtable->release(p);
}

void fn_7100d4a418(UnkRefContainer* self) {
    if (self->flat && self->count != 0) {
        for (u64 i = 0; i < self->count; i++)
            DropRef(self->flat[i]);
    }
    if (self->buckets != nullptr && self->count != 0) {
        for (u64 i = 0; i < self->count; i++) {
            UnkRefBucket* b = self->buckets + i;
            u64 j = 0;
            while (j < b->count)
                DropRef(b->items[j++]);
            b->count = 0;
        }
    }
    self->count = 0;
}

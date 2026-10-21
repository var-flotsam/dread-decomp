// Destructors (complete and deleting) of a fixed ring of 64 entries (string id + 8 bytes): restores the vtable,
// destroys the entries from head to tail, wrapping at the end of the buffer, and empties the
// ring.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id whose release decrements and tests the count in two steps.
struct UnkRingId {
    UnkStringData* data;

    // The destructor's body; the ring destroys only the live entries.
    void Release() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkRingEntry_0x10 {
    UnkRingId id;
    u64 field_0x8;
};

extern void* g_unk_7101caa1d8[];

struct UnkIdRing_7100ef1320 {
    void** vtable;
    UnkRingEntry_0x10* head;
    UnkRingEntry_0x10* tail;
    u64 count;
    UnkRingEntry_0x10 entries[64];

    __attribute__((always_inline)) ~UnkIdRing_7100ef1320() {
        vtable = &g_unk_7101caa1d8[2];
        for (UnkRingEntry_0x10* p = head; p != tail;) {
            p->id.Release();
            ++p;
            p = p == entries + 64 ? entries : p;
        }
        head = entries;
        tail = entries;
        count = 0;
    }
};

void fn_7100ef1320(UnkIdRing_7100ef1320* self) {
    self->~UnkIdRing_7100ef1320();
}

// The deleting destructor: the ring's reset is dropped, as the memory is freed.
void fn_7100ef13f0(UnkIdRing_7100ef1320& self) {
    delete &self;
}

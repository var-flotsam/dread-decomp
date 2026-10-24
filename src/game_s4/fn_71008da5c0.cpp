// Destructors of static arrays of string ids (registered with atexit by their static
// initialisers): the elements are released last to first, each only while the pool exists.
// The same shape as slice 5's (src/game_aca57c/fn_7100c9c750.cpp).
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id whose release decrements and tests the count in two steps.
template <UnkStringPool** Pool>
struct UnkStaticStrId {
    UnkStringData* data;

    ~UnkStaticStrId() {
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
using UnkStaticPathId = UnkStaticStrId<&g_unk_7101d47200>;
using UnkStaticId = UnkStaticStrId<&g_unk_7101d47108>;

// An 8-byte value followed by a string id.
struct UnkStaticValueId {
    u64 value;
    UnkStaticId id;
};

template <class T, int N, T (&Array)[N]>
static inline void DestroyArray() {
    for (int i = N; i-- > 0;)
        Array[i].~T();
}

#define ARRAY_DTOR(addr, T, n, array)                                                          \
    extern T array[n] __attribute__((visibility("hidden")));                                   \
    void fn_##addr() { DestroyArray<T, n, array>(); }

ARRAY_DTOR(71008da5c0, UnkStaticPathId, 3, g_unk_7172731840)
ARRAY_DTOR(71008ded60, UnkStaticPathId, 3, g_unk_7172732068)
ARRAY_DTOR(7100a8cc60, UnkStaticId, 8, g_unk_7172751a20)
ARRAY_DTOR(7100a8cee0, UnkStaticValueId, 8, g_unk_7172751a60)
ARRAY_DTOR(7100aad680, UnkStaticId, 12, g_unk_7172753e60)

// The same release over string-id members, last to first.
struct UnkIdRecord_0x68 {
    UnkStaticId ids[13];
};

void fn_7100964b64(UnkIdRecord_0x68* self) {
#pragma clang loop unroll(full)
    for (int i = 13; i-- > 0;)
        self->ids[i].~UnkStaticId();
}

struct UnkIdRecord_0x28 {
    UnkStaticId ids[5];
};

void fn_710086e288(UnkIdRecord_0x28* self) {
    for (int i = 5; i-- > 0;)
        self->ids[i].~UnkStaticId();
}

extern void* g_unk_7101a89160[];  // vtable

struct UnkIdObject {
    void* vtable;
    u8 pad_0x8[0x50];
    UnkStaticPathId field_0x58;
    UnkStaticId ids[6];
};

void fn_71011acd74(UnkIdObject* self);  // base destructor

// A destructor: releases the ids, then runs the base destructor.
void fn_71009cc6cc(UnkIdObject* self) {
    self->vtable = g_unk_7101a89160 + 2;
    for (int i = 6; i-- > 0;)
        self->ids[i].~UnkStaticId();
    self->field_0x58.~UnkStaticPathId();
    fn_71011acd74(self);
}

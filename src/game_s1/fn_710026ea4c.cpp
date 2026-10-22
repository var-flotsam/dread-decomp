// Copies of fn_710116ba70 and fn_710116baf4 (src/game_7100ece7fc/fn_710116baf4.cpp) for another object, naming their
// own callees and data (the comments are adapted from the original's): fn_710026ea4c, fn_710026eb5c. Destructors
// (complete and deleting) of an object with a member at 0x20 destroyed out of line (fn_710026ead0) and a string id at
// 0x88: restore the vtable, release the id, destroy the member and run the base destructor (fn_710026a9fc); the
// deleting one then frees the object.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkBase_0x20 {
    void** vtable;
    u8 unk_0x8[0x18];
    ~UnkBase_0x20();  // fn_710026a9fc
};

void fn_710026ead0(void* self);

struct UnkMember_0x68 {
    u8 unk_0x0[0x68];
    ~UnkMember_0x68() { fn_710026ead0(this); }
};

// A string id whose release decrements and tests the count in two steps.
struct UnkEntryId {
    UnkStringData* data;

    ~UnkEntryId() {
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

extern void* g_unk_7101a20da8[];

struct UnkObject_710026eb5c : UnkBase_0x20 {
    UnkMember_0x68 field_0x20;
    UnkEntryId field_0x88;

    ~UnkObject_710026eb5c() { vtable = &g_unk_7101a20da8[2]; }
};
static_assert(__builtin_offsetof(UnkObject_710026eb5c, field_0x88) == 0x88, "");

void fn_710026ea4c(UnkObject_710026eb5c& self) {
    self.~UnkObject_710026eb5c();
}

void fn_710026eb5c(UnkObject_710026eb5c& self) {
    delete &self;
}

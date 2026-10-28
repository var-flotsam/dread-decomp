// Destructors (complete and deleting) of an object with a name string at 0x40 and a string id
// at 0x70: restore the vtable, release the id, destroy the string and run the base destructor
// (fn_71010c4194); the deleting one then frees the object.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkBase_0x40 {
    void** vtable;
    u8 unk_0x8[0x38];
    ~UnkBase_0x40();  // fn_71010c4194
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

extern void* g_unk_7101cb41c0[];

struct UnkObject_710116baf4 : UnkBase_0x40 {
    CRntString field_0x40;
    UnkEntryId field_0x70;

    ~UnkObject_710116baf4() { vtable = &g_unk_7101cb41c0[2]; }
};

void fn_710116ba70(UnkObject_710116baf4& self) {
    self.~UnkObject_710116baf4();
}

void fn_710116baf4(UnkObject_710116baf4& self) {
    delete &self;
}

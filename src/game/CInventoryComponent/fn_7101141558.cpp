// Deleting destructor: restores the vtable, runs the class's cleanup (fn_7101141378), destroys
// the member at 0x90 (fn_71009168e0), the string id at 0x88 and the member at 0x40
// (fn_7101141468), runs the base destructor (fn_71010c4194) and frees the object.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkBase_0x40 {
    void** vtable;
    u8 unk_0x8[0x38];
    ~UnkBase_0x40() asm("fn_71010c4194");  // fn_71010c4194
};

struct UnkMember_0x48 {
    u8 unk_0x0[0x48];
    ~UnkMember_0x48() asm("fn_7101141468");  // fn_7101141468
};

struct UnkMember_0x90 {
    u8 unk_0x0[0x10];
    ~UnkMember_0x90() asm("fn_71009168e0");  // fn_71009168e0
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

extern void* g_unk_7101cb33c8[];

struct UnkObject_7101141558;
void fn_7101141378(UnkObject_7101141558* self);

struct UnkObject_7101141558 : UnkBase_0x40 {
    UnkMember_0x48 field_0x40;
    UnkEntryId field_0x88;
    UnkMember_0x90 field_0x90;

    ~UnkObject_7101141558() {
        vtable = &g_unk_7101cb33c8[2];
        fn_7101141378(this);
    }
};

void fn_7101141558(UnkObject_7101141558& self) {
    delete &self;
}

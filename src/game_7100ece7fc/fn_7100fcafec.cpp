// Destructors (complete and deleting) of an object holding an array of (8 bytes, string id) entries: restores
// the vtable, releases the ids and frees the array, then runs the base destructor (which resets
// and destroys its name string, fn_710007ed40 and CRntString's destructor) and frees the object.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStringData;
using base::global::UnkStringPool;

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

struct UnkIdEntry_0x10 {
    u64 field_0x0;
    UnkEntryId id;
};

extern void* g_unk_7101caba40[];
extern void* g_unk_7101cabad0[];

struct UnkNamedBase_0x38 {
    void** vtable;
    CRntString name;

    __attribute__((always_inline)) ~UnkNamedBase_0x38() {
        vtable = &g_unk_7101cabad0[2];
        fn_710007ed40(&name);
    }
};

struct UnkIdArrayOwner_7100fcafec : UnkNamedBase_0x38 {
    u8 unk_0x38[0x98];
    base::UnkVector<UnkIdEntry_0x10> entries;

    __attribute__((always_inline)) ~UnkIdArrayOwner_7100fcafec() {
        vtable = &g_unk_7101caba40[2];
        entries.Free();
    }
};

void fn_7100fcaeec(UnkIdArrayOwner_7100fcafec& self) {
    self.~UnkIdArrayOwner_7100fcafec();
}

void fn_7100fcafec(UnkIdArrayOwner_7100fcafec& self) {
    delete &self;
}

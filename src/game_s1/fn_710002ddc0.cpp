// Destructors (complete and deleting) of a reflection class (vtable unk_7101a19fc0) holding a
// string id at 8 and a string (base::global::CRntString) at 0x10: fn_710002ddc0, fn_710002de3c.
// The deleting one frees the object without resetting the id to the empty string.
#include "base/UnkDestructors.h"
#include "base/global/CRntString.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

extern char unk_7101a19fc0[];

struct UnkNamedId_710002ddc0 {
    char* vtable;
    base::UnkStrIdMember id;
    base::global::CRntString name;

    __attribute__((always_inline)) ~UnkNamedId_710002ddc0() { vtable = unk_7101a19fc0 + 0x10; }
};

void fn_710002ddc0(UnkNamedId_710002ddc0* self) { self->~UnkNamedId_710002ddc0(); }

void fn_710002de3c(UnkNamedId_710002ddc0* self) {
    self->vtable = unk_7101a19fc0 + 0x10;
    self->name.~CRntString();
    if (UnkStringPool* pool = g_unk_7101d47108) {
        UnkStringData*& data = self->id.data;
        if (data && data != &pool->empty) {
            --data->ref_count;
            if (data->ref_count == 0)
                fn_71000803a4(pool, data);
        }
    }
    operator delete(self);
}

// Unit 1115: adds a name and an id to an object's lists unless already present.
#include "base/UnkCloneRecords.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkRegistry_b0255;

struct UnkRegObj_b0255 {
    void* vtable;                    // 0x0
    u32 id;                          // 0x8
    u32 field_0xc;                   // 0xc
    base::global::CStrId name;       // 0x10
    base::UnkVector<u32> ids;        // 0x18
    base::UnkStrIdVec names;         // 0x40
    UnkRegistry_b0255* registry;     // 0x68
};
static_assert(__builtin_offsetof(UnkRegObj_b0255, registry) == 0x68, "layout");

void fn_71006f8944(UnkRegObj_b0255* self, const base::global::CStrId* name, u32 id) {
    {
        u64 n = self->names.count;
        const base::global::CStrId* p = self->names.data;
        for (; n; --n, ++p)
            if (p->data == name->data)
                goto have_name;
        self->names.PushBack(*name);
    have_name:;
    }
    {
        u64 n = self->ids.count;
        const u32* p;
        if (n)
            __builtin_memcpy(&p, &self->ids.data, 8);
        for (; n; --n, ++p)
            if (*p == id)
                return;
        self->ids.PushBack(id);
    }
}

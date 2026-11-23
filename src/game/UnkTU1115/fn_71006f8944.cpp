// Unit 1115: adds a name and an id to an object's lists unless already present.
#include "base/UnkCloneRecords.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkRegistry_b0255;

struct UnkRegObj_l6f8944 {
    void* vtable;                    // 0x0
    u32 id;                          // 0x8
    u32 field_0xc;                   // 0xc
    base::global::CStrId name;       // 0x10
    base::global::CRntVector<u32> ids;        // 0x18
    base::UnkStrIdVec names;         // 0x40
    UnkRegistry_b0255* registry;     // 0x68
};
static_assert(__builtin_offsetof(UnkRegObj_l6f8944, registry) == 0x68, "layout");

static inline bool HasName(const base::UnkStrIdVec* names, const base::global::CStrId* name) {
    const base::global::CStrId* p = names->data;
    for (u64 n = names->count; n; --n, ++p)
        if (p->data == name->data)
            return true;
    return false;
}

void fn_71006f8944(UnkRegObj_l6f8944* self, const base::global::CStrId* name, u32 id) {
    if (!HasName(&self->names, name))
        self->names.PushBack(*name);
    if (u64 n = self->ids.count) {
        for (const u32* p = self->ids.data; n; --n, ++p)
            if (*p == id)
                return;
    }
    self->ids.PushBack(id);
}

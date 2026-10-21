// Builds a handle from a string id: stores a copy of the id and, unless it is empty, the
// object fn_7100ae09a4 looks up for it in a global registry.
#include <new>

#include "base/global/CStrId.h"

using base::global::CStrId;

extern void* g_unk_71726ad550;
void* fn_7100ae09a4(void* registry, CStrId id);

struct UnkStrIdHandle {
    CStrId id;
    void* field_0x8;
};

void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id) {
    // The id's default constructor: the empty string of the pool, created if needed.
    self->id.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->id.data = &g_unk_7101d47108->empty;
    self->id = id;
    if (id.data == &g_unk_7101d47108->empty)
        self->field_0x8 = nullptr;
    else
        self->field_0x8 = fn_7100ae09a4(g_unk_71726ad550, id);
}

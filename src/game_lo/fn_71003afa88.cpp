// Constructor of a small record: a name (copied from the argument), a second id set to a
// global default, a pointer and a float initialised to -1.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_7101d470e0;  // a global string id

struct UnkStruct_71003afa88 {
    CStrId name;
    CStrId field_0x8;
    void* field_0x10;
    f32 field_0x18;
};

// An empty string id: the default constructor, inlined.
static inline void InitEmptyStrId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_71003afa88(UnkStruct_71003afa88* self, const CStrId* name, void* ptr) {
    InitEmptyStrId(&self->name);
    InitEmptyStrId(&self->field_0x8);
    self->name = *name;
    self->field_0x8 = g_unk_7101d470e0;
    self->field_0x10 = ptr;
    self->field_0x18 = -1.0f;
}

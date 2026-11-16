// CBlendSpaceResource unit: this unit's copy of the reader's object read (as fn_7100b09720): reads the
// object's name id, and, for a pointer, the id of the type it points to (the registered type replaces
// the object's own when there is one), then parses the object as that type. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "game/UnkTU80.h"
#include "game/UnkTU83.h"
#include "types.h"

using base::global::CRntFile;
using base::reflection::CType;
using base::reflection::UnkTypedRef;

struct UnkVersionEntry;
bool fn_710000fbb0(CRntFile* file, u32 kind, UnkTypedRef* value, const base::global::CStrId* name);

// Reads the id named `name` (kind 0 the object's own, 1 its pointee's type) into `id`.
static inline void ReadId(UnkFileReader* reader, u32 kind, UnkTypedRef* arg, u64* id) {
    arg->type = fn_710000fd7c();
    arg->ptr = id;
    fn_710000fbb0(reinterpret_cast<CRntFile*>(reader), kind, arg, &g_unk_7101d470e0);
}

void fn_710000f750(UnkParseContext* self, void* root) {
    base::reflection::UnkReflObject* obj = static_cast<base::reflection::UnkReflObject*>(root);
    UnkTypedRef arg;
    u64 name = (u64)-1;
    ReadId(self->reader, 0, &arg, &name);
    const CType* type = fn_710009c640(obj);
    if (fn_710009d50c(reinterpret_cast<const CType*>(obj->vf_0x0()),
                      reinterpret_cast<const CType*>(fn_710009bedc()))) {
        u64 id = (u64)-1;
        ReadId(self->reader, 1, &arg, &id);
        if (id != (u64)-1)
            type = reinterpret_cast<const CType*>(fn_71000a5118(g_unk_7101d4c8d0, &id));
    }
    fn_710000f850(self, reinterpret_cast<const CType*>(obj), const_cast<CType*>(type));
}

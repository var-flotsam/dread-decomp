// CActor unit (tools/tu_map.py, hint from reflection): a reflected getter (fn_710023e1ac) that answers,
// by reference, the string id at +0x98 of the actor's definition (+0x28, cast to CActorDef), or the
// global empty id when either of the actor's own ids at +0x18 (string pool) and +0x20 (file-path
// pool) is empty. The cast's null is not checked before taking the field.
// Names are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

struct UnkActorDef_b61 {
    u8 pad_0x0[0x98];
    base::global::CStrId field_0x98;
};

struct UnkActorIds_b61 {
    u8 pad_0x0[0x18];
    base::global::UnkStringData* name;  // 0x18
    base::global::UnkStringData* path;  // 0x20
    UnkReflObject* def;                 // 0x28
};

UnkReflValue fn_710023e1ac(void*, char* obj) {
    auto* self = *reinterpret_cast<UnkActorIds_b61**>(obj + 8);
    const base::global::CStrId* value = &g_unk_7101d470e0;
    if (self->name != &g_unk_7101d47108->empty && self->path != &g_unk_7101d47200->empty) {
        UnkReflObject* def = self->def;
        if (def && !fn_710009d50c(reinterpret_cast<const CType*>(def->vf_0x0()),
                                  reinterpret_cast<const CType*>(fn_7100254b14())))
            def = nullptr;
        value = &reinterpret_cast<UnkActorDef_b61*>(def)->field_0x98;
    }
    const CType* type = fn_7100009658();
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    fn_71000a18d4(&r, type, value, true);
    return r;
}

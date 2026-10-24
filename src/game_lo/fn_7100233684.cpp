// Looks up the component registered under the id "MODELUPDATER" and returns it when it is a
// CModelUpdaterComponent, else null.
#include "base/UnkCloneCalls.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::reflection::CClass;
using base::reflection::UnkReflObject;

const CClass* fn_710116f89c();  // CModelUpdaterComponent::refl_GetClass

struct UnkTag_7100233684;

struct UnkStruct_7100233684 {
    u8 unk_0x0[0xa0];
    base::UnkKeyedArray<UnkTag_7100233684, UnkStringData*, UnkReflObject*> components;
};

// The id lives in an inline function: its guard and object are reached through the GOT.
inline const CStrId& ModelUpdaterId_7100233684() {
    static CStrId s_id("MODELUPDATER", true);
    return s_id;
}

UnkReflObject* fn_7100233684(UnkStruct_7100233684* self) {
    UnkReflObject** p = self->components.Find(ModelUpdaterId_7100233684().data);
    if (p == self->components.End())
        return nullptr;
    UnkReflObject* c = *p;
    if (!c)
        return nullptr;
    return fn_710009d50c(c->vf_0x0(), fn_710116f89c()) ? c : nullptr;
}

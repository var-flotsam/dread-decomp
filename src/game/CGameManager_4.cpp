// CGameManager (unit of CGameManager::CTunableGameManager): clears a flag on every trigger.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

namespace {
struct ObjRef {
    u8 pad_0x0[0x10];
    void* obj;
};
struct ReflObj {
    virtual const base::reflection::CType* vf_0x0();
};
struct TriggerView {
    u8 pad_0x0[0x240];
    bool field_0x240;
};
}  // namespace

struct UnkHandleList;
UnkHandleList* fn_7100a5e1dc();

void fn_7100e01fc8() {
    struct List {
        ObjRef** data;
        u32 count;
    };
    List* triggers = reinterpret_cast<List*>(fn_7100a5e1dc());
    for (ObjRef **it = triggers->data, **end = it + triggers->count; it != end; ++it) {
        auto* o = static_cast<ReflObj*>((*it)->obj);
        auto* t = fn_710009d50c(o->vf_0x0(), fn_7100a5bab0()) ? reinterpret_cast<TriggerView*>(o) : nullptr;
        t->field_0x240 = false;
    }
}

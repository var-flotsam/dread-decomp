// From unit 2654 of tools/tu_map.py (CFXPool::CDefinition): the definition's resource, when it is a
// base::res::CRes, as the base at +0x10. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

struct UnkHolder_710104de9c {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* res;  // +0x10
};

struct UnkOwner_710104de9c {
    u8 pad_0x0[0x10];
    UnkHolder_710104de9c** holder;  // +0x10
};

char* fn_710104de9c(UnkOwner_710104de9c* self) {
    base::reflection::UnkReflObject* res = (*self->holder)->res;
    base::reflection::UnkReflObject* r = nullptr;
    if (res && fn_710009d50c(res->vf_0x0(), fn_71000e8a5c()))
        r = res;
    return reinterpret_cast<char*>(r) + 0x10;
}

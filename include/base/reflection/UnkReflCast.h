#pragma once

// Checked casts of reflected objects: the object's type (vtable slot 0) is tested against a
// class descriptor with fn_710009d50c (an is-kind-of test). Overrides that call their base and
// then cache a cast pointer are instantiated per copy by tools/gen_clones.py. Names are
// placeholders inferred from the code.

#include "types.h"

namespace base::reflection {

class CType;

struct UnkReflObject {
    virtual const CType* vf_0x0();  // the object's dynamic type
};

}  // namespace base::reflection

bool fn_710009d50c(const base::reflection::CType* type, const base::reflection::CType* base);

namespace base::reflection {

// type(): the target class's descriptor; base(...): the overridden method.
template <class Tag>
struct UnkCastTraits;

template <class Tag>
UnkReflObject* refl_CastTo(UnkReflObject* obj) {
    return fn_710009d50c(obj->vf_0x0(), UnkCastTraits<Tag>::type()) ? obj : nullptr;
}

// Calls the base method, then caches the argument at `Offset` if it is non-null, cast to the
// target class (fn_71003bdae8 and its copies).
template <class Tag, u32 Offset>
void refl_CallAndCastArg(char* self, UnkReflObject* arg) {
    UnkCastTraits<Tag>::base(self, arg);
    if (arg)
        *reinterpret_cast<UnkReflObject**>(self + Offset) = refl_CastTo<Tag>(arg);
}

// Calls the base method, then caches the object at `Src` at `Dst`, cast to the target class
// (fn_71002fd6bc and its copies).
template <class Tag, u32 Src, u32 Dst>
bool refl_CallAndCastField(char* self) {
    bool r = UnkCastTraits<Tag>::base(self);
    UnkReflObject* o = *reinterpret_cast<UnkReflObject**>(self + Src);
    *reinterpret_cast<UnkReflObject**>(self + Dst) = o ? refl_CastTo<Tag>(o) : nullptr;
    return r;
}

}  // namespace base::reflection

#pragma once

// Checked casts of reflected objects (UnkReflObject, CClass.h): the object's class (vtable
// slot 0) is tested against a class descriptor with fn_710009d50c (an is-kind-of test). Overrides that call their base and
// then cache a cast pointer are instantiated per copy by tools/gen_clones.py. Names are
// placeholders inferred from the code.

#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

// The is-kind-of test: whether `type` is `base` or derives from it, walking CType::m_parent
// (src/base/reflection/fn_710009d50c.cpp). Canonical declaration: include this header, never redeclare.
bool fn_710009d50c(const base::reflection::CType* type, const base::reflection::CType* base);
bool fn_710009d52c(const base::reflection::CType* type, const base::reflection::CType* current);
void fn_71000a1c60(base::reflection::UnkReflValue* value);
void fn_71000a1cc8(base::reflection::UnkReflValue* value, const base::reflection::CType* type, const void* object);

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

// Stores `obj` with its dynamic type in the reflected value at +0x18: in place when the value
// already holds a compatible type (fn_71000a1cc8), else by resetting and rebuilding it
// (fn_71002fa764 and its identical copies).
template <class Tag>
void refl_SetDynamicValue(char* self, UnkReflObject* obj) {
    UnkReflValue* v = reinterpret_cast<UnkReflValue*>(self + 0x18);
    const CType* t = obj->vf_0x0();
    const CType* cur = reinterpret_cast<const CType*>(v->type);
    if (cur && v->field_0x14 == 0 &&
        (cur == t ? *reinterpret_cast<void* const*>(reinterpret_cast<const char*>(t) + 0x48) != nullptr
                  : fn_710009d52c(t, cur))) {
        fn_71000a1cc8(v, t, obj);
    } else {
        fn_71000a1c60(v);
        fn_71000a18d4(v, t, obj, false);
    }
}

}  // namespace base::reflection

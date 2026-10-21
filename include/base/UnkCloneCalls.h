#pragma once

// Small repeated functions built around one or two calls, matched one family at a time
// (tools/gen_clones.py scan and reference kinds). The callees differ per copy and come from
// UnkCallTraits<Tag>; offsets are template arguments. Layouts and names are placeholders
// inferred from the code.

#include "UnkClone.h"
#include "base/UnkVector.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

void fn_71001eef74(char* owner, char* self, bool flag);

namespace base {

// The callees of one copy (each kind uses some of): call(...), get(self), base(self),
// member(p), vtable(), cls(), type().
template <class Tag>
struct UnkCallTraits;

// Sets the flag at 0xf6 and, when it changed and there is an owner at +8, tells the owner,
// passing the flag only if the one at 0xf9 is set too (fn_71001f31e8 and its identical copies).
template <class Tag>
void refl_SetFlagNotify(char* self, bool v) {
    if (*reinterpret_cast<bool*>(self + 0xf6) == v)
        return;
    *reinterpret_cast<bool*>(self + 0xf6) = v;
    if (char* o = *reinterpret_cast<char**>(self + 8))
        fn_71001eef74(o, self, *reinterpret_cast<bool*>(self + 0xf9) && v);
}

// Sets the flag at 0xf7; clearing it calls virtual slot 0x60 with true (fn_71001f3344 and its
// identical copies).
template <class Tag>
void refl_SetFlagVirtual(char* self, bool v) {
    if (*reinterpret_cast<bool*>(self + 0xf7) == v)
        return;
    *reinterpret_cast<bool*>(self + 0xf7) = v;
    if (v)
        return;
    reinterpret_cast<void (**)(char*, bool)>(*reinterpret_cast<char**>(self))[12](self, true);
}

// Passes get(self) to the child at +0x220 of the object at `Offset`, or returns false when
// there is none (fn_7100551548 and its copies).
template <class Tag, u32 Offset>
bool refl_CallWithOwnerChild(char* self) {
    char* p = *reinterpret_cast<char**>(*reinterpret_cast<char**>(self + Offset) + 0x220);
    if (!p)
        return false;
    return UnkCallTraits<Tag>::call(p, UnkCallTraits<Tag>::get(self));
}

// A constructor: the base's, the class's vtable, a member's at `Offset`, then the virtual at
// slot 0x18 (fn_71003ba70c and its copies).
template <class Tag, u32 Offset>
void refl_CtorThenInit(char* self) {
    UnkCallTraits<Tag>::base(self);
    *reinterpret_cast<char**>(self) = UnkCallTraits<Tag>::vtable() + 0x10;
    UnkCallTraits<Tag>::member(self + Offset);
    reinterpret_cast<void (**)(char*)>(*reinterpret_cast<char**>(self))[3](self);
}

// Calls call(o) on the object at `Offset` in the one `self` points to at +8 when it is a
// cls() (fn_7100427fb4 and its copies); the second form reads the outer pointer from its
// second argument.
template <class Tag, u32 Offset>
void refl_CastThenCall(char* self) {
    char* o = *reinterpret_cast<char**>(*reinterpret_cast<char**>(self + 8) + Offset);
    if (!o)
        return;
    if (fn_710009d50c(reinterpret_cast<reflection::UnkReflObject*>(o)->vf_0x0(), UnkCallTraits<Tag>::cls()))
        UnkCallTraits<Tag>::call(o);
}

template <class Tag, u32 Offset>
void refl_CastThenCallArg(void*, char* obj) {
    char* o = *reinterpret_cast<char**>(*reinterpret_cast<char**>(obj) + Offset);
    if (!o)
        return;
    if (fn_710009d50c(reinterpret_cast<reflection::UnkReflObject*>(o)->vf_0x0(), UnkCallTraits<Tag>::cls()))
        UnkCallTraits<Tag>::call(o);
}

// Finds the first element whose leading u32 equals the key (argument K, counted from 0) and
// forwards all arguments to call() with it (fn_71001fc7bc and its copies).
template <class Tag, u32 K, class... A>
void refl_FindByIdCall(UnkVector<u32*>* v, A... a) {
    u64 args[] = {(u64)a...};
    u32 key = (u32)args[K];
    u32* p = nullptr;
    for (u32 **it = v->data, **end = v->data + v->count; it != end; ++it)
        if (**it == key) {
            p = *it;
            break;
        }
    if (p)
        UnkCallTraits<Tag>::call(p, a...);
}

// A reflected method: passes its argument's value (null without a type) to call() and returns
// the s32 1 as a value of type() (fn_71007af1f0 and its copies).
template <class Tag>
reflection::UnkReflValue refl_CallArgReturnOne(void*, char* obj, reflection::UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    reflection::UnkReflValue* arg = args[0];
    const void* p = arg->type ? reflection::refl_ArgValue<char>(arg) : nullptr;
    UnkCallTraits<Tag>::call(target, p);
    s32 one = 1;
    const reflection::CType* type = UnkCallTraits<Tag>::type();
    reflection::UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &one, false);
    return r;
}

}  // namespace base

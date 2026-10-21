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
// member(p), vtable(), base_vtable(), mid_vtable(), cls(), type(), dflt().
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

// The address of element i of an array of `Size`-byte elements; the index arrives as a u64 and
// is truncated (fn_7100246624 and its copies).
template <class Tag, u64 Size>
UnkCloneElem<Tag, Size>* refl_ElemAt(UnkVector<UnkCloneElem<Tag, Size>>* v, u64 i) {
    return &v->data[(u32)i];
}

// A copy constructor through three levels: the first base's vtable and 0x20 bytes, the second
// base's vtable and a word, the class's vtable and a word (fn_71005f2814 and its copies). The
// words are copied as bytes, so the vtable stores between them stay.
template <class Tag>
void refl_CopyCtor3(char* self, const char* other) {
    *reinterpret_cast<char**>(self) = UnkCallTraits<Tag>::base_vtable() + 0x10;
    __builtin_memcpy(self + 8, other + 8, 0x20);
    *reinterpret_cast<char**>(self) = UnkCallTraits<Tag>::mid_vtable() + 0x10;
    __builtin_memcpy(self + 0x28, other + 0x28, 8);
    *reinterpret_cast<char**>(self) = UnkCallTraits<Tag>::vtable() + 0x10;
    __builtin_memcpy(self + 0x30, other + 0x30, 8);
}

// A pointer and a u32 with a link to them and a state (1 and 2 are reset to 0); assignment
// links the copy when the pointer is set (fn_710023655c and its identical copies). The two
// resets are written differently, as the original's code differs: a range check, and a switch
// read through the raw offset (a shared access would be hoisted above the branch).
template <class Tag>
struct UnkLinkSlot {
    char* p;
    u32 b;
    u32* link;
    char* p2;
    u32 state;

    UnkLinkSlot& operator=(const UnkLinkSlot& o) {
        if (this != &o) {
            b = o.b;
            p = o.p;
            if (p) {
                if (state - 1 < 2u)
                    state = 0;
                state = 2;
                link = &b;
                p2 = p;
            } else {
                u32& st = *reinterpret_cast<u32*>(reinterpret_cast<char*>(this) + 0x20);
                switch (st) {
                case 1:
                    st = 0;
                    break;
                case 2:
                    st = 0;
                    break;
                }
            }
        }
        return *this;
    }
};

// A reflected f32 read: the f32 at `Offset` in what get() finds for the object `obj` points to
// at +8, or dflt() when nothing is found (fn_71011d5868 and its copies).
template <class Tag, u32 Offset>
reflection::UnkReflValue refl_GetLookupF32(void*, char* obj) {
    char* p = UnkCallTraits<Tag>::get(*reinterpret_cast<char**>(obj + 8));
    f32 v = p ? *reinterpret_cast<f32*>(p + Offset) : UnkCallTraits<Tag>::dflt();
    const reflection::CType* type = UnkCallTraits<Tag>::type();
    reflection::UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &v, false);
    return r;
}

}  // namespace base

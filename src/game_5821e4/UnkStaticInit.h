#pragma once

// Static initialisers of reflected component units: each unit has its own copy of two
// header-defined string ids (after a u32, 6), then its own statics (string ids built from the
// class name through fn_7100de8f78, or directly from strings), each destructor registered with
// __cxa_atexit, and a registration of the class (its getter called, then fn_710009ec00 adding
// an entry to a list). Names and layouts are placeholders.

#include <new>

#include "base/UnkAtExit.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflConverter.h"
#include "types.h"

void fn_710000051c(void* id);  // a string id's destructor
void fn_7100000c7c(void* id);  // a file path id's destructor
base::global::CStrId fn_7100de8f78(const base::global::CStrId& name);

namespace unk_static_init {

using base::global::CStrId;
using base::global::UnkStrIdBase;

// The header statics every unit repeats.
struct HeaderIds {
    u32 field_0x0;
    UnkStrIdBase field_0x8;
    UnkStrIdBase field_0x10;
};

__attribute__((always_inline)) inline void InitHeaderIds(HeaderIds* ids) {
    ids->field_0x0 = 6;
    new (&ids->field_0x8) UnkStrIdBase("binaries", true);
    __cxa_atexit(fn_710000051c, &ids->field_0x8, g_unk_7101a19000);
    new (&ids->field_0x10) UnkStrIdBase("sources", true);
    __cxa_atexit(fn_710000051c, &ids->field_0x10, g_unk_7101a19000);
}

// A string id built from a class name through fn_7100de8f78.
__attribute__((always_inline)) inline void InitClassId(UnkStrIdBase* id, const char* name) {
    {
        CStrId n(name, true);
        new (id) CStrId(fn_7100de8f78(n));
    }
    __cxa_atexit(fn_710000051c, id, g_unk_7101a19000);
}

// Storage of a file path id (its destructor is registered separately).
struct UnkFilePathStrIdBase {
    base::global::UnkStringData* data;
};

// A file path id built from a string.
__attribute__((always_inline)) inline void InitPathId(UnkFilePathStrIdBase* id, const char* str) {
    new (id) base::global::CFilePathStrId(str, true);
    __cxa_atexit(fn_7100000c7c, id, g_unk_7101a19000);
}

// A temporary string id (an element of an initializer list) whose release decrements and tests
// the count in two steps; fn_710058a070 builds a vector of ids from such a list.
struct UnkStrIdTmp : UnkStrIdBase {
    using UnkStrIdBase::UnkStrIdBase;
    ~UnkStrIdTmp() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

void fn_710058a070(void* self, const UnkStrIdBase* ids, u32 count);

// A string id built directly from a string.
__attribute__((always_inline)) inline void InitId(UnkStrIdBase* id, const char* str) {
    new (id) UnkStrIdBase(str, true);
    __cxa_atexit(fn_710000051c, id, g_unk_7101a19000);
}

// A reflected enum's type object (a global shared between units, so reached through the GOT),
// built from its name by its own constructor, then two conversions registered with
// fn_71000a16c4: from the enum to int and back. Layouts are placeholders.
struct UnkTypeInfo {
    u8 pad_0x0[0x30];
    void* field_0x30;
};

struct UnkEnumType {
    void* vtable;
    UnkTypeInfo* field_0x8;
};

// The conversion record and fn_71000a16c4 are declared in base/reflection/UnkReflConverter.h.
using UnkConversion = ::UnkConverter;

// A reflected type's getter (base/reflection/UnkReflAttributes.h declares the plain types'), as
// the helpers see the type object: through the placeholder layout above.
template <const base::reflection::CType* (*Getter)()>
__attribute__((always_inline)) inline UnkEnumType* TypeOf() {
    return reinterpret_cast<UnkEnumType*>(const_cast<base::reflection::CType*>(Getter()));
}

// A tunable object (a global reached through the GOT): its base constructor is out of line,
// then its own vtable is stored and its destructor registered.
struct UnkTunable {
    void** vtable;
};

__attribute__((always_inline)) inline void InitTunable(UnkTunable* tunable, void (*ctor)(UnkTunable*),
                                                       void** vtable, void (*dtor)(void*)) {
    ctor(tunable);
    tunable->vtable = vtable + 2;
    __cxa_atexit(dtor, tunable, g_unk_7101a19000);
}

// A named property object (a global reached through the GOT) linked into a global list on
// construction; once built, it is checked through its vtable (slots 2 and 3) and, if those
// allow it, handed to the unit's own function. Layout and slot roles are placeholders.
struct UnkProperty {
    void** vtable;
    UnkProperty* next;
    UnkFilePathStrIdBase name;  // storage of a string id
    const void* type;
    u32 field_0x20;

    __attribute__((always_inline)) UnkProperty(void** vt, UnkProperty*& head, const char* str, const void* t) {
        next = head;
        head = this;
        vtable = vt + 2;
        new (&name) UnkStrIdBase(str, true);
        type = t;
        field_0x20 = 0;
    }
    bool vf_0x10() { return reinterpret_cast<bool (*)(UnkProperty*)>(vtable[2])(this); }
    bool vf_0x18() { return reinterpret_cast<bool (*)(UnkProperty*)>(vtable[3])(this); }
};

void fn_71002ef5bc(void* member);  // the base's member constructor

// A property of the same list whose base has an out-of-line member (fn_71002ef5bc) and whose
// own part holds a name, a pointer to the global it controls and a word.
struct UnkProperty2 {
    void** vtable;
    UnkProperty* next;
    u64 field_0x10[2];
    u64 field_0x20;
    UnkFilePathStrIdBase name;  // storage of a string id
    void* value;
    u32 field_0x38;

    bool vf_0x10() { return reinterpret_cast<bool (*)(UnkProperty2*)>(vtable[2])(this); }
    bool vf_0x18() { return reinterpret_cast<bool (*)(UnkProperty2*)>(vtable[3])(this); }
};

template <class Init>
__attribute__((always_inline)) inline void InitProperty2(UnkProperty2* p, void** base_vtable, UnkProperty*& head,
                                                         void** vtable, const char* name, void* value, Init on_init,
                                                         void (*dtor)(void*)) {
    p->next = head;
    head = reinterpret_cast<UnkProperty*>(p);
    p->vtable = base_vtable + 2;
    fn_71002ef5bc(p->field_0x10);
    p->vtable = vtable + 2;
    new (&p->name) UnkStrIdBase(name, true);
    p->value = value;
    p->field_0x38 = 0;
    if (p->vf_0x10() && !p->vf_0x18())
        on_init(p);
    __cxa_atexit(dtor, p, g_unk_7101a19000);
}

template <class Ctor>
__attribute__((always_inline)) inline void InitProperty(UnkProperty* p, void** vtable, UnkProperty*& head,
                                                        const char* name, const void* type, Ctor on_init,
                                                        void (*dtor)(void*)) {
    new (p) UnkProperty(vtable, head, name, type);
    if (p->vf_0x10() && !p->vf_0x18())
        on_init(p);
    __cxa_atexit(dtor, p, g_unk_7101a19000);
}

}  // namespace unk_static_init

void fn_7100099468(void* type);  // an enum type's destructor, as registered

namespace unk_static_init {

template <class Ctor, class Factory>
__attribute__((always_inline)) inline void InitEnum(UnkEnumType* type, const char* name, Ctor ctor,
                                                    Factory factory, char* list, UnkConversion* to_int,
                                                    void (*to_int_fn)(), UnkConversion* from_int,
                                                    void (*from_int_fn)()) {
    {
        CStrId n(name, true);
        ctor(type, n, factory, nullptr);
    }
    __cxa_atexit(fn_7100099468, type, g_unk_7101a19000);
    fn_710009ec00(list, type);
    void* from = type->field_0x8->field_0x30;
    *to_int = {from, TypeOf<fn_7100014768>()->field_0x8->field_0x30, to_int_fn, 0};
    fn_71000a16c4(to_int);
    void* i = TypeOf<fn_7100014768>()->field_0x8->field_0x30;
    *from_int = {i, type->field_0x8->field_0x30, from_int_fn, 0};
    fn_71000a16c4(from_int);
}

// The same for a reflected type built without a factory (a flag set), with its own destructor
// and conversions to and from another type (int_type).
template <class Ctor>
__attribute__((always_inline)) inline void InitFlagSet(UnkEnumType* type, const char* name, Ctor ctor,
                                                       void (*dtor)(void*), char* list, UnkEnumType* (*int_type)(),
                                                       UnkConversion* to_int, void (*to_int_fn)(),
                                                       UnkConversion* from_int, void (*from_int_fn)()) {
    {
        CStrId n(name, true);
        ctor(type, n, nullptr);
    }
    __cxa_atexit(dtor, type, g_unk_7101a19000);
    fn_710009ec00(list, type);
    void* from = type->field_0x8->field_0x30;
    *to_int = {from, int_type()->field_0x8->field_0x30, to_int_fn, 0};
    fn_71000a16c4(to_int);
    void* i = int_type()->field_0x8->field_0x30;
    *from_int = {i, type->field_0x8->field_0x30, from_int_fn, 0};
    fn_71000a16c4(from_int);
}

// A conversion between two reflected types, registered on its own.
__attribute__((always_inline)) inline void InitConversion(UnkConversion* c, UnkEnumType* (*from_type)(),
                                                          UnkEnumType* (*to_type)(), void (*fn)()) {
    void* from = from_type()->field_0x8->field_0x30;
    *c = {from, to_type()->field_0x8->field_0x30, fn, 0};
    fn_71000a16c4(c);
}

}  // namespace unk_static_init

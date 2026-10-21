#pragma once

// Static initialisers of reflected component units: each unit has its own copy of two
// header-defined string ids (after a u32, 6), then its own statics (string ids built from the
// class name through fn_7100de8f78, or directly from strings), each destructor registered with
// __cxa_atexit, and a registration of the class (its getter called, then fn_710009ec00 adding
// an entry to a list). Names and layouts are placeholders.

#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

extern "C" int __cxa_atexit(void (*dtor)(void*), void* obj, void* dso);
extern char g_unk_7101a19000[] __attribute__((visibility("hidden")));  // the module's DSO handle
void fn_710000051c(void* id);                                           // a string id's destructor
void fn_7100000c7c(void* id);  // a file path id's destructor
void fn_710009ec00(char* list, void* entry);
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

struct UnkConversion {
    void* from;
    void* to;
    void (*fn)();
    u64 field_0x18;
};

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

}  // namespace unk_static_init

unk_static_init::UnkEnumType* fn_7100014768();  // the int type's getter
void fn_71000a16c4(unk_static_init::UnkConversion* conversion);
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
    *to_int = {from, fn_7100014768()->field_0x8->field_0x30, to_int_fn, 0};
    fn_71000a16c4(to_int);
    void* i = fn_7100014768()->field_0x8->field_0x30;
    *from_int = {i, type->field_0x8->field_0x30, from_int_fn, 0};
    fn_71000a16c4(from_int);
}

}  // namespace unk_static_init

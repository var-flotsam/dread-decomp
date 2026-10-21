#pragma once

// Static initialisers of reflected component units: each unit has its own copy of two
// header-defined string ids (after a u32, 6), then its own statics (string ids built from the
// class name through fn_7100de8f78, or directly from strings), each destructor registered with
// __cxa_atexit, and a registration of the class (its getter called, then fn_710009ec00 adding
// an entry to a list). Names and layouts are placeholders.

#include <new>

#include "base/global/CStrId.h"
#include "types.h"

extern "C" int __cxa_atexit(void (*dtor)(void*), void* obj, void* dso);
extern char g_unk_7101a19000[] __attribute__((visibility("hidden")));  // the module's DSO handle
void fn_710000051c(void* id);                                           // a string id's destructor
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

// A string id built directly from a string.
__attribute__((always_inline)) inline void InitId(UnkStrIdBase* id, const char* str) {
    new (id) UnkStrIdBase(str, true);
    __cxa_atexit(fn_710000051c, id, g_unk_7101a19000);
}

}  // namespace unk_static_init

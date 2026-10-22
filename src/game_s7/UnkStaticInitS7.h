#pragma once

// Additions to slice 3's static-initialiser helpers (../game_5821e4/UnkStaticInit.h) for the
// larger units. Names and layouts are placeholders.
//
// The units here build each vector of ids from its list inside an immediately called lambda: in
// the original each global has its own initialiser function, inlined into the unit's, and the
// inliner then lets lists of the same length share one stack array (so their element addresses
// are computed once). One block per list in a single function gives each its own array.

#include "../game_5821e4/UnkStaticInit.h"

namespace unk_static_init {

// The header statics in units where a word sits between the u32 and the two ids.
struct HeaderIds2 {
    u32 field_0x0;
    u64 field_0x8;
    UnkStrIdBase field_0x10;
    UnkStrIdBase field_0x18;
};

__attribute__((always_inline)) inline void InitHeaderIds(HeaderIds2* ids) {
    ids->field_0x0 = 6;
    new (&ids->field_0x10) UnkStrIdBase("binaries", true);
    __cxa_atexit(fn_710000051c, &ids->field_0x10, g_unk_7101a19000);
    new (&ids->field_0x18) UnkStrIdBase("sources", true);
    __cxa_atexit(fn_710000051c, &ids->field_0x18, g_unk_7101a19000);
}


// The header statics in units whose merged globals put the u32 directly before the ids (4-byte
// aligned).
struct __attribute__((packed, aligned(4))) HeaderIds4 {
    u32 field_0x0;
    UnkStrIdBase field_0x4;
    UnkStrIdBase field_0xc;
};

__attribute__((always_inline)) inline void InitHeaderIds(HeaderIds4* ids) {
    ids->field_0x0 = 6;
    new (&ids->field_0x4) UnkStrIdBase("binaries", true);
    __cxa_atexit(fn_710000051c, &ids->field_0x4, g_unk_7101a19000);
    new (&ids->field_0xc) UnkStrIdBase("sources", true);
    __cxa_atexit(fn_710000051c, &ids->field_0xc, g_unk_7101a19000);
}

// The header statics in units built from the other order: "sources" first, the u32 last.
struct HeaderIdsRev {
    u32 field_0x0;
    UnkStrIdBase field_0x8;
    UnkStrIdBase field_0x10;
};

__attribute__((always_inline)) inline void InitHeaderIds(HeaderIdsRev* ids) {
    new (&ids->field_0x8) UnkStrIdBase("sources", true);
    __cxa_atexit(fn_710000051c, &ids->field_0x8, g_unk_7101a19000);
    new (&ids->field_0x10) UnkStrIdBase("binaries", true);
    __cxa_atexit(fn_710000051c, &ids->field_0x10, g_unk_7101a19000);
    ids->field_0x0 = 6;
}
}  // namespace unk_static_init

namespace unk_static_init {

// A conversion between a reflected type object of this unit and a type reached through its getter.
__attribute__((always_inline)) inline void InitConversion(UnkConversion* c, UnkEnumType* from_type,
                                                          UnkEnumType* (*to_type)(), void (*fn)()) {
    void* from = from_type->field_0x8->field_0x30;
    *c = {from, to_type()->field_0x8->field_0x30, fn, 0};
    fn_71000a16c4(c);
}

__attribute__((always_inline)) inline void InitConversion(UnkConversion* c, UnkEnumType* (*from_type)(),
                                                          UnkEnumType* to_type, void (*fn)()) {
    void* from = from_type()->field_0x8->field_0x30;
    *c = {from, to_type->field_0x8->field_0x30, fn, 0};
    fn_71000a16c4(c);
}

}  // namespace unk_static_init

namespace unk_static_init {

// The type object behind a named value: its hooks to set up a value (given the value's name) and
// to tear one down. Layout is a placeholder.
struct UnkValueType {
    u8 pad_0x0[0x30];
    void (*copy)(void* value, const void* from);
    void (*init)(void* value, const CStrId* name);
    void (*destroy)(void* value);
};

// A named value static: its type is fetched, any previous type's teardown is run, then the new
// type's set-up gets the name. Layout is a placeholder.
struct UnkNamedValue {
    u64 field_0x0;
    UnkValueType* type;
};

__attribute__((always_inline)) inline void InitNamedValue(UnkNamedValue* v, const char* name,
                                                          UnkEnumType* (*type_getter)(), void (*dtor)(void*)) {
    {
        CStrId n(name, true);
        v->type = nullptr;
        UnkValueType* t = reinterpret_cast<UnkValueType*>(type_getter());
        if (v->type) {
            v->type->destroy(v);
            v->type = nullptr;
        }
        v->type = t;
        if (t->init)
            t->init(v, &n);
    }
    __cxa_atexit(dtor, v, g_unk_7101a19000);
}

}  // namespace unk_static_init

namespace unk_static_init {

// A temporary copy of a named value, handed to an object's constructor.
struct UnkNamedValueTmp {
    u64 field_0x0;
    UnkValueType* type;

    __attribute__((always_inline)) explicit UnkNamedValueTmp(const UnkNamedValue& from) {
        type = nullptr;
        UnkValueType* t = from.type;
        if (t->copy) {
            type = t;
            t->copy(this, &from);
        }
    }
    __attribute__((always_inline)) ~UnkNamedValueTmp() {
        if (type) {
            type->destroy(this);
            type = nullptr;
        }
    }
};

// An object built from a named value of this unit (through a temporary copy) by its own
// constructor, with its destructor registered.
template <class T>
__attribute__((always_inline)) inline void InitFromNamedValue(T* obj, const UnkNamedValue& from,
                                                              void (*ctor)(T*, UnkNamedValueTmp*, bool),
                                                              void (*dtor)(void*)) {
    {
        UnkNamedValueTmp tmp(from);
        ctor(obj, &tmp, true);
    }
    __cxa_atexit(dtor, obj, g_unk_7101a19000);
}

}  // namespace unk_static_init

void fn_710007e6e4(void* self, void* inline_buffer);  // a small buffer's constructor
void fn_710007e77c(void* self, const char* str);      // assigns a string to it

namespace unk_static_init {

// A static of 0x80 bytes: three floats, a small buffer whose inline storage starts at +0x40
// (filled with "as"), and a few cleared words. Layout and roles are placeholders.
struct UnkBufferParam {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    u8 pad_0xc[4];
    u8 field_0x10[0x30];
    void** field_0x40;
    u8 pad_0x48[0x20];
    u64 field_0x68;
    u32 field_0x70;
    u8 pad_0x74[4];
    u32 field_0x78;
};

__attribute__((always_inline)) inline void InitBufferParam(UnkBufferParam* p, f32 a, f32 b, f32 c, void** vtable,
                                                           void (*dtor)(void*)) {
    p->field_0x0 = a;
    p->field_0x4 = b;
    p->field_0x8 = c;
    fn_710007e6e4(p->field_0x10, &p->field_0x40);
    p->field_0x40 = vtable + 2;
    p->field_0x68 = 0;
    p->field_0x70 = 0;
    fn_710007e77c(p->field_0x10, "as");
    p->field_0x78 = 0;
    __cxa_atexit(dtor, p, g_unk_7101a19000);
}

}  // namespace unk_static_init

namespace unk_static_init {

// An empty string id (the pool's empty string), with its destructor registered.
__attribute__((always_inline)) inline void InitEmptyId(UnkStrIdBase* id) {
    id->data = nullptr;
    if (!::g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &::g_unk_7101d47108->empty;
    __cxa_atexit(fn_710000051c, id, g_unk_7101a19000);
}

}  // namespace unk_static_init

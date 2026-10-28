#pragma once

// Helpers for the per-TU static initialisers (src/base/reflection/fn_7100b04900.cpp,
// fn_7100adcf10.cpp). Destructors are registered with fn_7100000250, the __cxa_atexit stub. Each
// TU first builds the same header data (a word and the "binaries" and "sources" ids, in some TUs
// followed by three function records), then registers its reflected types: a class, enum,
// flag-set or vector type built from its name (destroyed at exit) and noted in the TU's registry
// flags, and converter records registered with fn_71000a16c4. The TU data is laid out by the
// compiler; the structs below only mirror it.

#include <new>

#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflConverter.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTUSymbols.h"
#include "types.h"

using base::global::CStrId;

struct UnkReflTypeInfo {
    u8 unk_0x0[0x30];
    void* field_0x30;
};
struct UnkValue;
struct UnkReflType {
    void* vtable;
    UnkReflTypeInfo* info;
    u8 unk_0x10[0x28];
    void (*construct)(UnkValue* value, const void* src);  // copies a value of the type
    void (*destroy)(UnkValue* value);
};

// A reflected type's getter (base/reflection/UnkReflAttributes.h declares the plain types',
// base/reflection/UnkClassGetters.h the classes'), as the helpers see the type object: through
// the placeholder layout above. The getters return a pointer to const CType or CClass.
template <auto Getter>
__attribute__((always_inline)) static inline UnkReflType* refl_TypeOf() {
    return reinterpret_cast<UnkReflType*>(const_cast<void*>(static_cast<const void*>(Getter())));
}

// The type objects' destructors (fn_7100096604 and the others) come from UnkReflTypeDtors.h.

// The header data every TU's initialiser starts with.
struct UnkTUHeader {
    u32 field_0x0;
    CStrId binaries;
    CStrId sources;
};

__attribute__((always_inline)) static inline void InitHeader(UnkTUHeader& h) {
    h.field_0x0 = 6;
    new (&h.binaries) CStrId("binaries", true);
    fn_7100000250(fn_710000051c, &h.binaries, &g_unk_7101a19000);
    new (&h.sources) CStrId("sources", true);
    fn_7100000250(fn_710000051c, &h.sources, &g_unk_7101a19000);
}

// A class type: constructed from its name by `ctor` with its base type (if any) and an init
// function.
typedef void (*UnkClassCtor)(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(),
                             void* b);

__attribute__((always_inline)) static inline void InitClass(UnkReflType* type, const char* name, UnkClassCtor ctor,
                                                            void (*init)(), UnkReflType* base = nullptr) {
    {
        CStrId id(name, true);
        ctor(type, id, base, nullptr, init, nullptr);
    }
    fn_7100000250(fn_7100096604, type, &g_unk_7101a19000);
}

// An enum type: constructed from its name by `ctor` with an init function.
typedef void (*UnkEnumCtor)(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);

__attribute__((always_inline)) static inline void InitEnum(UnkReflType* type, const char* name, UnkEnumCtor ctor,
                                                           void (*init)()) {
    {
        CStrId id(name, true);
        ctor(type, id, init, nullptr);
    }
    fn_7100000250(fn_7100099468, type, &g_unk_7101a19000);
}

// A converter between two types (UnkConverter) is registered with fn_71000a16c4: both are
// declared in base/reflection/UnkReflConverter.h.

// The enums' integer type is fn_7100014768 (int); the flag sets' is fn_7100005178 (unsigned).
__attribute__((always_inline)) static inline void InitEnumConverters(UnkReflType* type, UnkConverter& to_int,
                                                                     void (*to_int_fn)(), UnkConverter& from_int,
                                                                     void (*from_int_fn)(),
                                                                     UnkReflType* (*int_type)() = refl_TypeOf<fn_7100014768>) {
    to_int = {type->info->field_0x30, int_type()->info->field_0x30, to_int_fn, nullptr};
    fn_71000a16c4(&to_int);
    from_int = {int_type()->info->field_0x30, type->info->field_0x30, from_int_fn, nullptr};
    fn_71000a16c4(&from_int);
}

// A flag-set type: constructed from its name by `ctor`.
typedef void (*UnkFlagSetCtor)(UnkReflType* self, const CStrId& name, void* extra);

__attribute__((always_inline)) static inline void InitFlagSet(UnkReflType* type, const char* name, UnkFlagSetCtor ctor) {
    {
        CStrId id(name, true);
        ctor(type, id, nullptr);
    }
    fn_7100000250(fn_710009a368, type, &g_unk_7101a19000);
}

// TU data that follows the header: the class type a getter builds, or other values.
struct UnkTUHeaderAndClass {
    UnkTUHeader header;
    u8 unk_0x18[0x8];
    UnkReflType type;
};

// TU data that starts with the header and three function records (the other words of a record
// start out zero), with a word before the records in some TUs and a class type after them.
struct UnkFnRecord {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    void (*fn)();
    u64 field_0x20;
    u64 field_0x28;
};

struct UnkTUHeaderFns {
    u32 field_0x0;
    CStrId binaries;
    CStrId sources;
    UnkFnRecord fns[3];
};

struct UnkTUHeaderFnsClass {
    u32 field_0x0;
    CStrId binaries;
    CStrId sources;
    u64 field_0x18;
    UnkFnRecord fns[3];
    UnkReflType type;
};

__attribute__((always_inline)) static inline void InitRecord(UnkFnRecord& r, void (*fn)()) {
    r.field_0x0 = 0;
    r.field_0x8 = 0;
    r.field_0x10 = 0;
    r.fn = fn;
    r.field_0x20 = 0;
    r.field_0x28 = 0;
}

template <class Header>
__attribute__((always_inline)) static inline void InitHeaderFns(Header& h) {
    h.field_0x0 = 6;
    new (&h.binaries) CStrId("binaries", true);
    fn_7100000250(fn_710000051c, &h.binaries, &g_unk_7101a19000);
    new (&h.sources) CStrId("sources", true);
    fn_7100000250(fn_710000051c, &h.sources, &g_unk_7101a19000);
    InitRecord(h.fns[0], fn_710000cf00);
    InitRecord(h.fns[1], fn_710000d470);
    InitRecord(h.fns[2], fn_710000d4f4);
}

// A vector type: constructed from its name by `ctor`.
typedef void (*UnkVectorCtor)(UnkReflType* self, const CStrId& name, void* a, void* b);

__attribute__((always_inline)) static inline void InitVector(UnkReflType* type, const char* name, UnkVectorCtor ctor) {
    {
        CStrId id(name, true);
        ctor(type, id, nullptr, nullptr);
    }
    fn_7100000250(fn_710009891c, type, &g_unk_7101a19000);
}

// A value of any reflected type: its storage and its type (destroyed at exit by fn_710010c424).
struct UnkValue {
    u64 storage;
    UnkReflType* type;

    template <class T>
    __attribute__((always_inline)) UnkValue(const T& v, UnkReflType* (*get_type)()) : type(nullptr) {
        UnkReflType* t = get_type();
        if (type) {
            type->destroy(this);
            type = nullptr;
        }
        type = t;
        if (t->construct)
            t->construct(this, &v);
    }
};

// A converter from `type` to the type `to` returns.
__attribute__((always_inline)) static inline void InitConverter(UnkConverter& c, UnkReflType* (*type)(),
                                                                UnkReflType* (*to)(), void (*fn)()) {
    c = {type()->info->field_0x30, to()->info->field_0x30, fn, nullptr};
    fn_71000a16c4(&c);
}

// A type kept in the TU data at `Offset`.
template <u32 Offset, class Header>
__attribute__((always_inline)) static inline UnkReflType* TUType(Header& h) {
    return reinterpret_cast<UnkReflType*>(reinterpret_cast<char*>(&h) + Offset);
}

// A converter from `type` to the type `to` returns, and back.
__attribute__((always_inline)) static inline void InitConverterTo(UnkConverter& c, UnkReflType* type,
                                                                  UnkReflType* (*to)(), void (*fn)()) {
    c = {type->info->field_0x30, to()->info->field_0x30, fn, nullptr};
    fn_71000a16c4(&c);
}
__attribute__((always_inline)) static inline void InitConverterFrom(UnkConverter& c, UnkReflType* type,
                                                                    UnkReflType* (*from)(), void (*fn)()) {
    c = {from()->info->field_0x30, type->info->field_0x30, fn, nullptr};
    fn_71000a16c4(&c);
}

// A value of type T kept in the TU data at `Offset`.
template <class T, u32 Offset, class Header>
__attribute__((always_inline)) static inline T* TUAt(Header& h) {
    return reinterpret_cast<T*>(reinterpret_cast<char*>(&h) + Offset);
}

// Named properties and tunables (UnkProperty, UnkPropertyEx<>, UnkTunable and their initialisers)
// are game/UnkTUProperty.h's, shared with the unk_static_init helpers.

// TU data whose layout is only known through offsets.
struct UnkTUData;

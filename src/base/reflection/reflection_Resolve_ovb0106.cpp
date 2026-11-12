// base::reflection's unit (tools/tu_map.py unit 1634): a one-argument accessor call with the argument
// converted to the accessor's type, a record's destructor, and the steps of resolving a path of
// arguments from the root object that come before and after a collection (fn_7100ad7db8 in
// src/base/reflection/UnkReflResolve.cpp does the rest). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntString.h"
#include "base/reflection.h"
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;
using base::reflection::UnkReflValueRef;

// A typed pointer that starts out null (returned in memory).
struct UnkReflResult_ovb0106 {
    const CType* type;
    void* ptr;

    UnkReflResult_ovb0106() : type(nullptr), ptr(nullptr) {}
    UnkReflResult_ovb0106(const UnkReflResult_ovb0106& other) : type(other.type), ptr(other.ptr) {}
};

// The object an accessor is called on: its type and where it is.
struct UnkReflTarget_ovb0106 {
    const void* type;
    void* ptr;
};

// An accessor taking one argument: the argument's type, the result's type and the function.
struct UnkReflAccessor_ovb0106 {
    u8 pad_0x0[0xe8];
    const CType* arg_type;     // 0xe8
    const CType* result_type;  // 0xf0
    u8 pad_0xf8[0x40];
    void* (*fn)(void* obj, const void* arg);  // 0x138
};

// Calls the accessor on `obj` (which must be of the accessor's own type) with `arg`, converted to
// the argument type when it differs and can be; the result typed, or null.
UnkReflResult_ovb0106 fn_7100ada848(UnkReflAccessor_ovb0106* self, const UnkReflTarget_ovb0106* obj,
                                    const UnkReflValueRef* arg) {
    UnkReflResult_ovb0106 r;
    if (!self->fn || obj->type != self)
        return r;
    void* result;
    if (self->arg_type == arg->type) {
        result = self->fn(obj->ptr, arg->value);
    } else {
        if (!fn_710009d52c(arg->type, self->arg_type))
            return r;
        UnkReflValue converted = fn_71000a1774(arg, self->arg_type);
        auto fn = self->fn;
        void* p = obj->ptr;
        const CType* t = reinterpret_cast<const CType*>(converted.type);
        const void* q = nullptr;
        if (t)
            q = converted.field_0x14 == 1 || t->m_size >= 9 ? converted.field_0x0 : &converted;
        result = fn(p, q);
        fn_71000a1c60(&converted);
    }
    if (result) {
        r.type = self->result_type;
        r.ptr = result;
    }
    return r;
}

// ---- fn_7100ad8864: a record's destructor ----

// A refcounted object deleted through its vtable.
struct UnkReflShared_ovb0106 {
    virtual void vf_0x0();
    virtual ~UnkReflShared_ovb0106();
    s32 ref_count;
};

struct UnkReflSharedPtr_ovb0106 {
    UnkReflShared_ovb0106* ptr;

    ~UnkReflSharedPtr_ovb0106() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

// Three strings, four ids and a fourth string, after a shared object.
struct UnkReflRecord_ovb0106 {
    u64 field_0x0;
    UnkReflSharedPtr_ovb0106 shared;  // 0x8
    u64 field_0x10;
    base::global::CRntString field_0x18;
    base::global::CRntString field_0x48;
    base::global::CRntString field_0x78;
    base::UnkStrIdMember field_0xa8;
    base::UnkStrIdMember field_0xb0;
    base::UnkStrIdMember field_0xb8;
    base::UnkStrIdMember field_0xc0;
    u8 pad_0xc8[0x18];
    base::global::CRntString field_0xe0;
};
static_assert(__builtin_offsetof(UnkReflRecord_ovb0106, field_0xe0) == 0xe0, "layout");

__attribute__((flatten)) void fn_7100ad8864(UnkReflRecord_ovb0106* self) { self->~UnkReflRecord_ovb0106(); }

// ---- fn_7100ad8120, fn_7100ad7cc0: resolving a path of arguments ----

// The value a typed reference holds, as a reflected value.
bool fn_710009d5d4(const char* type, base::reflection::UnkTypedRef* dst, const base::reflection::UnkTypedRef* src);

// A resolved value (0x30 bytes), as src/base/reflection/UnkReflResolve.cpp defines it, built here from a
// reflected value.
struct UnkResolvedPair {
    u64 a;
    u64 b;
};

struct UnkResolvedValue {
    UnkReflValue value;
    u64 field_0x18;
    UnkResolvedPair field_0x20;

    UnkResolvedValue() {
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        value.field_0x14 = 1;
        field_0x18 = 0;
        field_0x20.a = 0;
        field_0x20.b = 0;
    }
    explicit UnkResolvedValue(const UnkReflValue& v) {
        fn_71000a179c(&value, &v);
        field_0x18 = 0;
        field_0x20.a = 0;
        field_0x20.b = 0;
    }
    ~UnkResolvedValue() { fn_71000a1c60(&value); }
};

UnkResolvedValue fn_7100ad7db8(const UnkTypedRef* ref, void* a, u32 b);

// An argument of the path: a value and its type.
struct UnkReflPathArg_ovb0106 {
    u64 value;
    const CType* type;
};

struct UnkReflPath_ovb0106 {
    UnkReflPathArg_ovb0106* data;
    u32 count;
};

// The element of a collection the path's argument `index` names, resolved with the rest of the path.
UnkResolvedValue fn_7100ad8120(const UnkTypedRef* collection, void* a, u32 b) {
    UnkReflPath_ovb0106* path = static_cast<UnkReflPath_ovb0106*>(a);
    UnkReflPathArg_ovb0106* arg = &path->data[b];
    // Declared before the key (the original's stack order).
    alignas(8) u8 storage[sizeof(UnkReflResult_ovb0106)];
    UnkReflValueRef key = {arg->type, arg};
    const CType* type = collection->type;
    const CType* c =
        type && fn_710009d50c(reinterpret_cast<const UnkReflObject*>(type)->vf_0x0(), fn_71000986d0()) ? type : nullptr;
    UnkReflResult_ovb0106& element = *new (storage) UnkReflResult_ovb0106(
        fn_7100ada848(reinterpret_cast<UnkReflAccessor_ovb0106*>(const_cast<CType*>(c)),
                      reinterpret_cast<const UnkReflTarget_ovb0106*>(collection), &key));
    if (!element.type || !element.ptr)
        return UnkResolvedValue();
    if (b + 1 == path->count) {
        UnkReflValue v;
        fn_71000a1ae4(&v, reinterpret_cast<const UnkTypedRef*>(&element), 1, 2);
        UnkResolvedValue r(v);
        fn_71000a1c60(&v);
        return r;
    }
    return fn_7100ad7db8(reinterpret_cast<const UnkTypedRef*>(&element), a, b + 1);
}

// The argument the path's first one must convert to.
UnkReflPathArg_ovb0106* fn_7100ad2e20();

static inline UnkTypedRef RefOf(UnkReflPathArg_ovb0106* arg) {
    UnkTypedRef r;
    r.type = arg->type;
    r.ptr = arg;
    return r;
}

// Resolves a path of arguments from the root object (g_unk_7172757530, of type g_unk_7172756e50): the
// first argument must convert to fn_7100ad2e20's; a path of one names the root itself.
UnkResolvedValue fn_7100ad7cc0(UnkReflPath_ovb0106* path) {
    if (path->count == 0)
        return UnkResolvedValue();
    UnkReflPathArg_ovb0106* first = path->data;
    UnkReflPathArg_ovb0106* expected = fn_7100ad2e20();
    const CType* type = first->type;
    UnkTypedRef ref;
    ref.type = type;
    ref.ptr = first;
    UnkTypedRef src = RefOf(expected);
    if (!fn_710009d5d4(reinterpret_cast<const char*>(type), &ref, &src))
        return UnkResolvedValue();
    if (path->count == 1) {
        UnkReflValue v;
        v.field_0x0 = nullptr;
        v.type = nullptr;
        v.kind = 2;
        fn_71000a18d4(&v, &g_unk_7172756e50, g_unk_7172757530, 1);
        UnkResolvedValue r(v);
        fn_71000a1c60(&v);
        return r;
    }
    // The root object, in the first argument's place.
    ref.type = &g_unk_7172756e50;
    ref.ptr = g_unk_7172757530;
    return fn_7100ad7db8(&ref, path, 1);
}

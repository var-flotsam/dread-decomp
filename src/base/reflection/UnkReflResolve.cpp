// From unit 1634 of data/tu_map.csv (0x7100ad51e0-0x7100adc1d8, base::reflection by its names):
// the resolution of a typed reference into a value. A pointer is followed (its type's deref
// function at +0xf0) and the target resolved in turn; a collection and a class object each go to
// their own resolver (fn_7100ad8120, fn_7100ad8238, the class by the object's dynamic type); a
// result that is itself a pointer becomes a reference to the object it points to.
// Holds fn_7100ad7db8. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include <stdint.h>

#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflTypedPtr;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

// The reflected value's copy-assignment (declared as src/game_s1/fn_71000d6310.cpp does, over a
// record of UnkReflValue's size).
struct UnkAssignedRecord_a1ba4 {
    u64 unk_0x0[0x18 / 8];
};
void fn_71000a1ba4(UnkAssignedRecord_a1ba4* self, const UnkAssignedRecord_a1ba4* other);

void fn_71000a1760(base::reflection::UnkTypedRef* out, const base::reflection::UnkTypedRef* in);
// The object a pointer value points to, with its dynamic type.
UnkReflTypedPtr fn_710009c460(const UnkReflTypedPtr& pointer);
// The value a typed reference holds, as a reflected value.
void fn_71000a1ae4(UnkReflValue* out, const UnkTypedRef* ref, u32 a, u32 b);

// Two words assigned only from another object.
struct UnkResolvedPair {
    u64 a;
    u64 b;

    UnkResolvedPair& operator=(const UnkResolvedPair& o) {
        if (this != &o) {
            a = o.a;
            b = o.b;
        }
        return *this;
    }
};

// A resolved value (0x30 bytes): the reflected value and what came with it.
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
    UnkResolvedValue& operator=(const UnkResolvedValue& o) {
        fn_71000a1ba4(reinterpret_cast<UnkAssignedRecord_a1ba4*>(&value),
                      reinterpret_cast<const UnkAssignedRecord_a1ba4*>(&o.value));
        field_0x18 = o.field_0x18;
        field_0x20 = o.field_0x20;
        return *this;
    }
    ~UnkResolvedValue() { fn_71000a1c60(&value); }
};

UnkResolvedValue fn_7100ad8120(const UnkTypedRef* collection, void* a, u32 b);
UnkResolvedValue fn_7100ad8238(const UnkTypedRef* object, void* a, u32 b);

// A pointer type's descriptor as far as this reads it: the function that follows a pointer, at
// +0xf0. The original reads the slot even when the descriptor is not a pointer type (through a null
// descriptor, at address 0xf0), so it is read through an integer address.
struct UnkPointerType;
typedef UnkReflTypedPtr (*UnkPointerDeref)(void* pointer);

static inline UnkPointerDeref DerefOf(const UnkPointerType* type) {
    return *reinterpret_cast<const UnkPointerDeref*>(reinterpret_cast<uintptr_t>(type) + 0xf0);
}

// Whether `type` is a descriptor of the class `cls` returns (or of a derived one).
static inline bool IsKindOf(const CType* type, const base::reflection::CClass* (*cls)()) {
    return fn_710009d50c(reinterpret_cast<const UnkReflObject*>(type)->vf_0x0(), cls());
}

// The address of a reflected value's object: inline when it fits in 8 bytes, else through
// field_0x0; null for an empty value.
static inline void* ValueAddress(UnkReflValue* value) {
    if (!value->type)
        return nullptr;
    return value->field_0x14 == 1 || *reinterpret_cast<const u64*>(value->type + 0x18) > 8 ? value->field_0x0
                                                                                           : value;
}

static inline UnkReflTypedPtr NoPointee() {
    UnkReflTypedPtr none;
    none.type = nullptr;
    none.bits = 0;
    return none;
}

static inline UnkReflTypedPtr TypedAddress(UnkReflValue* value) {
    UnkReflTypedPtr r;
    r.type = const_cast<char*>(value->type);
    r.ptr = ValueAddress(value);
    return r;
}

// Rebuilds a value as a reference to `object` of type `type`.
static inline void SetRef(UnkReflValue* value, const CType* type, void* object) {
    fn_71000a1c60(value);
    fn_71000a18d4(value, type, object, 1);
}

// What the pointer `p` points to; nothing when its type is not a pointer type with a deref function
// (the deref slot is read before the type is checked, as in the original).
static inline UnkReflTypedPtr Pointee(const UnkTypedRef& p) {
    const CType* type = p.type;
    const UnkPointerType* pt =
        type && IsKindOf(type, fn_710009bedc) ? reinterpret_cast<const UnkPointerType*>(type) : nullptr;
    UnkPointerDeref deref = DerefOf(pt);
    return deref && reinterpret_cast<const UnkPointerType*>(p.type) == pt ? deref(p.ptr) : NoPointee();
}

static inline UnkTypedRef ToRef(const UnkReflTypedPtr& p) {
    UnkTypedRef r;
    r.type = reinterpret_cast<const CType*>(p.type);
    r.ptr = p.ptr;
    return r;
}

UnkResolvedValue fn_7100ad7db8(const UnkTypedRef* ref, void* a, u32 b) {
    UnkResolvedValue result;
    if (!ref->type || !ref->ptr)
        return result;
    if (IsKindOf(ref->type, fn_710009bedc)) {
        UnkTypedRef target;
        UnkTypedRef p;
        {
            UnkTypedRef src;
            fn_71000a16e0(&src, reinterpret_cast<const char*>(ref->type), ref->ptr);
            fn_71000a1760(&p, &src);
        }
        target = ToRef(Pointee(p));
        result = target.ptr && target.type ? fn_7100ad7db8(&target, a, b) : UnkResolvedValue();
    } else if (ref->type && IsKindOf(ref->type, fn_71000986d0)) {
        UnkTypedRef p;
        {
            UnkTypedRef src;
            fn_71000a16e0(&src, reinterpret_cast<const char*>(ref->type), ref->ptr);
            fn_71000a1760(&p, &src);
        }
        result = fn_7100ad8120(&p, a, b);
    } else {
        const CType* type = ref->type;
        const CType* cls = type && IsKindOf(type, fn_7100096100) ? type : nullptr;
        UnkReflValue v;
        fn_71000a1ae4(&v, ref, 1, 2);
        void* p = ValueAddress(&v);
        UnkTypedRef object;
        char* dynamic = fn_710009dcd8(const_cast<char*>(reinterpret_cast<const char*>(cls)), p);
        object.type = reinterpret_cast<const CType*>(dynamic);
        object.ptr = p;
        result = fn_7100ad8238(&object, a, b);
        fn_71000a1c60(&v);
    }
    if (result.value.type && IsKindOf(reinterpret_cast<const CType*>(result.value.type), fn_710009bedc)) {
        UnkReflTypedPtr target = fn_710009c460(TypedAddress(&result.value));
        SetRef(&result.value, reinterpret_cast<const CType*>(target.type), target.ptr);
    }
    return result;
}

// UnkTU437 (CAimComponent's unit): resolves a reflected path to the entity it names, directly or through
// a pointer value, and returns a counted reference to that entity's weak handle (null when the path
// resolves to nothing). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/reflection.h"
#include "base/reflection/CPointerType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU437.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::CPointerType;
using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

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

struct UnkReflPath_ovb0106;
UnkResolvedValue fn_7100ad7cc0(UnkReflPath_ovb0106* path);

// What a pointer type's getter returns: the pointee's type and address.
struct UnkPointee_l353be8 {
    char* type;
    void* ptr;

    UnkPointee_l353be8() : type(nullptr), ptr(nullptr) {}
    UnkPointee_l353be8(const UnkPointee_l353be8& other) : type(other.type), ptr(other.ptr) {}
};
using UnkPointeeGetter_l353be8 = UnkPointee_l353be8 (*)(void* data);

// The address of the value: inline when it fits in 8 bytes, else through field_0x0.
static inline void* DataOf(UnkReflValue& value) {
    const CType* type = reinterpret_cast<const CType*>(value.type);
    if (!type)
        return nullptr;
    return value.field_0x14 == 1 || type->m_size > 8 ? value.field_0x0 : &value.field_0x0;
}

static inline const CType* DynamicTypeOf(const CType* type) {
    return reinterpret_cast<const UnkReflObject*>(type)->vf_0x0();
}

static inline const CType* TypeOf(const UnkReflValue& value) {
    return reinterpret_cast<const CType*>(value.type);
}

// The pointee of a pointer value, or none when the type has no getter.
static inline UnkPointee_l353be8 PointeeOf(const CPointerType* pointer, const CType* type, void* data) {
    if (pointer->m_get && pointer == type)
        return reinterpret_cast<UnkPointeeGetter_l353be8>(pointer->m_get)(data);
    return UnkPointee_l353be8();
}

base::UnkRefPtrMember fn_7100353be8(void* self) {
    UnkResolvedValue resolved = fn_7100ad7cc0(static_cast<UnkReflPath_ovb0106*>(self));
    base::UnkRefPtrMember result;
    if (!TypeOf(resolved.value)) {
        result.ptr = nullptr;
        return result;
    }
    UnkWeakTarget* entity;
    if (fn_710009d50c(DynamicTypeOf(TypeOf(resolved.value)), fn_710009bedc())) {
        void* data = DataOf(resolved.value);
        const CType* type = TypeOf(resolved.value);
        const CPointerType* pointer = type && fn_710009d50c(DynamicTypeOf(type), fn_710009bedc())
                                          ? static_cast<const CPointerType*>(type)
                                          : nullptr;
        UnkPointee_l353be8 pointee = PointeeOf(pointer, type, data);
        // The kind test's result is unused in the original.
        fn_710009d50c(reinterpret_cast<const CType*>(fn_710009dcd8(pointee.type, pointee.ptr)), fn_7100d70c20());
        entity = static_cast<UnkWeakTarget*>(pointee.ptr);
    } else {
        // The kind test's result is unused in the original.
        fn_710009d50c(TypeOf(resolved.value), fn_7100d70c20());
        entity = static_cast<UnkWeakTarget*>(DataOf(resolved.value));
    }
    UnkWeakHandle* handle = GetWeakHandle(entity);
    if (handle) {
        result.ptr = reinterpret_cast<base::UnkRefCounted*>(handle);
        if (++handle->ref_count == 0)
            handle->vf_0x10();
    } else {
        result.ptr = nullptr;
    }
    return result;
}

// CBatalloonAIComponent's unit: resolves a reflected path (a link's key) to the spawn point it names,
// directly or through a pointer value, and returns a counted reference to its weak handle (null when the
// path resolves to nothing or to something else). Names and layouts are placeholders.
#include "base/reflection.h"
#include "base/reflection/CPointerType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBatalloonAIComponent.h"
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

struct UnkRef_c3 {
    UnkWeakHandle* ptr;

    __attribute__((always_inline)) UnkRef_c3& operator=(const UnkRef_c3& other) {
        if (this != &other)
            CopyWeakRef(&ptr, other.ptr);
        return *this;
    }
    ~UnkRef_c3() {
        if (ptr && --ptr->ref_count == 0)
            ptr->vf_0x10();
    }
};

struct UnkReflPath_ovb0106;
UnkResolvedValue fn_7100ad7cc0(UnkReflPath_ovb0106* path);

// What a pointer type's getter returns: the pointee's type and address.
struct UnkPointee_l3df480 {
    char* type;
    void* ptr;

    UnkPointee_l3df480() = default;
    UnkPointee_l3df480(const UnkPointee_l3df480& other) : type(other.type), ptr(other.ptr) {}
    UnkPointee_l3df480& operator=(const UnkPointee_l3df480& other) = default;
};
using UnkPointeeGetter_l3df480 = UnkPointee_l3df480 (*)(void* data);

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

static inline const CPointerType* AsPointerType(const CType* type) {
    return type && fn_710009d50c(DynamicTypeOf(type), fn_710009bedc()) ? static_cast<const CPointerType*>(type)
                                                                       : nullptr;
}

// The pointee of a pointer value, or none when the type has no getter.
static inline UnkPointee_l3df480 PointeeOf(const CPointerType* pointer, const CType* type, void* data) {
    if (pointer->m_get && pointer == type)
        return reinterpret_cast<UnkPointeeGetter_l3df480>(pointer->m_get)(data);
    return UnkPointee_l3df480();
}

UnkRef_c3 fn_71003df480(void* key) {
    UnkResolvedValue resolved = fn_7100ad7cc0(static_cast<UnkReflPath_ovb0106*>(key));
    UnkRef_c3 result;
    if (!TypeOf(resolved.value)) {
        result.ptr = nullptr;
        return result;
    }
    UnkWeakTarget* spawn_point;
    if (fn_710009d50c(DynamicTypeOf(TypeOf(resolved.value)), fn_710009bedc())) {
        void* data = DataOf(resolved.value);
        const CType* type = TypeOf(resolved.value);
        UnkPointee_l3df480 pointee = PointeeOf(AsPointerType(type), type, data);
        const CType* pointee_type = reinterpret_cast<const CType*>(fn_710009dcd8(pointee.type, pointee.ptr));
        spawn_point = fn_710009d50c(pointee_type, fn_7101123918()) ? static_cast<UnkWeakTarget*>(pointee.ptr)
                                                                    : nullptr;
    } else {
        // The kind test's result is unused in the original.
        fn_710009d50c(TypeOf(resolved.value), fn_7101123918());
        spawn_point = static_cast<UnkWeakTarget*>(DataOf(resolved.value));
    }
    UnkWeakHandle* handle = GetWeakHandle(spawn_point);
    if (handle) {
        result.ptr = handle;
        if (++handle->ref_count == 0)
            handle->vf_0x10();
    } else {
        result.ptr = nullptr;
    }
    return result;
}

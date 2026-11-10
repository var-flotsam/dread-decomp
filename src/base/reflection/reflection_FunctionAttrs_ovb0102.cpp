// base::reflection's unit (tools/tu_map.py unit 79): the reflected function's call (fn_710009ad50, which
// converts the arguments to the parameter types first), its destructor (UnkReflFunction,
// base/reflection/UnkReflAttributes.h: vtable g_unk_7101a1b460) and the growth of its parameter
// attribute containers (0x38 bytes each, at +0x88; fn_710009abf8 destroys one). Names and layouts
// are placeholders.
#include <new>

#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

// An attribute container's destructor (0x38 bytes; base/UnkCloneHelpers.h).
void fn_710009abf8(void* attrs);

// A reflected function's parameter attribute container: a keyed array of values with its growth
// settings, and two more words.
struct UnkReflParamAttrs_ovb0102 {
    void* values;
    void* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
    u64 field_0x28;
    u32 field_0x30;

    UnkReflParamAttrs_ovb0102()
        : values(nullptr), keys(nullptr), count(0), capacity(0), step(10), flags(0x3e),
          allocator(fn_7100083990()), field_0x28(0), field_0x30(4) {}
    UnkReflParamAttrs_ovb0102(UnkReflParamAttrs_ovb0102&& other)
        : values(other.values), keys(other.keys), count(other.count), capacity(other.capacity), step(other.step),
          flags(other.flags), allocator(other.allocator) {
        u64 w28 = other.field_0x28;
        u32 w30 = other.field_0x30;
        other.count = 0;
        other.values = nullptr;
        other.keys = nullptr;
        field_0x30 = w30;
        field_0x28 = w28;
    }
    ~UnkReflParamAttrs_ovb0102() { fn_710009abf8(this); }
};

// Grows the parameter attribute array to `n` containers (exactly n when it must reallocate).
__attribute__((flatten, noinline)) void fn_710009b6f0(base::UnkVector<UnkReflParamAttrs_ovb0102>* self, u32 n) {
    self->Reserve(n);
    for (u32 i = self->count; i < n; ++i)
        new (&self->data[i]) UnkReflParamAttrs_ovb0102();
    self->count = n;
}

// A reflected function's parameter attributes as this file sees the function.
struct UnkReflFunctionParams_ovb0102 {
    u8 pad_0x0[0x88];
    base::UnkVector<UnkReflParamAttrs_ovb0102> param_attrs;  // +0x88
};

// The container at `index`, growing the array to index + 1 containers first when it may grow.
__attribute__((flatten)) static inline UnkReflParamAttrs_ovb0102& ParamAttrsAt(
    base::UnkVector<UnkReflParamAttrs_ovb0102>* v, u32 index) {
    if (v->field_0x14 && v->count <= index) {
        u32 cap = v->capacity;
        if (cap <= index && v->step != 0) {
            u32 grown = (index / v->step + 1) * v->step;
            if (cap < grown)
                v->Reserve(grown);
        }
        fn_710009b6f0(v, index + 1);
    }
    return v->data[index];
}

// Resizes the function's parameter attribute containers to `count` and sets each one's +0x30 to 3.
void fn_710009ae5c(base::reflection::UnkReflFunction* function, u32 count) {
    base::UnkVector<UnkReflParamAttrs_ovb0102>* v =
        &reinterpret_cast<UnkReflFunctionParams_ovb0102*>(function)->param_attrs;
    if (v->count < count) {
        fn_710009b6f0(v, count);
    } else if (v->count > count) {
        for (u64 i = count; i < v->count; ++i)
            v->data[i].~UnkReflParamAttrs_ovb0102();
        v->count = count;
    }
    for (s32 i = 0; i < (s32)count; ++i)
        ParamAttrsAt(v, i).field_0x30 = 3;
}

extern char g_unk_7101a1b460[];  // UnkReflFunction's virtual table (its address point is +0x10)

struct UnkReflFunctionAttrs_ovb0102 {
    u8 unk_0x0[0x38];
    ~UnkReflFunctionAttrs_ovb0102() { fn_710009abf8(this); }
};

// UnkReflFunction (base/reflection/UnkReflAttributes.h) with the members its destructor releases.
struct UnkReflFunctionData_ovb0102 {
    char* vtable;
    base::UnkVector<const void*> params;  // +0x8
    base::UnkStrIdMember name;            // +0x30
    u8 unk_0x38[0x18];
    UnkReflFunctionAttrs_ovb0102 attrs;                           // +0x50
    base::UnkVector<UnkReflFunctionAttrs_ovb0102> param_attrs;  // +0x88
};
static_assert(__builtin_offsetof(UnkReflFunctionData_ovb0102, param_attrs) == 0x88, "layout");

// The reflected function's destructor: its parameter attributes, attributes, name and parameter
// types.
void fn_710009aae8(void* self_) {
    UnkReflFunctionData_ovb0102* self = static_cast<UnkReflFunctionData_ovb0102*>(self_);
    self->vtable = g_unk_7101a1b460 + 0x10;
    self->param_attrs.Free();
    self->attrs.~UnkReflFunctionAttrs_ovb0102();
    self->name.~UnkStrIdMemberOf();
    self->params.Free();
}

using base::reflection::CType;
using base::reflection::UnkReflValue;
using base::reflection::UnkReflValueRef;

// The value argument fn_71000e7a80 takes, built from a reflected value by fn_71000a1710.
struct UnkReflValueArg;
void fn_71000a1710(UnkReflValueArg* out, const UnkReflValue* value);
// A value converted to `type` (returned through x8).
UnkReflValue fn_71000a1774(const UnkReflValueRef* value, const CType* type);
struct UnkAssignedRecord_a1ba4;
void fn_71000a1ba4(UnkAssignedRecord_a1ba4* self, const UnkAssignedRecord_a1ba4* other);

// The arguments of a call: reflected values.
struct UnkReflCallArgs_ovb0102 {
    UnkReflValue* data;
    u64 count;
};

using UnkReflInvoker_ovb0102 = UnkReflValue (*)(void* function, void* object, UnkReflCallArgs_ovb0102* args);

struct UnkReflCallable_ovb0102 {
    void* vtable;
    base::UnkVector<const CType*> params;  // +0x8
    u8 pad_0x30[0x10];
    void* field_0x40;
    UnkReflInvoker_ovb0102 invoker;  // +0x48
    u8 pad_0x50[0x40];
    u32 param_count;  // +0x90: the count of the parameter attribute containers
};

static inline UnkReflValue Empty() {
    UnkReflValue empty;
    empty.field_0x0 = nullptr;
    empty.type = nullptr;
    empty.kind = 2;
    empty.field_0x14 = 1;
    return empty;
}

// Calls the reflected function on `object`: each argument whose type isn't a kind of its
// parameter's is converted first; an empty result (kind 2) when there is no invoker, too few
// arguments, or an argument that can't be converted.
UnkReflValue fn_710009ad50(UnkReflCallable_ovb0102* self, void* object, UnkReflCallArgs_ovb0102* args) {
    if (!self->invoker || args->count < self->param_count)
        return Empty();
    for (u64 i = 0; i < self->params.count; ++i) {
        UnkReflValue* arg = &args->data[i];
        const CType* type = self->params.data[i];
        if (fn_710009d50c(reinterpret_cast<const CType*>(arg->type), type))
            continue;
        {
            UnkReflValue converted;
            UnkReflValueRef ref;
            fn_71000a1710(reinterpret_cast<UnkReflValueArg*>(&ref), arg);
            new (&converted) UnkReflValue(fn_71000a1774(&ref, type));
            fn_71000a1ba4(reinterpret_cast<UnkAssignedRecord_a1ba4*>(arg),
                          reinterpret_cast<const UnkAssignedRecord_a1ba4*>(&converted));
            fn_71000a1c60(&converted);
        }
        if (!arg->type)
            return Empty();
    }
    return self->invoker(self->field_0x40, object, args);
}

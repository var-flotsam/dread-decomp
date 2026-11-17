// base::reflection::CClass's unit (72): finishes the constructor of a reflected function extended with
// a list of argument values (+0xc0) and a list of parameter attributes (+0x100). For each of the owner
// function's parameter types it takes the next given argument when it has a type; otherwise it adds a
// default value, the parameter type, and (when the owner has parameter attributes) a copy of the
// parameter's attributes. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBlackboard.h"
#include "types.h"

using base::global::CRntVector;
using base::reflection::CType;
using base::reflection::UnkReflValue;

void fn_7100095d68(UnkPropertyArrays* self, const UnkPropertyArrays* other);
void fn_7100094fc0(void* self, void* owner, void* x2);

namespace refl_init_params_l094fc0 {

// A reflected value that copies (fn_71000a179c) and destroys (fn_71000a1c60) its contents.
struct UnkArgValue : UnkReflValue {
    UnkArgValue() {
        field_0x0 = nullptr;
        type = nullptr;
        kind = 2;
        field_0x14 = 1;
    }
    UnkArgValue(const UnkArgValue& other) { fn_71000a179c(this, &other); }
    ~UnkArgValue() { fn_71000a1c60(this); }
};

struct UnkParamAttrs;

// The function the values are for: its parameter types, how many parameter attributes it has
// and each one's attributes.
struct UnkOwnerFunction {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual const UnkParamAttrs* vf_0x50(u32 index);
    virtual void vf_0x58();
    virtual s32 vf_0x60();
    CRntVector<const CType*> params;
};

struct UnkFunctionEx {
    void* vtable;
    CRntVector<const CType*> params;
    u8 pad_0x30[0xc0 - 0x30];
    CRntVector<UnkArgValue> values;
    UnkReflValue value;
    CRntVector<void*> attrs;
};
static_assert(__builtin_offsetof(UnkFunctionEx, attrs) == 0x100, "layout");

struct UnkValueList {
    UnkArgValue* data;
    u32 count;
};

}  // namespace refl_init_params_l094fc0

// Appends a copy of `value`, growing the array by its step first when it is full. A capacity that
// doesn't grow past the count (no step) hangs, as in the original.
static inline void AppendValue(CRntVector<refl_init_params_l094fc0::UnkArgValue>& values,
                               const refl_init_params_l094fc0::UnkArgValue& value) {
    u32 n = values.count;
    u32 new_capacity = values.capacity;
    if (new_capacity <= n && values.step != 0)
        new_capacity = (n / values.step + 1) * values.step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    values.Reserve(new_capacity);
    new (&values.data[values.count]) refl_init_params_l094fc0::UnkArgValue(value);
    ++values.count;
}

using namespace refl_init_params_l094fc0;

void fn_7100094fc0(void* self_ptr, void* owner_ptr, void* args_ptr) {
    // A parameter's attributes: the attribute values with their names, and two more fields.
    // A parameter's attributes: the attribute values with their names (copied by fn_7100095d68),
    // and two more fields, read first and written in the other order.
    struct UnkParamAttrs_l094fc0 {
        struct Arrays : UnkPropertyArrays {
            Arrays(const Arrays& other) { fn_7100095d68(this, &other); }
            ~Arrays() {
                if (ids)
                    for (u32 i = 0; i < count; ++i)
                        ids[i].~CStrId();
                if (values)
                    for (u32 i = 0; i < count; ++i)
                        fn_71000a1c60(&static_cast<UnkReflValue*>(values)[i]);
                count = 0;
                if (ids)
                    allocator->vf_0x48(ids);
                if (values)
                    allocator->vf_0x48(values);
            }
        };
        Arrays arrays;
        void* field_0x28;
        u32 field_0x30;

        UnkParamAttrs_l094fc0(const UnkParamAttrs_l094fc0& other) : arrays(other.arrays) {
            void* a = other.field_0x28;
            u32 b = other.field_0x30;
            field_0x30 = b;
            field_0x28 = a;
        }
    };

    UnkFunctionEx* self = static_cast<UnkFunctionEx*>(self_ptr);
    UnkOwnerFunction* owner = static_cast<UnkOwnerFunction*>(owner_ptr);
    UnkValueList* args = static_cast<UnkValueList*>(args_ptr);

    CRntVector<const CType*> types(owner->params);
    self->values.Reserve(args->count);
    UnkArgValue* arg = args->data;
    UnkArgValue* args_end = arg + args->count;
    s32 attr_count = owner->vf_0x60();
    u32 i = 0;
    for (const CType **p = types.data, **end = p + types.count; p != end; ++p, ++i) {
        const CType* type = *p;
        if (arg != args_end && arg->type) {
            AppendValue(self->values, *arg);
        } else {
            AppendValue(self->values, UnkArgValue());
            self->params.PushBack(type);
            if (attr_count > 0) {
                UnkParamAttrs_l094fc0 attrs(*reinterpret_cast<const UnkParamAttrs_l094fc0*>(owner->vf_0x50(i)));
                self->attrs.PushBack(&attrs);
            }
        }
        if (arg != args_end)
            ++arg;
    }
    types.Free();
}

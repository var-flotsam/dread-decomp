// Unit 341 (CActor): runs a script command on an actor: with no component named, hands it to every
// component; else calls the named reflected function of the named component, with the command's
// packed parameters or with its file and a result value. Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflTypedPtr;
using base::reflection::UnkReflValue;

struct UnkCommand_b0163;

// A script command: the component and function it names, its parameters and its packed file.
struct UnkScriptCall_l233a38 {
    CStrId target;                                       // 0x0: the component; empty for all of them
    CStrId function;                                     // 0x8
    base::global::CRntVector<UnkReflValue> params;       // 0x10
    base::global::CRntFile file;                         // 0x38
    u8 pad_0x70[0x10];
};
static_assert(sizeof(UnkScriptCall_l233a38) == 0x80, "UnkScriptCall_l233a38 layout");

// A component, as a reflected object that also takes script commands (vf_0xc8).
struct UnkScriptComponent_l233a38 {
    virtual const base::reflection::CClass* vf_0x0() const;
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8(UnkScriptCall_l233a38* call);
};

// A keyed array: values and their string-id keys side by side.
template <class V>
struct UnkKeyedArray_l233a38 {
    V* values;
    CStrId* keys;
    u32 count;

    V* End() { return &values[count]; }
    V* Find(const CStrId& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i].data == key.data)
                return &values[i];
        return End();
    }
};

// The actor's components by name.
struct UnkScriptActor_l233a38 {
    u8 pad_0x0[0xa0];
    UnkKeyedArray_l233a38<UnkScriptComponent_l233a38*> components;  // 0xa0
};

// Arguments to a reflected function call.
struct UnkReflArgs_l233a38 {
    const UnkReflValue* data;
    u64 count;
};

// A reflected function: called through vf_0x10, its attributes from vf_0x40.
struct UnkReflFunction_l233a38 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkReflValue vf_0x10(UnkReflTypedPtr* target, const UnkReflArgs_l233a38* args, bool flag);
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual UnkKeyedArray_l233a38<UnkReflValue>* vf_0x40();
    u8 pad_0x8[0x8];
    u32 param_count;  // 0x10
};

// Up to two argument values built in place.
struct UnkReflArgList_l233a38 {
    u32 count = 0;
    u8 storage[2 * sizeof(UnkReflValue)];

    UnkReflValue* At(u32 i) { return reinterpret_cast<UnkReflValue*>(storage) + i; }
    void Push(const UnkReflValue& value) {
        fn_71000a179c(At(count), &value);
        ++count;
    }
};

struct UnkClone_fn_71000978e4;
extern template void* base::refl_KeyedFind<UnkClone_fn_71000978e4, 0x110, 0xb8>(char*, const base::UnkStrIdMember*);
const base::reflection::CType* fn_710009d884();  // base::reflection::refl_GetType<base::reflection::CTypedValue>
extern const base::reflection::CType g_unk_7101d58bd0;

void fn_7100233a38(void* object, UnkCommand_b0163* command) {
    using Component = UnkScriptComponent_l233a38;
    auto* actor = static_cast<UnkScriptActor_l233a38*>(object);
    auto* call = reinterpret_cast<UnkScriptCall_l233a38*>(command);
    auto& components = actor->components;

    if (call->target.data == &g_unk_7101d47108->empty) {
        for (Component **p = components.values, **end = p + components.count; p != end; ++p)
            if (*p)
                (*p)->vf_0xc8(call);
        return;
    }

    Component** it = components.Find(call->target);
    if (it == components.End())
        return;
    Component* component = *it;
    if (!component || !fn_710009d50c(component->vf_0x0(), fn_71010c23d0()))
        return;

    char* type = fn_710009dcd8(const_cast<char*>(reinterpret_cast<const char*>(component->vf_0x0())), component);
    UnkReflTypedPtr target;
    target.type = type;
    target.ptr = component;
    char* cls = type && fn_710009d50c(reinterpret_cast<UnkReflObject*>(type)->vf_0x0(), fn_7100096100()) ? type
                                                                                                          : nullptr;
    auto* function = static_cast<UnkReflFunction_l233a38*>(base::refl_KeyedFind<UnkClone_fn_71000978e4, 0x110, 0xb8>(
        cls, reinterpret_cast<const base::UnkStrIdMember*>(&call->function)));
    if (!function) {
        component->vf_0xc8(call);
        return;
    }

    u32 param_count = function->param_count;
    UnkReflValue result;
    result.field_0x0 = nullptr;
    result.type = nullptr;
    result.kind = 2;
    result.field_0x14 = 1;

    static const CStrId s_packed_params("RntFilePackedParams", true);
    UnkKeyedArray_l233a38<UnkReflValue>* attrs = function->vf_0x40();
    if (attrs->Find(s_packed_params) != attrs->End()) {
        // The parameters come packed in the file: rewind it and pass them as they are.
        fn_710007c00c(&call->file, fn_710007c1a4(&call->file), 0);
        UnkReflArgs_l233a38 args;
        args.data = call->params.data;
        args.count = call->params.count;
        if (args.count >= param_count) {
            UnkReflValue returned = function->vf_0x10(&target, &args, false);
            fn_71000a1ba4(reinterpret_cast<UnkAssignedRecord_a1ba4*>(&result),
                          reinterpret_cast<const UnkAssignedRecord_a1ba4*>(&returned));
            fn_71000a1c60(&returned);
        }
    } else {
        // The file and the result value, by reference.
        UnkReflArgList_l233a38 list;
        {
            UnkReflValue value;
            value.field_0x0 = nullptr;
            value.type = nullptr;
            value.kind = 2;
            fn_71000a18d4(&value, &g_unk_7101d58bd0, &call->file, true);
            list.Push(value);
            fn_71000a1c60(&value);
        }
        {
            UnkReflValue value;
            value.kind = 2;
            value.field_0x0 = nullptr;
            value.type = nullptr;
            fn_71000a18d4(&value, fn_710009d884(), &result, true);
            list.Push(value);
            fn_71000a1c60(&value);
        }

        UnkReflArgs_l233a38 args;
        args.data = list.At(0);
        args.count = list.count;
        if (args.count >= param_count) {
            UnkReflValue returned = function->vf_0x10(&target, &args, false);
            fn_71000a1c60(&returned);
        }
        for (u32 i = 0; i < list.count; ++i)
            fn_71000a1c60(list.At(i));
    }
    fn_71000a1c60(&result);
}

// Unit 94 (base::serialization::CTypedRefWriter's unit): pushes a context for a value of the given
// type onto the stack (an object context for a class, a collection context for a collection, a
// base context otherwise) and returns the new innermost context. Names and layouts are placeholders.
#include <memory>
#include <new>

#include "base/global/CRntVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/CCollectionType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU80.h"
#include "game/UnkTU94.h"
#include "types.h"

using base::global::CRntVector;
using base::reflection::CCollectionType;
using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflObjectB;
using base::reflection::UnkReflValue;

extern UnkReflType g_unk_7101d67ec0;  // SObjectContext's class descriptor
extern UnkReflType g_unk_7101d68000;  // SCollectionContext's class descriptor

// The caller's view of the stack and of a context (src/base/UnkUnit94/UnkUnit94_SetValue.cpp).
struct UnkUnit94Entry {
    u8 pad_0x0[0x8];
    UnkReflValue value;
};

struct UnkUnit94Holder {
    u32 id;
    u32 field_0x4;
    u8 pad_0x8[0x5468 - 0x8];
    bool field_0x5468;
};

// An empty reflected value of a given kind, destroyed with what it holds.
struct UnkContextValue_l0f0f38 : UnkReflValue {
    explicit UnkContextValue_l0f0f38(u32 value_kind) {
        field_0x0 = nullptr;
        type = nullptr;
        kind = value_kind;
        field_0x14 = 1;
    }
    ~UnkContextValue_l0f0f38() { fn_71000a1c60(this); }
};

// base::serialization::CTypedRefWriter::SBaseContext.
struct UnkBaseContext_l0f0f38 {
    const CType* type;
    UnkContextValue_l0f0f38 value;  // 0x8
    void* field_0x20;
    UnkContextValue_l0f0f38 field_0x28;
    u64 field_0x40;
    CRntVector<UnkContextValue_l0f0f38> field_0x48;
    u32 field_0x70;
    u64 field_0x78;

    UnkBaseContext_l0f0f38(const CType* value_type, u32 kind, UnkBaseContext_l0f0f38* parent)
        : type(value_type), value(kind), field_0x20(parent ? parent->field_0x20 : nullptr), field_0x28(kind),
          field_0x40(0), field_0x48(0x59), field_0x70(0), field_0x78(0) {}
    ~UnkBaseContext_l0f0f38() { field_0x48.Free(); }
};
static_assert(sizeof(UnkBaseContext_l0f0f38) == 0x80, "SBaseContext layout");

// base::serialization::CTypedRefWriter::SObjectContext.
struct UnkObjectContext_l0f0f38 : UnkBaseContext_l0f0f38 {
    u64 field_0x80;
    std::unique_ptr<UnkReflObjectB> field_0x88;

    UnkObjectContext_l0f0f38(const CType* value_type, u32 kind, UnkBaseContext_l0f0f38* parent)
        : UnkBaseContext_l0f0f38(value_type, kind, parent), field_0x80(0) {}
};

// base::serialization::CTypedRefWriter::SCollectionContext.
struct UnkCollectionContext_l0f0f38 : UnkBaseContext_l0f0f38 {
    const CType* element_type;  // 0x80
    void* function;             // 0x88
    std::unique_ptr<UnkReflObjectB> call;
    bool field_0x98;
    UnkContextValue_l0f0f38 key;  // 0xa0
    bool field_0xb8;
    u64 field_0xc0;

    UnkCollectionContext_l0f0f38(const CCollectionType* value_type, u32 kind, UnkBaseContext_l0f0f38* parent)
        : UnkBaseContext_l0f0f38(value_type, kind, parent),
          element_type(fn_710009c640(reinterpret_cast<UnkReflObject*>(const_cast<CType*>(value_type->m_element)))),
          function(nullptr), field_0x98(false), key(kind), field_0xb8(false), field_0xc0(0) {}
};
static_assert(sizeof(UnkCollectionContext_l0f0f38) == 0xc8, "SCollectionContext layout");

// Each context class's descriptor.
template <class T>
struct UnkContextClass_l0f0f38;
template <>
struct UnkContextClass_l0f0f38<UnkBaseContext_l0f0f38> {
    static const CType* Get() { return reinterpret_cast<const CType*>(&g_unk_7101d67d80); }
};
template <>
struct UnkContextClass_l0f0f38<UnkObjectContext_l0f0f38> {
    static const CType* Get() { return reinterpret_cast<const CType*>(&g_unk_7101d67ec0); }
};
template <>
struct UnkContextClass_l0f0f38<UnkCollectionContext_l0f0f38> {
    static const CType* Get() { return reinterpret_cast<const CType*>(&g_unk_7101d68000); }
};

// A value of any reflected type held in place, moved and destroyed through its type's functions.
struct UnkHeldValue_l0f0f38 {
    alignas(8) u8 storage[0xc8];
    const CType* type;

    template <class T>
    explicit UnkHeldValue_l0f0f38(T&& value) : type(UnkContextClass_l0f0f38<T>::Get()) {
        if (type->m_moveConstruct)
            type->m_moveConstruct(storage, &value);
    }
    UnkHeldValue_l0f0f38(UnkHeldValue_l0f0f38&& other) : type(other.type) {
        if (type && type->m_moveConstruct)
            type->m_moveConstruct(storage, other.storage);
        other.type = nullptr;
    }
    ~UnkHeldValue_l0f0f38() {
        if (type) {
            type->m_destroy(storage);
            type = nullptr;
        }
    }
    // The held value as a T, or null when it is not one.
    template <class T>
    T* Cast() {
        return fn_710009d50c(type, UnkContextClass_l0f0f38<T>::Get()) ? reinterpret_cast<T*>(storage) : nullptr;
    }
};

// A held context and the context as a T.
template <class T>
struct UnkTypedRef_l0f0f38 {
    T* context;
    UnkHeldValue_l0f0f38 held;

    explicit UnkTypedRef_l0f0f38(T&& value) : held(static_cast<T&&>(value)) { context = held.Cast<T>(); }
    template <class U>
    explicit UnkTypedRef_l0f0f38(UnkTypedRef_l0f0f38<U>&& other) : held(static_cast<UnkHeldValue_l0f0f38&&>(other.held)) {
        context = held.Cast<T>();
    }
    UnkTypedRef_l0f0f38(UnkTypedRef_l0f0f38&& other) : held(static_cast<UnkHeldValue_l0f0f38&&>(other.held)) {
        context = held.Cast<T>();
    }
};
static_assert(sizeof(UnkTypedRef_l0f0f38<UnkBaseContext_l0f0f38>) == 0xd8, "entry layout");

// The writer's context stack.
struct UnkContextStack_l0f0f38 {
    u32 kind;
    u32 count;
    alignas(8) u8 storage[sizeof(UnkTypedRef_l0f0f38<UnkBaseContext_l0f0f38>)];

    UnkTypedRef_l0f0f38<UnkBaseContext_l0f0f38>* entries() {
        return reinterpret_cast<UnkTypedRef_l0f0f38<UnkBaseContext_l0f0f38>*>(storage);
    }
    // The innermost context.
    UnkTypedRef_l0f0f38<UnkBaseContext_l0f0f38>& Top() { return (entries() + count)[-1]; }
    template <class T>
    void Push(T&& context) {
        UnkTypedRef_l0f0f38<T> ref(static_cast<T&&>(context));
        new (&entries()[count])
            UnkTypedRef_l0f0f38<UnkBaseContext_l0f0f38>(static_cast<UnkTypedRef_l0f0f38<T>&&>(ref));
        ++count;
    }
};

UnkUnit94Entry* fn_71000f0f38(UnkUnit94Holder* holder, const char* type_name) {
    auto* self = reinterpret_cast<UnkContextStack_l0f0f38*>(holder);
    const CType* type = reinterpret_cast<const CType*>(type_name);
    UnkBaseContext_l0f0f38* parent = nullptr;
    if (u32 count = self->count) {
        u32 top = count - 1;
        parent = self->entries()[top].held.Cast<UnkBaseContext_l0f0f38>();
    }
    const auto* object = reinterpret_cast<const UnkReflObject*>(type);
    if (fn_710009d50c(object->vf_0x0(), fn_7100096100())) {
        self->Push(UnkObjectContext_l0f0f38(type, self->kind, parent));
    } else if (fn_710009d50c(object->vf_0x0(), fn_71000986d0())) {
        self->Push(UnkCollectionContext_l0f0f38(static_cast<const CCollectionType*>(type), self->kind, parent));
    } else {
        self->Push(UnkBaseContext_l0f0f38(type, self->kind, parent));
    }
    return reinterpret_cast<UnkUnit94Entry*>(self->Top().context);
}

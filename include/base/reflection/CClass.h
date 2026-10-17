#pragma once

// Mercury Engine reflection: type descriptors (CType / CClass) and the per-type getter
// and descriptor-constructor patterns.
//
// Names: `base::reflection::CClass` and `CType` are the engine's own names (CClass's
// constructor hashes the string "base::reflection::CClass"; "base::reflection::CType" is a
// reflected type). Members that aren't understood yet keep offset-based names.

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

// Hash of a type-kind name ("base::reflection::CClass", length 0x18).
u64 fn_7100001570(const char* str, u64 length);

namespace base::reflection {

using UnkFn = void (*)();

class CType;
class CClass;

// Type-erased per-type functions a descriptor points to (CType+0x28..+0x68). Signatures are
// inferred from the code shapes in main; the roles in the comments are guesses.
using UnkFn_0x30 = void (*)(void* dst, const void* src);   // copy-constructs (0x30) / moves (0x38)
using UnkFn_0x40 = void (*)(void* obj);                    // destroys in place (complete dtor)
using UnkFn_0x48 = void* (*)(void* dst, const void* src);  // assigns; null when not assignable
using UnkFn_0x58 = bool (*)(const void* a, const void* b); // compares (identity or always false)
using UnkFn_0x60 = s32 (*)();                              // returns -1 for every class
using UnkFn_0x68 = const CType& (*)(const void* obj);      // the object's dynamic class
// The two functions a class getter passes in. Both receive the descriptor: the first
// (CType+0x70) adds class attributes to the container at CType+0x80, the second (CClass+0xe0)
// registers member fields. Most of them are empty.
using UnkFn_0x70 = void (*)(CClass* cls);

// Stand-ins for a reflected object's virtual interface, as far as the helpers use it. Most
// classes put the dynamic class getter in vtable slot 0 and the destructor after it; a few
// (CAIAction and relatives) start with the destructor and have the getter in slot 3.
struct UnkReflObject {
    virtual const CClass* vf_0x0() const;
    virtual ~UnkReflObject();
};

struct UnkReflObjectB {
    virtual ~UnkReflObjectB();
    virtual void vf_0x10();
    virtual const CClass* vf_0x18() const;
};

// Container used at CType+0xb8 and CClass+0xe8/+0x110 (0x28 bytes).
// At CClass+0x110 it holds the class's functions (0xb8 bytes each) and a parallel array of
// their names (CStrId).
struct UnkReflArray {
    void* data = nullptr;
    void* keys = nullptr;
    u32 count = 0;
    u32 capacity = 0;
    u32 field_0x18;  // growth step (fn_7100097550 grows by it)
    u32 field_0x1c;
    base::UnkAllocator* allocator = fn_7100083990();

    UnkReflArray(u32 a, u32 b) : field_0x18(a), field_0x1c(b) {}
    u64 size() const { return count; }
};

class CType {
public:
    // Type-specific functions installed by the constructor body; absent ones stay null.
    struct UnkFnSet {
        UnkFn fn_0x28;
        UnkFn_0x30 fn_0x30, fn_0x38;
        UnkFn_0x40 fn_0x40;
        UnkFn_0x48 fn_0x48, fn_0x50;
    };

    CType(u64 kind, const base::global::CStrId& name, u64 size, UnkFn_0x58 fn_0x58, UnkFn_0x60 fn_0x60,
          UnkFn_0x68 fn_0x68,
          UnkFn_0x70 fn_0x70, const CType* parent, const UnkFnSet& fns, bool abstract)
        : m_name(name), m_kind(kind), m_size(size), m_flags(0), field_0x24(-1), m_fn_0x58(fn_0x58),
          m_fn_0x60(fn_0x60), m_fn_0x68(fn_0x68), m_fn_0x70(fn_0x70), m_parent(parent),
          m_array_0xb8(10, 0x3e) {
        if (fns.fn_0x28)
            m_fn_0x28 = fns.fn_0x28;
        if (fns.fn_0x30)
            m_fn_0x30 = fns.fn_0x30;
        if (fns.fn_0x38)
            m_fn_0x38 = fns.fn_0x38;
        if (fns.fn_0x40)
            m_fn_0x40 = fns.fn_0x40;
        if (fns.fn_0x48)
            m_fn_0x48 = fns.fn_0x48;
        if (fns.fn_0x50)
            m_fn_0x50 = fns.fn_0x50;
        m_flags &= ~3u;
        if (abstract)
            m_flags |= 1;  // set for interfaces/abstract classes (e.g. ISkeletalPoseProvider)
    }
    virtual ~CType();

    base::global::CStrId m_name;
    u64 m_kind;
    u64 m_size;
    u32 m_flags;
    s32 field_0x24;
    UnkFn m_fn_0x28 = nullptr;
    UnkFn_0x30 m_fn_0x30 = nullptr;
    UnkFn_0x30 m_fn_0x38 = nullptr;
    UnkFn_0x40 m_fn_0x40 = nullptr;
    UnkFn_0x48 m_fn_0x48 = nullptr;
    UnkFn_0x48 m_fn_0x50 = nullptr;
    UnkFn_0x58 m_fn_0x58;
    UnkFn_0x60 m_fn_0x60;
    UnkFn_0x68 m_fn_0x68;
    UnkFn_0x70 m_fn_0x70;
    const CType* m_parent;
    void* field_0x80 = nullptr;  // attribute container, created on first use (fn_710009dd6c)
    void* field_0x88 = nullptr;
    base::UnkVector<void*> m_array_0x90;
    UnkReflArray m_array_0xb8;
};

// Per-type facts for CClass's constructor, specialised for each reflected class (generated
// into src/reflection/). Members: size, abstract, fn_0x58/0x60/0x68, and fn_0x28..fn_0x50 (null when the
// type lacks that function).
template <class T>
struct UnkReflClassTraits;

class CClass : public CType {
public:
    // One instantiation per reflected type (data/functions: T::refl_InitClass).
    // Defined out of the class body: the original calls it rather than inlining it.
    // `extra` is always null and unused; typing it T* is our device for deducing T.
    template <class T>
    CClass(const base::global::CStrId& name, const CClass* parent, UnkFn_0x70 fn_0x70, UnkFn_0x70 fn_0xe0, T* extra);
    ~CClass() override;

    UnkFn_0x70 m_fn_0xe0;
    UnkReflArray m_array_0xe8;
    UnkReflArray m_array_0x110;
};

template <class T>
CClass::CClass(const base::global::CStrId& name, const CClass* parent, UnkFn_0x70 fn_0x70, UnkFn_0x70 fn_0xe0, T* extra)
    : CType(fn_7100001570("base::reflection::CClass", 0x18), name, UnkReflClassTraits<T>::size,
            UnkReflClassTraits<T>::fn_0x58, UnkReflClassTraits<T>::fn_0x60, UnkReflClassTraits<T>::fn_0x68,
            fn_0x70, parent,
            {UnkReflClassTraits<T>::fn_0x28, UnkReflClassTraits<T>::fn_0x30, UnkReflClassTraits<T>::fn_0x38,
             UnkReflClassTraits<T>::fn_0x40, UnkReflClassTraits<T>::fn_0x48, UnkReflClassTraits<T>::fn_0x50},
            UnkReflClassTraits<T>::abstract),
      m_fn_0xe0(fn_0xe0), m_array_0xe8(10, 0xe), m_array_0x110(10, 0xe) {}

static_assert(sizeof(CClass) == 0x138, "descriptor stride observed in .bss");

}  // namespace base::reflection

// Declares the tag type for one reflected class. INIT is the address of its descriptor
// constructor instantiation (T::refl_InitClass), so that instantiation's symbol carries it.
#define DREAD_REFL_TAG(INIT) struct UnkRefl_fn_##INIT

// Defines the class getter for one reflected type (data/functions: T::refl_GetClass): a
// guarded static descriptor built from the type name, the parent's descriptor and the
// type's attribute and member-field functions (CType+0x70, CClass+0xe0). Arguments are bare
// hex addresses (no 0x).
// It also explicitly instantiates the type's descriptor constructor. The original keeps that
// constructor out of line (implicit instantiation would inline it into the getter). Whether
// it did so with an explicit instantiation or a noinline attribute is unknown; both produce
// the same code.
#define DREAD_REFL_CLASS_GETTER(ADDR, INIT, NAME, PARENT, FN_0x70, FN_0xE0)                        \
    void fn_##FN_0x70(base::reflection::CClass*);                                                  \
    void fn_##FN_0xE0(base::reflection::CClass*);                                                  \
    const base::reflection::CClass* fn_##PARENT();                                                 \
    const base::reflection::CClass* fn_##ADDR() {                                                  \
        static base::reflection::CClass s_class(base::global::CStrId(NAME, true),                  \
                                                fn_##PARENT(), &fn_##FN_0x70, &fn_##FN_0xE0,       \
                                                static_cast<UnkRefl_fn_##INIT*>(nullptr));         \
        return &s_class;                                                                           \
    }                                                                                              \
    template base::reflection::CClass::CClass(const base::global::CStrId&,                         \
                                              const base::reflection::CClass*,                     \
                                              base::reflection::UnkFn_0x70,                        \
                                              base::reflection::UnkFn_0x70, UnkRefl_fn_##INIT*)

// The same for a root class (no parent descriptor).
#define DREAD_REFL_ROOT_CLASS_GETTER(ADDR, INIT, NAME, FN_0x70, FN_0xE0)                        \
    void fn_##FN_0x70(base::reflection::CClass*);                                                  \
    void fn_##FN_0xE0(base::reflection::CClass*);                                                  \
    const base::reflection::CClass* fn_##ADDR() {                                                  \
        static base::reflection::CClass s_class(base::global::CStrId(NAME, true),                  \
                                                nullptr, &fn_##FN_0x70, &fn_##FN_0xE0,             \
                                                static_cast<UnkRefl_fn_##INIT*>(nullptr));         \
        return &s_class;                                                                           \
    }                                                                                              \
    template base::reflection::CClass::CClass(const base::global::CStrId&,                         \
                                              const base::reflection::CClass*,                     \
                                              base::reflection::UnkFn_0x70,                        \
                                              base::reflection::UnkFn_0x70, UnkRefl_fn_##INIT*)

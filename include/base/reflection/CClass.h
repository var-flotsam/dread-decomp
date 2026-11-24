#pragma once

// Mercury Engine reflection: type descriptors (CType / CClass) and the per-type getter
// and descriptor-constructor patterns.
//
// Names: `base::reflection::CClass` and `CType` are the engine's own names (CClass's
// constructor hashes the string "base::reflection::CClass"; "base::reflection::CType" is a
// reflected type), and CType's sName is the member reflection names. The other members' names are ours,
// each for what the code that uses it does with it.

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

// Type-kind names are hashed with StringCrc64 (base/global/CStrId.h): "base::reflection::CClass",
// length 0x18.

namespace base::reflection {

// A descriptor's function slot, typed as nothing in particular: a description names the function and the
// constructor casts it to the slot's own type below.
using SlotFn = void (*)();

class CType;
class CClass;

// Type-erased per-type functions a descriptor points to (CType+0x28..+0x68). Signatures are
// inferred from the code shapes in main; the roles in the comments are guesses.
using CopyConstructFn = void (*)(void* dst, const void* src);  // copy-constructs (0x30) / moves (0x38)
using DestroyFn = void (*)(void* obj);                         // destroys in place (complete dtor)
using AssignFn = void* (*)(void* dst, const void* src);        // assigns; null when not assignable
using EqualsFn = bool (*)(const void* a, const void* b);       // compares (identity or always false)
using HashFn = s32 (*)();                                      // returns -1 for every class
using TypeOfFn = const CType& (*)(const void* obj);            // the object's dynamic class
// The two functions a class getter passes in. Both receive the descriptor: the first
// (CType+0x70) adds class attributes to the container at CType+0x80, the second (CClass+0xe0)
// registers member fields. Most of them are empty.
using ClassSetupFn = void (*)(CClass* cls);

// Stand-ins for a reflected object's virtual interface, as far as the helpers use it. Most
// classes put the dynamic class getter in vtable slot 0 and the destructor after it; a few
// (CAIAction and relatives) start with the destructor and have the getter in slot 3. The getter's
// own name is not known: refl_GetDynamicClass says what it returns, the class of the object as
// opposed to the class a static T::refl_GetClass names.
struct ReflectedObject {
    virtual const CClass* refl_GetDynamicClass() const;
    virtual ~ReflectedObject();
};

struct ReflectedObjectDestructorFirst {
    virtual ~ReflectedObjectDestructorFirst();
    virtual void vf_0x10();  // not understood yet
    virtual const CClass* refl_GetDynamicClass() const;
};

// A small dictionary whose key and value types aren't spelled: base::global::CRntSmallDictionary's
// layout (base/global/CRntSmallDictionary.h: the values and the keys in two parallel arrays, the count,
// the capacity, the growth step, the allocation flags and the allocator; 0x28 bytes). The descriptors
// keep three: a class's member fields under their names' hashes (CClass+0xe8), its functions under
// their names (CClass+0x110), and a type's conversions (CType+0xb8).
struct UntypedSmallDictionary {
    void* data = nullptr;
    void* keys = nullptr;
    u32 count = 0;
    u32 capacity = 0;
    u32 step;   // capacity grows by this many entries (AddReflectedFunction grows by it)
    u32 flags;  // passed to the allocator
    base::UnkAllocator* allocator = GetDefaultAllocator();

    UntypedSmallDictionary(u32 step, u32 flags) : step(step), flags(flags) {}
    u64 size() const { return count; }
};

class CType {
public:
    // A type's value operations other than equals, hash and type_of, as the constructor installs
    // them; a type lacks the ones that are null.
    struct ValueFunctions {
        SlotFn construct;
        CopyConstructFn copy_construct, move_construct;
        DestroyFn destroy;
        AssignFn copy_assign, move_assign;
    };

    CType(u64 kind, const base::global::CStrId& name, u64 size, EqualsFn equals, HashFn hash,
          TypeOfFn type_of,
          ClassSetupFn add_attributes, const CType* parent, const ValueFunctions& functions, bool abstract)
        : sName(name), m_kind(kind), m_size(size), m_flags(0), m_ownHash(-1), m_equals(equals),
          m_hash(hash), m_typeOf(type_of), m_addAttributes(add_attributes), m_parent(parent),
          m_conversions(10, 0x3e) {
        if (functions.construct)
            m_construct = functions.construct;
        if (functions.copy_construct)
            m_copyConstruct = functions.copy_construct;
        if (functions.move_construct)
            m_moveConstruct = functions.move_construct;
        if (functions.destroy)
            m_destroy = functions.destroy;
        if (functions.copy_assign)
            m_copyAssign = functions.copy_assign;
        if (functions.move_assign)
            m_moveAssign = functions.move_assign;
        m_flags &= ~3u;
        if (abstract)
            m_flags |= 1;  // set for interfaces/abstract classes (e.g. ISkeletalPoseProvider)
    }
    virtual ~CType();

    base::global::CStrId sName;
    u64 m_kind;
    u64 m_size;
    u32 m_flags;
    s32 m_ownHash;  // 0x24: the type's own hash, computed on first use and kept; -1 until then
    // The value operations (base/reflection/ReflectedClass.h, ClassDescription, names their roles).
    SlotFn m_construct = nullptr;               // 0x28
    CopyConstructFn m_copyConstruct = nullptr;  // 0x30
    CopyConstructFn m_moveConstruct = nullptr;  // 0x38
    DestroyFn m_destroy = nullptr;              // 0x40
    AssignFn m_copyAssign = nullptr;            // 0x48
    AssignFn m_moveAssign = nullptr;            // 0x50
    EqualsFn m_equals;                          // 0x58
    HashFn m_hash;                              // 0x60
    TypeOfFn m_typeOf;                          // 0x68
    ClassSetupFn m_addAttributes;               // 0x70: adds the type's attributes to m_attributes
    const CType* m_parent;
    void* m_attributes = nullptr;  // 0x80: the attribute container, created on first use (TypeAttributes)
    void* m_nextRegistered = nullptr;  // 0x88: the next type in the registry's list (fn_710009ec00 links it)
    // 0x90: the types derived from this one, as the drafts of the registry's start-up pass (fn_71000a4b60)
    // and of CPointerType's referred types (fn_710009c150) read it; no matching function fills it yet.
    base::global::CRntVector<void*> m_derivedTypes;
    // 0xb8: the conversions registered for the type, each under the hash of the other type's name; a null
    // one says the type is not converted (fn_710009d52c, fn_710009d7a8).
    UntypedSmallDictionary m_conversions;
};

// Per-type facts for CClass's constructor: the type's size, whether it is abstract, and its value
// operations under the names ClassDescription gives them (base/reflection/ReflectedClass.h; null when the
// type lacks that operation). Older sources specialise it for a tag type; a class described with
// base/reflection/ReflectedClass.h gets it from this primary template, which reads the description.
template <class T>
struct ClassTraits {
    static constexpr u64 size = T::size;
    static constexpr bool abstract = T::abstract;
    static constexpr auto equals = T::equals;
    static constexpr auto hash = T::hash;
    static constexpr auto type_of = T::type_of;
    static constexpr auto construct = T::construct;
    static constexpr auto copy_construct = T::copy_construct;
    static constexpr auto move_construct = T::move_construct;
    static constexpr auto destroy = T::destroy;
    static constexpr auto copy_assign = T::copy_assign;
    static constexpr auto move_assign = T::move_assign;
};

class CClass : public CType {
public:
    // One instantiation per reflected type (data/functions: T::refl_InitClass).
    // Defined out of the class body: the original calls it rather than inlining it.
    // `extra` is always null and unused; typing it T* is our device for deducing T.
    template <class T>
    CClass(const base::global::CStrId& name, const CClass* parent, ClassSetupFn add_attributes,
           ClassSetupFn add_fields, T* extra);
    ~CClass() override;

    ClassSetupFn m_addFields;            // 0xe0: registers the class's member fields and functions
    UntypedSmallDictionary m_fields;     // 0xe8: CVariable (0xb0 bytes each) under the name's hash
    UntypedSmallDictionary m_functions;  // 0x110: CFunction (0xb8 bytes each) under the name (CStrId)
};

template <class T>
CClass::CClass(const base::global::CStrId& name, const CClass* parent, ClassSetupFn add_attributes,
               ClassSetupFn add_fields, T* extra)
    : CType(StringCrc64("base::reflection::CClass", 0x18), name, ClassTraits<T>::size,
            ClassTraits<T>::equals, ClassTraits<T>::hash, ClassTraits<T>::type_of,
            add_attributes, parent,
            {reinterpret_cast<SlotFn>(ClassTraits<T>::construct), ClassTraits<T>::copy_construct,
             ClassTraits<T>::move_construct, ClassTraits<T>::destroy, ClassTraits<T>::copy_assign,
             ClassTraits<T>::move_assign},
            ClassTraits<T>::abstract),
      m_addFields(add_fields), m_fields(10, 0xe), m_functions(10, 0xe) {}

static_assert(sizeof(CClass) == 0x138, "descriptor stride observed in .bss");

}  // namespace base::reflection

#include "base/reflection/UnkReflNames.h"  // the earlier placeholder names, for the sources that still use them

// Adds a type to the list its translation unit's static initialiser registers it in (the list
// is a byte array in the unit's data, after the type objects; fn_710009ec00 is in CType's unit).
void fn_710009ec00(void* list, void* type);

// Declares the tag type for one reflected class. INIT is the address of its descriptor
// constructor instantiation (T::refl_InitClass), so that instantiation's symbol carries it.
#define DREAD_REFL_TAG(INIT) struct UnkRefl_fn_##INIT

// The getter's linkage. On the Switch an ordinary function (the bytes are the original's). On the
// port it is inline (and `used`, so each unit still emits it): its descriptor and the descriptor's
// guard then become weak symbols, which tools/port_link.py binds to the game image's ("bound
// statics"). The game's lifted code registers and reads the image's descriptor; with an ordinary
// function the descriptor is a local symbol of the executable, which the link cannot redirect, and
// the image's stays unconstructed (2,354 classes without a name).
#if defined(__aarch64__)
#define DREAD_REFL_GETTER_LINKAGE
#else
#define DREAD_REFL_GETTER_LINKAGE inline __attribute__((used))
#endif

// Defines the class getter for one reflected type (data/functions: T::refl_GetClass): a
// guarded static descriptor built from the type name, the parent's descriptor and the
// type's attribute and member-field functions (CType+0x70, CClass+0xe0). Arguments are bare
// hex addresses (no 0x).
// It also explicitly instantiates the type's descriptor constructor. The original keeps that
// constructor out of line (implicit instantiation would inline it into the getter). Whether
// it did so with an explicit instantiation or a noinline attribute is unknown; both produce
// the same code.
#define DREAD_REFL_GETTER_IMPL_(ADDR, INIT, NAME_ID, PARENT_EXPR, FN_0x70, FN_0xE0)                     void fn_##FN_0x70(base::reflection::CClass*);                                                      void fn_##FN_0xE0(base::reflection::CClass*);                                                      DREAD_REFL_GETTER_LINKAGE const base::reflection::CClass* fn_##ADDR() {                                                          static base::reflection::CClass s_class(NAME_ID, PARENT_EXPR, &fn_##FN_0x70, &fn_##FN_0xE0,                                                 static_cast<UnkRefl_fn_##INIT*>(nullptr));                 return &s_class;                                                                               }                                                                                                  template base::reflection::CClass::CClass(const base::global::CStrId&,                                                                       const base::reflection::CClass*,                                                                   base::reflection::ClassSetupFn,                                                                      base::reflection::ClassSetupFn, UnkRefl_fn_##INIT*)

#define DREAD_REFL_CLASS_GETTER(ADDR, INIT, NAME, PARENT, FN_0x70, FN_0xE0)                            const base::reflection::CClass* fn_##PARENT();                                                     DREAD_REFL_GETTER_IMPL_(ADDR, INIT, base::global::CStrId(NAME, true), fn_##PARENT(), FN_0x70, FN_0xE0)

// The same for a root class (no parent descriptor).
#define DREAD_REFL_ROOT_CLASS_GETTER(ADDR, INIT, NAME, FN_0x70, FN_0xE0)                               DREAD_REFL_GETTER_IMPL_(ADDR, INIT, base::global::CStrId(NAME, true), nullptr, FN_0x70, FN_0xE0)

// Variants with the name's constructor inlined (a few base library types, see UnkStrIdInline).
#define DREAD_REFL_CLASS_GETTER_INLINE_NAME(ADDR, INIT, NAME, PARENT, FN_0x70, FN_0xE0)                const base::reflection::CClass* fn_##PARENT();                                                     DREAD_REFL_GETTER_IMPL_(ADDR, INIT, base::global::CStrId(base::global::UnkStrIdInline{}, NAME, true),                             fn_##PARENT(), FN_0x70, FN_0xE0)
#define DREAD_REFL_ROOT_CLASS_GETTER_INLINE_NAME(ADDR, INIT, NAME, FN_0x70, FN_0xE0)                   DREAD_REFL_GETTER_IMPL_(ADDR, INIT, base::global::CStrId(base::global::UnkStrIdInline{}, NAME, true),                             nullptr, FN_0x70, FN_0xE0)
void* fn_71000978e4(char*, const base::global::CStrId*);

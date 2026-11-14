#pragma once

// How a reflected class is described in src/reflection/ (tools/gen_reflection.py writes these).
//
// The game's classes aren't declared to those sources, so each class's reflection functions live in a
// namespace with the class's own name: `namespace CActor { const CClass* refl_GetClass(); }`. A
// namespace and a class mangle the same way, so these are the symbols the class's own static members
// would be, and the names data/functions/ gives them (T::refl_GetClass, T::refl_Equals, ...). A use
// looks like this:
//
//     namespace CAIAimAction {
//     bool refl_Equals(const void* a, const void* b) { return a == b; }
//     s32 refl_Hash() { return -1; }
//     const CType& refl_TypeOf(const void* obj) { return ClassOfDestructorFirst(obj); }
//     void refl_Construct(void* obj);
//     void refl_Destroy(void* obj) { DestroyDestructorFirst(obj); }
//     void refl_AddAttributes(CClass* cls) { ... }
//     void refl_AddFields(CClass*) {}
//     struct refl_Description : ClassDescription {
//         static constexpr u64 size = 0xd0;
//         static constexpr auto equals = &refl_Equals;
//         ...
//     };
//     }  // namespace CAIAimAction
//     DREAD_REFLECTED_CLASS(CAIAimAction, CAIAction);
//
// The descriptor's per-type functions (CType+0x28..+0x68) are the class's value operations. Their
// roles come from how the engine calls them (CClass.h has the slot types); a class lacks the ones its
// description leaves out. A function declared and not defined here isn't reconstructed yet.

#include "base/reflection/CClass.h"
#include "types.h"

namespace base::reflection {

// The default description: every operation absent, not abstract. A class's description derives from
// it and names the operations it has.
struct ClassDescription {
    static constexpr bool abstract = false;
    // Compares two values (CType+0x58).
    static constexpr bool (*equals)(const void* a, const void* b) = nullptr;
    // A value's hash (CType+0x60): an enum returns its value; every class returns -1.
    static constexpr s32 (*hash)() = nullptr;
    // A value's dynamic type (CType+0x68).
    static constexpr const CType& (*type_of)(const void* obj) = nullptr;
    // Default-constructs a value in place (CType+0x28).
    static constexpr void (*construct)(void* obj) = nullptr;
    // Copy- and move-constructs a value in place (CType+0x30, +0x38).
    static constexpr void (*copy_construct)(void* dst, const void* src) = nullptr;
    static constexpr void (*move_construct)(void* dst, const void* src) = nullptr;
    // Destroys a value in place (CType+0x40).
    static constexpr void (*destroy)(void* obj) = nullptr;
    // Copy- and move-assigns a value; null when the class can't be assigned (CType+0x48, +0x50).
    static constexpr void* (*copy_assign)(void* dst, const void* src) = nullptr;
    static constexpr void* (*move_assign)(void* dst, const void* src) = nullptr;
};

// A reflected object's class and destructor, through its virtual table. Most classes put the class
// getter in slot 0 and the destructor after it; a few (the AI and Samus actions, the media streams)
// start with the destructor and have the getter in slot 3 (UnkReflObject, UnkReflObjectB in CClass.h).
inline const CType& ClassOf(const void* obj) {
    return *static_cast<const UnkReflObject*>(obj)->vf_0x0();
}
inline const CType& ClassOfDestructorFirst(const void* obj) {
    return *static_cast<const UnkReflObjectB*>(obj)->vf_0x18();
}
inline void Destroy(void* obj) {
    static_cast<UnkReflObject*>(obj)->~UnkReflObject();
}
inline void DestroyDestructorFirst(void* obj) {
    static_cast<UnkReflObjectB*>(obj)->~UnkReflObjectB();
}

}  // namespace base::reflection

// The class getter (T::refl_GetClass) and descriptor constructor (T::refl_InitClass) of class T,
// whose namespace holds its description and the functions it names; PARENT is the parent class.
// The getter returns a guarded static descriptor built from T's name, the parent's descriptor and
// T's attribute and member-field functions (CType+0x70, CClass+0xe0). The descriptor constructor
// is CClass's constructor for T, instantiated out of line as the original keeps it.
#define DREAD_REFL_GETTER_BODY_(T, NAME_ID, PARENT_EXPR)                                                \
    DREAD_REFL_GETTER_LINKAGE const base::reflection::CClass* T::refl_GetClass() {                      \
        static base::reflection::CClass s_class(NAME_ID, PARENT_EXPR, &T::refl_AddAttributes,           \
                                                &T::refl_AddFields,                                     \
                                                static_cast<T::refl_Description*>(nullptr));            \
        return &s_class;                                                                                \
    }                                                                                                   \
    DREAD_REFLECTED_DESCRIPTOR(T)

#define DREAD_REFLECTED_CLASS(T, PARENT)                                                                \
    namespace PARENT {                                                                                  \
    const base::reflection::CClass* refl_GetClass();                                                    \
    }                                                                                                   \
    namespace T {                                                                                       \
    const base::reflection::CClass* refl_GetClass();                                                    \
    }                                                                                                   \
    DREAD_REFL_GETTER_BODY_(T, base::global::CStrId(#T, true), PARENT::refl_GetClass())

// A class with no parent descriptor.
#define DREAD_REFLECTED_ROOT_CLASS(T)                                                                   \
    namespace T {                                                                                       \
    const base::reflection::CClass* refl_GetClass();                                                    \
    }                                                                                                   \
    DREAD_REFL_GETTER_BODY_(T, base::global::CStrId(#T, true), nullptr)

// Variants with the name's constructor inlined (a few base library types, see UnkStrIdInline).
#define DREAD_REFLECTED_CLASS_INLINE_NAME(T, PARENT)                                                    \
    namespace PARENT {                                                                                  \
    const base::reflection::CClass* refl_GetClass();                                                    \
    }                                                                                                   \
    namespace T {                                                                                       \
    const base::reflection::CClass* refl_GetClass();                                                    \
    }                                                                                                   \
    DREAD_REFL_GETTER_BODY_(T, base::global::CStrId(base::global::UnkStrIdInline{}, #T, true),           \
                            PARENT::refl_GetClass())
#define DREAD_REFLECTED_ROOT_CLASS_INLINE_NAME(T)                                                       \
    namespace T {                                                                                       \
    const base::reflection::CClass* refl_GetClass();                                                    \
    }                                                                                                   \
    DREAD_REFL_GETTER_BODY_(T, base::global::CStrId(base::global::UnkStrIdInline{}, #T, true), nullptr)

// Only the descriptor constructor (T::refl_InitClass), for a class whose getter isn't generated.
#define DREAD_REFLECTED_DESCRIPTOR(T)                                                                   \
    template base::reflection::CClass::CClass(const base::global::CStrId&,                             \
                                              const base::reflection::CClass*,                         \
                                              base::reflection::UnkFn_0x70,                            \
                                              base::reflection::UnkFn_0x70, T::refl_Description*)

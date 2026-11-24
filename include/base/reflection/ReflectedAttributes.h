#pragma once

// Reflection attributes: (key, typed value) pairs attached to a type (CType+0x80) or to a
// member field. The keys are guarded static objects named by their own strings
// ("Description", "TrackGroup", ...); their getters are declared here under those names
// (base::reflection::attr::Description()); the two keys whose values are of their own enum type are
// PersistenceKey() and EditorModeDataFlowKey(). A value is passed type-erased, as its type descriptor
// plus a pointer.
//
// AttributeKey, Attributes and TypedValueRef are our names for what they hold; the engine's names
// for them are not known.

#include <initializer_list>

#include "base/global/CRntVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CName.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "base/reflection/CClass.h"
#include "base/reflection/CPointerType.h"
#include "base/reflection/PrimitiveType.h"
#include "base/reflection/UnkReflTypeOf.h"  // the value types' getters by address, for the sources that still name them so
#include "types.h"

namespace base::reflection {

// An attribute key: a guarded static built by its getter (refl_GetAttrKey<G>, UnkReflStatics.h)
// from its name and its value type's descriptor. The destructor is out of line.
struct AttributeKey {
    base::global::CStrId name;
    const CType* type;  // the value type
    u32 targets;        // what the attribute may be attached to, a bit per base::reflection::EAttributeTarget
                        // (the insertion in src/base/reflection/UnkReflAttrs.cpp tests it); 0xf in all keys seen
    u16 field_0x14;

    AttributeKey(const base::global::CStrId& name, const CType* type, u32 targets)
        : name(name), type(type), targets(targets), field_0x14(0) {}
    ~AttributeKey();
};
static_assert(sizeof(AttributeKey) == 0x18, "AttributeKey layout");
struct Attributes;   // an element's attributes (0x38 bytes), created on first use
struct CVariable;    // a member field descriptor; its attributes are at +0x38
// A reflected function (0xb8 bytes, in CClass+0x110). Getter/setter-backed fields register
// one getter and one setter function and then a field that refers to them by index.
struct CFunction;
// The reflected function's virtual table, as far as the registration functions use it.
struct UnkReflFunctionVtbl {
    void* unk_0x0[9];
    Attributes* (*vf_0x48)(CFunction* self);  // the function's attributes
    void* unk_0x50;
    Attributes* (*vf_0x58)(CFunction* self, u32 index);  // a parameter's attributes
};
struct CFunction {
    const UnkReflFunctionVtbl* vtbl;
    base::global::CRntVector<const CType*> params;  // parameter types (fn_7100a19530 adds one)
    base::global::UnkStrId name;
    const CType* return_type;
    void* field_0x40;
    void* invoker;  // the function that calls the accessor
    u8 unk_0x50[0x62];
    u16 field_0xb2;
    u8 unk_0xb4[0x4];

    Attributes* Attrs() { return vtbl->vf_0x48(this); }
    Attributes* ParamAttrs(u32 index) { return vtbl->vf_0x58(this, index); }
};
static_assert(sizeof(CFunction) == 0xb8, "function array stride");

struct UnkTunableRegistry;  // 0x58 bytes, created on first use by the CTunable* field functions
struct UnkTunable;          // the global a CTunable* class registers

// A value passed type-erased: its type's descriptor and where it is.
struct TypedValueRef {
    const CType* type;
    const void* value;
};

}  // namespace base::reflection

// Get (creating on first use) a type's attribute container (CType+0x80).
base::reflection::Attributes* TypeAttributes(base::reflection::CType* type);
// Insert a (key, value) pair.
void SetAttribute(base::reflection::Attributes* attrs, const base::reflection::AttributeKey& key,
                   const base::reflection::TypedValueRef& value);

// Register a member field: name, type, offset in the object (0xffffffff for a property, which has
// none), and for a property the indices of its getter and setter among the class's functions (-1 for
// a field, and for the setter of a read-only property).
base::reflection::CVariable* AddReflectedField(base::reflection::CClass* cls, base::global::CStrId name,
                                              const base::reflection::CType* type, u64 offset, s64 getter_index,
                                              s64 setter_index);

// The names of an associative collection's entry fields, "key" and "element", built once at start-up.
extern base::global::CStrId g_unk_7101d5a450;
extern base::global::CStrId g_unk_7101d5a458;
namespace base::reflection {
inline base::global::CStrId& KeyFieldName() { return g_unk_7101d5a450; }
inline base::global::CStrId& ElementFieldName() { return g_unk_7101d5a458; }
}  // namespace base::reflection

// Add a function named `name` to a class; null if the class already has one by that name.
base::reflection::CFunction* AddReflectedFunction(base::reflection::CClass* cls, const base::global::CStrId& name);

// Resize a function's parameter attribute containers (0x38 bytes each, at +0x88) to `count`.
void ResizeParameterAttributes(base::reflection::CFunction* fn, u32 count);

// Allocation (size, flags; the flags are 0x3f for the tunable registry), and the registry's
// constructor.
void fn_710013ac14(base::reflection::UnkTunableRegistry* self);

// The CRntString inside the string the global empty id base::global::CStrId::Empty() (base/global/CStrId.h)
// points to (+0x10), whose u32 at +0x44 counts these uses; default values copy it.
inline const base::global::CRntString& refl_GlobalRntString() {
    u8* str = reinterpret_cast<u8*>(base::global::CStrId::Empty().data);
    ++*reinterpret_cast<u32*>(str + 0x44);
    return *reinterpret_cast<const base::global::CRntString*>(str + 0x10);
}

namespace base::reflection {

// The same for an id in a string-id table (UnkStrIdTable): the count is incremented through a
// may_alias pointer, so the id is reloaded after it, as in the original.
inline const base::global::CRntString& refl_RntStringOf(const base::global::CStrId& id) {
    typedef u32 __attribute__((may_alias)) a_u32;
    ++*reinterpret_cast<a_u32*>(reinterpret_cast<u8*>(id.data) + 0x44);
    return *reinterpret_cast<const base::global::CRntString*>(reinterpret_cast<u8*>(id.data) + 0x10);
}

// A class's member field by name (CClass+0xe8: fields of 0xb0 bytes, keyed by the name's
// hash), or null.
inline CVariable* refl_FindField(CClass* cls, const base::global::CName& name) {
    struct Fields {
        struct Slot {
            u8 bytes[0xb0];
        };
        Slot* data;
        const u64* keys;
        u32 count;

        Slot* Find(u64 key) {
            for (u64 i = 0; i < count; ++i)
                if (keys[i] == key)
                    return &data[i];
            return &data[count];
        }
    };
    Fields& fields = *reinterpret_cast<Fields*>(&cls->m_fields);
    auto* it = fields.Find(name.hash);
    return it == &fields.data[fields.count] ? nullptr : reinterpret_cast<CVariable*>(it);
}

// A list value (base::global::CRntVector's layout) built from an initializer list, moved into a
// reflected value and destroyed like CRntVector::Free.
template <class T>
struct UnkReflList {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    // Out of line (fn_7100228ca0 for CRntString).
    UnkReflList(std::initializer_list<T> items, u32 step, base::UnkAllocator* allocator, u32 flags);
    UnkReflList(UnkReflList&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    ~UnkReflList() {
        if (data) {
            for (u32 i = 0; i < count; ++i)
                data[i].~T();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

template <>
UnkReflList<base::global::CRntString>::UnkReflList(std::initializer_list<base::global::CRntString> items, u32 step,
                                                   base::UnkAllocator* allocator, u32 flags);  // fn_7100228ca0

// A default value made of a u32 and a list of strings (e.g. "vSegments:*:vPos"), typed by a
// global descriptor.
struct UnkReflStringList {
    u32 field_0x0;
    UnkReflList<base::global::CRntString> items;

    UnkReflStringList(u32 a, UnkReflList<base::global::CRntString>&& list)
        : field_0x0(a), items(static_cast<UnkReflList<base::global::CRntString>&&>(list)) {}
};

}  // namespace base::reflection

namespace base::reflection {

// A table of string ids built once on first use by a weak function-local static (guard and
// object reached through the GOT; destroyed through __cxa_atexit). Default values copy one of
// its ids. CTOR is its constructor's address and names the table; the size is a placeholder.
template <u64 CTOR>
struct UnkStrIdTable {
    base::global::CStrId ids[0x400];

    UnkStrIdTable();  // fn_<CTOR>
    ~UnkStrIdTable();
};

template <u64 CTOR>
UnkStrIdTable<CTOR>& refl_GetStrIdTable() {
    static UnkStrIdTable<CTOR> s_table;
    return s_table;
}

// Enum default values kept in weak function-local statics, shared program-wide (one per
// ADDR, the static's address): zero, set by a dynamic initialiser (a non-constexpr call), and
// the complement of another one's low byte.
inline u32 refl_EnumZero() {
    return 0;
}

template <u64 ADDR>
const u32& refl_GetEnumZero() {
    static u32 s_value = refl_EnumZero();
    return s_value;
}

template <u64 ADDR, u64 SRC>
const u32& refl_GetEnumNot8() {
    static u32 s_value = u8(~refl_GetEnumZero<SRC>());
    return s_value;
}

}  // namespace base::reflection

// Attribute keys: guarded statics, each built by its getter from its name and value type.
namespace base::reflection::attr {
// The keys whose values are of their own enum type, "Persistence" and "EditorModeDataFlow"
// (Persistence() and EditorModeDataFlow() in base/reflection/ReflectedFields.h pair each with that type).
const AttributeKey& PersistenceKey();
const AttributeKey& EditorModeDataFlowKey();
// The other keys, each named by the string it registers.
const AttributeKey& Version();
const AttributeKey& Description();
const AttributeKey& sBinaryDeserialize();
const AttributeKey& XmlAttribute();
const AttributeKey& sSerializationType();
const AttributeKey& DefaultValue();
const AttributeKey& Name();
const AttributeKey& UniqueId();
const AttributeKey& DeleteElement();
const AttributeKey& EraseElement();
const AttributeKey& CreateElement();
const AttributeKey& InsertElement();
const AttributeKey& AfterElementDeserialization();
const AttributeKey& FixedCollection();
const AttributeKey& ElementKeyVariable();
const AttributeKey& Required();
const AttributeKey& RntFilePackedParams();
const AttributeKey& bBinaryDumpable();
const AttributeKey& IsLink();
const AttributeKey& EditorVisible();
const AttributeKey& CharEditorVisible();
const AttributeKey& EventGroup();
const AttributeKey& TrackGroup();
const AttributeKey& MinValue();
const AttributeKey& MaxValue();
const AttributeKey& EmptyTrack();
const AttributeKey& MinValueProperty();
const AttributeKey& MaxValueProperty();
const AttributeKey& MinValueI();
const AttributeKey& MaxValueI();
const AttributeKey& Extension();
const AttributeKey& DisplayName();
const AttributeKey& TypeSelectionEnabled();
const AttributeKey& SPointChClassSelectEnabled();
const AttributeKey& AddNewItemButtonEnabled();
const AttributeKey& DeleteItemButtonEnabled();
const AttributeKey& bEditorModeDataFlowInit();
const AttributeKey& bSkipDataFlowOrder();
const AttributeKey& CompleteGameModelActionTreeFrom();
const AttributeKey& DisableListAddRemove();
const AttributeKey& DisableListAdd();
const AttributeKey& ShowInConfig();
const AttributeKey& ShowInDevConfig();
const AttributeKey& DeprecatedVar();
const AttributeKey& WidgetReadOnly();
const AttributeKey& SetRenderItemSelectionIdCall();
const AttributeKey& ePersistenceTarget();
const AttributeKey& eLinkTarget();
const AttributeKey& bWeakLink();
const AttributeKey& ePersistenceTargetExclude();
const AttributeKey& ControlFloatStep();
const AttributeKey& InternalTag();
const AttributeKey& ControlIntStep();
const AttributeKey& InternalDevTag();
const AttributeKey& DefaultPasteValue();
const AttributeKey& FileFilter();
const AttributeKey& AssetType();
const AttributeKey& BasePath();
const AttributeKey& FolderFilter();
const AttributeKey& FileNameOnly();
const AttributeKey& eActorLinkToComponentLinkTransform();
}  // namespace base::reflection::attr

// The getters of the attribute and field value types' descriptors (refl_GetType<T>: a plain type's is
// PrimitiveType<T>::Get, base/reflection/PrimitiveType.h; a string literal's, "char const*", is the pointer
// type's, base/reflection/CPointerType.h). Each is defined in one unit of src/reflection_types/ and called here.
extern template const base::reflection::CType& base::reflection::PrimitiveType<u32>::Get();
extern template const base::reflection::CType& base::reflection::PrimitiveType<s32>::Get();
extern template const base::reflection::CType& base::reflection::PrimitiveType<f32>::Get();
extern template const base::reflection::CType& base::reflection::PrimitiveType<bool>::Get();
extern template const base::reflection::CType& base::reflection::PrimitiveType<base::global::CStrId>::Get();
extern template const base::reflection::CType& base::reflection::PrimitiveType<base::global::CRntString>::Get();
extern template const base::reflection::CType& base::reflection::PrimitiveType<base::global::CFilePathStrId>::Get();
extern template const base::reflection::CType& base::reflection::PrimitiveType<base::global::CName>::Get();
extern template const base::reflection::CType& base::reflection::PrimitiveType<base::math::CVector2D>::Get();
extern template const base::reflection::CType& base::reflection::PrimitiveType<base::math::CVector3D>::Get();
extern template const base::reflection::CType& base::reflection::PrimitiveType<base::math::CVector4D>::Get();
extern template const base::reflection::CType* base::reflection::PointerType<const char*>::Get();

namespace base::reflection {

// The descriptor of a value's type T.
template <class T>
inline const CType* ValueType() {
    return &PrimitiveType<T>::Get();
}
template <>
inline const CType* ValueType<const char*>() {
    return PointerType<const char*>::Get();
}

// An attribute key whose values are enum values of the key's own enum type.
struct TypedAttrKey {
    const AttributeKey& key;
    const CType* type;
};

// An element's attributes: values under their keys' names. Only its size is written here (its layout
// is in src/base/reflection/UnkReflAttrs.cpp, which defines the insertion).
struct Attributes {
    u8 unk_0x0[0x38];

    template <class T>
    void Add(const AttributeKey& key, const T& value) {
        SetAttribute(this, key, TypedValueRef{ValueType<T>(), &value});
    }
    // With an explicit type descriptor (enum values of a field typed by a global descriptor).
    template <class T>
    void Add(const AttributeKey& key, const CType* type, const T& value) {
        SetAttribute(this, key, TypedValueRef{type, &value});
    }
    template <class T>
    void Add(const TypedAttrKey& key, const T& value) {
        Add<T>(key.key, key.type, value);
    }
    // The same with the type getter called after the value is stored.
    template <class T, auto Get>
    void AddTyped(const AttributeKey& key, const T& value) {
        SetAttribute(this, key, TypedValueRef{Get(), &value});
    }
};

struct CVariable {
    u8 unk_0x0[0x38];
    Attributes attrs;
    Attributes attrs_0x70;  // a second container (e.g. "TypeSelectionEnabled" on vector fields)

    // Its own reflection: the functions its descriptor points to and the description naming them
    // (src/reflection/, tools/gen_reflection.py).
    static bool refl_Equals(const void* a, const void* b);
    static s32 refl_Hash();
    static const CType& refl_TypeOf(const void* obj);
    static void refl_CopyConstruct(void* dst, const void* src);
    static void refl_MoveConstruct(void* dst, const void* src);
    static void refl_Destroy(void* obj);
    struct refl_Description;
};

}  // namespace base::reflection

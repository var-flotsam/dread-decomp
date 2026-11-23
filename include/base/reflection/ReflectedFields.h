#pragma once

// Member fields of a reflected class, as its refl_AddFields registers them (src/reflection/,
// base/reflection/ReflectedClass.h). One line per field:
//
//     DREAD_REFLECTED_FIELD(f32, "fSteeringSpeed", 0x510, (DefaultValue, 1.0f), (MinValue, 0.0f));
//     DREAD_REFLECTED_PROPERTY(f32, "fCurrentAutoAimWidth", (DefaultValue, 0.0f));
//     DREAD_REFLECTED_FIELD_OF("CGameLink<CActor>", "wpPathToFollow", 0x1350);
//
// registers the member at that offset under its name and type, then adds the field's attributes,
// each under its key (attr:: below). The name's string id lives only for the registration, which is
// why this is a statement-like macro and not a function. It is used in refl_AddFields(CClass* cls).

#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTU36.h"  // base::reflection::TypeObject<gameeditor::EEditorModeDataFlow>::object, EditorModeDataFlow's enum type
#include "game/UnkTU89.h"  // base::reflection::TypeObject<base::serialization::EPersistence>::object, Persistence's enum type
#include "types.h"

#include <type_traits>

// The field's attributes follow as (key, value) pairs, each added in a statement of its own as the
// original does (a value's temporary ends with its statement).
#define DREAD_REFLECTED_FIELD(T, NAME, OFFSET, ...) DREAD_REFL_FIELD_(T, NAME, OFFSET, -1, -1, __VA_ARGS__)

// A property: a field with no offset, read and written through the two functions registered just
// before it (its getter, then its setter), or with only a getter.
#define DREAD_REFLECTED_PROPERTY(T, NAME, ...)                                                         \
    DREAD_REFL_FIELD_(T, NAME, 0xffffffff, cls->m_array_0x110.size() - 2, cls->m_array_0x110.size() - 1, \
                      __VA_ARGS__)
#define DREAD_REFLECTED_READ_ONLY_PROPERTY(T, NAME, ...)                                               \
    DREAD_REFL_FIELD_(T, NAME, 0xffffffff, cls->m_array_0x110.size() - 1, -1, __VA_ARGS__)

// A field typed by a type object a static initialiser builds, by the type's name: the generated
// src/reflection/ReflectedTypes.h, which a file using this includes, looks the name up.
// Its integer attribute values (an enum's default) are values of that type.
#define DREAD_REFLECTED_FIELD_OF(TYPE_NAME, NAME, OFFSET, ...)                                         \
    DREAD_REFL_FIELD_TYPED_(base::reflection::TypeConstant<base::reflection::TypeNamed(TYPE_NAME)>::value, \
                            NAME, OFFSET, -1, -1, __VA_ARGS__)

// A reflected function of the class, named NAME: the statement or block that follows describes it through
// `fn` (its name, return type, parameters, invoker and attributes), and runs unless the class already has one by that name.
#define DREAD_REFLECTED_FUNCTION(NAME) \
    if (base::reflection::CFunction* fn = AddReflectedFunction(cls, base::global::CStrId(NAME, true)))

// The description a registration gives `fn` in place: its name (the string it is registered under), return
// type, flags and invoker (the function that calls the accessor), set in this order.
#define DREAD_REFLECTED_FUNCTION_DESCRIPTION(NAME, RETURN_TYPE, FLAGS, INVOKER) \
    fn->name = base::global::UnkStrId(NAME, true);                                \
    fn->return_type = RETURN_TYPE;                                                \
    fn->field_0xb2 = FLAGS;                                                       \
    fn->field_0x40 = nullptr;                                                     \
    fn->invoker = reinterpret_cast<void*>(&INVOKER)

// A reflected function whose registration is its description alone, in one line.
#define DREAD_REFLECTED_FUNCTION_OF(NAME, RETURN_TYPE, FLAGS, INVOKER) \
    DREAD_REFLECTED_FUNCTION(NAME) { DREAD_REFLECTED_FUNCTION_DESCRIPTION(NAME, RETURN_TYPE, FLAGS, INVOKER); }

// A field holding an object of reflected class T (its descriptor from T::refl_GetClass).
#define DREAD_REFLECTED_CLASS_FIELD(T, NAME, OFFSET, ...)                                              \
    DREAD_REFL_FIELD_OF_(T::refl_GetClass(), NAME, OFFSET, -1, -1, __VA_ARGS__)

#define DREAD_REFL_FIELD_(T, NAME, OFFSET, GETTER, SETTER, ...)                                        \
    DREAD_REFL_FIELD_OF_(base::reflection::UnkReflTypeOf<T>::Get(), NAME, OFFSET, GETTER, SETTER, __VA_ARGS__)
#define DREAD_REFL_FIELD_OF_(TYPE, NAME, OFFSET, GETTER, SETTER, ...)                                  \
    do {                                                                                               \
        [[maybe_unused]] constexpr const base::reflection::CType* field_type_ = nullptr;                \
        constexpr bool field_typed_ = false;                                                           \
        [[maybe_unused]] base::reflection::CVariable* field_ =                                      \
            AddReflectedField(cls, base::global::CStrId(NAME, true), TYPE, OFFSET, GETTER, SETTER);        \
        DREAD_REFL_ATTRS_(DREAD_REFL_NARGS_(__VA_ARGS__), __VA_ARGS__)                                 \
    } while (false)
#define DREAD_REFL_FIELD_TYPED_(TYPE, NAME, OFFSET, GETTER, SETTER, ...)                               \
    do {                                                                                               \
        [[maybe_unused]] constexpr const base::reflection::CType* field_type_ = TYPE;                   \
        constexpr bool field_typed_ = true;                                                            \
        [[maybe_unused]] base::reflection::CVariable* field_ =                                      \
            AddReflectedField(cls, base::global::CStrId(NAME, true), field_type_, OFFSET, GETTER, SETTER); \
        DREAD_REFL_ATTRS_(DREAD_REFL_NARGS_(__VA_ARGS__), __VA_ARGS__)                                 \
    } while (false)

// A string literal's value is the pointer it decays to.
#define DREAD_REFL_ATTR_(KEY, ...)                                                                     \
    base::reflection::AddAttribute<std::decay_t<decltype(__VA_ARGS__)>, field_typed_>(field_->attrs, field_type_, \
                                                                       base::reflection::attr::KEY(),  \
                                                                       __VA_ARGS__);
#define DREAD_REFL_NARGS_(...) DREAD_REFL_NARGS_I_(_, ##__VA_ARGS__, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define DREAD_REFL_NARGS_I_(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, N, ...) N
#define DREAD_REFL_ATTRS_(N, ...) DREAD_REFL_ATTRS_I_(N, __VA_ARGS__)
#define DREAD_REFL_ATTRS_I_(N, ...) DREAD_REFL_ATTRS_##N(__VA_ARGS__)
#define DREAD_REFL_ATTRS_0(...)
#define DREAD_REFL_ATTRS_1(A) DREAD_REFL_ATTR_ A
#define DREAD_REFL_ATTRS_2(A, ...) DREAD_REFL_ATTR_ A DREAD_REFL_ATTRS_1(__VA_ARGS__)
#define DREAD_REFL_ATTRS_3(A, ...) DREAD_REFL_ATTR_ A DREAD_REFL_ATTRS_2(__VA_ARGS__)
#define DREAD_REFL_ATTRS_4(A, ...) DREAD_REFL_ATTR_ A DREAD_REFL_ATTRS_3(__VA_ARGS__)
#define DREAD_REFL_ATTRS_5(A, ...) DREAD_REFL_ATTR_ A DREAD_REFL_ATTRS_4(__VA_ARGS__)
#define DREAD_REFL_ATTRS_6(A, ...) DREAD_REFL_ATTR_ A DREAD_REFL_ATTRS_5(__VA_ARGS__)
#define DREAD_REFL_ATTRS_7(A, ...) DREAD_REFL_ATTR_ A DREAD_REFL_ATTRS_6(__VA_ARGS__)
#define DREAD_REFL_ATTRS_8(A, ...) DREAD_REFL_ATTR_ A DREAD_REFL_ATTRS_7(__VA_ARGS__)
#define DREAD_REFL_ATTRS_9(A, ...) DREAD_REFL_ATTR_ A DREAD_REFL_ATTRS_8(__VA_ARGS__)
#define DREAD_REFL_ATTRS_10(A, ...) DREAD_REFL_ATTR_ A DREAD_REFL_ATTRS_9(__VA_ARGS__)
#define DREAD_REFL_ATTRS_11(A, ...) DREAD_REFL_ATTR_ A DREAD_REFL_ATTRS_10(__VA_ARGS__)
#define DREAD_REFL_ATTRS_12(A, ...) DREAD_REFL_ATTR_ A DREAD_REFL_ATTRS_11(__VA_ARGS__)

namespace base::reflection {

// A type object as a compile-time constant (DREAD_REFLECTED_FIELD_OF).
template <const CType* TYPE>
struct TypeConstant {
    static_assert(TYPE != nullptr, "no type object of that name in src/reflection/ReflectedTypes.h");
    static constexpr const CType* value = TYPE;
};

// Adds one attribute of a field. In a field typed by a type object (TYPED), an integer value is a
// value of that type, added under it.
template <class T, bool TYPED>
inline void AddAttribute(UnkReflAttrs& attrs, const CType* field_type, const UnkReflAttrKey& key, const T& value) {
    if constexpr (TYPED && std::is_same_v<T, s32>)
        attrs.Add<T>(key, field_type, value);
    else
        attrs.Add<T>(key, value);
}
template <class T, bool TYPED>
inline void AddAttribute(UnkReflAttrs& attrs, const CType*, const TypedAttrKey& key, const T& value) {
    attrs.Add<T>(key, value);
}

}  // namespace base::reflection

// The attribute keys, by the names they register (each a guarded static key).
namespace base::reflection::attr {
inline const UnkReflAttrKey& Version() { return fn_7100005070(); }
inline const UnkReflAttrKey& Description() { return fn_7100007e80(); }
inline TypedAttrKey Persistence() { return {fn_710000bf80(), &base::reflection::TypeObject<base::serialization::EPersistence>::object}; }
inline const UnkReflAttrKey& sBinaryDeserialize() { return fn_7100011040(); }
inline const UnkReflAttrKey& XmlAttribute() { return fn_710001b9f0(); }
inline const UnkReflAttrKey& sSerializationType() { return fn_7100026bf0(); }
inline const UnkReflAttrKey& DefaultValue() { return fn_7100075240(); }
inline const UnkReflAttrKey& Name() { return fn_710008f920(); }
inline const UnkReflAttrKey& UniqueId() { return fn_710008fa30(); }
inline const UnkReflAttrKey& DeleteElement() { return fn_710008fb40(); }
inline const UnkReflAttrKey& EraseElement() { return fn_710008fc50(); }
inline const UnkReflAttrKey& CreateElement() { return fn_710008fd60(); }
inline const UnkReflAttrKey& InsertElement() { return fn_710008fe70(); }
inline const UnkReflAttrKey& AfterElementDeserialization() { return fn_710008ff80(); }
inline const UnkReflAttrKey& FixedCollection() { return fn_7100090090(); }
inline const UnkReflAttrKey& ElementKeyVariable() { return fn_71000901a0(); }
inline const UnkReflAttrKey& Required() { return fn_71000903c0(); }
inline const UnkReflAttrKey& RntFilePackedParams() { return fn_71000904d0(); }
inline const UnkReflAttrKey& bBinaryDumpable() { return fn_7100090800(); }
inline const UnkReflAttrKey& IsLink() { return fn_7100090910(); }
inline const UnkReflAttrKey& EditorVisible() { return fn_7100090a20(); }
inline const UnkReflAttrKey& CharEditorVisible() { return fn_7100090c40(); }
inline const UnkReflAttrKey& EventGroup() { return fn_7100090d50(); }
inline const UnkReflAttrKey& TrackGroup() { return fn_7100090e60(); }
inline const UnkReflAttrKey& MinValue() { return fn_7100091080(); }
inline const UnkReflAttrKey& MaxValue() { return fn_7100091190(); }
inline const UnkReflAttrKey& EmptyTrack() { return fn_71000912a0(); }
inline const UnkReflAttrKey& MinValueProperty() { return fn_71000913b0(); }
inline const UnkReflAttrKey& MaxValueProperty() { return fn_71000914c0(); }
inline const UnkReflAttrKey& MinValueI() { return fn_71000916e0(); }
inline const UnkReflAttrKey& MaxValueI() { return fn_71000917f0(); }
inline const UnkReflAttrKey& Extension() { return fn_7100091900(); }
inline const UnkReflAttrKey& DisplayName() { return fn_7100091c30(); }
inline const UnkReflAttrKey& TypeSelectionEnabled() { return fn_7100091d40(); }
inline const UnkReflAttrKey& SPointChClassSelectEnabled() { return fn_7100091e50(); }
inline const UnkReflAttrKey& AddNewItemButtonEnabled() { return fn_7100091f60(); }
inline const UnkReflAttrKey& DeleteItemButtonEnabled() { return fn_7100092070(); }
inline TypedAttrKey EditorModeDataFlow() { return {fn_71000923a0(), &base::reflection::TypeObject<gameeditor::EEditorModeDataFlow>::object}; }
inline const UnkReflAttrKey& bEditorModeDataFlowInit() { return fn_71000924b0(); }
inline const UnkReflAttrKey& bSkipDataFlowOrder() { return fn_71000925c0(); }
inline const UnkReflAttrKey& CompleteGameModelActionTreeFrom() { return fn_71000926d0(); }
inline const UnkReflAttrKey& DisableListAddRemove() { return fn_7100092b10(); }
inline const UnkReflAttrKey& DisableListAdd() { return fn_7100092c20(); }
inline const UnkReflAttrKey& ShowInConfig() { return fn_7100093170(); }
inline const UnkReflAttrKey& ShowInDevConfig() { return fn_7100093280(); }
inline const UnkReflAttrKey& DeprecatedVar() { return fn_7100093390(); }
inline const UnkReflAttrKey& WidgetReadOnly() { return fn_7100234410(); }
inline const UnkReflAttrKey& SetRenderItemSelectionIdCall() { return fn_7100234780(); }
inline const UnkReflAttrKey& ePersistenceTarget() { return fn_710025d990(); }
inline const UnkReflAttrKey& eLinkTarget() { return fn_710025daa0(); }
inline const UnkReflAttrKey& bWeakLink() { return fn_710025dbb0(); }
inline const UnkReflAttrKey& ePersistenceTargetExclude() { return fn_7100263720(); }
inline const UnkReflAttrKey& ControlFloatStep() { return fn_7100324af0(); }
inline const UnkReflAttrKey& InternalTag() { return fn_7100324c00(); }
inline const UnkReflAttrKey& ControlIntStep() { return fn_7100324d10(); }
inline const UnkReflAttrKey& InternalDevTag() { return fn_71003ba540(); }
inline const UnkReflAttrKey& DefaultPasteValue() { return fn_710075a400(); }
inline const UnkReflAttrKey& FileFilter() { return fn_7100ed8b60(); }
inline const UnkReflAttrKey& AssetType() { return fn_7100f9e870(); }
inline const UnkReflAttrKey& BasePath() { return fn_7100f9e980(); }
inline const UnkReflAttrKey& FolderFilter() { return fn_7100f9ea90(); }
inline const UnkReflAttrKey& FileNameOnly() { return fn_7100f9eba0(); }
inline const UnkReflAttrKey& eActorLinkToComponentLinkTransform() { return fn_71010434d0(); }
}  // namespace base::reflection::attr

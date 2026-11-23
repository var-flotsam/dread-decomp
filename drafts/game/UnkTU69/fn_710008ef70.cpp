// Unit 69 (tools/tu_map.py): fn_710008ef70 looks a collection field's "CreateElement" function up and
// walks what a reader of the field would need: the element class's fields (stopping at the first one
// marked "XmlAttribute") and the function's parameters (each one's name and "Required" attribute). The
// results are not used. Names and layouts are placeholders.
#include <algorithm>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU80.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::CType;
using base::reflection::UnkReflAttrKey;
using base::reflection::CVariable;
using base::reflection::CFunction;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

// A value name, as fn_710009b050 returns them (src/base/reflection/UnkEnumValueNames.cpp).
struct UnkEnumNameId {
    base::global::UnkStringData* data;

    UnkEnumNameId(const CStrId& id) : data(id.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkEnumNameId(UnkEnumNameId&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
    ~UnkEnumNameId() { reinterpret_cast<CStrId*>(this)->~CStrId(); }
};

struct UnkEnumName {
    UnkEnumNameId name;
    union {
        void* value;
        u64 bits;
    };
};
static_assert(sizeof(UnkEnumName) == 0x10, "UnkEnumName layout");

struct UnkEnumNames : base::global::CRntVector<UnkEnumName> {
    explicit UnkEnumNames(u32 flags) : base::global::CRntVector<UnkEnumName>(flags) {}
    UnkEnumNames(const UnkEnumNames& other);
};

struct UnkEnumType;
UnkEnumNames fn_710009b050(const UnkEnumType* type);
CFunction* fn_71000979a0(CClass* self, CVariable* field, const UnkReflAttrKey& key);

// An attribute's value: its type, and the value inline when it fits in 8 bytes (else through
// field_0x0).
struct UnkAttrValue_l08ef70 {
    void* field_0x0;
    const CType* type;
    u32 kind;
    u32 field_0x14;

    UnkAttrValue_l08ef70() : field_0x0(nullptr), type(nullptr), kind(2), field_0x14(1) {}
    UnkAttrValue_l08ef70(const UnkAttrValue_l08ef70& other) {
        field_0x0 = nullptr;
        type = nullptr;
        kind = other.kind;
        fn_71000a18d4(Raw(), other.type, other.type ? other.Value() : nullptr, false);
    }
    ~UnkAttrValue_l08ef70() { fn_71000a1c60(Raw()); }

    UnkReflValue* Raw() { return reinterpret_cast<UnkReflValue*>(this); }
    u64 TypeSize() const { return *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(type) + 0x18); }
    const void* Value() const { return (field_0x14 == 1 || TypeSize() > 8) ? field_0x0 : this; }

    // Whether the value is a bool, and the value as one.
    bool IsBool() const { return type && fn_710009d50c(type, fn_710000d228()); }
    bool AsBool() const { return *static_cast<const bool*>(Value()); }
    // Whether the value is a bool and true.
    bool IsTrue() const { return IsBool() && AsBool(); }
};
static_assert(sizeof(UnkAttrValue_l08ef70) == 0x18, "UnkAttrValue_l08ef70 layout");

// An attribute set: the values and their keys (the key names' string data), `count` of each.
struct UnkAttrSet_l08ef70 {
    UnkAttrValue_l08ef70* values;
    const void** keys;
    u32 count;

    const UnkAttrValue_l08ef70* Find(const UnkReflAttrKey& key) const {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key.name.data)
                return &values[i];
        return values + count;
    }
    const UnkAttrValue_l08ef70* End() const { return values + count; }

    // A copy of the value under `key` (an empty value without one).
    UnkAttrValue_l08ef70 Get(const UnkReflAttrKey& key) const {
        const UnkAttrValue_l08ef70* found = Find(key);
        return found != End() ? UnkAttrValue_l08ef70(*found) : UnkAttrValue_l08ef70();
    }

};

// A member field of a class (0xb0 bytes): its type object and its attributes.
struct UnkClassField_l08ef70 {
    u8 pad_0x0[0x10];
    UnkReflObject* type;  // 0x10
    u8 pad_0x18[0x20];
    UnkAttrSet_l08ef70 attrs;  // 0x38
    u8 pad_0x50[0x60];
};
static_assert(sizeof(UnkClassField_l08ef70) == 0xb0, "UnkClassField_l08ef70 layout");

// A collection type: its element's type object at +0xf0.
struct UnkCollectionType_l08ef70 {
    u8 pad_0x0[0xf0];
    UnkReflObject* element;
};

// Whether the type object is of the class `get` returns (its dynamic class asked first).
static inline bool IsA(const CType* type, const CClass* (*get)()) {
    const CClass* dynamic = reinterpret_cast<const UnkReflObject*>(type)->vf_0x0();
    return fn_710009d50c(dynamic, get());
}

// The type object if it is of the class `get` returns, else null.
static inline const CType* CastTo(const CType* type, const CClass* (*get)()) {
    return IsA(type, get) ? type : nullptr;
}

// The attributes of the function's parameter `index` (vtable slot 0x50).
static inline const UnkAttrSet_l08ef70* ParamAttrs(CFunction* function, u32 index) {
    using Fn = const UnkAttrSet_l08ef70* (*)(CFunction*, u32);
    return reinterpret_cast<Fn>(function->vtbl->unk_0x50)(function, index);
}

// `self` and `node` are not read here.
void fn_710008ef70(void* self, void* node, UnkClassField_l08ef70* field, CClass* cls) {
    CFunction* create = fn_71000979a0(cls, reinterpret_cast<CVariable*>(field), fn_710008fd60());
    if (!create)
        return;
    const CType* collection = fn_710009c640(field->type);
    const CType* element =
        fn_710009c640(reinterpret_cast<const UnkCollectionType_l08ef70*>(CastTo(collection, fn_71000986d0))->element);
    if (!element || !IsA(element, fn_7100096100))
        return;

    // Whether an element field is written as an XML attribute (the result is not used).
    const CClass* element_class = static_cast<const CClass*>(element);
    UnkClassField_l08ef70* fields = static_cast<UnkClassField_l08ef70*>(element_class->m_array_0xe8.data);
    UnkClassField_l08ef70* end = fields + element_class->m_array_0xe8.count;
    bool has_attribute = std::any_of(fields, end, [](const UnkClassField_l08ef70& f) {
        return f.attrs.Get(fn_710001b9f0()).IsTrue();
    });
    (void)has_attribute;

    // The parameters' names and whether each is required (the results are not used).
    UnkEnumNames params = fn_710009b050(reinterpret_cast<const UnkEnumType*>(create));
    for (u64 i = 0; i < params.count; ++i) {
        reinterpret_cast<const CStrId&>(params.data[i].name).Text();
        const UnkAttrSet_l08ef70* attrs = ParamAttrs(create, i);
        if (attrs->count != 0) {
            bool required = attrs->Get(fn_71000903c0()).IsTrue();
            (void)required;
        }
    }
    params.Free();
}

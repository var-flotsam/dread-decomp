// base::reflection::CClass's unit (unit 72): the class's function (CClass+0x110, by name) that a
// member field's string-id attribute under `key` names; null when the field has no such attribute, its
// value is not a string id, or the class has no function by that name. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrIdInline;
using base::reflection::CClass;
using base::reflection::CType;
using base::reflection::UnkReflAttrKey;
using base::reflection::UnkReflField;
using base::reflection::UnkReflFunction;
using base::reflection::UnkReflValue;

// A reflected value as an attribute set stores it: its type, and the value inline when it fits in
// 8 bytes (else through field_0x0).
struct UnkAttrValue_l0979a0 {
    void* field_0x0;
    const CType* type;
    u32 kind;
    u32 field_0x14;

    UnkAttrValue_l0979a0() : field_0x0(nullptr), type(nullptr), kind(2), field_0x14(1) {}
    UnkAttrValue_l0979a0(const UnkAttrValue_l0979a0& other) {
        field_0x0 = nullptr;
        type = nullptr;
        kind = other.kind;
        fn_71000a18d4(Raw(), other.type, other.type ? other.Value() : nullptr, false);
    }
    ~UnkAttrValue_l0979a0() { fn_71000a1c60(Raw()); }

    UnkReflValue* Raw() { return reinterpret_cast<UnkReflValue*>(this); }
    u64 TypeSize() const { return *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(type) + 0x18); }
    const void* Value() const { return (field_0x14 == 1 || TypeSize() > 8) ? field_0x0 : this; }

    // Whether the value is a string id, and the value as one.
    bool IsStrId() const { return type && fn_710009d50c(type, fn_7100009658()); }
    const CStrId& AsStrId() const { return *static_cast<const CStrId*>(Value()); }
};
static_assert(sizeof(UnkAttrValue_l0979a0) == 0x18, "UnkAttrValue_l0979a0 layout");

// A field's attribute set: the values and their keys (the key names' string data), `count` of each.
struct UnkAttrSet_l0979a0 {
    UnkAttrValue_l0979a0* values;
    const void** keys;
    u32 count;

    const UnkAttrValue_l0979a0* Find(const UnkReflAttrKey& key) const {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key.name.data)
                return &values[i];
        return values + count;
    }
    const UnkAttrValue_l0979a0* End() const { return values + count; }
};

// The class's functions (CClass+0x110) and their names' string data, `count` of each.
struct UnkFunctionSet_l0979a0 {
    UnkReflFunction* functions;
    const void** keys;
    u32 count;

    UnkReflFunction* Find(const CStrId& name) const {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == name.data)
                return &functions[i];
        return functions + count;
    }
    UnkReflFunction* End() const { return functions + count; }
};

// The class's function named `name`, or null.
static inline UnkReflFunction* FindFunction(CClass* cls, const CStrId& name) {
    const auto* set = reinterpret_cast<const UnkFunctionSet_l0979a0*>(&cls->m_array_0x110);
    UnkReflFunction* it = set->Find(name);
    return it == set->End() ? nullptr : it;
}

UnkReflFunction* fn_71000979a0(CClass* self, UnkReflField* field, const UnkReflAttrKey& key) {
    const auto* attrs = reinterpret_cast<const UnkAttrSet_l0979a0*>(&field->attrs);
    const UnkAttrValue_l0979a0* found = attrs->Find(key);
    UnkAttrValue_l0979a0 value = found != attrs->End() ? UnkAttrValue_l0979a0(*found) : UnkAttrValue_l0979a0();
    UnkReflFunction* function = nullptr;
    if (value.type) {
        CStrId name(UnkStrIdInline(), nullptr, false);
        if (value.IsStrId()) {
            name = value.AsStrId();
            function = FindFunction(self, name);
        }
    }
    return function;
}

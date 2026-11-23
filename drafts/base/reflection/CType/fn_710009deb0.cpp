// CType (unit 81): the type a type is serialized as. Its "sSerializationType" attribute names it
// by string id, looked up in the registry of types by hash; a type without one is its own.
// Names and layouts are placeholders.
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU83.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkHashedString;
using base::reflection::CType;
using base::reflection::UnkReflValue;
using base::reflection::refl_ArgValue;

namespace {

// The type's attribute container (CType+0x80) and its lookup by name.
struct UnkAttrTable_l09deb0 {
    UnkReflValue* values;
    const void* const* keys;
    u32 count;

    UnkReflValue* Find(const CStrId& name) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == name.data)
                return &values[i];
        return &values[count];
    }
};

// A copy of the attribute stored under `name`, or an empty value when the type has none.
inline UnkReflValue Attribute(const CType* type, const CStrId& name) {
    UnkReflValue r;
    if (auto* attrs = static_cast<UnkAttrTable_l09deb0*>(type->field_0x80)) {
        UnkReflValue* it = attrs->Find(name);
        if (it != &attrs->values[attrs->count]) {
            r.field_0x0 = nullptr;
            r.type = nullptr;
            r.kind = it->kind;
            const CType* t = reinterpret_cast<const CType*>(it->type);
            const void* value = nullptr;
            if (t)
                value = it->field_0x14 == 1 || t->m_size > 8 ? it->field_0x0 : static_cast<const void*>(it);
            fn_71000a18d4(&r, t, value, false);
            return r;
        }
    }
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}

// Whether `type` (not null) is T's type or derives from it.
template <class T>
inline bool IsKindOf(const CType* type) {
    const CType* base = base::reflection::UnkReflTypeOf<T>::Get();
    do {
        if (type == base)
            return true;
        type = type->m_parent;
    } while (type);
    return false;
}

// The 64-bit hash of an id's string.
inline u64 HashOf(const CStrId& id) {
    return *reinterpret_cast<const u64*>(static_cast<const UnkHashedString*>(id.data)->hash);
}

}  // namespace

const CType* fn_710009deb0(const CType* self) {
    static CStrId s_name("sSerializationType", true);
    UnkReflValue value = Attribute(self, s_name);
    if (value.type) {
        CStrId name(base::global::UnkStrIdInline(), nullptr, false);
        if (value.type && IsKindOf<CStrId>(reinterpret_cast<const CType*>(value.type)))
            name = *refl_ArgValue<CStrId>(&value);
        UnkVersionRegistry* registry = g_unk_7101d4c8d0;
        u64 hash = HashOf(name);
        self = reinterpret_cast<const CType*>(fn_71000a5118(registry, &hash));
    }
    fn_71000a1c60(&value);
    return self;
}

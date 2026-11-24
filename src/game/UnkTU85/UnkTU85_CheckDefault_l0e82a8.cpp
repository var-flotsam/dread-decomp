// UnkTU85 (tools/tu_map.py unit 85): checks a reflected field's default value. The value stored under the
// id table's slot +0xda8 in the field's attributes is copied, the field's current value is read from the
// object (when one is given) and the default is taken as the field's type; the comparison itself is
// compiled out. Names and layouts are placeholders.
#include "base/core/CBaseObject.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;
using base::math::CVector4D;
using base::math::UnkVector4D;
using base::reflection::CType;
using base::reflection::UnkReflTypeOf;
using base::reflection::UnkReflValue;

struct UnkFieldRecord_l0e82a8 {
    u8 pad_0x0[0x10];
    const CType* type;  // 0x10
    u8 pad_0x18[0x20];
    base::global::CRntSmallDictionary<CStrId, UnkReflValue> attributes;  // 0x38
    u8 pad_0x60[0x50];
};
static_assert(__builtin_offsetof(UnkFieldRecord_l0e82a8, attributes) == 0x38, "layout");
static_assert(sizeof(UnkFieldRecord_l0e82a8) == 0xb0, "layout");

// What the field getter takes: the value's type and where to put it.
struct UnkFieldValueArg_l0e82a8 {
    const CType* type;
    void* value;
};

// Where the value is held: inline when it fits in 8 bytes, else through field_0x0.
inline const void* ValueData(const UnkReflValue& value) {
    if (!value.type)
        return nullptr;
    if (value.field_0x14 == 1 || reinterpret_cast<const CType*>(value.type)->m_size > 8)
        return value.field_0x0;
    return &value;
}

inline UnkReflValue CopyValue(const UnkReflValue& from) {
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = from.kind;
    fn_71000a18d4(&r, reinterpret_cast<const CType*>(from.type), ValueData(from), false);
    return r;
}

template <class T>
inline bool ValueIs(const UnkReflValue& value) {
    return value.type && fn_710009d50c(reinterpret_cast<const CType*>(value.type), UnkReflTypeOf<T>::Get());
}

template <class T>
inline const T* ValueAs(const UnkReflValue& value) {
    return static_cast<const T*>(ValueData(value));
}

inline void GetField(UnkFieldRecord_l0e82a8* field, void* obj, UnkFieldValueArg_l0e82a8* arg) {
    fn_71000e78e0(reinterpret_cast<UnkReflField*>(field), static_cast<const UnkReflFieldObject*>(obj),
                  reinterpret_cast<UnkReflValueArg*>(arg));
}

void fn_71000e82a8(UnkElem_0xb0* entry, void* obj) {
    UnkFieldRecord_l0e82a8* self = reinterpret_cast<UnkFieldRecord_l0e82a8*>(entry);
    const UnkReflValue* found = self->attributes.Find(GameManagerIdTable().field_0xda8);
    UnkReflValue def = found != self->attributes.End() ? CopyValue(*found) : base::reflection::refl_EmptyValue();
    // Each case reads the current value and takes the default as the field's type; what compared them
    // is not in this build.
    switch (CBaseObject_GetTypeCodeChar(self->type)) {
    case 'b': {
        bool value = false;
        if (obj) {
            UnkFieldValueArg_l0e82a8 arg = {UnkReflTypeOf<bool>::Get(), &value};
            GetField(self, obj, &arg);
        }
        bool default_value = false;
        if (ValueIs<bool>(def))
            default_value = *ValueAs<bool>(def);
        break;
    }
    case 'f': {
        f32 value = 0.0f;
        if (obj) {
            UnkFieldValueArg_l0e82a8 arg = {UnkReflTypeOf<f32>::Get(), &value};
            GetField(self, obj, &arg);
        }
        f32 default_value = 0.0f;
        if (ValueIs<f32>(def))
            default_value = *ValueAs<f32>(def);
        break;
    }
    case 'i': {
        s32 value = 0;
        if (obj) {
            UnkFieldValueArg_l0e82a8 arg = {UnkReflTypeOf<s32>::Get(), &value};
            GetField(self, obj, &arg);
        }
        s32 default_value = 0;
        if (ValueIs<s32>(def))
            default_value = *ValueAs<s32>(def);
        break;
    }
    case 's': {
        CRntString value(g_unk_710162e2ed);
        if (obj) {
            UnkFieldValueArg_l0e82a8 arg = {UnkReflTypeOf<CRntString>::Get(), &value};
            GetField(self, obj, &arg);
        }
        CRntString default_value(g_unk_710162e2ed);
        if (ValueIs<CRntString>(def))
            CRntString_Assign(&default_value, ValueAs<CRntString>(def));
        break;
    }
    case 'v': {
        CVector3D value;
        value.x = 0.0f;
        value.y = 0.0f;
        value.z = 0.0f;
        if (obj) {
            UnkFieldValueArg_l0e82a8 arg = {UnkReflTypeOf<CVector3D>::Get(), &value};
            GetField(self, obj, &arg);
        }
        CVector3D default_value = {0.0f, 0.0f, 0.0f};
        if (ValueIs<CVector3D>(def))
            default_value = *ValueAs<CVector3D>(def);
        break;
    }
    case 'w': {
        // CVector4D has a user-written constructor and assignment (fn_710007b518).
        UnkVector4D value;
        if (obj) {
            UnkFieldValueArg_l0e82a8 arg = {UnkReflTypeOf<CVector4D>::Get(), &value};
            GetField(self, obj, &arg);
        }
        UnkVector4D default_value;
        if (ValueIs<CVector4D>(def))
            fn_710007b518(&default_value, ValueAs<CVector4D>(def));
        break;
    }
    case 'y': {
        CVector2D value = {0.0f, 0.0f};
        if (obj) {
            UnkFieldValueArg_l0e82a8 arg = {UnkReflTypeOf<CVector2D>::Get(), &value};
            GetField(self, obj, &arg);
        }
        CVector2D default_value = {0.0f, 0.0f};
        if (ValueIs<CVector2D>(def))
            default_value = *ValueAs<CVector2D>(def);
        break;
    }
    default:
        break;
    }
    fn_71000a1c60(&def);
}

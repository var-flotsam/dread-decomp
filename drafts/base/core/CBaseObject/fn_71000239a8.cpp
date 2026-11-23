// base::core::CBaseObject's unit (unit 29): copies one reflected field of `src` into the field of
// the same name in `dst` (the clone fn_71000238c8 runs it for every field), reading and writing the
// value as the field's type code says. Names and layouts are placeholders.
#include "base/core/CBaseObject.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "base/reflection.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStringData;
using base::reflection::CClass;
using base::reflection::CType;
using base::reflection::UnkReflValue;

struct UnkClassField_b49a1 {
    u8 data[0xb0];
};

struct UnkBaseObject_b49a1 {
    virtual const CClass* vf_0x0() const;
};

// A reflected field (0xb0 bytes) as read here: its name and its type.
struct UnkCopyField {
    const UnkStringData* name;
    u8 pad_0x8[0x8];
    const CType* type;  // +0x10
    u8 pad_0x18[0x98];
};
static_assert(sizeof(UnkCopyField) == 0xb0, "UnkCopyField layout");

// A class descriptor's fields, with their name keys alongside.
struct UnkCopyClass {
    u8 pad_0x0[0xe8];
    UnkCopyField* fields;  // +0xe8
    u64* keys;             // +0xf0
    u32 count;             // +0xf8

    UnkCopyField* End() { return fields + count; }
    UnkCopyField* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (key == keys[i])
                return &fields[i];
        return End();
    }
};

// An object or a value with its type, as the field accessors take them.
struct UnkCopyRef {
    const void* type;
    const void* ptr;
};

inline bool GetField(UnkCopyField* field, const UnkCopyRef* obj, UnkCopyRef* value) {
    return fn_71000e78e0(reinterpret_cast<CVariable*>(field), reinterpret_cast<const UnkReflFieldObject*>(obj),
                         reinterpret_cast<UnkReflValueArg*>(value));
}
inline bool SetField(UnkCopyField* field, const UnkCopyRef* obj, UnkCopyRef* value) {
    return fn_71000e7a80(reinterpret_cast<CVariable*>(field), reinterpret_cast<const UnkReflFieldObject*>(obj),
                         reinterpret_cast<UnkReflValueArg*>(value));
}

// Where a reflected value's data is: inline when it fits in 8 bytes, else through field_0x0.
inline void* ValueData(UnkReflValue* v) {
    if (!v->type)
        return nullptr;
    if (v->field_0x14 == 1 || reinterpret_cast<const CType*>(v->type)->m_size > 8)
        return v->field_0x0;
    return v;
}

// Reads the field of `source` as a T and writes it to the field of `target`.
template <class T, const CType* (*TypeOf)()>
inline bool CopyAs(UnkCopyField* from, const UnkCopyRef* source, UnkCopyField* to, const UnkCopyRef* target) {
    T value = T();
    UnkCopyRef arg = {TypeOf(), &value};
    if (!GetField(from, source, &arg))
        return false;
    arg.type = TypeOf();
    arg.ptr = &value;
    return SetField(to, target, &arg);
}

bool fn_71000239a8(void* dst, UnkBaseObject_b49a1* src, UnkClassField_b49a1* field, u32 flag) {
    UnkBaseObject_b49a1* self = static_cast<UnkBaseObject_b49a1*>(dst);
    UnkCopyField* from = reinterpret_cast<UnkCopyField*>(field);
    UnkCopyRef source = {src->vf_0x0(), src};
    UnkCopyRef target = {fn_710009dcd8(reinterpret_cast<char*>(const_cast<CClass*>(self->vf_0x0())), self), self};
    UnkCopyClass* cls = reinterpret_cast<UnkCopyClass*>(const_cast<CClass*>(self->vf_0x0()));
    UnkCopyField* to = cls->Find(from->name->Key());
    if (!to || to == cls->End())
        return false;

    switch (fn_7100024140(from->type)) {
    case 'b':
        return CopyAs<bool, fn_710000d228>(from, &source, to, &target);
    case 'e': {
        UnkReflValue value;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        value.field_0x14 = 1;
        fn_71000a2014(&value, from->type);
        UnkCopyRef arg = {value.type, ValueData(&value)};
        if (GetField(from, &source, &arg)) {
            fn_71000a1710(reinterpret_cast<UnkReflValueArg*>(&arg), &value);
            bool result = SetField(to, &target, &arg);
            fn_71000a1c60(&value);
            return result;
        }
        fn_71000a1c60(&value);
        break;
    }
    case 'f':
        return CopyAs<f32, fn_710000abbc>(from, &source, to, &target);
    case 'i':
        return CopyAs<s32, fn_7100014768>(from, &source, to, &target);
    case 's': {
        CRntString value(g_unk_710162e2ed);
        UnkCopyRef arg = {fn_7100007f84(), &value};
        if (GetField(from, &source, &arg)) {
            arg.type = fn_7100007f84();
            arg.ptr = &value;
            return SetField(to, &target, &arg);
        }
        break;
    }
    case 'v':
        return CopyAs<base::math::CVector3D, fn_71000246e4>(from, &source, to, &target);
    case 'w':
        return CopyAs<base::math::CVector4D, fn_71000248c4>(from, &source, to, &target);
    case 'y':
        return CopyAs<base::math::CVector2D, fn_7100024504>(from, &source, to, &target);
    }
    return false;
}

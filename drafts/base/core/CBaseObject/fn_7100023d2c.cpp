// base::core::CBaseObject's unit (unit 29): reads one reflected field of `self` from a file, as the
// field's type code says (raw bytes for numbers and vectors, a string for strings and enum names, the
// flag set's own reader for flag sets), and sets the field. Names and layouts are placeholders.
#include "base/animation/CBlendSpaceResource.h"
#include "base/core/CBaseObject.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "base/reflection/CClass.h"
#include "base/reflection/CEnumType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/UnkTU56.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CRntString;
using base::reflection::CEnumType;
using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflTypedPtr;

struct UnkFlagsetType_b0068;
struct UnkReflFieldInfo;
struct UnkFieldOwner;

bool fn_710009a430(UnkFlagsetType_b0068* self, const UnkReflFieldObject* obj, CVariable* field, CRntFile* file);
bool fn_7100099890(const CEnumType* self, const char* name, s32* out);
bool fn_71000e74bc(CVariable* field);  // whether a field without an accessor can be set
UnkReflTypedPtr fn_71000e755c(const UnkReflFieldInfo* field, UnkFieldOwner* owner);
// Sets the enum value `value` in the object `obj` points to.
void fn_7100099960(const CEnumType* self, const UnkReflFieldObject* obj, s32 value);

// A reflected field as read here: its type.
struct UnkReadField {
    u8 pad_0x0[0x10];
    UnkReflObject* type;  // +0x10
};

// An object or a value with its type, as the field setter takes them.
struct UnkReadRef {
    const void* type;
    void* ptr;
};

inline bool SetField(UnkReadField* field, const UnkReadRef* obj, UnkReadRef* value) {
    return fn_71000e7a80(reinterpret_cast<CVariable*>(field), reinterpret_cast<const UnkReflFieldObject*>(obj),
                         reinterpret_cast<UnkReflValueArg*>(value));
}

// Where the field's member of `self` is, with its type.
inline UnkReflTypedPtr MemberOf(UnkReflObject* self, UnkReadField* field) {
    UnkReadRef obj = {self->vf_0x0(), self};
    return fn_71000e755c(reinterpret_cast<const UnkReflFieldInfo*>(field), reinterpret_cast<UnkFieldOwner*>(&obj));
}

// Reads a T's bytes from the file into `value` and sets the field to it.
template <class T, const CType* (*TypeOf)()>
inline bool ReadInto(UnkReflObject* self, UnkReadField* field, CRntFile* file, T& value) {
    CRntFile_Read(file, &value, sizeof(T), 1);
    UnkReadRef obj = {self->vf_0x0(), self};
    UnkReadRef arg = {TypeOf(), &value};
    return SetField(field, &obj, &arg);
}

bool fn_7100023d2c(UnkReflObject* self, UnkReadField* field, CRntFile* file) {
    if (!field)
        return false;

    switch (fn_7100024140(reinterpret_cast<const CType*>(field->type))) {
    case 'F': {
        UnkReflObject* type = field->type;
        UnkReflObject* flagset = type && fn_710009d50c(type->vf_0x0(), fn_710009a11c()) ? type : nullptr;
        UnkReadRef obj = {self->vf_0x0(), self};
        // The reader's result is not used: the field counts as not read.
        fn_710009a430(reinterpret_cast<UnkFlagsetType_b0068*>(flagset), reinterpret_cast<const UnkReflFieldObject*>(&obj),
                      reinterpret_cast<CVariable*>(field), file);
        return false;
    }
    case 'I': {
        s64 value;  // read from the file
        return ReadInto<s64, fn_7100024324>(self, field, file, value);
    }
    case 'U': {
        u64 value;  // read from the file
        return ReadInto<u64, fn_7100010e58>(self, field, file, value);
    }
    case 'b': {
        bool value;  // read from the file
        return ReadInto<bool, fn_710000d228>(self, field, file, value);
    }
    case 'f': {
        f32 value;  // read from the file
        return ReadInto<f32, fn_710000abbc>(self, field, file, value);
    }
    case 'i': {
        s32 value;  // read from the file
        return ReadInto<s32, fn_7100014768>(self, field, file, value);
    }
    case 's': {
        const char* text;
        CRntFile_ReadCString(file, &text);
        UnkReadRef obj = {self->vf_0x0(), self};
        CRntString value(text);
        UnkReadRef arg = {fn_7100007f84(), &value};
        return SetField(field, &obj, &arg);
    }
    case 'w': {
        base::math::CVector4D value = {0.0f, 0.0f, 0.0f, 0.0f};
        return ReadInto<base::math::CVector4D, fn_71000248c4>(self, field, file, value);
    }
    case 'y': {
        base::math::CVector2D value = {0.0f, 0.0f};
        return ReadInto<base::math::CVector2D, fn_7100024504>(self, field, file, value);
    }
    case 'e': {
        const char* name;
        CRntFile_ReadCString(file, &name);
        UnkReflObject* type = field->type;
        const CEnumType* enum_type = type && fn_710009d50c(type->vf_0x0(), fn_710009921c())
                                         ? reinterpret_cast<const CEnumType*>(type)
                                         : nullptr;
        s32 value;  // set by the lookup
        if (!fn_7100099890(enum_type, name, &value))
            return false;
        if (!fn_71000e74bc(reinterpret_cast<CVariable*>(field)))
            return false;
        UnkReflTypedPtr member = MemberOf(self, field);
        UnkReadRef target = {fn_710009dcd8(fn_710009dcd8(member.type, member.ptr), member.ptr), member.ptr};
        fn_7100099960(enum_type, reinterpret_cast<const UnkReflFieldObject*>(&target), value);
        return true;
    }
    case 'u': {
        u32 value;  // read from the file
        return ReadInto<u32, fn_7100005178>(self, field, file, value);
    }
    case 'v': {
        base::math::CVector3D value = {0.0f, 0.0f, 0.0f};
        return ReadInto<base::math::CVector3D, fn_71000246e4>(self, field, file, value);
    }
    }
    return false;
}

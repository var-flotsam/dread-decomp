// ICharClassFXCreateAndLinkEvent (unit 1883): its reflected member fields, each with its default and
// range; the path's accepted extensions are those of two resource types. Names and layouts are
// placeholders.
#include "base/UnkResKey.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedFields.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/UnkTU2526.h"
#include "game/UnkTU57.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::CType;
using base::reflection::UnkReflAttrKey;
using base::reflection::CVariable;
#include "base/reflection/TypeObject.h"
enum class EFXType : u32;
template <> base::reflection::CType base::reflection::TypeObject<EFXType>::object asm("g_unk_71727b8dd8");
enum class ELinkType : u32;
template <> base::reflection::CType base::reflection::TypeObject<ELinkType>::object asm("g_unk_71727b8ef8");

// An id's string, counting the read.
inline const CRntString& StringOf(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

// String concatenation.
inline CRntString Join(const CRntString& a, const char* b) {
    return CRntString_ConcatCString(&a, b);
}
inline CRntString Join(const CRntString& a, const CRntString& b) {
    return CRntString_Concat(&a, &b);
}

void fn_7100beaf3c(CClass* cls) {
    DREAD_REFLECTED_FIELD(CStrId, "sName", 0x10, (DefaultValue, CStrId(base::global::CStrId::Empty())));
    CVariable* type = AddReflectedField(cls, CStrId("eType", true), reinterpret_cast<CType*>(&base::reflection::TypeObject<EFXType>::object), 0x18, -1, -1);
    type->attrs.Add<s32>(base::reflection::attr::DefaultValue(), reinterpret_cast<CType*>(&base::reflection::TypeObject<EFXType>::object), 0);
    CVariable* path = AddReflectedField(cls, CStrId("sPath", true), fn_710000ad9c(), 0x20, -1, -1);
    path->attrs.Add<CFilePathStrId>(base::reflection::attr::DefaultValue(), CFilePathStrId(g_unk_7101d470e8));
    path->attrs.Add<CRntString>(base::reflection::attr::Extension(),
                                Join(Join(StringOf(g_unk_716e454078.extensions[0]), "|"), StringOf(g_unk_716e453ff8.extensions[0])));
    CVariable* link = AddReflectedField(cls, CStrId("eLinkType", true), reinterpret_cast<CType*>(&base::reflection::TypeObject<ELinkType>::object), 0x28, -1, -1);
    link->attrs.Add<s32>(base::reflection::attr::DefaultValue(), reinterpret_cast<CType*>(&base::reflection::TypeObject<ELinkType>::object), 0);
    DREAD_REFLECTED_FIELD(CStrId, "sTarget", 0x30, (DefaultValue, CStrId(base::global::CStrId::Empty())));
    DREAD_REFLECTED_FIELD(f32, "fPositionOffsetX", 0x38, (DefaultValue, 0.0f), (MinValue, -3.4028235e+38f), (MaxValue, 3.4028235e+38f));
    DREAD_REFLECTED_FIELD(f32, "fPositionOffsetY", 0x3c, (DefaultValue, 0.0f), (MinValue, -3.4028235e+38f), (MaxValue, 3.4028235e+38f));
    DREAD_REFLECTED_FIELD(f32, "fPositionOffsetZ", 0x40, (DefaultValue, 0.0f), (MinValue, -3.4028235e+38f), (MaxValue, 3.4028235e+38f));
    DREAD_REFLECTED_FIELD(f32, "fAngleOffsetX", 0x44, (DefaultValue, 0.0f), (MinValue, -3.14f), (MaxValue, 3.14f));
    DREAD_REFLECTED_FIELD(f32, "fAngleOffsetY", 0x48, (DefaultValue, 0.0f), (MinValue, -3.14f), (MaxValue, 3.14f));
    DREAD_REFLECTED_FIELD(f32, "fAngleOffsetZ", 0x4c, (DefaultValue, 0.0f), (MinValue, -3.14f), (MaxValue, 3.14f));
    DREAD_REFLECTED_FIELD(f32, "fScaleX", 0x50, (DefaultValue, 1.0f));
    DREAD_REFLECTED_FIELD(f32, "fScaleY", 0x54, (DefaultValue, 1.0f));
    DREAD_REFLECTED_FIELD(f32, "fScaleZ", 0x58, (DefaultValue, 1.0f));
    DREAD_REFLECTED_FIELD(CStrId, "sAdditional", 0x60, (DefaultValue, CStrId(base::global::CStrId::Empty())));
    DREAD_REFLECTED_FIELD(bool, "bDropToFloor", 0x68, (DefaultValue, false));
    DREAD_REFLECTED_FIELD(f32, "fLifeTime", 0x6c, (DefaultValue, -1.0f));
}

// CGameObject's unit (tools/tu_map.py): describes an object in a property bag (game_s7/UnkPropBagS7.h): every
// reflected field of its class (0xb0 bytes each, the name id first) of the types b, f, i and s is read through
// reflection and stored under the field's name. Names and layouts are placeholders.
#include "../../game_s7/UnkPropBagS7.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;
using unk_prop_bag::UnkPropBag;

struct UnkReflField;
struct UnkReflFieldObject;
struct UnkReflValueArg;
bool fn_71000e78e0(UnkReflField* field, const UnkReflFieldObject* obj, UnkReflValueArg* value);  // gets
s32 fn_7100024140(const base::reflection::CType* type);  // the type's letter (b, f, i, s, v)

struct UnkObjField {
    CStrId id;  // 0x0
    u8 pad_0x8[8];
    const CType* type;  // 0x10
    u8 pad_0x18[0xb0 - 0x18];
};
static_assert(sizeof(UnkObjField) == 0xb0, "UnkObjField layout");

struct UnkObjClass {
    u8 pad_0x0[0xe8];
    UnkObjField* fields;  // 0xe8
    u8 pad_0xf0[8];
    u32 count;  // 0xf8
};

struct UnkObj {
    virtual UnkObjClass* GetClass();
};

struct UnkObjValue {
    const CType* type;
    void* value;
};

void fn_7101043f74(UnkObj* self, UnkPropBag* bag) {
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(self->GetClass());
    typed.ptr = self;
    UnkObjClass* cls = self->GetClass();
    UnkObjField* it = cls->fields;
    UnkObjField* end = it + cls->count;
    for (; it != end; ++it) {
        CStrId name(it->id);
        switch (fn_7100024140(it->type)) {
        case 'b': {
            bool v = false;
            UnkObjValue value = {fn_710000d228(), &v};
            fn_71000e78e0(reinterpret_cast<UnkReflField*>(it), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                          reinterpret_cast<UnkReflValueArg*>(&value));
            unk_prop_bag::fn_710050f4ac(bag, name, v);
            break;
        }
        case 'f': {
            f32 v = 0.0f;
            UnkObjValue value = {fn_710000abbc(), &v};
            fn_71000e78e0(reinterpret_cast<UnkReflField*>(it), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                          reinterpret_cast<UnkReflValueArg*>(&value));
            unk_prop_bag::fn_710033187c(bag, name, v);
            break;
        }
        case 'i': {
            u32 v = 0;
            UnkObjValue value = {fn_7100014768(), &v};
            fn_71000e78e0(reinterpret_cast<UnkReflField*>(it), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                          reinterpret_cast<UnkReflValueArg*>(&value));
            unk_prop_bag::fn_7100aa3b9c(bag, name, v);
            break;
        }
        case 's': {
            CRntString v(g_unk_710162e2ed);
            UnkObjValue value = {fn_7100007f84(), &v};
            fn_71000e78e0(reinterpret_cast<UnkReflField*>(it), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                          reinterpret_cast<UnkReflValueArg*>(&value));
            unk_prop_bag::fn_710050e4dc(bag, name, v.data ? v.data : g_unk_710162e2ed);
            break;
        }
        default:
            break;
        }
    }
}

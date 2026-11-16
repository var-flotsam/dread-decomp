// Unit 2666 (lua_bind): pushes the value of a reflected field (named by argument 2) of the camera
// controller named by argument 1, when it is a bool, float, int or string. Names and layouts are
// placeholders.
#include "base/core/CBaseObject.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/CCameraComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU57.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;
using base::reflection::UnkReflTypeOf;

struct lua_State;

// A reflected field (0xb0 bytes): its type at +0x10.
struct UnkCameraField_l086f00 {
    u8 pad_0x0[0x10];
    const CType* type;
    u8 pad_0x18[0x98];
};

// A class as the lookup sees it: its fields and their name hashes.
struct UnkCameraClass_l086f00 {
    u8 pad_0x0[0xe8];
    UnkCameraField_l086f00* fields;  // 0xe8
    u64* keys;                       // 0xf0
    u32 count;                       // 0xf8

    UnkCameraField_l086f00* End() { return fields + count; }
    UnkCameraField_l086f00* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
    // The field with that name hash, null when there is none.
    UnkCameraField_l086f00* FindField(u64 key) {
        UnkCameraField_l086f00* it = Find(key);
        return it != End() ? it : nullptr;
    }
};

// A reflected object: its class is vtable slot 0.
struct UnkCameraObject_l086f00 {
    virtual UnkCameraClass_l086f00* GetClass();
};

struct UnkCameraValue_l086f00 {
    const CType* type;
    void* value;
};

// An empty string (fn_710007e6c4).
struct UnkEmptyString_l086f00 {
    u8 str[0x30];

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkEmptyString_l086f00() { fn_710007e6c4(this); }
    ~UnkEmptyString_l086f00() { fn_710007ecd4(Str()); }
};

// Reads the field with that name hash into `out`; false when there is none or it cannot be read.
template <class T>
inline bool ReadField(UnkCameraObject_l086f00* obj, u64 key, T* out) {
    UnkCameraClass_l086f00* cls = obj->GetClass();
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(obj->GetClass());
    typed.ptr = obj;
    UnkCameraValue_l086f00 value = {UnkReflTypeOf<T>::Get(), out};
    UnkCameraField_l086f00* field = cls->FindField(key);
    return field && fn_71000e78e0(reinterpret_cast<UnkReflField*>(field),
                                  reinterpret_cast<const UnkReflFieldObject*>(&typed),
                                  reinterpret_cast<UnkReflValueArg*>(&value));
}

int fn_7101086f00(lua_State* L) {
    const char* camera_name;
    const char* field_name;
    if (!fn_71010c0270(L, "ss", &camera_name, &field_name))
        return 0;
    u64 key = fn_7100001570(field_name, __builtin_strlen(field_name));
    UnkGameManagerTarget* target = fn_7100decab0(g_unk_7101d47310);
    if (!target)
        return 0;
    void* holder = target->field_0x248;
    if (!holder)
        return 0;
    UnkCameraObject_l086f00* camera;
    {
        CStrId id(camera_name, true);
        camera = reinterpret_cast<UnkCameraObject_l086f00*>(fn_71010e389c(holder, &id));
    }
    if (!camera)
        return 0;
    UnkCameraField_l086f00* field = camera->GetClass()->FindField(key);
    if (!field)
        return 0;

    switch (fn_7100024140(field->type)) {
    case 'b': {
        bool value = false;
        if (ReadField(camera, key, &value))
            return fn_71010c0b40(L, "b", value);
        break;
    }
    case 'f': {
        f32 value = 0.0f;
        if (ReadField(camera, key, &value))
            return fn_71010c0b40(L, "f", (f64)value);
        break;
    }
    case 'i': {
        s32 value = 0;
        if (ReadField(camera, key, &value))
            return fn_71010c0b40(L, "i", value);
        break;
    }
    case 's': {
        UnkEmptyString_l086f00 value;
        if (ReadField(camera, key, value.Str())) {
            const char* text = value.Str()->data;
            return fn_71010c0b40(L, "s", text ? text : g_unk_710162e2ed);
        }
        break;
    }
    }
    return 0;
}

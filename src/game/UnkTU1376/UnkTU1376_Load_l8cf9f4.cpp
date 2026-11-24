// CDoorLifeComponent's unit (tools/tu_map.py unit 1376; the name is a hint): loads the door's settings from a
// file after the base component's, and sets each on the door's reflected fields by name: bStayOpen,
// fMinTimeOpened, fMaxDistanceOpened, iAreaLeft, iAreaRight and sShieldEntity. Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/CActorComponent.h"
#include "game/UnkTU56.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;

struct UnkDoorField_l8cf9f4 {
    u8 pad_0x0[0xb0];
};

struct UnkDoorClass_l8cf9f4 {
    u8 pad_0x0[0xe8];
    UnkDoorField_l8cf9f4* fields;  // 0xe8
    u64* keys;                     // 0xf0
    u32 count;                     // 0xf8

    UnkDoorField_l8cf9f4* End() { return fields + count; }
    UnkDoorField_l8cf9f4* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
    // The field with that name hash, null when there is none.
    UnkDoorField_l8cf9f4* FindField(u64 key) {
        UnkDoorField_l8cf9f4* it = Find(key);
        return it != End() ? it : nullptr;
    }
};

// A reflected object: its class is vtable slot 0.
struct UnkDoorObject_l8cf9f4 {
    virtual UnkDoorClass_l8cf9f4* GetClass();
};

struct UnkDoorValue_l8cf9f4 {
    const CType* type;
    void* value;
};

// Sets the field named by `key` from the value `in` points at, of the type `Type` gives.
template <const CType* (*Type)()>
static inline void Set(UnkDoorObject_l8cf9f4* obj, u64 key, void* in) {
    UnkDoorClass_l8cf9f4* cls = obj->GetClass();
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(obj->GetClass());
    typed.ptr = obj;
    UnkDoorValue_l8cf9f4 value = {Type(), in};
    if (UnkDoorField_l8cf9f4* field = cls->FindField(key))
        SetReflectedFieldValue(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                      reinterpret_cast<UnkReflValueArg*>(&value));
}

void ReadDoorOpenAndShieldFieldsFromFile(UnkDoorObject_l8cf9f4* self, CRntFile* file) {
    fn_710024fc0c(reinterpret_cast<UnkPickableItem_A4_b76*>(self), file);
    if (file->position >= file->size)
        return;

    f32 minTimeOpened;
    f32 maxDistanceOpened;
    bool stayOpen;
    s32 areaLeft;
    s32 areaRight;
    const char* shieldEntity;
    CRntFile_Read(file, &minTimeOpened, 4, 1);
    CRntFile_Read(file, &maxDistanceOpened, 4, 1);
    CRntFile_Read(file, &stayOpen, 1, 1);
    CRntFile_ReadCString(file, &shieldEntity);
    CRntFile_Read(file, &areaLeft, 4, 1);
    CRntFile_Read(file, &areaRight, 4, 1);

    Set<fn_710000d228>(self, StringCrc64("bStayOpen", 9), &stayOpen);
    Set<fn_710000abbc>(self, StringCrc64("fMinTimeOpened", 14), &minTimeOpened);
    Set<fn_710000abbc>(self, StringCrc64("fMaxDistanceOpened", 18), &maxDistanceOpened);
    Set<fn_7100014768>(self, StringCrc64("iAreaLeft", 9), &areaLeft);
    Set<fn_7100014768>(self, StringCrc64("iAreaRight", 10), &areaRight);
    u64 key = StringCrc64("sShieldEntity", 13);
    CStrId shield(shieldEntity, true);
    Set<fn_7100009658>(self, key, &shield);
}

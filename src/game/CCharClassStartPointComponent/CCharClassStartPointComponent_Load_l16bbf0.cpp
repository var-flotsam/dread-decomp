// CCharClassStartPointComponent's unit (tools/tu_map.py unit 2717): loads the start point's settings from
// a file after the base component's and sets each on its reflected field by name: sOnTeleport, then, while
// the file has more, sOnTeleportLogicCamera, bOnTeleportLogicCameraRaw, bProjectOnFloor and bMorphballMode.
// Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/CActorComponent.h"
#include "game/UnkTU56.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::global::CRntFile;
using base::global::UnkStringPool;
using base::global::UnkStrIdBase;
using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;

struct UnkStartPointField_l16bbf0 {
    u8 pad_0x0[0xb0];
};

struct UnkStartPointClass_l16bbf0 {
    u8 pad_0x0[0xe8];
    UnkStartPointField_l16bbf0* fields;  // 0xe8
    u64* keys;                           // 0xf0
    u32 count;                           // 0xf8

    UnkStartPointField_l16bbf0* End() { return fields + count; }
    UnkStartPointField_l16bbf0* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
    // The field with that name hash, null when there is none.
    UnkStartPointField_l16bbf0* FindField(u64 key) {
        UnkStartPointField_l16bbf0* it = Find(key);
        return it != End() ? it : nullptr;
    }
};

// A reflected object: its class is vtable slot 0.
struct UnkStartPointObject_l16bbf0 {
    virtual UnkStartPointClass_l16bbf0* GetClass();
};

struct UnkStartPointValue_l16bbf0 {
    const CType* type;
    void* value;
};

// Sets the field named by `key` from the value `in` points at, of the type `Type` gives.
template <const CType* (*Type)()>
static inline void Set(UnkStartPointObject_l16bbf0* obj, u64 key, void* in) {
    UnkStartPointClass_l16bbf0* cls = obj->GetClass();
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(obj->GetClass());
    typed.ptr = obj;
    UnkStartPointValue_l16bbf0 value = {Type(), in};
    if (UnkStartPointField_l16bbf0* field = cls->FindField(key))
        fn_71000e7a80(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                      reinterpret_cast<UnkReflValueArg*>(&value));
}

// A string id as this loader builds it for a field: its destructor releases the string as CStrId's
// does, but decrements and tests the count in one step.
struct UnkFieldId_l16bbf0 : UnkStrIdBase {
    using UnkStrIdBase::UnkStrIdBase;
    ~UnkFieldId_l16bbf0() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

static inline bool HasMore(CRntFile* file) { return file->position < file->size; }

void fn_710116bbf0(UnkStartPointObject_l16bbf0* self, CRntFile* file) {
    fn_710024fc0c(reinterpret_cast<UnkPickableItem_A4_b76*>(self), file);

    const char* onTeleport;
    CRntFile_ReadCString(file, &onTeleport);
    {
        u64 key = StringCrc64("sOnTeleport", 11);
        UnkFieldId_l16bbf0 id(onTeleport, true);
        Set<fn_7100009658>(self, key, &id);
    }
    if (HasMore(file)) {
        const char* onTeleportLogicCamera;
        CRntFile_ReadCString(file, &onTeleportLogicCamera);
        u64 key = StringCrc64("sOnTeleportLogicCamera", 22);
        UnkFieldId_l16bbf0 id(onTeleportLogicCamera, true);
        Set<fn_7100009658>(self, key, &id);
    }
    if (HasMore(file)) {
        bool onTeleportLogicCameraRaw;
        CRntFile_Read(file, &onTeleportLogicCameraRaw, 1, 1);
        u64 key = StringCrc64("bOnTeleportLogicCameraRaw", 25);
        Set<fn_710000d228>(self, key, &onTeleportLogicCameraRaw);
    }
    if (HasMore(file)) {
        bool projectOnFloor;
        CRntFile_Read(file, &projectOnFloor, 1, 1);
        u64 key = StringCrc64("bProjectOnFloor", 15);
        Set<fn_710000d228>(self, key, &projectOnFloor);
    }
    if (HasMore(file)) {
        bool morphballMode;
        CRntFile_Read(file, &morphballMode, 1, 1);
        u64 key = StringCrc64("bMorphballMode", 14);
        Set<fn_710000d228>(self, key, &morphballMode);
    }
}

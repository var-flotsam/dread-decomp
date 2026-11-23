// CTriggerComponent unit (tools/tu_map.py unit 1578): loads the trigger's settings from a file after the
// base component's and sets each on its reflected fields by name; later fields are read only when the file
// still has data. Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/CActorComponent.h"
#include "game/UnkTU56.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::global::CRntFile;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::global::UnkStrIdBase;
using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;

struct UnkTriggerField_la6081c {
    u8 pad_0x0[0xb0];
};

struct UnkTriggerClass_la6081c {
    u8 pad_0x0[0xe8];
    UnkTriggerField_la6081c* fields;  // 0xe8
    u64* keys;                        // 0xf0
    u32 count;                        // 0xf8

    UnkTriggerField_la6081c* End() { return fields + count; }
    UnkTriggerField_la6081c* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
    // The field with that name hash, null when there is none.
    UnkTriggerField_la6081c* FindField(u64 key) {
        UnkTriggerField_la6081c* it = Find(key);
        return it != End() ? it : nullptr;
    }
};

// A reflected object: its class is vtable slot 0.
struct UnkTriggerObject_la6081c {
    virtual UnkTriggerClass_la6081c* GetClass();
};

struct UnkTriggerValue_la6081c {
    const CType* type;
    void* value;
};

// Sets the field named by `key` from the value `in` points at, of the type `Type` gives.
template <const CType* (*Type)()>
static inline void Set(UnkTriggerObject_la6081c* obj, u64 key, void* in) {
    UnkTriggerClass_la6081c* cls = obj->GetClass();
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(obj->GetClass());
    typed.ptr = obj;
    UnkTriggerValue_la6081c value = {Type(), in};
    if (UnkTriggerField_la6081c* field = cls->FindField(key))
        fn_71000e7a80(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                      reinterpret_cast<UnkReflValueArg*>(&value));
}

// A string id as this loader builds it for a field: its destructor releases the string as CStrId's
// does, but decrements and tests the count in one step.
struct UnkFieldId_la6081c : UnkStrIdBase {
    using UnkStrIdBase::UnkStrIdBase;
    ~UnkFieldId_la6081c() {
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

struct UnkColliderTrigger_b0058;

void fn_7100a6081c(UnkColliderTrigger_b0058* component, CRntFile* file) {
    UnkTriggerObject_la6081c* self = reinterpret_cast<UnkTriggerObject_la6081c*>(component);
    fn_710024fc0c(reinterpret_cast<UnkPickableItem_A4_b76*>(self), file);
    bool startEnabled;
    const char* onEnable;
    const char* onDisable;
    const char* onEnter;
    const char* onExit;
    const char* onAllExit;
    bool onEnableAlways = false;
    bool onDisableAlways = false;
    bool onEnterAlways = false;
    bool onExitAlways = false;
    bool onAllExitAlways = false;
    CRntFile_Read(file, &startEnabled, 1, 1);
    CRntFile_ReadCString(file, &onEnable);
    CRntFile_ReadCString(file, &onDisable);
    CRntFile_ReadCString(file, &onEnter);
    CRntFile_ReadCString(file, &onExit);
    CRntFile_ReadCString(file, &onAllExit);
    CRntFile_Read(file, &onEnableAlways, 1, 1);
    CRntFile_Read(file, &onDisableAlways, 1, 1);
    CRntFile_Read(file, &onEnterAlways, 1, 1);
    CRntFile_Read(file, &onExitAlways, 1, 1);
    CRntFile_Read(file, &onAllExitAlways, 1, 1);

    {
        u64 key = StringCrc64("sOnEnable", 9);
        UnkFieldId_la6081c id(onEnable, true);
        Set<fn_7100009658>(self, key, &id);
    }
    {
        u64 key = StringCrc64("sOnDisable", 10);
        UnkFieldId_la6081c id(onDisable, true);
        Set<fn_7100009658>(self, key, &id);
    }
    Set<fn_710000d228>(self, StringCrc64("bOnEnableAlways", 15), &onEnableAlways);
    Set<fn_710000d228>(self, StringCrc64("bOnDisableAlways", 16), &onDisableAlways);
    {
        u64 key = StringCrc64("sOnEnter", 8);
        UnkFieldId_la6081c id(onEnter, true);
        Set<fn_7100009658>(self, key, &id);
    }
    Set<fn_710000d228>(self, StringCrc64("bOnEnterAlways", 14), &onEnterAlways);
    {
        u64 key = StringCrc64("sOnExit", 7);
        UnkFieldId_la6081c id(onExit, true);
        Set<fn_7100009658>(self, key, &id);
    }
    Set<fn_710000d228>(self, StringCrc64("bOnExitAlways", 13), &onExitAlways);
    {
        u64 key = StringCrc64("sOnAllExit", 10);
        UnkFieldId_la6081c id(onAllExit, true);
        Set<fn_7100009658>(self, key, &id);
    }
    Set<fn_710000d228>(self, StringCrc64("bOnAllExitAlways", 16), &onAllExitAlways);
    Set<fn_710000d228>(self, StringCrc64("bStartEnabled", 13), &startEnabled);

    // Fields newer files add, each read only while the file still has data.
    if (file->position < file->size) {
        bool checkAllEntities;
        CRntFile_Read(file, &checkAllEntities, 1, 1);
        Set<fn_710000d228>(self, StringCrc64("bCheckAllEntities", 17), &checkAllEntities);
    }
    if (file->position < file->size) {
        bool water;
        CRntFile_Read(file, &water, 1, 1);
        Set<fn_710000d228>(self, StringCrc64("bWater", 6), &water);
    }
    if (file->position < file->size) {
        bool slowDown;
        CRntFile_Read(file, &slowDown, 1, 1);
        Set<fn_710000d228>(self, StringCrc64("bSlowDown", 9), &slowDown);
    }
    if (file->position < file->size) {
        bool persistentState;
        CRntFile_Read(file, &persistentState, 1, 1);
        Set<fn_710000d228>(self, StringCrc64("bPersistentState", 16), &persistentState);
    }
    if (file->position < file->size) {
        const char* sfxType;
        CRntFile_ReadCString(file, &sfxType);
        u64 key = StringCrc64("sSfxType", 8);
        UnkFieldId_la6081c id(sfxType, true);
        Set<fn_7100009658>(self, key, &id);
    }
}

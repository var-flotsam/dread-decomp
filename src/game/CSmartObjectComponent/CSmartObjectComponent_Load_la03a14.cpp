// CSmartObjectComponent unit (tools/tu_map.py unit 1512): loads the component's settings from a file
// after the base component's, and sets each on its reflected fields by name: sOnUseStart,
// sOnUseFailure, sOnUseSuccess, sUsableEntity, sDefaultUseAction, sDefaultAbortAction and
// bStartEnabled; two later values are read and dropped. Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
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

struct UnkSmartObjectField_la03a14 {
    u8 pad_0x0[0xb0];
};

struct UnkSmartObjectClass_la03a14 {
    u8 pad_0x0[0xe8];
    UnkSmartObjectField_la03a14* fields;  // 0xe8
    u64* keys;                            // 0xf0
    u32 count;                            // 0xf8

    UnkSmartObjectField_la03a14* End() { return fields + count; }
    UnkSmartObjectField_la03a14* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
    // The field with that name hash, null when there is none.
    UnkSmartObjectField_la03a14* FindField(u64 key) {
        UnkSmartObjectField_la03a14* it = Find(key);
        return it != End() ? it : nullptr;
    }
};

// A reflected object: its class is vtable slot 0.
struct UnkSmartObjectObject_la03a14 {
    virtual UnkSmartObjectClass_la03a14* GetClass();
};

struct UnkSmartObjectValue_la03a14 {
    const CType* type;
    void* value;
};

// Sets the field named by `key` from the value `in` points at, of the type `Type` gives.
template <const CType* (*Type)()>
static inline void Set(UnkSmartObjectObject_la03a14* obj, u64 key, void* in) {
    UnkSmartObjectClass_la03a14* cls = obj->GetClass();
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(obj->GetClass());
    typed.ptr = obj;
    UnkSmartObjectValue_la03a14 value = {Type(), in};
    if (UnkSmartObjectField_la03a14* field = cls->FindField(key))
        fn_71000e7a80(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                      reinterpret_cast<UnkReflValueArg*>(&value));
}

// A string id as this loader builds it for a field: its destructor releases the string as CStrId's
// does, but decrements and tests the count in one step.
struct UnkFieldId_la03a14 : UnkStrIdBase {
    using UnkStrIdBase::UnkStrIdBase;
    ~UnkFieldId_la03a14() {
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

void fn_7100a03a14(UnkSmartObjectObject_la03a14* self, CRntFile* file) {
    fn_710024fc0c(reinterpret_cast<UnkPickableItem_A4_b76*>(self), file);

    bool startEnabled;
    const char* onUseStart;
    const char* onUseSuccess;
    const char* onUseFailure;
    const char* usableEntity;
    const char* defaultUseAction;
    const char* defaultAbortAction;
    CRntFile_Read(file, &startEnabled, 1, 1);
    CRntFile_ReadCString(file, &onUseStart);
    CRntFile_ReadCString(file, &onUseSuccess);
    CRntFile_ReadCString(file, &onUseFailure);
    CRntFile_ReadCString(file, &usableEntity);
    CRntFile_ReadCString(file, &defaultUseAction);
    CRntFile_ReadCString(file, &defaultAbortAction);

    {
        u64 key = StringCrc64("sOnUseStart", 11);
        UnkFieldId_la03a14 id(onUseStart, true);
        Set<fn_7100009658>(self, key, &id);
    }
    {
        u64 key = StringCrc64("sOnUseFailure", 13);
        UnkFieldId_la03a14 id(onUseFailure, true);
        Set<fn_7100009658>(self, key, &id);
    }
    {
        u64 key = StringCrc64("sOnUseSuccess", 13);
        UnkFieldId_la03a14 id(onUseSuccess, true);
        Set<fn_7100009658>(self, key, &id);
    }
    {
        u64 key = StringCrc64("sUsableEntity", 13);
        UnkFieldId_la03a14 id(usableEntity, true);
        Set<fn_7100009658>(self, key, &id);
    }
    {
        u64 key = StringCrc64("sDefaultUseAction", 17);
        UnkFieldId_la03a14 id(defaultUseAction, true);
        Set<fn_7100009658>(self, key, &id);
    }
    {
        u64 key = StringCrc64("sDefaultAbortAction", 19);
        UnkFieldId_la03a14 id(defaultAbortAction, true);
        Set<fn_7100009658>(self, key, &id);
    }
    Set<fn_710000d228>(self, StringCrc64("bStartEnabled", 13), &startEnabled);

    // Two values older files no longer use: read and dropped.
    if (file->position < file->size) {
        bool unused = false;
        CRntFile_Read(file, &unused, 1, 1);
    }
    if (file->position < file->size) {
        s32 unused = -1;
        CRntFile_Read(file, &unused, 4, 1);
    }
}

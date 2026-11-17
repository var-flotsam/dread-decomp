// UnkTU2704 (CSpawnPointComponent's unit): loads the spawn point's settings from a file after the base
// component's and sets each on its reflected field by name; the later groups only while the file has more.
// Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
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

struct UnkSpawnPointField_l12812c {
    u8 pad_0x0[0xb0];
};

struct UnkSpawnPointClass_l12812c {
    u8 pad_0x0[0xe8];
    UnkSpawnPointField_l12812c* fields;  // 0xe8
    u64* keys;                           // 0xf0
    u32 count;                           // 0xf8

    UnkSpawnPointField_l12812c* End() { return fields + count; }
    UnkSpawnPointField_l12812c* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
    // The field with that name hash, null when there is none.
    UnkSpawnPointField_l12812c* FindField(u64 key) {
        UnkSpawnPointField_l12812c* it = Find(key);
        return it != End() ? it : nullptr;
    }
};

// A reflected object: its class is vtable slot 0.
struct UnkSpawnPointObject_l12812c {
    virtual UnkSpawnPointClass_l12812c* GetClass();
};

struct UnkSpawnPointValue_l12812c {
    const CType* type;
    void* value;
};

// Sets the field named by `key` from the value `in` points at, of the type `Type` gives.
template <const CType* (*Type)()>
static inline void Set(UnkSpawnPointObject_l12812c* obj, u64 key, void* in) {
    UnkSpawnPointClass_l12812c* cls = obj->GetClass();
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(obj->GetClass());
    typed.ptr = obj;
    UnkSpawnPointValue_l12812c value = {Type(), in};
    if (UnkSpawnPointField_l12812c* field = cls->FindField(key))
        fn_71000e7a80(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                      reinterpret_cast<UnkReflValueArg*>(&value));
}

// A string id as this loader builds it for a field: its destructor decrements and tests the count in one
// step.
struct UnkFieldId_l12812c : UnkStrIdBase {
    using UnkStrIdBase::UnkStrIdBase;
    ~UnkFieldId_l12812c() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

static inline bool HasMore(CRntFile* file) { return file->position < file->size; }

void fn_710112812c(UnkSpawnPointObject_l12812c* self, CRntFile* file) {
    fn_710024fc0c(reinterpret_cast<UnkPickableItem_A4_b76*>(self), file);

    const char* unused;  // read and skipped
    const char* startAnimation;
    const char* onBeforeGenerate;
    const char* onEntityGenerated;
    const char* charClass;
    bool spawnOnFloor = false;
    fn_710007cee4(file, &unused);
    fn_710007cee4(file, &charClass);
    fn_710007cee4(file, &startAnimation);
    fn_710007cee4(file, &onBeforeGenerate);
    fn_710007cee4(file, &onEntityGenerated);
    fn_710007c714(file, &spawnOnFloor, 1, 1);

    {
        u64 key = fn_7100001570("sCharClass", 10);
        UnkFieldId_l12812c id(charClass, true);
        Set<fn_7100009658>(self, key, &id);
    }
    {
        u64 key = fn_7100001570("sOnBeforeGenerate", 17);
        UnkFieldId_l12812c id(onBeforeGenerate, true);
        Set<fn_7100009658>(self, key, &id);
    }
    {
        u64 key = fn_7100001570("sOnEntityGenerated", 18);
        UnkFieldId_l12812c id(onEntityGenerated, true);
        Set<fn_7100009658>(self, key, &id);
    }
    {
        u64 key = fn_7100001570("sStartAnimation", 15);
        UnkFieldId_l12812c id(startAnimation, true);
        Set<fn_7100009658>(self, key, &id);
    }
    {
        u64 key = fn_7100001570("bSpawnOnFloor", 13);
        Set<fn_710000d228>(self, key, &spawnOnFloor);
    }
    if (HasMore(file)) {
        bool checkCollisions;
        fn_710007c714(file, &checkCollisions, 1, 1);
        u64 key = fn_7100001570("bCheckCollisions", 16);
        Set<fn_710000d228>(self, key, &checkCollisions);
    }
    if (HasMore(file)) {
        f32 timeToActivate;
        s32 maxNumToGenerate;
        fn_710007c714(file, &timeToActivate, 4, 1);
        fn_710007c714(file, &maxNumToGenerate, 4, 1);
        u64 key = fn_7100001570("fTimeToActivate", 15);
        Set<fn_710000abbc>(self, key, &timeToActivate);
        key = fn_7100001570("iMaxNumToGenerate", 17);
        Set<fn_7100014768>(self, key, &maxNumToGenerate);
    }
    if (HasMore(file)) {
        const char* sceneShapeId;
        fn_710007cee4(file, &sceneShapeId);
        u64 key = fn_7100001570("sSceneShapeId", 13);
        UnkFieldId_l12812c id(sceneShapeId, true);
        Set<fn_7100009658>(self, key, &id);
    }
    if (HasMore(file)) {
        const char* collisionSceneShapeId;
        fn_710007cee4(file, &collisionSceneShapeId);
        u64 key = fn_7100001570("sCollisionSceneShapeId", 22);
        UnkFieldId_l12812c id(collisionSceneShapeId, true);
        Set<fn_7100009658>(self, key, &id);
    }
    if (HasMore(file)) {
        bool startEnabled;
        bool automanaged;
        fn_710007c714(file, &startEnabled, 1, 1);
        fn_710007c714(file, &automanaged, 1, 1);
        u64 key = fn_7100001570("bStartEnabled", 13);
        Set<fn_710000d228>(self, key, &startEnabled);
        key = fn_7100001570("bAutomanaged", 12);
        Set<fn_710000d228>(self, key, &automanaged);
    }
    if (HasMore(file)) {
        const char* navigableShape;
        fn_710007cee4(file, &navigableShape);
        u64 key = fn_7100001570("sNavigableShape", 15);
        UnkFieldId_l12812c id(navigableShape, true);
        Set<fn_7100009658>(self, key, &id);
    }
}

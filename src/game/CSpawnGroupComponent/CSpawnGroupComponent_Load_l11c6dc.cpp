// CSpawnGroupComponent unit (tools/tu_map.py unit 2703): loads the spawn group's settings from a file
// after the base component's and sets each on its reflected field by name; the later values are
// read only while the file has more. Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/CActorComponent.h"
#include "game/UnkTU56.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CRntString;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::global::UnkStrIdBase;
using base::math::CVector3D;
using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;

struct UnkSpawnGroupField_l11c6dc {
    u8 pad_0x0[0xb0];
};

struct UnkSpawnGroupClass_l11c6dc {
    u8 pad_0x0[0xe8];
    UnkSpawnGroupField_l11c6dc* fields;  // 0xe8
    u64* keys;                           // 0xf0
    u32 count;                           // 0xf8

    UnkSpawnGroupField_l11c6dc* End() { return fields + count; }
    UnkSpawnGroupField_l11c6dc* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
    // The field with that name hash, null when there is none.
    UnkSpawnGroupField_l11c6dc* FindField(u64 key) {
        UnkSpawnGroupField_l11c6dc* it = Find(key);
        return it != End() ? it : nullptr;
    }
};

// A reflected object: its class is vtable slot 0.
struct UnkSpawnGroupObject_l11c6dc {
    virtual UnkSpawnGroupClass_l11c6dc* GetClass();
};

struct UnkSpawnGroupValue_l11c6dc {
    const CType* type;
    void* value;
};

// Sets the field named by `key` from the value `in` points at, of the type `Type` gives.
template <const CType* (*Type)()>
static inline void Set(UnkSpawnGroupObject_l11c6dc* obj, u64 key, void* in) {
    UnkSpawnGroupClass_l11c6dc* cls = obj->GetClass();
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(obj->GetClass());
    typed.ptr = obj;
    UnkSpawnGroupValue_l11c6dc value = {Type(), in};
    if (UnkSpawnGroupField_l11c6dc* field = cls->FindField(key))
        fn_71000e7a80(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                      reinterpret_cast<UnkReflValueArg*>(&value));
}

// A string id as this loader builds it for a field: its destructor releases the string as CStrId's
// does, but decrements and tests the count in one step.
struct UnkFieldId_l11c6dc : UnkStrIdBase {
    using UnkStrIdBase::UnkStrIdBase;
    ~UnkFieldId_l11c6dc() {
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

void fn_710111c6dc(UnkSpawnGroupObject_l11c6dc* self, CRntFile* file) {
    fn_710024fc0c(reinterpret_cast<UnkPickableItem_A4_b76*>(self), file);

    bool isGenerator;
    bool isInfinite;
    s32 maxToGenerate;
    s32 maxSimultaneous;
    f32 generateEvery;
    const char* onBeforeGenerateEntity;
    const char* onEntityGenerated;
    const char* onEnable;
    const char* onDisable;
    const char* onMaxSimultaneous;
    const char* onMaxGenerated;
    const char* onEntityDead;
    const char* onEntityDamaged;
    const char* onAllEntitiesDead;
    fn_710007c714(file, &isGenerator, 1, 1);
    fn_710007c714(file, &isInfinite, 1, 1);
    fn_710007c714(file, &maxToGenerate, 4, 1);
    fn_710007c714(file, &maxSimultaneous, 4, 1);
    fn_710007c714(file, &generateEvery, 4, 1);
    fn_710007cee4(file, &onBeforeGenerateEntity);
    fn_710007cee4(file, &onEntityGenerated);
    fn_710007cee4(file, &onEnable);
    fn_710007cee4(file, &onDisable);
    fn_710007cee4(file, &onMaxSimultaneous);
    fn_710007cee4(file, &onMaxGenerated);
    fn_710007cee4(file, &onEntityDead);
    fn_710007cee4(file, &onEntityDamaged);
    fn_710007cee4(file, &onAllEntitiesDead);

    Set<fn_710000d228>(self, fn_7100001570("bIsGenerator", 12), &isGenerator);
    Set<fn_710000d228>(self, fn_7100001570("bIsInfinite", 11), &isInfinite);
    Set<fn_7100014768>(self, fn_7100001570("iMaxToGenerate", 14), &maxToGenerate);
    Set<fn_7100014768>(self, fn_7100001570("iMaxSimultaneous", 16), &maxSimultaneous);
    Set<fn_710000abbc>(self, fn_7100001570("fGenerateEvery", 14), &generateEvery);
    {
        u64 key = fn_7100001570("sOnBeforeGenerateEntity", 23);
        CRntString str(onBeforeGenerateEntity);
        Set<fn_7100007f84>(self, key, &str);
    }
    {
        u64 key = fn_7100001570("sOnEntityGenerated", 18);
        CRntString str(onEntityGenerated);
        Set<fn_7100007f84>(self, key, &str);
    }
    {
        u64 key = fn_7100001570("sOnEnable", 9);
        CRntString str(onEnable);
        Set<fn_7100007f84>(self, key, &str);
    }
    {
        u64 key = fn_7100001570("sOnDisable", 10);
        CRntString str(onDisable);
        Set<fn_7100007f84>(self, key, &str);
    }
    {
        u64 key = fn_7100001570("sOnMaxSimultaneous", 18);
        CRntString str(onMaxSimultaneous);
        Set<fn_7100007f84>(self, key, &str);
    }
    {
        u64 key = fn_7100001570("sOnMaxGenerated", 15);
        CRntString str(onMaxGenerated);
        Set<fn_7100007f84>(self, key, &str);
    }
    {
        u64 key = fn_7100001570("sOnEntityDead", 13);
        CRntString str(onEntityDead);
        Set<fn_7100007f84>(self, key, &str);
    }
    {
        u64 key = fn_7100001570("sOnEntityDamaged", 16);
        CRntString str(onEntityDamaged);
        Set<fn_7100007f84>(self, key, &str);
    }
    {
        u64 key = fn_7100001570("sOnAllEntitiesDead", 18);
        CRntString str(onAllEntitiesDead);
        Set<fn_7100007f84>(self, key, &str);
    }

    // Values newer files add, read only while the file has more.
    if (file->position < file->size) {
        bool automanaged = false;
        fn_710007c714(file, &automanaged, 1, 1);
        Set<fn_710000d228>(self, fn_7100001570("bAutomanaged", 12), &automanaged);
    }
    if (file->position < file->size) {
        bool autoenabled = false;
        fn_710007c714(file, &autoenabled, 1, 1);
        Set<fn_710000d228>(self, fn_7100001570("bAutoenabled", 12), &autoenabled);
    }
    const char* shapeId;
    if (file->position < file->size) {
        fn_710007cee4(file, &shapeId);
        u64 key = fn_7100001570("sLogicCollisionShapeID", 22);
        UnkFieldId_l11c6dc id(shapeId, true);
        Set<fn_7100009658>(self, key, &id);
    }

    CVector3D areaOffset;
    areaOffset.x = 0.0f;
    areaOffset.y = 0.0f;
    areaOffset.z = 0.0f;
    CVector3D areaSize;
    areaSize.x = 10000.0f;
    areaSize.y = 10000.0f;
    areaSize.z = 10000.0f;
    if (file->position < file->size) {
        fn_710007c714(file, &areaOffset, 12, 1);
        fn_710007c714(file, &areaSize, 12, 1);
        Set<fn_71000246e4>(self, fn_7100001570("vActiveAreaOffset", 17), &areaOffset);
        Set<fn_71000246e4>(self, fn_7100001570("vActiveAreaSize", 15), &areaSize);
    }
    // A flag older versions read and no longer use.
    if (file->position < file->size) {
        bool unused = false;
        fn_710007c714(file, &unused, 1, 1);
    }
    if (file->position < file->size) {
        f32 unused;  // a second value after the probability, read and dropped
        f32 dropAmmoProb = 0.0f;
        fn_710007c714(file, &dropAmmoProb, 4, 1);
        fn_710007c714(file, &unused, 4, 1);
        Set<fn_710000abbc>(self, fn_7100001570("fDropAmmoProb", 13), &dropAmmoProb);
    }
    if (file->position < file->size) {
        s32 initToGenerate = 0;
        fn_710007c714(file, &initToGenerate, 4, 1);
        Set<fn_7100014768>(self, fn_7100001570("iInitToGenerate", 15), &initToGenerate);
    }
    if (file->position < file->size) {
        const char* arenaId;
        fn_710007cee4(file, &arenaId);
        u64 key = fn_7100001570("sArenaId", 8);
        UnkFieldId_l11c6dc id(arenaId, true);
        Set<fn_7100009658>(self, key, &id);
    }
    if (file->position < file->size) {
        bool checkActiveDrops = false;
        fn_710007c714(file, &checkActiveDrops, 1, 1);
        Set<fn_710000d228>(self, fn_7100001570("bCheckActiveDrops", 17), &checkActiveDrops);
    }
}

// CCharClassLifeComponent (unit 1368): a float tunable read through reflection by the name in an id, then
// adjusted for one game mode (the player life tunable's easy-mode members): with one-hit enemy projectiles
// some names read 1; one is scaled by the Kraid bouncing creature life factor. Names and layouts are placeholders.
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/global/CStrId.h"
#include "game/CCharClassLifeComponent.h"
#include "game/CPlayerLifeComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkPlayerLifeTunable.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;

struct UnkReflField;
struct UnkReflFieldObject;
struct UnkReflValueArg;

// A reflected field (0xb0 bytes).
struct UnkLifeField_l8bbc24 {
    u8 pad_0x0[0xb0];
};

// A class as the lookup sees it: its fields and their name hashes.
struct UnkLifeClass_l8bbc24 {
    u8 pad_0x0[0xe8];
    UnkLifeField_l8bbc24* fields;  // 0xe8
    u64* keys;                     // 0xf0
    u32 count;                     // 0xf8

    UnkLifeField_l8bbc24* End() { return fields + count; }
    UnkLifeField_l8bbc24* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
    UnkLifeField_l8bbc24* FindField(u64 key) {
        UnkLifeField_l8bbc24* it = Find(key);
        return it != End() ? it : nullptr;
    }
};

struct UnkLifeObject_l8bbc24 {
    virtual UnkLifeClass_l8bbc24* GetClass();
};

struct UnkLifeStrData_l8bbc24 {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkLifeValue_l8bbc24 {
    const CType* type;
    void* value;
};

static inline bool Get(UnkLifeObject_l8bbc24* obj, u64 key, f32* out) {
    UnkLifeClass_l8bbc24* cls = obj->GetClass();
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(obj->GetClass());
    typed.ptr = obj;
    UnkLifeValue_l8bbc24 value = {fn_710000abbc(), out};
    if (UnkLifeField_l8bbc24* field = cls->FindField(key)) {
        ReadReflectedFieldIntoTypedRef(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                      reinterpret_cast<UnkReflValueArg*>(&value));
        return true;
    }
    return false;
}

static inline u64 Key(const base::global::CStrId* id) {
    return reinterpret_cast<UnkLifeStrData_l8bbc24*>(id->data)->key;
}

f32 fn_71008bbc24(const base::global::CStrId* id) {
    f32 v = 0.0f;
    Get(reinterpret_cast<UnkLifeObject_l8bbc24*>(&g_unk_717272fd38), Key(id), &v);
    if (fn_7100dffc58(g_unk_7101d47310) == 3) {
        if (g_unk_7172a0a0a8.bEasyModeEnemyProjectileOneHit) {
            switch (Key(id)) {
            case 0x1e3aac87b61753fbull:
            case 0x23984f5a2f80d606ull:
            case 0xbebc5c8e34aae7efull:
            case 0xee2d05816e995216ull:
            case 0x3d90af5beecb25f4ull:
            case 0x431bcf41df7cca8bull:
            case 0x9c7fe40f61ee2638ull:
            case 0x9d7e38b03b03ba6cull:
            case 0x373e1611afc44163ull:
            case 0x3c17186280efd531ull:
            case 0xee2e6adbd0fadc86ull:
            case 0x18c7ebb840a7188cull:
            case 0x720ed355b400cbf6ull:
            case 0x751b84d7941f51d6ull:
                return 1.0f;
            }
        }
        if (Key(id) == 0x87463de79a0a9546ull) {
            f32 scaled = g_unk_7172a0a0a8.fEasyModeKraidBouncingCreatureLifeFactor * v;
            v = scaled > 1.0f ? scaled : 1.0f;
        }
    }
    return v;
}

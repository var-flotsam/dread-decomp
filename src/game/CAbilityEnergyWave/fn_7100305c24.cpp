// CAbilityEnergyWave unit: the weapon boost damage multiplier tunable named "fWeaponBoostDamageMult_<name>"
// (the name is the string at the start of `stats`), read through reflection; 1.0 when there is none.
// Names and layouts are placeholders.
#include <stdio.h>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/CAbilityEnergyWave.h"
#include "game/UnkTU0.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;

// The ability's tunables (the reflection registers it as a CTunable).
struct UnkTunableValue_ovb0262g;

struct UnkBoostField_l305c24 {
    u8 pad_0x0[0xb0];
};

// A class as the lookup sees it: its fields and their name hashes.
struct UnkBoostClass_l305c24 {
    u8 pad_0x0[0xe8];
    UnkBoostField_l305c24* fields;  // 0xe8
    u64* keys;                      // 0xf0
    u32 count;                      // 0xf8

    UnkBoostField_l305c24* End() { return fields + count; }
    UnkBoostField_l305c24* Find(const u64& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
    // The field with that name hash, null when there is none.
    UnkBoostField_l305c24* FindField(const u64& key) {
        UnkBoostField_l305c24* it = Find(key);
        return it != End() ? it : nullptr;
    }
};

// A reflected object: its class is vtable slot 0.
struct UnkBoostObject_l305c24 {
    virtual UnkBoostClass_l305c24* GetClass();
};

struct UnkBoostValue_l305c24 {
    const CType* type;
    void* value;
};

static inline bool Get(UnkBoostObject_l305c24* obj, const u64& key, f32* out) {
    UnkBoostClass_l305c24* cls = obj->GetClass();
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(obj->GetClass());
    typed.ptr = obj;
    UnkBoostValue_l305c24 value = {fn_710000abbc(), out};
    if (UnkBoostField_l305c24* field = cls->FindField(key)) {
        ReadReflectedFieldIntoTypedRef(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                      reinterpret_cast<UnkReflValueArg*>(&value));
        return true;
    }
    return false;
}

struct UnkBoostStats_l305c24 {
    base::global::CStrId name;
};

f32 CAbilityEnergyWave_GetWeaponBoostDamageMult(void* object) {
    auto* stats = static_cast<UnkBoostStats_l305c24*>(object);
    f32 v = 1.0f;
    if (stats->name.data != &g_unk_7101d47108->empty) {
        char buf[0x100];
        sprintf(buf, "fWeaponBoostDamageMult_%s", stats->name.Text());
        u64 key;
        fn_710006663c(&key, buf);
        UnkBoostObject_l305c24* tunable = reinterpret_cast<UnkBoostObject_l305c24*>(&g_unk_71726af060);
        if (Get(tunable, key, &v))
            return v;
    }
    return 1.0f;
}

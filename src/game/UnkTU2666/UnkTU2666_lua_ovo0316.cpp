// Unit 2666 Lua bindings: entity lookups by name (the name's string-id hash and the kind
// g_unk_71727bebb8), a component type check, a spawn placement, entity sound pitch/volume and the
// in-game time preset. Names and layouts are placeholders.
#include <string.h>

#include "base/file/UnkFileDevice.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CAudioComponent.h"
#include "game/CCharClassStartPointComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU101.h"
#include "game/UnkTU1176.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU341.h"
#include "game/sound/CMusicManager.h"
#include "base/global/UnkStringPool.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkActor.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTUProperty.h"
#include "game/UnkGameManager.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::CFilePathStrId;
struct lua_State;
using base::reflection::UnkReflObject;

struct UnkNamedKey_b0163 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};
struct UnkWeakTarget;
struct UnkSoundRef;
struct UnkEntitySoundComponent;
struct UnkInterp_7100105d34;

struct UnkSoundRef_b0163 {
    UnkInterp_7100105d34* sound;
    s64 serial;
};
struct UnkSoundOwner_b0163 {
    u8 pad_0x0[0x2b8];
    UnkEntitySoundComponent* field_0x2b8;
};

// Sets the pitch (or the volume) of the sound `sound_name` of the entity `entity_name`.
template <bool kVolume>
static inline s32 SetEntitySound_b0163(lua_State* L) {
    // The entity's key and then the sound's path id use one stack slot.
    union Slot {
        Slot() {}
        ~Slot() {}
        u64 key;
        CFilePathStrId path;
    } slot;
    alignas(8) u32 kind;
    const char* entity_name;
    const char* sound_name;
    f32 value;
    if (!fn_71010c0270(L, "ssf", &entity_name, &sound_name, &value))
        return 0;
    UnkSoundOwner_b0163* entity;
    {
        UnkGameManager* manager = g_unk_7101d47310;
        CStrId id(entity_name, true);
        u32 kind_value = g_unk_71727bebb8;
        u64 hash = reinterpret_cast<UnkNamedKey_b0163*>(id.data)->hash;
        slot.key = hash;
        kind = kind_value;
        entity = reinterpret_cast<UnkSoundOwner_b0163*>(fn_7100debbc8(manager, &slot.key, &kind));
    }
    if (!entity)
        return 0;
    UnkEntitySoundComponent* component = entity->field_0x2b8;
    UnkSoundHandle handle;
    new (&slot.path) CFilePathStrId(sound_name, true);
    handle = fn_710071a338(component, &slot.path);
    slot.path.~CFilePathStrId();
    UnkSoundRef_b0163* ref = reinterpret_cast<UnkSoundRef_b0163*>(handle.ref);
    if (ref && handle.serial != -1 && ref->serial == handle.serial && ref->sound) {
        if (kVolume)
            fn_7100105d34(ref->sound, value, false, 0.2f);
        else
            fn_7100105da8(ref->sound, value, 0.2f);
    }
    return 0;
}

s32 fn_710108c1d0(lua_State* L) {
    return SetEntitySound_b0163<false>(L);
}

s32 fn_710108c350(lua_State* L) {
    return SetEntitySound_b0163<true>(L);
}

s32 fn_710107d500(lua_State* L) {
    // The entity's key and then the component's name id use one stack slot.
    union Slot {
        Slot() {}
        ~Slot() {}
        u64 key;
        CStrId id;
    } slot;
    alignas(8) u32 kind;
    const char* entity_name;
    const char* component_name;
    if (!fn_71010c0270(L, "ss", &entity_name, &component_name))
        return 0;
    void* entity;
    {
        UnkGameManager* manager = g_unk_7101d47310;
        CStrId id(entity_name, true);
        u32 kind_value = g_unk_71727bebb8;
        u64 hash = reinterpret_cast<UnkNamedKey_b0163*>(id.data)->hash;
        slot.key = hash;
        kind = kind_value;
        entity = fn_7100deba8c(manager, &slot.key, &kind);
    }
    if (!entity)
        return 0;
    new (&slot.id) CStrId(component_name, true);
    bool result = false;
    if (UnkReflObject* component = fn_710023341c(entity, slot.id)) {
        const base::reflection::CClass* type = component->vf_0x0();
        result = fn_710009d50c(type, fn_710024e3c8());
    }
    s32 pushed = fn_71010c0b40(L, "b", result);
    slot.id.~CStrId();
    return pushed;
}

struct UnkScenarioObject_ovb0032;
void fn_71010e46cc(void* object, bool value);

inline void* FindByName_b0163(UnkGameManager* manager, const char* name) {
    u32 kind_value = g_unk_71727bebb8;
    u64 key = fn_7100001570(name, strlen(name));
    alignas(8) u32 kind = kind_value;
    return fn_7100deba8c(manager, &key, &kind);
}

s32 fn_710107de40(lua_State* L) {
    alignas(8) u32 kind;
    const char* entity_name;
    const char* target_name;
    f32 amount;
    bool flag;
    if (!fn_71010c0270(L, "ssfb", &entity_name, &target_name, &amount, &flag))
        return 0;
    UnkWeakTarget* entity;
    {
        UnkGameManager* manager = g_unk_7101d47310;
        u64 key;
        CStrId id(entity_name, true);
        u32 kind_value = g_unk_71727bebb8;
        u64 hash = reinterpret_cast<UnkNamedKey_b0163*>(id.data)->hash;
        key = hash;
        kind = kind_value;
        entity = fn_7100debbc8(manager, &key, &kind);
    }
    if (!entity)
        return 1;
    void* target = FindByName_b0163(g_unk_7101d47310, target_name);
    if (!target)
        return 1;
    if (void* place = fn_7100774014(reinterpret_cast<UnkScenarioObject_ovb0032*>(target)))
        fn_710116c5dc(place, entity);
    if (amount > 0.0f)
        fn_7100d7babc(entity, true, amount);
    if (flag) {
        if (void* menu = fn_7100defcec(g_unk_7101d47310))
            fn_71010e46cc(menu, flag);
    }
    return 0;
}

struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

void fn_7100dffe04(UnkGameManager* self, s32 value);

struct UnkGameManagerDef : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedId qid;
};
static inline const UnkQualifiedId& Qid(UnkGameManagerDef& def) {
    if (def.qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&def.qid, &def, g_unk_7101d470e0);
    return def.qid;
}
static inline const UnkQualifiedId& DefId(UnkPropertyEx<>& def) {
    return Qid(reinterpret_cast<UnkGameManagerDef&>(def));
}
static inline const CStrId& TableId(u32 offset) {
    return *reinterpret_cast<CStrId*>(GameManagerIdTable().pad_0x0 + offset);
}
template <class T>
static inline void SetValue(CBlackboard* blackboard, const CStrId& section, const UnkQualifiedId& name,
                            const base::reflection::CType* (*type)(), const T& value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = type();
            v.value = const_cast<T*>(&value);
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
        }
    }
}

struct UnkTimeArgs_b0163 {
    s32 field_0x0;
    s32 mode;  // 0x4
};

void fn_7101096ecc(UnkTimeArgs_b0163* args) {
    fn_7100dffe04(g_unk_7101d47310, args->field_0x0);
    f32 seconds = 0.0f;
    UnkGameManager* manager = g_unk_7101d47310;
    switch (args->mode) {
    case 0:
        seconds = 28801.0f;
        break;
    case 1:
        seconds = 14401.0f;
        break;
    case 2:
        seconds = 7200.0f;
        break;
    }
    SetValue(&manager->field_0x968, TableId(0x1148), DefId(g_unk_7172758cf0), fn_710000abbc, seconds);
    fn_7100dffc3c(g_unk_7101d47310, seconds);
}

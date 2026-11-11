// Unit 2666 Lua bindings: each reads its arguments from the script stack (fn_71010c0270) and forwards them to
// the game manager, the render manager, the sound manager or a script manager; most find an object by the
// hash of a name argument (fn_7100deba8c) or build a string key or id from it. Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "base/UnkNamedRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/snd/CSoundSystem.h"
#include "game/CCharClassEventScenarioComponent.h"
#include "game/CCollisionMaterialCacheComponent.h"
#include "game/CDrop.h"
#include "game/CSpawnGroupComponent.h"
#include "game/CThermalReactionComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2669.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU2768.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

struct lua_State;

extern u32 g_unk_71727bebb8;

inline void* FindByName_b0158(UnkGameManager* manager, const char* name) {
    u32 kind_value = g_unk_71727bebb8;
    u64 key = fn_7100001570(name, strlen(name));
    alignas(8) u32 kind = kind_value;
    return fn_7100deba8c(manager, &key, &kind);
}

struct UnkEntityState;
void fn_7100232864(UnkEntityState* entity, bool value);

s32 fn_710107d9e0(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    void* object = FindByName_b0158(g_unk_7101d47310, name);
    if (!object)
        return 1;
    fn_7100232864(static_cast<UnkEntityState*>(object), true);
    return 0;
}

s32 fn_710107da80(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    void* object = FindByName_b0158(g_unk_7101d47310, name);
    if (!object)
        return 1;
    fn_7100232864(static_cast<UnkEntityState*>(object), false);
    return 0;
}

s32 fn_710107ec40(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    void* object = FindByName_b0158(g_unk_7101d47310, name);
    return fn_71010c0b40(L, "b", object != nullptr);
}

struct UnkVtable_b0158 {
    void (*slots[0x80])(void* self);
};
struct UnkVirtual_b0158 {
    UnkVtable_b0158* vtable;
};

s32 fn_710107db20(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    void* object = FindByName_b0158(g_unk_7101d47310, name);
    if (!object)
        return 1;
    static_cast<UnkVirtual_b0158*>(object)->vtable->slots[0xe0 / 8](object);
    return 0;
}

struct UnkWeakTarget;
struct UnkThermalComponent_ovb0097;
struct UnkStruct_710024fa0c;
void fn_710111b634(void* p);
void fn_7101152008(void* p, bool value);

s32 fn_71010847d0(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        void* object = FindByName_b0158(g_unk_7101d47310, name);
        if (object) {
            if (UnkThermalComponent_ovb0097* target = fn_7100a2933c(static_cast<UnkWeakTarget*>(object)))
                reinterpret_cast<UnkVirtual_b0158*>(target)->vtable->slots[0x190 / 8](target);
        }
    }
    return 0;
}

s32 fn_7101084d30(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    void* object = FindByName_b0158(g_unk_7101d47310, name);
    if (!object)
        return 1;
    if (void* target = fn_71007c04f0(object)) {
        fn_7101119a50(target);
        return 0;
    }
    return 1;
}

s32 fn_7101084dd0(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    void* object = FindByName_b0158(g_unk_7101d47310, name);
    if (!object)
        return 1;
    if (void* target = fn_71007c04f0(object)) {
        fn_710111b634(target);
        return 0;
    }
    return 1;
}

s32 fn_710107fbb0(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        void* object = FindByName_b0158(g_unk_7101d47310, name);
        if (object) {
            if (void* target = fn_71007e4924(object))
                fn_7100083990();
        }
    }
    return 0;
}

s32 fn_710108fbd0(lua_State* L) {
    const char* name;
    bool flag;
    if (fn_71010c0270(L, "sb", &name, &flag)) {
        void* object = FindByName_b0158(g_unk_7101d47310, name);
        if (object) {
            if (void* target = fn_7100798eec(object))
                fn_7101152008(target, flag);
        }
    }
    return 0;
}

struct UnkFlagTarget_b0158 {
    u8 pad_0x0[0x13f];
    bool field_0x13f;
};

s32 fn_7101084f20(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    void* object = FindByName_b0158(g_unk_7101d47310, name);
    if (!object)
        return 1;
    if (UnkFlagTarget_b0158* target = static_cast<UnkFlagTarget_b0158*>(fn_71007c04f0(object))) {
        target->field_0x13f = false;
        return 0;
    }
    return 1;
}

s32 fn_7101084e70(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    void* object = FindByName_b0158(g_unk_7101d47310, name);
    if (!object)
        return 1;
    if (UnkFlagTarget_b0158* target = static_cast<UnkFlagTarget_b0158*>(fn_71007c04f0(object))) {
        target->field_0x13f = true;
        return 0;
    }
    return 1;
}

struct UnkUserDataOwner;

extern UnkUserDataOwner* g_unk_7101d47308;
bool fn_7101203760(UnkUserDataOwner* self);
bool fn_7100dfe364(UnkGameManager* self);
void* fn_7100deab70(UnkGameManager*, const CRntString&);
bool fn_7100dfbe3c(UnkGameManager*);
void fn_7100dfbe78(UnkGameManager*, void*);
s32 fn_7100dff1a8(UnkGameManager*, const base::UnkNamedEntry*, bool, bool);
s32 fn_7100df78cc(UnkGameManager*, const base::UnkNamedEntry*, bool, bool);

s32 fn_7101079200(lua_State* L) {
    UnkUserDataOwner* owner = g_unk_7101d47308;
    if (!owner) {
        owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(owner);
        g_unk_7101d47308 = owner;
    }
    bool value = true;
    if (!fn_7101203760(owner))
        value = fn_7100dfe364(g_unk_7101d47310);
    return fn_71010c0b40(L, "b", value);
}

struct UnkIngameGui_b0158 {
    u8 pad_0x0[0xb8];
    CRntString field_0xb8;
};

s32 fn_7101077f30(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        UnkGameManager* manager = g_unk_7101d47310;
        UnkIngameGui_b0158* gui = static_cast<UnkIngameGui_b0158*>(fn_7100deab70(manager, CRntString("INGAME")));
        if (gui) {
            CRntString value(name);
            fn_710007eaa0(&gui->field_0xb8, &value);
        }
    }
    return 0;
}

s32 fn_71010785e0(lua_State* L) {
    const char* name;
    bool value = false;
    if (fn_71010c0270(L, "s", &name)) {
        UnkGameManager* manager = g_unk_7101d47310;
        base::UnkNamedEntry key;
        fn_710007e704(&key, name);
        value = fn_7100dff1a8(manager, &key, true, true) == 0;
        fn_710007ecd4(reinterpret_cast<CRntString*>(&key));
    }
    return fn_71010c0b40(L, "b", value);
}

s32 fn_7101078a90(lua_State* L) {
    const char* name;
    bool value = false;
    if (fn_71010c0270(L, "s", &name) && fn_7100dfbe3c(g_unk_7101d47310)) {
        UnkGameManager* manager = g_unk_7101d47310;
        base::UnkNamedEntry key;
        fn_710007e704(&key, name);
        auto check = reinterpret_cast<bool (*)(UnkGameManager*, const base::UnkNamedEntry*)>(fn_7100dfbe78);
        value = check(manager, &key);
        fn_710007ecd4(reinterpret_cast<CRntString*>(&key));
    }
    return fn_71010c0b40(L, "b", value);
}

s32 fn_7101078ef0(lua_State* L) {
    const char* name;
    bool flag;
    s32 result = 5;
    if (fn_71010c0270(L, "sb", &name, &flag)) {
        UnkGameManager* manager = g_unk_7101d47310;
        base::UnkNamedEntry key;
        fn_710007e704(&key, name);
        result = fn_7100df78cc(manager, &key, flag, false);
        fn_710007ecd4(reinterpret_cast<CRntString*>(&key));
    }
    return fn_71010c0b40(L, "i", result);
}

CRntString fn_7100dfb330(UnkGameManager* self);

s32 fn_7101079030(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    UnkGameManager* manager = g_unk_7101d47310;
    union Slot {
        Slot() {}
        ~Slot() {}
        CRntString string;
    } result;
    base::UnkNamedEntry key;
    fn_710007e704(&key, name);
    auto lookup = reinterpret_cast<CRntString (*)(UnkGameManager*, const base::UnkNamedEntry*)>(fn_7100dfb330);
    new (&result.string) CRntString(lookup(manager, &key));
    fn_710007ecd4(reinterpret_cast<CRntString*>(&key));
    const char* text = result.string.data ? result.string.data : g_unk_710162e2ed;
    s32 ret = fn_71010c0b40(L, "s", text);
    fn_710007ecd4(&result.string);
    return ret;
}

struct UnkSoundMgr;
struct UnkPathNode;

extern UnkSoundMgr* g_unk_7101d4d820;
void fn_7100df5cec(UnkGameManager*, void*);
void fn_7100e03a78(UnkGameManager*, void*);
void fn_7100df21e8(UnkGameManager* self, CStrId* id);

s32 fn_710107d2d0(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        UnkGameManager* manager = g_unk_7101d47310;
        CStrId id(name, true);
        fn_7100df5cec(manager, &id);
    }
    return 0;
}

s32 fn_71010940c0(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        UnkGameManager* manager = g_unk_7101d47310;
        CStrId id(name, true);
        fn_7100e03a78(manager, &id);
    }
    return 0;
}

s32 fn_710107d150(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name) == 1) {
        UnkGameManager* manager = g_unk_7101d47310;
        CStrId id(name, true);
        fn_7100df21e8(manager, &id);
    }
    return 0;
}

s32 fn_710108b360(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        UnkSoundMgr* sound = g_unk_7101d4d820;
        CFilePathStrId id(name, true);
        fn_710010808c(sound, reinterpret_cast<UnkPathNode*>(&id));
    }
    return 0;
}

struct UnkScriptManager_ovb0085;

void fn_7100219c6c(UnkRenderManager* self, bool enable, u64 layer, u32 arg3, bool arg4);
void fn_7100219c5c(UnkRenderManager* self, bool enable, void* layer, u32 arg3, bool arg4);
bool fn_7100219c7c(UnkRenderManager* self, u64 layer, bool arg);

s32 fn_7101082f00(lua_State* L) {
    const char* name;
    bool enable;
    if (fn_71010c0270(L, "sb", &name, &enable)) {
        auto show = reinterpret_cast<u32 (*)(UnkRenderManager*, bool, u64, u32, bool)>(fn_7100219c6c);
        while (show(g_unk_7101d47300, enable, fn_7100001570(name, strlen(name)), 0, false) == 2) {
        }
    }
    return 0;
}

s32 fn_7101082df0(lua_State* L) {
    const char* name;
    bool enable;
    bool flag = true;
    if (fn_71010c0270(L, "sb|b", &name, &enable, &flag)) {
        UnkRenderManager* manager = g_unk_7101d47300;
        bool value = enable;
        fn_7100219c5c(manager, value, reinterpret_cast<void*>(fn_7100001570(name, strlen(name))), 0, flag);
    }
    return 0;
}

s32 fn_7101082f90(lua_State* L) {
    const char* name;
    bool result = false;
    if (fn_71010c0270(L, "s", &name, &result)) {
        UnkRenderManager* manager = g_unk_7101d47300;
        result = fn_7100219c7c(manager, fn_7100001570(name, strlen(name)), false);
    }
    return fn_71010c0b40(L, "b", result);
}

void fn_710109e1ac(UnkScriptManager_ovb0085* self, const base::UnkNamedEntry* key, s32 arg, bool flag);
extern UnkScriptManager_ovb0085* g_unk_71727bed90;

s32 fn_71010772c0(lua_State* L) {
    bool flag = true;
    const char* name;
    if (fn_71010c0270(L, "s|b", &name, &flag)) {
        UnkScriptManager_ovb0085* manager = g_unk_71727bed90;
        if (!manager) {
            manager = static_cast<UnkScriptManager_ovb0085*>(fn_71000839a0(0x88, 0x3f));
            fn_710109d810(manager);
            g_unk_71727bed90 = manager;
        }
        base::UnkNamedEntry key;
        fn_710007e704(&key, name);
        fn_710109e1ac(manager, &key, 0, flag);
        fn_710007ecd4(reinterpret_cast<CRntString*>(&key));
    }
    return 0;
}

struct UnkSettings_b0158 {
    void* field_0x0 = nullptr;
    void* field_0x8 = nullptr;
    u64 field_0x10 = 0;
    u64 field_0x18 = 0xf;
    base::UnkAllocator* field_0x20;
    u64 field_0x28;
};
extern UnkSettings_b0158* g_unk_71726bc568;

s32 fn_7101085480(lua_State* L) {
    bool flag = true;
    if (fn_71010c0270(L, "b", &flag)) {
        UnkSettings_b0158* settings = g_unk_71726bc568;
        if (!settings) {
            settings = static_cast<UnkSettings_b0158*>(fn_71000839a0(0x30, 0x3f));
            new (settings) UnkSettings_b0158;
            *reinterpret_cast<u32*>(&settings->field_0x10) = 10;
            reinterpret_cast<u8*>(settings)[0x14] = 1;
            settings->field_0x20 = nullptr;
            settings->field_0x28 = 0;
            settings->field_0x20 = fn_7100083990();
            reinterpret_cast<u8*>(settings)[0x28] = 1;
            g_unk_71726bc568 = settings;
        }
        reinterpret_cast<u8*>(settings)[0x28] = flag;
    }
    return 0;
}

struct UnkInventory;
struct UnkPlayer;

f32 fn_7101142928(UnkInventory*, const UnkQualifiedId&);

struct UnkSuitPlayer_b0158 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;
};
UnkPlayer* fn_7100dec788(UnkGameManager*);

static inline f64 ItemAmount_b0158(UnkPropertyEx<>& item) {
    f64 amount = 0.0;
    if (UnkSuitPlayer_b0158* player = reinterpret_cast<UnkSuitPlayer_b0158*>(fn_7100dec788(g_unk_7101d47310))) {
        if (UnkInventory* inventory = player->inventory) {
            UnkQualifiedId& qid = ItemQualifiedId(item);
            if ((u64)qid.id.data == (u64)g_unk_7101d47108 + 0x10)
                fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
            amount = fn_7101142928(inventory, qid);
        }
    }
    return amount;
}

s32 fn_710107c710(lua_State* L) {
    return fn_71010c0b40(L, "f", ItemAmount_b0158(g_unk_717275a498));
}

s32 fn_710107c7b0(lua_State* L) {
    return fn_71010c0b40(L, "f", ItemAmount_b0158(g_unk_717275a458));
}

s32 fn_710107c850(lua_State* L) {
    return fn_71010c0b40(L, "f", ItemAmount_b0158(g_unk_717275a418));
}

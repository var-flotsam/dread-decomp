// Unit 2666 Lua bindings (second batch): each reads its arguments from the script stack and forwards
// them to the game manager, the sound, music, render or environment managers. Names and layouts are
// placeholders.
#include <string.h>

#include "base/UnkNamedRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "base/reflection/UnkTunableRegistry.h"
#include "base/snd/CSoundSystem.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CBossAIComponent.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU23.h"
#include "game/UnkTU2538.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2656.h"
#include "game/UnkTU2669.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU315.h"
#include "game/UnkTU99.h"
#include "game/lua_lib.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

using base::global::CRntString;

struct lua_State;
struct UnkSoundMgr;
struct UnkSoundHandle_b46a1;

// ---- game manager, one name key ----
void fn_7100df71d4(UnkGameManager* self, const CRntString& name);

s32 fn_7101078df0(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        UnkGameManager* manager = g_unk_7101d47310;
        base::UnkNamedEntry key;
        fn_710007e704(&key, name);
        fn_7100df71d4(manager, *reinterpret_cast<CRntString*>(&key));
        fn_710007ecd4(reinterpret_cast<CRntString*>(&key));
    }
    return 0;
}

void fn_7100def668(UnkGameManager* self, const CRntString& name);

s32 fn_7101082ab0(lua_State* L) {
    const char* name = nullptr;
    if (fn_71010c0270(L, "s", &name)) {
        UnkGameManager* manager = g_unk_7101d47310;
        base::UnkNamedEntry key;
        fn_710007e704(&key, name);
        fn_7100def668(manager, *reinterpret_cast<CRntString*>(&key));
        fn_710007ecd4(reinterpret_cast<CRntString*>(&key));
    }
    return 0;
}

// ---- sound manager ----
void fn_7100108b90(char* self, u32 id, const UnkSoundHandle_b46a1& handle, f32 value);

s32 fn_710108e3a0(lua_State* L) {
    s32 id;
    s32 bit;
    f32 value;
    if (fn_71010c0270(L, "iif", &id, &bit, &value)) {
        char* sound = reinterpret_cast<char*>(g_unk_7101d4d820);
        u32 mask = 1u << bit;
        fn_7100108b90(sound, id, *reinterpret_cast<const UnkSoundHandle_b46a1*>(&mask), value);
    }
    return 0;
}

s32 fn_710108e200(lua_State* L) {
    f32 a = 1.0f;
    f32 b = 0.2f;
    if (fn_71010c0270(L, "ff", &a, &b))
        fn_71000fbe1c(reinterpret_cast<u8*>(g_unk_7101d4d820) + 8, 0, a, b);
    return 0;
}

struct UnkTunable;

s32 fn_7101092690(lua_State* L) {
    f32 scale = 1.0f;
    if (fn_71010c0270(L, "f", &scale)) {
        UnkSoundMgr* sound = g_unk_7101d4d820;
        f32 factor = scale;
        f32 volume = *reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(&g_unk_7101d68580) + 0x3c);
        fn_71000fbe1c(reinterpret_cast<u8*>(sound) + 8, 5, factor * volume, 0.0f);
    }
    return 0;
}

u32 fn_710010a8a0(u32 v);
void fn_7100108c2c(UnkSoundMgr* self, const u32* mask, u32 v, f32 f);

s32 fn_710108e4f0(lua_State* L) {
    s32 value = 0;
    if (fn_71010c0270(L, "i", &value)) {
        UnkSoundMgr* sound = g_unk_7101d4d820;
        u32 mask = 1;
        fn_7100108c2c(sound, &mask, fn_710010a8a0(value), 0.2f);
    }
    return 0;
}

// ---- effects ----

s32 fn_710108f870(lua_State* L) {
    f32 a = 0.1f;
    f32 b;
    if (fn_71010c0270(L, "ff", &a, &b)) {
        void* fx = g_unk_7101d47310->field_0x1040;
        base::math::CVector3D color;
        color.x = 0.0f;
        color.y = 0.0f;
        color.z = 0.0f;
        fn_7101053c14(fx, &color, a, b, 1.0f);
    }
    return 0;
}

// ---- name key lookups ----
bool fn_710109d390(void* name, void* arg, bool a, bool b);

s32 fn_71010771d0(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        base::UnkNamedEntry first;
        fn_710007e704(&first, name);
        base::UnkNamedEntry second;
        fn_710007e704(&second, g_unk_710162e2ed);
        fn_710109d390(&first, &second, false, true);
        fn_710007ecd4(reinterpret_cast<CRntString*>(&second));
        fn_710007ecd4(reinterpret_cast<CRntString*>(&first));
    }
    return 0;
}

s32 fn_7101093a90(lua_State* L) {
    if (void* object = fn_7100df5440(g_unk_7101d47310)) {
        bool all = false;
        bool force_off = false;
        fn_71010c0270(L, "bb", &all, &force_off);
        fn_71002fda50(static_cast<CAbilityComponent*>(object), all, force_off);
    }
    return 0;
}

s32 fn_7101082e80(lua_State* L) {
    const char* name;
    bool enable;
    if (fn_71010c0270(L, "sb", &name, &enable)) {
        UnkRenderManager* render = g_unk_7101d47300;
        u64 id = fn_7100001570(name, strlen(name));
        fn_7100219ca4(render, reinterpret_cast<void*>(id), enable, 0);
    }
    return 0;
}

#define SET_FLAG_BINDING(NAME, OFFSET)                                                    \
    s32 NAME(lua_State* L) {                                                              \
        const char* name;                                                                 \
        u8 flag;                                                                          \
        if (fn_71010c0270(L, "sb", &name, &flag)) {                                       \
            UnkGameManager* manager = g_unk_7101d47310;                                   \
            base::UnkNamedEntry key;                                                      \
            fn_710007e704(&key, name);                                                    \
            void* object = fn_7100deab70(manager, *reinterpret_cast<CRntString*>(&key));  \
            fn_710007ecd4(reinterpret_cast<CRntString*>(&key));                           \
            if (object)                                                                   \
                reinterpret_cast<u8*>(object)[OFFSET] = flag;                             \
        }                                                                                 \
        return 0;                                                                         \
    }

SET_FLAG_BINDING(fn_71010780a0, 0x38)
SET_FLAG_BINDING(fn_7101078120, 0x39)
SET_FLAG_BINDING(fn_71010781a0, 0x3b)

extern u32 g_unk_71727bebb8;

// fn_7100fbc4e0 hands back the number of results it pushed.
static inline s32 PushObject(lua_State* L, void* arg) {
    return fn_7100fbc4e0(L, arg);
}

s32 fn_710107a070(lua_State* L) {
    UnkGameManager* manager = g_unk_7101d47310;
    u32 mode = g_unk_71727bebb8;
    u64 id = fn_7100001570("MainCamera", 10);
    alignas(8) u32 flags = mode;
    return PushObject(L, fn_7100deba8c(manager, &id, &flags));
}

// ---- the music manager singleton, created on first use ----
void fn_7100fd8404(void* manager, bool flag, f32 value);

static inline void* MusicManager() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = static_cast<UnkMgr_0xf18*>(manager);
    }
    return manager;
}

s32 fn_7101090e90(lua_State* L) {
    return fn_71010c0b40(L, "b", (f64)fn_7100fd7b3c(MusicManager()));
}

s32 fn_7101090e00(lua_State* L) {
    f32 level = 1.0f;
    f32 time = 0.2f;
    if (fn_71010c0270(L, "ff", &level, &time))
        fn_7100fd7af8(MusicManager(), level, time);
    return 0;
}

struct UnkMusicVtable_b0157 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(f32 value);
};

s32 fn_710108e130(lua_State* L) {
    f32 value;
    if (fn_71010c0270(L, "f", &value)) {
        fn_710010806c(g_unk_7101d4d820, value);
        static_cast<UnkMusicVtable_b0157*>(MusicManager())->vf_0x10(0.0f);
    }
    return 0;
}

s32 fn_7101091cc0(lua_State* L) {
    bool flag = false;
    f32 value = -1.0f;
    if (fn_71010c0ad0(L, 0, 1, "b|f", &flag, &value))
        fn_7100fd8404(MusicManager(), flag, value);
    return 0;
}

struct UnkMusicFlag_b0157 {
    u8 pad_0x0[0xaa];
    u8 field_0xaa;
};

s32 fn_7101091de0(lua_State* L) {
    bool music = static_cast<UnkMusicFlag_b0157*>(MusicManager())->field_0xaa != 0;
    bool sound = reinterpret_cast<u8*>(g_unk_7101d4d820)[0x400911] != 0;
    return fn_71010c0b40(L, "b", music & sound);
}

// ---- game manager name lookups ----

s32 fn_7101078670(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        UnkGameManager* manager = g_unk_7101d47310;
        base::UnkNamedEntry key;
        fn_710007e704(&key, name);
        fn_7100dff1a8(manager, &key, false, true);
        fn_710007ecd4(reinterpret_cast<CRntString*>(&key));
    }
    return fn_71010c0b40(L, "b", false);
}

s32 fn_7100df916c(UnkGameManager* self, const CRntString& name);

s32 fn_7101079110(lua_State* L) {
    bool result = false;
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        UnkGameManager* manager = g_unk_7101d47310;
        base::UnkNamedEntry key;
        fn_710007e704(&key, name);
        result = fn_7100df916c(manager, *reinterpret_cast<CRntString*>(&key)) == 0;
        fn_710007ecd4(reinterpret_cast<CRntString*>(&key));
    }
    return fn_71010c0b40(L, "b", result);
}

s32 fn_7100df97b4(UnkGameManager* self, const CRntString& name, bool a, bool b);

s32 fn_7101078e60(lua_State* L) {
    const char* name;
    s32 result = 5;
    if (fn_71010c0270(L, "s", &name)) {
        UnkGameManager* manager = g_unk_7101d47310;
        base::UnkNamedEntry key;
        fn_710007e704(&key, name);
        result = fn_7100df97b4(manager, *reinterpret_cast<CRntString*>(&key), false, true);
        fn_710007ecd4(reinterpret_cast<CRntString*>(&key));
    }
    return fn_71010c0b40(L, "i", result);
}

s32 fn_7101087af0(lua_State* L) {
    f32 a = 1.0f;
    f32 b = 1.0f;
    const char* name;
    if (fn_71010c0270(L, "ffs", &a, &b, &name)) {
        UnkGameManager* manager = g_unk_7101d47310;
        f32 first = a;
        f32 second = b;
        base::UnkNamedEntry key;
        fn_710007e704(&key, name);
        fn_7100df6d3c(manager, &key, first, second);
        fn_710007ecd4(reinterpret_cast<CRntString*>(&key));
    }
    return 0;
}

struct UnkAnyWeapon_b0157 {
    u8 pad_0x0[0xd8];
    s32 field_0xd8;
};

s32 fn_7101093c10(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        UnkAnyWeapon_b0157* state = reinterpret_cast<UnkAnyWeapon_b0157*>(g_unk_7101d47310->field_0x1af8);
        s32 mode = 0;
        if (strcmp(name, "AnyWeapon"))
            mode = strcmp(name, "AnyWeaponButBeams") ? 2 : 1;
        state->field_0xd8 = mode;
    }
    return 0;
}

// ---- other singletons, created on first use ----
struct UnkScriptManager_ovb0085;

s32 fn_7101077240(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        UnkScriptManager_ovb0085* scripts = g_unk_71727bed90;
        if (!scripts) {
            scripts = static_cast<UnkScriptManager_ovb0085*>(fn_71000839a0(0x88, 0x3f));
            fn_710109d810(scripts);
            g_unk_71727bed90 = scripts;
        }
        base::UnkNamedEntry key;
        fn_710007e704(&key, name);
        fn_710109d890(scripts, reinterpret_cast<CRntString*>(&key));
        fn_710007ecd4(reinterpret_cast<CRntString*>(&key));
    }
    return 0;
}

struct UnkObj_71011a50cc;
void fn_71011a50cc(UnkObj_71011a50cc* self);
void fn_71011a5cdc(UnkObj_71011a50cc* self, const CRntString& name);

s32 fn_710108b170(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        UnkObj_71011a50cc* object = g_unk_71727bc260;
        if (!object) {
            object = static_cast<UnkObj_71011a50cc*>(fn_71000839a0(0x98, 0x3f));
            fn_71011a50cc(object);
            g_unk_71727bc260 = object;
        }
        base::UnkNamedEntry key;
        fn_710007e704(&key, name);
        fn_71011a5cdc(object, *reinterpret_cast<CRntString*>(&key));
        fn_710007ecd4(reinterpret_cast<CRntString*>(&key));
    }
    return 0;
}

s32 fn_7101083db0(lua_State* L) {
    bool third = false;
    bool first = false;
    bool second = false;
    if (fn_71010c0270(L, "bbb", &first, &second, &third)) {
        void* environment = g_unk_7101d47368;
        if (!environment) {
            environment = fn_71000839a0(0x4c8, 0x3f);
            fn_7100d8ced8(environment);
            g_unk_7101d47368 = environment;
        }
        fn_7100d8efd0(environment, first, second, third);
    }
    return 0;
}

// ---- push an object held by a singleton onto the script stack ----

struct UnkHeldObject_b0157 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};
struct UnkHolder_0x100_b0157 {
    u8 pad_0x0[0x100];
    UnkHeldObject_b0157* field_0x100;
};

extern char* g_unk_71726b2398;

s32 fn_710107cb80(lua_State* L) {
    UnkHolder_0x100_b0157* holder = reinterpret_cast<UnkHolder_0x100_b0157*>(g_unk_71726b2398);
    if (!holder) {
        holder = static_cast<UnkHolder_0x100_b0157*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(reinterpret_cast<char*>(holder));
        g_unk_71726b2398 = reinterpret_cast<char*>(holder);
    }
    if (holder->field_0x100 && holder->field_0x100->field_0x10) {
        return PushObject(L, holder->field_0x100->field_0x10);
    }
    return 0;
}

#include "game/UnkManager_71726af468.h"

struct UnkAiHeld_b0157 {
    UnkHeldObject_b0157* field_0x0;
};

s32 fn_7101085d10(lua_State* L) {
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    UnkHeldObject_b0157* held = reinterpret_cast<UnkAiHeld_b0157*>(manager)->field_0x0;
    if (held && held->field_0x10)
        return PushObject(L, held->field_0x10);
    return 0;
}

struct UnkTypedPair_b0157 {
    void* type;
    void* obj;
};
void* fn_7100136df0();
s32 fn_7100fbde70(void* state, UnkTypedPair_b0157* value, s32 arg0, s32 arg1);

s32 fn_71010948a0(lua_State* L) {
    base::reflection::UnkTunableRegistry* registry = g_unk_7101d47350;
    if (!registry) {
        registry = static_cast<base::reflection::UnkTunableRegistry*>(fn_71000839a0(0x58, 0x3f));
        fn_710013ac14(registry);
        g_unk_7101d47350 = registry;
    }
    base::reflection::UnkTunableRegistry* object = registry;
    UnkTypedPair_b0157 value = {fn_7100136df0(), &object};
    return fn_7100fbde70(L, &value, 1, 2);
}

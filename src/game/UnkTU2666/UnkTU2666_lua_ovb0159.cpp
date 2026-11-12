// Unit 2666 Lua bindings (third batch): each reads its arguments from the script stack and forwards them to
// the music manager, the game manager, the render manager, the GUI, the debug statistics or a lookup by name.
// Names and layouts are placeholders.
#include "base/UnkNamedRegistry.h"
#include "base/UnkVector.h"
#include "base/file/UnkFileDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CActorComponent.h"
#include "game/CBossAIComponent.h"
#include "game/CDrop.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2566.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU2656.h"
#include "game/UnkTU2669.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU2704.h"
#include "game/UnkTU315.h"
#include "game/UnkTU341.h"
#include "game/UnkTU365.h"
#include "game/lua_lib.h"
#include "game/malloc.h"
#include "game/sound/CMusicManager.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflObject;

struct lua_State;

struct UnkMgr_0xf18;

// Creates the object on first use: storage from the default allocator, built by its constructor function.
template <class T, u64 Size, void (*Ctor)(T*)>
__attribute__((always_inline)) static inline T* Singleton(T*& instance) {
    if (!instance) {
        T* p = static_cast<T*>(fn_71000839a0(Size, 0x3f));
        Ctor(p);
        instance = p;
    }
    return instance;
}

// The object the game manager knows under a name (the name's CRC64 and the kind g_unk_71727bebb8).
static inline void* FindByName(UnkGameManager* manager, const char* name) {
    u32 kind_value = g_unk_71727bebb8;
    u64 hash;
    alignas(8) u32 kind;
    hash = fn_7100001570(name, __builtin_strlen(name));
    kind = kind_value;
    return fn_7100deba8c(manager, &hash, &kind);
}

// A mode word passed by address (the word after it is not set).
struct UnkMusicMode {
    u32 kind;
    u32 field_0x4;
};

s32 fn_710108d440(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        void* music = Singleton<void, 0xf18, fn_7100fd3804>(reinterpret_cast<void*&>(g_unk_71726bc560));
        base::UnkNamedEntry key;
        fn_710007e704(&key, name);
        UnkMusicMode mode;
        mode.kind = 0x14;
        fn_7100fd90d8(1.1f, 0.5f, music, reinterpret_cast<CRntString*>(&key), 8, &mode, 1, 0, nullptr);
        fn_710007ecd4(reinterpret_cast<CRntString*>(&key));
    }
    return 0;
}

struct UnkScriptManager_ovb0085;
struct UnkUnit2669Str;

s32 fn_7101077360(lua_State* L) {
    bool flag = true;
    const char* first;
    const char* second;
    if (fn_71010c0270(L, "ss|b", &first, &second, &flag)) {
        UnkScriptManager_ovb0085* map = Singleton<UnkScriptManager_ovb0085, 0x88, fn_710109d810>(g_unk_71727bed90);
        base::UnkNamedEntry source;
        fn_710007e704(&source, first);
        base::UnkNamedEntry name;
        fn_710007e704(&name, second);
        fn_710109dd6c(map, &source, reinterpret_cast<UnkUnit2669Str*>(&name), true, flag);
        fn_710007ecd4(reinterpret_cast<CRntString*>(&name));
        fn_710007ecd4(reinterpret_cast<CRntString*>(&source));
    }
    return 0;
}

s32 fn_710107a2f0(lua_State*) {
    fn_7100df31b8(g_unk_7101d47310, true, false, 1, g_unk_7101d470e0, false);
    return 0;
}

struct UnkLuaVec3_b0159 {
    f32 x, y, z;
    UnkLuaVec3_b0159() : x(0.0f), y(0.0f), z(0.0f) {}
};

struct UnkTarget_b0159;
struct UnkTargetVTable_b0159 {
    void* pad_0x0[26];
    void (*vf_0xd0)(UnkTarget_b0159* self, const UnkLuaVec3_b0159* v, bool flag);
};
struct UnkTarget_b0159 {
    UnkTargetVTable_b0159* vtable;
};

s32 fn_710107ce90(lua_State* L) {
    const char* name;
    UnkLuaVec3_b0159 v;
    bool flag;
    if (fn_71010c0270(L, "svb", &name, &v, &flag)) {
        UnkGameManager* manager = g_unk_7101d47310;
        if (UnkTarget_b0159* target = static_cast<UnkTarget_b0159*>(FindByName(manager, name)))
            target->vtable->vf_0xd0(target, &v, flag);
    }
    return 0;
}

struct UnkTracker;

struct UnkTrackerFields_b0159 {
    u8 pad_0x0[0x30];
    void* field_0x30;
};

s32 fn_71010953f0(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    if (void* object = FindByName(g_unk_7101d47310, name)) {
        if (UnkTracker* tracker = fn_710028aca8(object)) {
            fn_7100e95038(g_unk_7101d47310->field_0x2228, reinterpret_cast<UnkTrackerFields_b0159*>(tracker)->field_0x30);
            return 0;
        }
    }
    return 1;
}

s32 fn_71010954a0(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    if (void* object = FindByName(g_unk_7101d47310, name)) {
        if (UnkTracker* tracker = fn_710028aca8(object)) {
            fn_7100e950b4(g_unk_7101d47310->field_0x2228, reinterpret_cast<UnkTrackerFields_b0159*>(tracker)->field_0x30);
            return 0;
        }
    }
    return 1;
}

struct UnkVec3Raw_b0159 {
    u32 x, y, z;
};
struct UnkPoseSource_b0159 {
    u8 pad_0x0[0x44];
    u32 x;
    u8 pad_0x48[0xc];
    u32 y;
    u8 pad_0x58[0xc];
    u32 z;
};

s32 fn_710107cdd0(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        if (UnkPoseSource_b0159* source = static_cast<UnkPoseSource_b0159*>(FindByName(g_unk_7101d47310, name))) {
            u32 x = source->x;
            u32 y = source->y;
            u32 z = source->z;
            u32 pos[3] = {x, y, z};
            return fn_71010c0b40(L, "v", pos);
        }
    }
    return 0;
}

struct UnkArenaEntries_ovb0108;
void fn_7100e14ec0(UnkArenaEntries_ovb0108* self, const CStrId& key);

s32 fn_7101084270(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        if (UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608) {
            UnkArenaEntries_ovb0108* entries = reinterpret_cast<UnkArenaEntries_ovb0108*>(reinterpret_cast<u8*>(sub) + 0x858);
            CStrId id(name, true);
            fn_7100e14ec0(entries, id);
        }
    }
    return 0;
}

s32 fn_7101077090(lua_State* L) {
    const char* name = nullptr;
    u32 value = 0;
    if (fn_71010c0270(L, "s|i", &name, &value)) {
        UnkRenderManager* manager = g_unk_7101d47300;
        CStrId id(name, true);
        // The original passes the word as it is, not as a bool.
        reinterpret_cast<void (*)(UnkRenderManager*, const CStrId*, u32)>(fn_7100219558)(manager, &id, value);
    }
    return 0;
}

struct UnkStatsObject;

s32 fn_7101080a50(lua_State*) {
    UnkStatsObject* stats = Singleton<UnkStatsObject, 0x78, fn_7100131f48>(g_unk_7101d47100);
    fn_7100132248(stats, CStrId("DebugChecker", true), 1.0f);
    return 0;
}

s32 fn_7101080b10(lua_State*) {
    UnkStatsObject* stats = Singleton<UnkStatsObject, 0x78, fn_7100131f48>(g_unk_7101d47100);
    fn_7100132248(stats, CStrId("DebugChecker", true), 0.0f);
    return 0;
}

s32 fn_7101080bd0(lua_State*) {
    UnkStatsObject* stats = Singleton<UnkStatsObject, 0x78, fn_7100131f48>(g_unk_7101d47100);
    fn_7100132248(stats, CStrId("DebugMipmap", true), 1.0f);
    return 0;
}

s32 fn_7101080c90(lua_State*) {
    UnkStatsObject* stats = Singleton<UnkStatsObject, 0x78, fn_7100131f48>(g_unk_7101d47100);
    fn_7100132248(stats, CStrId("DebugMipmap", true), 0.0f);
    return 0;
}

struct UnkStruct_7100233684;
struct UnkSwitchFields_b0159 {
    u8 pad_0x0[0x50];
    u8* field_0x50;
};
struct UnkStruct_7100233684;
void fn_710020deb0(void* self, bool flag);

s32 fn_71010902a0(lua_State* L) {
    const char* name;
    bool flag;
    if (fn_71010c0270(L, "sb", &name, &flag)) {
        if (void* object = FindByName(g_unk_7101d47310, name)) {
            if (UnkSwitchFields_b0159* inner = reinterpret_cast<UnkSwitchFields_b0159*>(fn_7100233684(reinterpret_cast<UnkStruct_7100233684*>(object)))) {
                if (u8* part = inner->field_0x50) {
                    fn_710020deb0(part + 0x20, flag);
                    return 0;
                }
            }
        }
    }
    return 1;
}

s32 fn_7101091a20(lua_State* L) {
    const char* name = nullptr;
    bool playing = false;
    if (fn_71010c0ad0(L, 0, 1, "s", &name)) {
        void* music = Singleton<void, 0xf18, fn_7100fd3804>(reinterpret_cast<void*&>(g_unk_71726bc560));
        base::UnkNamedEntry key;
        fn_710007e704(&key, name);
        playing = fn_7100fd76d4(music, reinterpret_cast<CRntString*>(&key));
        fn_710007ecd4(reinterpret_cast<CRntString*>(&key));
    }
    return fn_71010c0b40(L, "b", playing);
}

struct UnkVec;

struct UnkZero3_b0159 {
    f32 x, y, z;
    UnkZero3_b0159() : x(0.0f), y(0.0f), z(0.0f) {}
};

s32 fn_7101094000(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        UnkGameManager* manager = g_unk_7101d47310;
        CStrId id(name, true);
        UnkZero3_b0159 v;
        fn_7100e038b0(manager, &id, reinterpret_cast<UnkVec*>(&v), 0);
    }
    return 0;
}

s32 fn_7101094170(lua_State* L) {
    const char* name;
    UnkZero3_b0159 v;
    if (fn_71010c0270(L, "sv", &name, &v)) {
        UnkGameManager* manager = g_unk_7101d47310;
        CStrId id(name, true);
        fn_7100e038b0(manager, &id, reinterpret_cast<UnkVec*>(&v), 0);
    }
    return 0;
}

s32 fn_7101095190(lua_State* L) {
    bool flag;
    const char* name;
    if (fn_71010c0270(L, "bs", &flag, &name)) {
        char* world = static_cast<char*>(g_unk_7101d47310->field_0x2228);
        bool remove = flag;
        CStrId id(name, true);
        fn_7100e8f9a0(world, remove, &id);
    }
    return 0;
}

s32 fn_7101095250(lua_State* L) {
    bool flag;
    if (fn_71010c0270(L, "b", &flag)) {
        char* world = static_cast<char*>(g_unk_7101d47310->field_0x2228);
        bool remove = flag;
        CStrId id("debug", true);
        fn_7100e8f9a0(world, remove, &id);
    }
    return 0;
}

// ---- every trigger component's enabled state ----
struct UnkTriggerEntry_b0159 {
    u8 pad_0x0[0x10];
    UnkReflObject* field_0x10;
};
struct UnkTriggerList_b0159 {
    UnkTriggerEntry_b0159** data;
    u32 count;
};
UnkTriggerList_b0159* fn_7100a5e1f4();
struct UnkStruct_710024fa0c;

s32 fn_7101084370(lua_State* L) {
    bool enabled = false;
    if (fn_71010c0270(L, "b", &enabled)) {
        UnkTriggerList_b0159* list = fn_7100a5e1f4();
        for (UnkTriggerEntry_b0159 **it = list->data, **end = it + list->count; it != end; ++it) {
            UnkReflObject* object = nullptr;
            if (UnkTriggerEntry_b0159* entry = *it) {
                if (UnkReflObject* c = entry->field_0x10)
                    object = fn_710009d50c(c->vf_0x0(), fn_7100a5bab0()) ? c : nullptr;
            }
            fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(object), enabled);
        }
    }
    return 0;
}

s32 fn_7101095550(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        void* world = g_unk_7101d47310->field_0x2228;
        const base::global::CStrId* current = fn_7100de9bf8(g_unk_7101d47310);
        CStrId id(name, true);
        fn_7100e9cbac(world, current, &id, true);
    }
    return 0;
}

struct UnkGuiHolder_b0159 {
    u8 pad_0x0[0x40];
    struct {
        u8 pad_0x0[0x10];
        UnkReflObject* field_0x10;
    }* field_0x40;
};
void fn_71011991c4(void* gui, UnkReflObject* entity);
void fn_7101198450(void* gui, UnkReflObject* entity);

s32 fn_71010880b0(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        UnkGuiHolder_b0159* holder;
        {
            CStrId id(name, true);
            holder = reinterpret_cast<UnkGuiHolder_b0159*>(fn_7101126814(&id));
        }
        if (holder && holder->field_0x40 && holder->field_0x40->field_0x10)
            fn_71011991c4(g_unk_7101d47330, holder->field_0x40->field_0x10);
    }
    return 0;
}

s32 fn_7101088250(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        UnkGuiHolder_b0159* holder;
        {
            CStrId id(name, true);
            holder = reinterpret_cast<UnkGuiHolder_b0159*>(fn_7101126814(&id));
        }
        if (holder && holder->field_0x40 && holder->field_0x40->field_0x10)
            fn_7101198450(g_unk_7101d47330, holder->field_0x40->field_0x10);
    }
    return 0;
}

// fn_7100fbc4e0 hands back the number of results it pushed.
static inline s32 PushObject(lua_State* L, void* arg) {
    return fn_7100fbc4e0(L, arg);
}
void* fn_71011986a8(void* gui, const CStrId* id);
void* fn_71011986b8(void* gui, const CStrId* id);

s32 fn_7101088d40(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    void* gui = g_unk_7101d47330;
    void* object;
    {
        CStrId id(name, true);
        object = fn_71011986a8(gui, &id);
    }
    return PushObject(L, object);
}

s32 fn_7101088e00(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    void* gui = g_unk_7101d47330;
    void* object;
    {
        CStrId id(name, true);
        object = fn_71011986b8(gui, &id);
    }
    return PushObject(L, object);
}

struct UnkVec;

struct UnkManager;
void fn_7100ee9bfc(void* self, const CStrId* id);

s32 fn_7101083fb0(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        void* manager = Singleton<void, 0xae8, fn_7100ee1f34>(reinterpret_cast<void*&>(g_unk_7101d47360));
        CStrId id(name, true);
        fn_7100ee9bfc(manager, &id);
    }
    return 0;
}

struct UnkSpawnGroupEach_ovb0065;
using UnkSpawnEachFn_ovb0065 = void (UnkSpawnGroupEach_ovb0065::*)(void*);
void fn_710111c3d8(UnkSpawnGroupEach_ovb0065* self, UnkSpawnEachFn_ovb0065 fn);
void fn_710111c470(UnkSpawnGroupEach_ovb0065* self, UnkSpawnEachFn_ovb0065 fn);
void fn_710111c68c(UnkSpawnGroupEach_ovb0065* self, void* object);

// A pointer to a member function from its two words (the function, or a virtual slot's offset with the flag).
static inline UnkSpawnEachFn_ovb0065 MemberPointer(u64 function, u64 adjust) {
    u64 words[2] = {function, adjust};
    UnkSpawnEachFn_ovb0065 fn;
    __builtin_memcpy(&fn, words, sizeof(fn));
    return fn;
}

s32 fn_7101085300(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    if (void* object = FindByName(g_unk_7101d47310, name)) {
        if (void* spawner = fn_71007c04f0(object)) {
            UnkSpawnGroupEach_ovb0065* each = static_cast<UnkSpawnGroupEach_ovb0065*>(spawner);
            fn_710111c3d8(each, MemberPointer(0x188, 1));
            fn_710111c470(each, MemberPointer(reinterpret_cast<u64>(&fn_710111c68c), 0));
            return 0;
        }
    }
    return 1;
}

void fn_7100e03b08(UnkGameManager* self, const char* first, const char* second, unk_prop_bag::UnkPropBag* bag);

s32 fn_7101090cd0(lua_State* L) {
    const char* first;
    const char* second;
    unk_prop_bag::UnkPropBag bag;
    if (fn_71010c0270(L, "sst", &first, &second, &bag))
        fn_7100e03b08(g_unk_7101d47310, first, second, &bag);
    return 0;
}

extern base::file::UnkFileDevice* g_unk_7101d4d828;
void fn_71000898b4(base::file::UnkFileDevice* self, const base::global::CFilePathStrId& path);

s32 fn_7101093310(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        base::file::UnkFileDevice* device = Singleton<base::file::UnkFileDevice, 0x1348, fn_71000893a8>(g_unk_7101d4d828);
        base::global::CFilePathStrId path(name, true);
        fn_71000898b4(device, path);
    }
    return 0;
}

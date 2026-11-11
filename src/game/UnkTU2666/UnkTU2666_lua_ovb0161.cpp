// Unit 2666 Lua bindings (fourth batch): each reads its arguments from the script stack (fn_71010c0270)
// and forwards them to the game manager, an entity found by the hash or the string id of a name, the
// music, sound, rumble or item managers, or builds a property bag. Names and layouts are placeholders.
#include <string.h>

#include "../../../lib/lua/lauxlib.h"
#include "../../../lib/lua/lua.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "base/UnkNamedRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/input/UnkRumbleManager.h"
#include "base/reflection/CEnumType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAISmartObjectComponent.h"
#include "game/CCameraComponent.h"
#include "game/CCollisionMaterialCacheComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU2704.h"
#include "game/UnkTU341.h"
#include "game/malloc.h"
#include "game/sound/CMusicManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

struct UnkWeakTarget;

template <class T, u64 Size, void (*Ctor)(T*)>
__attribute__((always_inline)) static inline T* Singleton_b0161(T*& instance) {
    if (!instance) {
        T* p = static_cast<T*>(fn_71000839a0(Size, 0x3f));
        Ctor(p);
        instance = p;
    }
    return instance;
}

struct UnkWeakTarget;
int fn_7100fbc4e0(lua_State* L, void* arg);
UnkWeakTarget* fn_7100debbc8(UnkGameManager* manager, const u64* key, const u32* kind);

// ---- the stats object ----
struct UnkStatsObject;
extern UnkStatsObject* g_unk_7101d47100;
f32 fn_7100132240(UnkStatsObject* self, const CStrId& name);

static inline f32 StatByName_b0161(UnkStatsObject* stats, const char* name) {
    CStrId id(name, true);
    return fn_7100132240(stats, id);
}

s32 fn_710107f9e0(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    UnkStatsObject* stats = Singleton_b0161<UnkStatsObject, 0x78, fn_7100131f48>(g_unk_7101d47100);
    return fn_71010c0b40(L, "f", (f64)StatByName_b0161(stats, name));
}

// ---- the music manager ----
extern void* g_unk_71726bc560;
struct UnkTuneMgrView_b0019;
f32 fn_7100fdc468(UnkTuneMgrView_b0019* self, const u64* id);
f32 fn_7100fdc500(UnkTuneMgrView_b0019* self, const u64* id);

static inline void* MusicManager_b0161() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = manager;
    }
    return manager;
}

s32 fn_7101092ac0(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return -1;
    void* manager = MusicManager_b0161();
    CFilePathStrId id(name, true);
    f32 value = fn_7100fdc468(static_cast<UnkTuneMgrView_b0019*>(manager), reinterpret_cast<const u64*>(&id));
    return fn_71010c0b40(L, "f", (f64)value);
}

s32 fn_7101092bb0(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return -1;
    void* manager = MusicManager_b0161();
    CFilePathStrId id(name, true);
    f32 value = fn_7100fdc500(static_cast<UnkTuneMgrView_b0019*>(manager), reinterpret_cast<const u64*>(&id));
    return fn_71010c0b40(L, "f", (f64)value);
}

// ---- the item inventory ----
struct UnkInventory;
struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
bool fn_71011428bc(UnkInventory* inventory, const UnkQualifiedId& item);
void fn_7101142a28(UnkInventory* inventory, const UnkQualifiedId& item, bool a, bool b, bool c, f32 amount,
                   f32 extra);

struct UnkSuitPlayer_b0161 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;
};

s32 fn_7101093740(lua_State* L) {
    UnkSuitPlayer_b0161* player = reinterpret_cast<UnkSuitPlayer_b0161*>(fn_7100dec788(g_unk_7101d47310));
    if (player && player->inventory) {
        UnkInventory* inventory = player->inventory;
        UnkPropertyEx<>& item = g_unk_717275a898;
        UnkQualifiedId& qid = ItemQualifiedId(item);
        if ((u64)qid.id.data == (u64)g_unk_7101d47108 + 0x10)
            fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
        bool has = fn_71011428bc(inventory, qid);
        if ((u64)qid.id.data == (u64)g_unk_7101d47108 + 0x10)
            fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
        f32 amount = has ? 0.0f : 1.0f;
        fn_7101142a28(inventory, qid, true, true, true, amount, 0.0f);
    }
    return 0;
}

// ---- sound and music with a file-path id ----
struct UnkSoundMgr;
struct UnkMgr;
struct UnkHandle;
extern UnkSoundMgr* g_unk_7101d4d820;
void fn_7100108abc(UnkMgr*, UnkHandle*, f32, f32);
void fn_7100fdc598(void* manager, f32 value, const CFilePathStrId* id);

s32 fn_71010929c0(lua_State* L) {
    f32 value;
    const char* name;
    bool flag = false;
    if (fn_71010c0270(L, "sf|b", &name, &value, &flag)) {
        CFilePathStrId id(name, true);
        fn_7100108abc(reinterpret_cast<UnkMgr*>(g_unk_7101d4d820), reinterpret_cast<UnkHandle*>(&id), 0.0f, value);
        if (flag)
            fn_7100fdc598(MusicManager_b0161(), value, &id);
    }
    return 0;
}

// ---- a music request by name ----
extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);
extern u32 g_unk_717274f508;
extern u64 g_unk_717274f510;
struct UnkMusicMode;
void fn_7100fd90d8(f32 a, f32 b, void* music, const CRntString* name, u32 c, const UnkMusicMode* d, u32 e, u32 f,
                   void* g);

s32 fn_7101090f50(lua_State* L) {
    f32 first;
    f32 second;
    s32 arg;
    const char* name;
    if (fn_71010c0270(L, "sffi", &name, &first, &second, &arg)) {
        void* manager = MusicManager_b0161();
        base::UnkNamedEntry key;
        fn_710007e704(&key, name);
        u32* zero = &g_unk_717274f508;
        if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_717274f510), __ATOMIC_ACQUIRE) & 1), 0)) {
            if (__cxa_guard_acquire(&g_unk_717274f510)) {
                *zero = 0;
                __cxa_guard_release(&g_unk_717274f510);
            }
        }
        alignas(8) u32 mode = *zero;
        fn_7100fd90d8(first, second, manager, reinterpret_cast<CRntString*>(&key), 7,
                      reinterpret_cast<const UnkMusicMode*>(&mode), arg, 0, nullptr);
        fn_710007ecd4(reinterpret_cast<CRntString*>(&key));
    }
    return 0;
}

// ---- the rumble manager ----
using base::input::UnkRumbleManager;
extern UnkRumbleManager* g_unk_7101d47348;
void fn_7100074f94(base::input::UnkRumbleManager* self, const u32* mask);
struct UnkTunableRumbleManager_b0068;
void fn_7100075020(UnkTunableRumbleManager_b0068* self, bool flag, const u32* channels);

// A zero whose constructor isn't constexpr, so the static is guarded.
struct UnkRumbleStaticZero_b0161 {
    u32 value;
    UnkRumbleStaticZero_b0161() : value(0) {}
};

inline const UnkRumbleStaticZero_b0161& UnkRumbleStaticZero() {
    static UnkRumbleStaticZero_b0161 s_value asm("g_unk_7101d58738");
    return s_value;
}

// The channel mask both bindings pass: the complement of that zero, limited to five bits.
inline u32 UnkRumbleDefaultMask() {
    static u32 s_mask asm("g_unk_7101d58748") = ~UnkRumbleStaticZero().value & 0x1f;
    return s_mask;
}

s32 fn_710107f440(lua_State*) {
    base::input::UnkRumbleManager* manager =
        Singleton_b0161<base::input::UnkRumbleManager, 0x2210, fn_71000733e4>(g_unk_7101d47348);
    alignas(8) u32 mask = UnkRumbleDefaultMask();
    fn_7100074f94(manager, &mask);
    return 0;
}

s32 fn_710107f540(lua_State*) {
    base::input::UnkRumbleManager* manager =
        Singleton_b0161<base::input::UnkRumbleManager, 0x2210, fn_71000733e4>(g_unk_7101d47348);
    alignas(8) u32 mask = UnkRumbleDefaultMask();
    fn_7100075020(reinterpret_cast<UnkTunableRumbleManager_b0068*>(manager), true, &mask);
    return 0;
}

// ---- the game manager, two names ----
void fn_7100e037f8(UnkGameManager* self, const CStrId& first, const CStrId& second);

s32 fn_7101082ca0(lua_State* L) {
    const char* first;
    const char* second;
    if (fn_71010c0270(L, "ss", &first, &second)) {
        UnkGameManager* manager = g_unk_7101d47310;
        CStrId a(first, true);
        CStrId b(second, true);
        fn_7100e037f8(manager, a, b);
    }
    return 0;
}

struct UnkArenaEntries_ovb0108;
void fn_7100e1593c(UnkArenaEntries_ovb0108* self, const CStrId& key, const CStrId& value);
void fn_7100e15a84(UnkArenaEntries_ovb0108* self, const CStrId& key, const CStrId& item_name);

s32 fn_7101084070(lua_State* L) {
    const char* first;
    const char* second;
    if (fn_71010c0270(L, "ss", &first, &second)) {
        if (void* arena = g_unk_7101d47310->field_0x608) {
            UnkArenaEntries_ovb0108* entries =
                reinterpret_cast<UnkArenaEntries_ovb0108*>(reinterpret_cast<char*>(arena) + 0x858);
            CStrId a(first, true);
            CStrId b(second, true);
            fn_7100e1593c(entries, a, b);
        }
    }
    return 0;
}

s32 fn_7101084170(lua_State* L) {
    const char* first;
    const char* second;
    if (fn_71010c0270(L, "ss", &first, &second)) {
        if (void* arena = g_unk_7101d47310->field_0x608) {
            UnkArenaEntries_ovb0108* entries =
                reinterpret_cast<UnkArenaEntries_ovb0108*>(reinterpret_cast<char*>(arena) + 0x858);
            CStrId a(first, true);
            CStrId b(second, true);
            fn_7100e15a84(entries, a, b);
        }
    }
    return 0;
}

// ---- the id table singleton ----
extern void* g_unk_7101d47360;
void fn_7100ee1f34(void* self);
void fn_7100eeb15c(void* self, const CStrId& id);
void fn_7100eeb24c(void* self);

s32 fn_71010838a0(lua_State* L) {
    const char* name;
    bool flag;
    if (fn_71010c0270(L, "sb", &name, &flag)) {
        CStrId id(name, true);
        bool load = flag;
        void* table = Singleton_b0161<void, 0xae8, fn_7100ee1f34>(g_unk_7101d47360);
        if (load)
            fn_7100eeb15c(table, id);
        else
            fn_7100eeb24c(table);
    }
    return 0;
}

s32 fn_710107f640(lua_State*) {
    base::input::UnkRumbleManager* manager =
        Singleton_b0161<base::input::UnkRumbleManager, 0x2210, fn_71000733e4>(g_unk_7101d47348);
    alignas(8) u32 mask = UnkRumbleDefaultMask();
    fn_7100075020(reinterpret_cast<UnkTunableRumbleManager_b0068*>(manager), false, &mask);
    return 0;
}

// ---- the metroid counters ----
s32 fn_71011442a0(UnkInventory* inventory, const UnkQualifiedId& item, s32 fallback);
void fn_71011406b4(UnkInventory* inventory, const UnkQualifiedId& item, s32 amount, bool a, bool b, bool c, f32 extra);

s32 fn_71010866b0(lua_State* L) {
    s32 added = 0;
    if (fn_71010c0270(L, "i", &added)) {
        if (UnkSuitPlayer_b0161* player = reinterpret_cast<UnkSuitPlayer_b0161*>(fn_7100dec788(g_unk_7101d47310))) {
            if (UnkInventory* inventory = player->inventory) {
                UnkPropertyEx<>& count_item = g_unk_7172759fd8;
                UnkQualifiedId& count_id = ItemQualifiedId(count_item);
                if ((u64)count_id.id.data == (u64)g_unk_7101d47108 + 0x10)
                    fn_7100ae9a10(&count_id, ItemSource(count_item), g_unk_7101d470e0);
                s32 current = fn_71011442a0(inventory, count_id, 0);
                s32 total = added + current;
                UnkPropertyEx<>& total_item = g_unk_717275a018;
                UnkQualifiedId& total_id = ItemQualifiedId(total_item);
                if ((u64)total_id.id.data == (u64)g_unk_7101d47108 + 0x10)
                    fn_7100ae9a10(&total_id, ItemSource(total_item), g_unk_7101d470e0);
                fn_71011406b4(inventory, total_id, total, true, true, true, 0.0f);
            }
        }
    }
    return 0;
}

// ---- a camera controller by name ----
struct UnkCameraCtrl;
bool fn_7101043a04(UnkCameraCtrl* controller, const char* name, lua_State* L, s32 mode);

s32 fn_7101086df0(lua_State* L) {
    const char* first = luaL_checkstring(L, 1);
    const char* second = luaL_checkstring(L, 2);
    bool result = false;
    if (UnkGameManagerTarget* target = fn_7100decab0(g_unk_7101d47310)) {
        if (void* holder = target->field_0x248) {
            UnkCameraCtrl* controller;
            {
                CStrId id(first, true);
                controller = fn_71010e389c(holder, &id);
            }
            if (controller)
                result = fn_7101043a04(controller, second, L, 3);
        }
    }
    return fn_71010c0b40(L, "b", result);
}

// ---- objects found by the hash of a name ----

static inline void* FindByName_b0161(UnkGameManager* manager, const char* name) {
    u32 kind_value = g_unk_71727bebb8;
    u64 key = fn_7100001570(name, strlen(name));
    alignas(8) u32 kind = kind_value;
    return fn_7100deba8c(manager, &key, &kind);
}

struct UnkObj_7101129240;
struct UnkSpawnRegistered_ovb0065;

// The text of a string id's data, which counts its readers at +0x44.
struct UnkStringText_b0161 {
    u8 pad_0x0[0x10];
    const char* text;
    u8 pad_0x18[0x2c];
    union {
        s32 readers;
    };
};
struct UnkSpawnEntry_b0161 {
    u8 pad_0x0[0x30];
    CStrId name;
};

s32 fn_7101084870(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    void* object = FindByName_b0161(g_unk_7101d47310, name);
    if (!object)
        return 1;
    void* registered = fn_7100360044(object);
    if (!registered)
        return 1;
    UnkSpawnEntry_b0161* entry = static_cast<UnkSpawnEntry_b0161*>(reinterpret_cast<void* (*)(UnkObj_7101129240*, bool)>(
        &fn_7101129b08)(reinterpret_cast<UnkObj_7101129240*>(registered), false));
    const CStrId& id = entry ? entry->name : g_unk_7101d470e0;
    ++reinterpret_cast<UnkStringText_b0161*>(id.data)->readers;
    const char* text = reinterpret_cast<UnkStringText_b0161*>(id.data)->text;
    return fn_71010c0b40(L, "s", text ? text : g_unk_710162e2ed);
}

void* fn_7100d7842c(void* object);
struct UnkTriggerList_A5_b72;
bool fn_7100a60104(UnkTriggerList_A5_b72* self, const CRntString& name);

s32 fn_71010846e0(lua_State* L) {
    const char* name;
    const char* second;
    if (!fn_71010c0270(L, "ss", &name, &second))
        return 0;
    void* object = FindByName_b0161(g_unk_7101d47310, name);
    if (!object)
        return 1;
    void* triggers = fn_7100d7842c(object);
    if (!triggers)
        return 1;
    base::UnkNamedEntry key;
    fn_710007e704(&key, second);
    bool found = fn_7100a60104(static_cast<UnkTriggerList_A5_b72*>(triggers), *reinterpret_cast<const CRntString*>(&key));
    s32 result = fn_71010c0b40(L, "b", found);
    fn_710007ecd4(reinterpret_cast<CRntString*>(&key));
    return result;
}

struct UnkStruct_710024fa0c;
void fn_7101156058(void* self, u64 hash, f32 x, f32 y, bool a, bool b, f32 z);

s32 fn_710108fe30(lua_State* L) {
    const char* name;
    const char* effect;
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
    bool a = false;
    bool b = false;
    if (!fn_71010c0270(L, "ssfffbb", &name, &effect, &x, &y, &z, &a, &b))
        return 0;
    void* object = FindByName_b0161(g_unk_7101d47310, name);
    if (!object)
        return 0;
    void* target = fn_7100798eec(object);
    if (!target)
        return 0;
    fn_7101156058(target, fn_7100001570(effect, strlen(effect)), x, y, a, b, z);
    return 0;
}

// ---- a landmark node by name ----
struct UnkLuaNode_b0160;
UnkLuaNode_b0160* fn_71008a3288(const CStrId* id);
void fn_71008a3384(UnkLuaNode_b0160* node, const f32* position, const f32* rotation);

struct UnkVec3_b0161 {
    f32 x, y, z;
    UnkVec3_b0161() : x(0.0f), y(0.0f), z(0.0f) {}
};

s32 fn_710107e840(lua_State* L) {
    const char* name;
    UnkVec3_b0161 position;
    UnkVec3_b0161 rotation;
    if (fn_71010c0270(L, "sffffff", &name, &position.x, &position.y, &position.z, &rotation.x, &rotation.y,
                      &rotation.z)) {
        if (g_unk_7101d47310->field_0x608) {
            UnkLuaNode_b0160* node;
            {
                CStrId id(name, true);
                node = fn_71008a3288(&id);
            }
            if (node)
                fn_71008a3384(node, &position.x, &rotation.x);
        }
    }
    return 0;
}

// ---- an effect at a position ----
void fn_710101f3d4(void* fx, const CFilePathStrId* id, const f32* position, bool a, bool b, f32 c, f32 d, f32 e, f32 f,
                   f32 g);

s32 fn_710108c790(lua_State* L) {
    const char* path;
    UnkVec3_b0161 position;
    f32 a;
    f32 b;
    f32 c;
    f32 d;
    if (fn_71010c0270(L, "sfffffff", &path, &position.x, &position.y, &position.z, &a, &b, &c, &d)) {
        void* fx = g_unk_71727b85d0;
        CFilePathStrId id(path, true);
        fn_710101f3d4(fx, &id, &position.x, false, false, a, b, c, d, 0.1f);
    }
    return 0;
}

// ---- the player's damage request ----
struct UnkEnumType;
extern UnkEnumType g_unk_7172730858;
s32 fn_710009976c(const base::reflection::CEnumType* self, const base::global::CStrId* name);
struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;

struct UnkPlayerRequest_b0161 {
    u8 pad_0x0[0x18];
    bool field_0x18;
    u8 pad_0x19[0x30];
    bool field_0x49;
    u8 pad_0x4a[2];
    f32 field_0x4c;
    f32 field_0x50;
    u8 pad_0x54[0x74];
    s32 field_0xc8;
    u8 pad_0xcc[0x34];

    UnkPlayerRequest_b0161() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkPlayerRequest_b0161() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};
static_assert(sizeof(UnkPlayerRequest_b0161) == 0x100, "request layout");

struct UnkRequestTarget_b0161 {
    void** vtable;
    void vf_0x188(UnkPlayerRequest_b0161* request) {
        reinterpret_cast<void (*)(UnkRequestTarget_b0161*, UnkPlayerRequest_b0161*)>(vtable[0x188 / 8])(this, request);
    }
};
struct UnkRequestPlayer_b0161 {
    u8 pad_0x0[0x1b8];
    UnkRequestTarget_b0161* field_0x1b8;
};

s32 fn_71010961e0(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        UnkRequestPlayer_b0161* player = reinterpret_cast<UnkRequestPlayer_b0161*>(fn_7100dec788(g_unk_7101d47310));
        if (player->field_0x1b8) {
            UnkPlayerRequest_b0161 request;
            request.field_0x18 = true;
            request.field_0x49 = true;
            request.field_0x4c = -1.0f;
            request.field_0x50 = 1.0f;
            {
                CStrId id(name, true);
                request.field_0xc8 = fn_710009976c(reinterpret_cast<const base::reflection::CEnumType*>(&g_unk_7172730858), &id);
            }
            player->field_0x1b8->vf_0x188(&request);
        }
    }
    return 0;
}

s32 fn_71010962e0(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        UnkRequestPlayer_b0161* player = reinterpret_cast<UnkRequestPlayer_b0161*>(fn_7100dec788(g_unk_7101d47310));
        if (player->field_0x1b8) {
            UnkPlayerRequest_b0161 request;
            request.field_0x18 = true;
            request.field_0x49 = true;
            request.field_0x4c = 1.0f;
            request.field_0x50 = 1.0f;
            {
                CStrId id(name, true);
                request.field_0xc8 = fn_710009976c(reinterpret_cast<const base::reflection::CEnumType*>(&g_unk_7172730858), &id);
            }
            player->field_0x1b8->vf_0x188(&request);
        }
    }
    return 0;
}

// ---- a table of every registered object's name ----
struct UnkRegisteredNode_b0161 {
    u8 pad_0x0[8];
    char* object;  // 0x8
    UnkRegisteredNode_b0161* next;  // 0x10
};
struct UnkNameHolder_b0161 {
    unk_prop_bag::UnkName* name;
};

s32 fn_710107ccd0(lua_State* L) {
    unk_prop_bag::UnkPropBag bag;
    UnkRegisteredNode_b0161* node = *reinterpret_cast<UnkRegisteredNode_b0161**>(
        reinterpret_cast<char*>(g_unk_7101d47310) + 0x2638);
    for (; node; node = node->next) {
        char* object = node->object;
        unk_prop_bag::fn_710050e4dc(&bag, *reinterpret_cast<const CStrId*>(object + 0x30),
                                    unk_prop_bag::UseName(fn_7100232fcc(object)->name));
    }
    return fn_71010c0b40(L, "t", &bag);
}

// ---- every entity of a kind ----
s32 fn_710113912c(void* object);

struct UnkSlotObject_b0161 {
    void** vtable;
    void vf_0x180(bool a, s32 b) {
        reinterpret_cast<void (*)(UnkSlotObject_b0161*, bool, s32)>(vtable[0x180 / 8])(this, a, b);
    }
};
struct UnkListedEntity_b0161 {
    u8 pad_0x0[0x1b8];
    UnkSlotObject_b0161* field_0x1b8;
    u8 pad_0x1c0[0x80];
    base::reflection::UnkReflObject* field_0x240;
    u8 pad_0x248[0x20];
    void* field_0x268;
};

s32 fn_710107fe60(lua_State*) {
    UnkRegisteredNode_b0161* node = *reinterpret_cast<UnkRegisteredNode_b0161**>(
        reinterpret_cast<char*>(g_unk_7101d47310) + 0x2638);
    for (; node; node = node->next) {
        base::reflection::UnkReflObject* object = reinterpret_cast<base::reflection::UnkReflObject*>(node->object);
        if (!object)
            continue;
        if (!fn_710009d50c(object->vf_0x0(), fn_7100d70c20()))
            continue;
        UnkListedEntity_b0161* entity = reinterpret_cast<UnkListedEntity_b0161*>(object);
        void* kind_source = entity->field_0x268;
        if (!kind_source)
            continue;
        if (fn_710113912c(kind_source) != 4 && fn_710113912c(kind_source) != 2)
            continue;
        UnkSlotObject_b0161* slot = entity->field_0x1b8;
        base::reflection::UnkReflObject* component = entity->field_0x240;
        base::reflection::UnkReflObject* emmy = nullptr;
        if (component)
            emmy = fn_710009d50c(component->vf_0x0(), fn_710053b934()) ? component : nullptr;
        if (slot && !emmy)
            slot->vf_0x180(true, 0);
    }
    return 0;
}

// ---- entities found by a string id ----
// The entity registered under a string id, looked up by the id's hash.
static inline UnkWeakTarget* FindById_b0161(UnkGameManager* manager, const CStrId& id, u64& key, u32& kind) {
    u32 kind_value = g_unk_71727bebb8;
    key = *reinterpret_cast<const u64*>(reinterpret_cast<const char*>(id.data) + 0x30);
    kind = kind_value;
    return fn_7100debbc8(manager, &key, &kind);
}

struct UnkVtable_b0161 {
    void* slots[0x80];
};
struct UnkVirtual_b0161 {
    UnkVtable_b0161* vtable;
};
struct UnkEntity_b0161 {
    u8 pad_0x0[0x1b8];
    UnkVirtual_b0161* field_0x1b8;
};
struct UnkSlotTarget_b0161 {
    UnkVtable_b0161* vtable;
    u8 pad_0x8[0x48];
    f32 field_0x50;
};

typedef void (*SlotFn_b0161)(UnkSlotTarget_b0161*, bool, f32);

s32 fn_7101080910(lua_State* L) {
    u64 key;
    alignas(8) u32 kind;
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    UnkGameManager* manager = g_unk_7101d47310;
    UnkWeakTarget* entity;
    {
        CStrId id(name, true);
        entity = FindById_b0161(manager, id, key, kind);
    }
    if (entity) {
        if (UnkSlotTarget_b0161* target = reinterpret_cast<UnkSlotTarget_b0161*>(reinterpret_cast<UnkEntity_b0161*>(entity)->field_0x1b8))
            reinterpret_cast<SlotFn_b0161>(target->vtable->slots[0x168 / 8])(target, true, target->field_0x50);
    }
    return 0;
}

s32 fn_7101080290(lua_State* L) {
    u64 key;
    alignas(8) u32 kind;
    const char* name;
    f32 value;
    if (!fn_71010c0270(L, "sf", &name, &value))
        return 0;
    UnkGameManager* manager = g_unk_7101d47310;
    UnkWeakTarget* entity;
    {
        CStrId id(name, true);
        entity = FindById_b0161(manager, id, key, kind);
    }
    if (entity) {
        if (UnkSlotTarget_b0161* target = reinterpret_cast<UnkSlotTarget_b0161*>(reinterpret_cast<UnkEntity_b0161*>(entity)->field_0x1b8))
            reinterpret_cast<SlotFn_b0161>(target->vtable->slots[0x168 / 8])(target, true, value);
    }
    return 0;
}

s32 fn_7101080650(lua_State* L) {
    u64 key;
    alignas(8) u32 kind;
    const char* name;
    f32 value;
    if (!fn_71010c0270(L, "sf", &name, &value))
        return 0;
    UnkGameManager* manager = g_unk_7101d47310;
    UnkWeakTarget* entity;
    {
        CStrId id(name, true);
        entity = FindById_b0161(manager, id, key, kind);
    }
    if (entity) {
        if (UnkSlotTarget_b0161* target = reinterpret_cast<UnkSlotTarget_b0161*>(reinterpret_cast<UnkEntity_b0161*>(entity)->field_0x1b8))
            reinterpret_cast<SlotFn_b0161>(target->vtable->slots[0x168 / 8])(target, true, target->field_0x50 * value);
    }
    return 0;
}

s32 fn_710107f290(lua_State* L) {
    u64 key;
    alignas(8) u32 kind;
    const char* name;
    f32 value;
    if (!fn_71010c0270(L, "sf", &name, &value))
        return 0;
    UnkGameManager* manager = g_unk_7101d47310;
    UnkWeakTarget* entity;
    {
        CStrId id(name, true);
        entity = FindById_b0161(manager, id, key, kind);
    }
    if (!entity)
        return 1;
    fn_7100d7babc(entity, true, value);
    return 0;
}

// Returns the number of values the callee left on the script stack.
static inline s32 PushObject_b0161(lua_State* L, void* object) {
    return fn_7100fbc4e0(L, object);
}

s32 fn_710107c990(lua_State* L) {
    u64 key;
    alignas(8) u32 kind;
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    UnkGameManager* manager = g_unk_7101d47310;
    UnkWeakTarget* entity;
    {
        CStrId id(name, true);
        entity = FindById_b0161(manager, id, key, kind);
    }
    if (!entity)
        return 0;
    UnkActor* actor = fn_7100d7c990(reinterpret_cast<UnkActor*>(entity));
    if (!actor)
        return 0;
    return PushObject_b0161(L, actor);
}

static inline void* FindByIdOld_b0161(UnkGameManager* manager, const CStrId& id, u64& key, u32& kind) {
    u32 kind_value = g_unk_71727bebb8;
    key = *reinterpret_cast<const u64*>(reinterpret_cast<const char*>(id.data) + 0x30);
    kind = kind_value;
    return fn_7100deba8c(manager, &key, &kind);
}

s32 fn_710107ca90(lua_State* L) {
    u64 key;
    alignas(8) u32 kind;
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    UnkGameManager* manager = g_unk_7101d47310;
    void* object;
    {
        CStrId id(name, true);
        object = FindByIdOld_b0161(manager, id, key, kind);
    }
    if (!object)
        return 0;
    return PushObject_b0161(L, object);
}

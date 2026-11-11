// Unit 2666 Lua bindings (fourth batch): arguments read from the script stack, forwarded to the game, sound,
// render, GUI or script managers. Names and layouts are placeholders.
#include <string.h>

#include "../../../lib/lua/lua.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "base/UnkNamedRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CActorComponent.h"
#include "game/CAttack.h"
#include "game/CCameraComponent.h"
#include "game/CCharClassAttack.h"
#include "game/CReserveTankInfo.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkItemProperties.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1176.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU2730.h"
#include "game/UnkTU2733.h"
#include "game/UnkTU99.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using unk_prop_bag::UnkPropBag;

inline void* FindByName_b0162(UnkGameManager* manager, const char* name) {
    u32 kind_value = g_unk_71727bebb8;
    u64 key = fn_7100001570(name, strlen(name));
    alignas(8) u32 kind = kind_value;
    return fn_7100deba8c(manager, &key, &kind);
}

struct UnkStruct_7100233684;
struct UnkModelUpdaterEvents_ovb0032;
base::reflection::UnkReflObject* fn_7100233684(UnkStruct_7100233684* self);
void fn_7101173c60(UnkModelUpdaterEvents_ovb0032* self, void* source);

s32 fn_710107eae0(lua_State* L) {
    const char* name;
    UnkPropBag bag;
    s32 result = 0;
    if (fn_71010c0270(L, "st", &name, &bag)) {
        result = 1;
        if (void* object = FindByName_b0162(g_unk_7101d47310, name)) {
            if (void* target = fn_7100233684(static_cast<UnkStruct_7100233684*>(object))) {
                fn_7101173c60(static_cast<UnkModelUpdaterEvents_ovb0032*>(target), &bag);
                result = 0;
            }
        }
    }
    return result;
}

void* fn_7100df1ff4(UnkGameManager* self, const CStrId& id);
void fn_7101051784(void* self, UnkPropBag* bag);

s32 fn_710107d020(lua_State* L) {
    const char* name;
    UnkPropBag bag;
    if (fn_71010c0270(L, "st", &name, &bag) == 1) {
        UnkGameManager* manager = g_unk_7101d47310;
        void* object;
        {
            CStrId id(name, true);
            object = fn_7100df1ff4(manager, id);
        }
        if (object)
            fn_7101051784(object, &bag);
    }
    return 0;
}

struct UnkCameraCtrl;
void fn_7101043f74(UnkCameraCtrl* self, UnkPropBag* bag);
void fn_710104418c(UnkCameraCtrl* self, UnkPropBag* bag);

s32 fn_7101086b60(lua_State* L) {
    UnkPropBag bag;
    const char* name;
    if (void* holder = fn_7100defcec(g_unk_7101d47310)) {
        if (fn_71010c0270(L, "s", &name)) {
            UnkCameraCtrl* ctrl;
            {
                CStrId id(name, true);
                ctrl = fn_71010e389c(holder, &id);
            }
            if (ctrl)
                fn_7101043f74(ctrl, &bag);
        }
    }
    return fn_71010c0b40(L, "t", &bag);
}

s32 fn_7101086cb0(lua_State* L) {
    const char* name;
    void* holder = fn_7100defcec(g_unk_7101d47310);
    if (!holder)
        return 0;
    UnkPropBag bag;
    if (fn_71010c0270(L, "st", &name, &bag)) {
        UnkCameraCtrl* ctrl;
        {
            CStrId id(name, true);
            ctrl = fn_71010e389c(holder, &id);
        }
        if (ctrl)
            fn_710104418c(ctrl, &bag);
    }
    return 0;
}

// ---- the music manager, created on first use ----
extern void* g_unk_71726bc560;
struct UnkMusicMode;
void fn_7100fd90d8(f32 a, f32 b, void* music, const CRntString* name, u32 c, const UnkMusicMode* d, u32 e, u32 f, void* g);

static inline void* MusicManager() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = manager;
    }
    return manager;
}

// Plays a music track by name: bit 0x20 loops, 0x10 does not, 0x40 is set by the last flag.

s32 fn_710108d0f0(lua_State* L) {
    const char* name;
    s32 kind = 7;
    bool loop = true;
    bool extra = false;
    bool fade = true;
    if (fn_71010c0ad0(L, 0, 1, "s|bbbi", &name, &loop, &fade, &extra, &kind)) {
        u32 bits = loop ? 0x20 : 0x10;
        if (extra)
            bits |= 0x40;
        void* music = MusicManager();
        CRntString key(name);
        alignas(8) u32 mode = bits;
        fn_7100fd90d8(-1.0f, -1.0f, music, &key, kind, reinterpret_cast<const UnkMusicMode*>(&mode), fade ? 2 : 1, 0,
                      nullptr);
    }
    return 0;
}

s32 fn_710108d200(lua_State* L) {
    const char* name;
    s32 kind = 7;
    bool loop = true;
    bool extra = false;
    bool fade = true;
    if (fn_71010c0ad0(L, 0, 1, "s|bbbi", &name, &loop, &fade, &extra, &kind)) {
        u32 bits = loop ? 0x20 : 0x10;
        if (extra)
            bits |= 0x40;
        bits |= 0x80;
        void* music = MusicManager();
        CRntString key(name);
        alignas(8) u32 mode = bits;
        fn_7100fd90d8(-1.0f, -1.0f, music, &key, kind, reinterpret_cast<const UnkMusicMode*>(&mode), fade ? 2 : 1, 0,
                      nullptr);
    }
    return 0;
}

s32 fn_710108d310(lua_State* L) {
    const char* name;
    s32 kind = 7;
    bool loop = true;
    bool extra = false;
    bool fade = true;
    f32 seconds = 0.0f;
    if (fn_71010c0ad0(L, 0, 1, "sf|bbbi", &name, &seconds, &loop, &fade, &extra, &kind)) {
        u32 bits = loop ? 0x20 : 0x10;
        if (extra)
            bits |= 0x40;
        void* music = MusicManager();
        CRntString key(name);
        alignas(8) u32 mode = bits;
        fn_7100fd90d8(-1.0f, -1.0f, music, &key, kind, reinterpret_cast<const UnkMusicMode*>(&mode), fade ? 2 : 1,
                      (s32)(seconds * 1000.0f), nullptr);
    }
    return 0;
}

// ---- music manager and sound manager ----
struct UnkLoadingMask {
    u32 bits;
    UnkLoadingMask() : bits(0) {}
    UnkLoadingMask(u32 b) : bits(b) {}
    UnkLoadingMask(const UnkLoadingMask& o) : bits(o.bits) {}
};
template <class T>
struct UnkLoadingMaskOf_b0162 {
    static const UnkLoadingMask& None() {
        static UnkLoadingMask s;
        return s;
    }
    static const UnkLoadingMask& All() {
        static UnkLoadingMask s(~None().bits & 0x1ff);
        return s;
    }
};
struct UnkLoadingMaskTag_b0162;
void fn_7100deac9c(UnkGameManager* self, const CRntString& name);

s32 fn_7101077d50(lua_State*) {
    void* music = MusicManager();
    fn_7100fd7b44(music, UnkLoadingMask(2), UnkLoadingMaskOf_b0162<UnkLoadingMaskTag_b0162>::All(), 0.0f);
    UnkGameManager* manager = g_unk_7101d47310;
    fn_7100deac9c(manager, CRntString("MAINMENU"));
    return 0;
}

struct UnkSoundMgr;
extern UnkSoundMgr* g_unk_7101d4d820;
struct UnkSoundEntry_A5_b76 {
    void* playing;
    u64 id;
};
struct UnkSoundRef_A5_b76 {
    UnkSoundEntry_A5_b76* entry;
    u64 id;
};
UnkSoundRef_A5_b76 fn_7100108aa4(UnkSoundMgr* self, const CFilePathStrId* path, bool flag);
void fn_7100fdc7e0(void* music, const CFilePathStrId* path, f32 value);

s32 fn_7101092ca0(lua_State* L) {
    const char* name;
    f32 volume = 1.0f;
    if (!fn_71010c0270(L, "sf", &name, &volume))
        return -1;
    CFilePathStrId path(name, true);
    fn_7100fdc7e0(MusicManager(), &path, volume);
    UnkSoundRef_A5_b76 ref = fn_7100108aa4(g_unk_7101d4d820, &path, false);
    if (ref.entry && ref.id != (u64)-1 && ref.entry->id == ref.id && ref.entry->playing) {
        UnkSoundMgr* sound = g_unk_7101d4d820;
        f32 level = volume;
        fn_71000fbe1c(reinterpret_cast<u8*>(sound) + 8, 5, level * *reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(&g_unk_7101d68580) + 0x3c), 0.0f);
    }
    return 0;
}

// ---- blackboard flags ----
struct UnkGameManagerIdTable {
    u8 pad_0x0[0xd80];
    CStrId field_0xd80;
    u8 pad_0xd88[0x1148 - 0xd88];
    CStrId field_0x1148;
    u8 pad_0x1150[0x2578 - 0x1150];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

static inline const UnkQualifiedId& GetQualifiedId_b0162(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if ((u64)qid.id.data == (u64)g_unk_7101d47108 + 0x10)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

struct UnkDef_7100fc4b34;
extern UnkPropertyEx<> g_unk_7172758df0;

struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

// Stores true under the property's id in the current blackboard section, unless the blackboard is locked.
static inline void SetFlag_b0162(UnkPropertyEx<>& property) {
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    const CStrId& section_id = GameManagerIdTable().field_0x1148;
    UnkQualifiedId& qid = ItemQualifiedId(property);
    if ((u64)qid.id.data == (u64)g_unk_7101d47108 + 0x10)
        fn_7100ae9a10(&qid, ItemSource(property), g_unk_7101d470e0);
    bool value = true;
    if (CBlackboard::CSection* section = fn_71010472e0(blackboard, section_id)) {
        if (!manager->field_0x968.field_0xe8) {
            UnkTypedValue typed;
            typed.type = fn_710000d228();
            typed.value = &value;
            fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(&qid), &typed);
        }
    }
}

s32 fn_7101093920(lua_State*) {
    SetFlag_b0162(reinterpret_cast<UnkPropertyEx<>&>(g_unk_71729dbc00));
    return 0;
}

s32 fn_7101096c50(lua_State*) {
    SetFlag_b0162(g_unk_7172758df0);
    return 0;
}

// ---- the player's inventory ----
struct UnkInventory;
struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
void fn_7101142a28(UnkInventory* inventory, const UnkQualifiedId& item, bool a, bool b, bool c, f32 amount, f32 extra);
bool fn_7100395b34(CBlackboard::CSection* section, const void* name, f32* out);

struct UnkInventoryOwner_b0162 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;
};

s32 fn_710108ef00(lua_State*) {
    CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
    f32 amount = 0.0f;
    {
        CStrId id("PLAYER_INVENTORY", true);
        const UnkQualifiedId& max_item = GetQualifiedId_b0162(g_unk_717275a218);
        if (CBlackboard::CSection* section = fn_7101047270(blackboard, id))
            fn_7100395b34(section, &max_item, &amount);
    }
    if (UnkInventoryOwner_b0162* player = reinterpret_cast<UnkInventoryOwner_b0162*>(fn_7100dec788(g_unk_7101d47310))) {
        if (UnkInventory* inventory = player->inventory)
            fn_7101142a28(inventory, GetQualifiedId_b0162(g_unk_717275a1d8), true, true, true, amount, 0.0f);
    }
    return 0;
}

// ---- sub-area manager, created on first use ----
extern void* g_unk_7101d47360;
void fn_7100ee1f34(void*);
void fn_7100eeb330(void* manager, CStrId* a, CStrId* b, f32 x, f32 y);
void fn_7100eee61c(void* manager, CStrId* a, CStrId* b, bool flag, bool other);
void fn_7100eee1f8(void* manager, CStrId* subArea, const CStrId* setup, bool a, bool b);

static inline void* SubAreaManager() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = manager;
    }
    return manager;
}

s32 fn_71010839a0(lua_State* L) {
    const char* first;
    const char* second;
    f32 y = 0.0f;
    f32 x = 0.0f;
    if (fn_71010c0270(L, "ssff", &first, &second, &x, &y)) {
        CStrId a(first, true);
        CStrId b(second, true);
        fn_7100eeb330(SubAreaManager(), &a, &b, x, y);
    }
    return 0;
}

s32 fn_7101083640(lua_State* L) {
    const char* first;
    const char* second;
    bool flag;
    bool other = false;
    if (fn_71010c0ad0(L, 0, 1, "ssb|b", &first, &second, &flag, &other)) {
        void* manager = SubAreaManager();
        CStrId a(first, true);
        CStrId b(second, true);
        fn_7100eee61c(manager, &a, &b, flag, other);
    }
    return 0;
}

s32 fn_7101083770(lua_State* L) {
    const char* first;
    const char* second;
    bool flag;
    bool other = false;
    if (fn_71010c0ad0(L, 0, 1, "ssb|b", &first, &second, &flag, &other)) {
        void* manager = SubAreaManager();
        CStrId a(first, true);
        CStrId b(second, true);
        fn_7100eee61c(manager, &a, &b, flag, other);
    }
    return 0;
}

s32 fn_7101083250(lua_State* L) {
    const char* name;
    bool flag;
    bool other = false;
    if (fn_71010c0ad0(L, 0, 1, "sb|b", &name, &flag, &other)) {
        void* manager = SubAreaManager();
        CStrId id(name, true);
        fn_7100eee1f8(manager, &id, &GameManagerIdTable().field_0xd80, flag, other);
    }
    return 0;
}

// ---- render manager ----
void fn_7100219544(UnkRenderManager* self, CStrId* a, CStrId* b, CStrId* c, s32 first, s32 second);

s32 fn_7101076f40(lua_State* L) {
    const char* a = nullptr;
    const char* b = nullptr;
    const char* c = nullptr;
    s32 first = 0;
    s32 second = 0;
    if (fn_71010c0270(L, "sssii", &a, &b, &c, &first, &second)) {
        UnkRenderManager* manager = g_unk_7101d47300;
        CStrId id_a(a, true);
        CStrId id_b(b, true);
        CStrId id_c(c, true);
        fn_7100219544(manager, &id_a, &id_b, &id_c, first, second);
    }
    return 0;
}

// ---- game manager: save, credits ----
void fn_7100df99e4(UnkGameManager* self, s32 a, s32 b);
void fn_7100dfe36c(UnkGameManager* self);
s32 fn_7100dfedd0(UnkGameManager* self);
s32 fn_7100dfecec(UnkGameManager* self, s32 a);

s32 fn_7101078490(lua_State* L) {
    const char* kind;
    bool result = false;
    if (fn_71010c0270(L, "s", &kind)) {
        if (!strcmp(kind, "current")) {
            fn_7100df99e4(g_unk_7101d47310, 0, 1);
            fn_7100dfe36c(g_unk_7101d47310);
            result = true;
        } else if (!strcmp(kind, "checkpoint")) {
            fn_7100dfe36c(g_unk_7101d47310);
            result = true;
        } else if (!strcmp(kind, "savedatacache")) {
            result = fn_7100dfedd0(g_unk_7101d47310) == 0;
        } else if (!strcmp(kind, "savedata")) {
            result = fn_7100dfecec(g_unk_7101d47310, 0) == 0;
        }
    }
    return fn_71010c0b40(L, "b", result);
}

void* fn_7100deab70(UnkGameManager*, const CRntString&);
void fn_7100deac9c(UnkGameManager* self, const CRntString& name);
void fn_7101185dc8(void* credits);
void fn_7100f17adc(void* rewards, s32 a, s32 b);

s32 fn_71010782d0(lua_State* L) {
    void* credits = fn_7100deab70(g_unk_7101d47310, CRntString("CREDITS"));
    if (credits) {
        int count = lua_gettop(L);
        bool flag = false;
        s32 first = -1;
        s32 second = -1;
        if (count >= 1)
            fn_71010c0270(L, "b|ii", &flag, &first, &second);
        void* rewards = fn_7100deab70(g_unk_7101d47310, CRntString("REWARDS"));
        if (rewards && flag) {
            fn_7101185dc8(credits);
            if (!((first | second) < 0))
                fn_7100f17adc(rewards, first, second);
        }
        fn_7100deac9c(g_unk_7101d47310, CRntString("CREDITS"));
    }
    return 0;
}

// ---- central units ----
using base::reflection::UnkReflObject;

struct UnkCentralUnitEntry {
    u8 pad_0x0[0x10];
    UnkReflObject* object;
};
struct UnkCentralUnitList {
    UnkCentralUnitEntry** data;
    u32 count;
};
void fn_7100772698(UnkReflObject* component, const CStrId* id, f32 value);
void fn_7100772610(UnkReflObject* component, const CStrId* id, f32 a, f32 b);

s32 fn_7101095060(lua_State* L) {
    const char* name;
    f32 value;
    if (fn_71010c0270(L, "sf", &name, &value)) {
        UnkCentralUnitList* list = fn_7100772c98();
        for (UnkCentralUnitEntry** it = list->data, **end = list->data + list->count; it != end; ++it) {
            UnkReflObject* component = nullptr;
            if (*it) {
                if (UnkReflObject* o = (*it)->object)
                    component = fn_710009d50c(o->vf_0x0(), fn_710076cf58()) ? o : nullptr;
            }
            CStrId id(name, true);
            fn_7100772698(component, &id, value);
        }
    }
    return 0;
}

s32 fn_7101094f30(lua_State* L) {
    const char* name;
    f32 b;
    f32 a;
    if (fn_71010c0270(L, "sff", &name, &a, &b)) {
        UnkCentralUnitList* list = fn_7100772c98();
        for (UnkCentralUnitEntry** it = list->data, **end = list->data + list->count; it != end; ++it) {
            UnkReflObject* component = nullptr;
            if (*it) {
                if (UnkReflObject* o = (*it)->object)
                    component = fn_710009d50c(o->vf_0x0(), fn_710076cf58()) ? o : nullptr;
            }
            CStrId id(name, true);
            fn_7100772610(component, &id, a, b);
        }
    }
    return 0;
}

// ---- actors found by name ----
struct UnkStrIdHash_b0162 {
    u8 pad_0x0[0x30];
    u64 hash;
};
struct UnkWeakTarget;
UnkWeakTarget* fn_7100debbc8(UnkGameManager* manager, const u64* key, const u32* kind);

static inline UnkWeakTarget* FindByStrId_b0162(UnkGameManager* manager, const CStrId& id) {
    u32 kind_value = g_unk_71727bebb8;
    u64 key = reinterpret_cast<UnkStrIdHash_b0162*>(id.data)->hash;
    alignas(8) u32 kind = kind_value;
    return fn_7100debbc8(manager, &key, &kind);
}

s32 fn_710108c090(lua_State* L) {
    const char* actor;
    const char* path_name;
    f32 value;
    if (fn_71010c0270(L, "ssf", &actor, &path_name, &value)) {
        UnkGameManager* manager = g_unk_7101d47310;
        UnkWeakTarget* target;
        {
            CStrId id(actor, true);
            target = FindByStrId_b0162(manager, id);
        }
        if (target) {
            CFilePathStrId path(path_name, true);
            fn_7100d7c48c(target, &path, value, 0.0f);
        }
    }
    return 0;
}

s32 fn_710108ddc0(lua_State* L) {
    const char* actor;
    const char* path_name;
    f32 value;
    if (fn_71010c0270(L, "ssf", &actor, &path_name, &value)) {
        UnkGameManager* manager = g_unk_7101d47310;
        UnkWeakTarget* target;
        {
            CStrId id(actor, true);
            target = FindByStrId_b0162(manager, id);
        }
        if (target) {
            CFilePathStrId path(path_name, true);
            fn_7100d7c48c(target, &path, value, 0.0f);
        }
    }
    return 0;
}

base::reflection::UnkReflObject* fn_710023341c(void* owner, const base::global::CStrId& name);
struct UnkStruct_710024fa0c;

s32 fn_710107dbc0(lua_State* L) {
    const char* actor;
    const char* component_name;
    if (!fn_71010c0270(L, "ss", &actor, &component_name))
        return 0;
    void* object = FindByName_b0162(g_unk_7101d47310, actor);
    if (!object)
        return 1;
    UnkReflObject* component;
    {
        CStrId id(component_name, true);
        component = fn_710023341c(object, id);
        if (component && !fn_710009d50c(component->vf_0x0(), fn_710024e3c8()))
            component = nullptr;
    }
    if (!component)
        return 1;
    fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(component), true);
    return 0;
}

s32 fn_710107dd00(lua_State* L) {
    const char* actor;
    const char* component_name;
    if (!fn_71010c0270(L, "ss", &actor, &component_name))
        return 0;
    void* object = FindByName_b0162(g_unk_7101d47310, actor);
    if (!object)
        return 1;
    UnkReflObject* component;
    {
        CStrId id(component_name, true);
        component = fn_710023341c(object, id);
        if (component && !fn_710009d50c(component->vf_0x0(), fn_710024e3c8()))
            component = nullptr;
    }
    if (!component)
        return 1;
    fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(component), false);
    return 0;
}

// ---- the name of the id the game manager maps an id to ----
struct UnkIdTextData_b0162 {
    u8 pad_0x0[0x10];
    const char* text;
    u8 pad_0x18[0x2c];
    union {
        s32 reads;
    };
};

s32 fn_7101082b70(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    UnkGameManager* manager = g_unk_7101d47310;
    CStrId mapped = fn_7100dfbda4(manager, CStrId(name, true));
    ++reinterpret_cast<UnkIdTextData_b0162*>(mapped.data)->reads;
    const char* text = reinterpret_cast<UnkIdTextData_b0162*>(mapped.data)->text;
    return fn_71010c0b40(L, "s", text ? text : g_unk_710162e2ed);
}

// ---- the language ----
struct UnkLanguage;
extern UnkLanguage* g_unk_7101d47358;
s32 fn_710119be24(const CRntString* name);
void fn_710119cb14(UnkLanguage* language, s32 index);
void fn_71011b6b5c(UnkGuiRoot* root, u32 value);

static inline UnkLanguage* Language_b0162() {
    UnkLanguage* language = g_unk_7101d47358;
    if (!language) {
        language = static_cast<UnkLanguage*>(fn_71000839a0(0x2068, 0x3f));
        fn_710119c020(language);
        g_unk_7101d47358 = language;
    }
    return language;
}

s32 fn_7101077930(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        s32 current = fn_710119c9f4(Language_b0162());
        s32 next;
        {
            CRntString key(name);
            next = fn_710119be24(&key);
        }
        if (next != 0x7fffffff && next != current) {
            fn_71011b6b5c(g_unk_717271e8f8, 1);
            fn_710119cb14(Language_b0162(), next);
            // Languages 0xa..0xe have their texts in separate resources.
            if (current == 0xe || (current | 1) == 0xd || (current | 1) == 0xb || next == 0xe ||
                (next | 1) == 0xd || (next | 1) == 0xb)
                fn_7100000790("GUIManager.LoadLanguageResources", "s", name);
        }
    }
    return 0;
}

// ---- save game ----
bool fn_7100dfbe3c(UnkGameManager*);
void fn_7100dfaf88(UnkGameManager* self, void* name, const CRntString& tag, bool flag);
struct UnkSaveGamePair_b54;
void fn_7100dfd174(UnkGameManager* self, CRntString* name, CRntString* tag, bool flag, u32 kind,
                   const UnkSaveGamePair_b54& extra);

s32 fn_7101078950(lua_State* L) {
    const char* kind;
    const char* name;
    const char* tag;
    bool flag;
    bool result = false;
    if (fn_71010c0270(L, "sssb", &kind, &name, &tag, &flag) && fn_7100dfbe3c(g_unk_7101d47310)) {
        if (!strcmp(kind, "checkpoint")) {
            UnkGameManager* manager = g_unk_7101d47310;
            CRntString name_key(name);
            CRntString tag_key(tag);
            auto save = reinterpret_cast<bool (*)(UnkGameManager*, void*, const CRntString&, bool)>(fn_7100dfaf88);
            result = save(manager, &name_key, tag_key, flag);
        } else if (!strcmp(kind, "savedata")) {
            UnkGameManager* manager = g_unk_7101d47310;
            CRntString name_key(name);
            CRntString tag_key(tag);
            auto save = reinterpret_cast<s32 (*)(UnkGameManager*, CRntString*, CRntString*, bool, u32,
                                                 const unk_prop_bag::UnkPair&)>(fn_7100dfd174);
            result = save(manager, &name_key, &tag_key, flag, 1, unk_prop_bag::UnkPair()) == 0;
        }
    }
    return fn_71010c0b40(L, "b", result);
}

// ---- GUI: a value for a named target, with the mask of every layer ----
extern void* g_unk_7101d47330;

template <class T>
struct UnkLayerMaskOf_b0162 {
    static const UnkLoadingMask& None() {
        static UnkLoadingMask s;
        return s;
    }
    static const UnkLoadingMask& All() {
        static UnkLoadingMask s(~None().bits & 0x7ff);
        return s;
    }
};
struct UnkLayerMaskTag_b0162;

s32 fn_710108af30(lua_State* L) {
    const char* name;
    f32 first = 1.0f;
    f32 second = 0.0f;
    const char* other;
    if (fn_71010c0270(L, "sffs", &name, &first, &second, &other)) {
        void* gui = g_unk_7101d47330;
        u64 key = fn_7100001570(name, strlen(name));
        f32 a = first;
        f32 b = second;
        alignas(8) u32 mask = UnkLayerMaskOf_b0162<UnkLayerMaskTag_b0162>::All().bits;
        fn_7101199ac4(gui, &key, &mask, a, b);
    }
    return 0;
}

// ---- the attack list of the first actor's AI ----
struct UnkAttackTarget_A2_b68;
struct UnkAttackListSelf_b0143;
CAttack* fn_7101105148(UnkAttackListSelf_b0143* self, const char* name);
s32 fn_71003623a4(CAttack* attack);

struct UnkAttackEntry_b0162 {
    u8 pad_0x0[0x10];
    bool field_0x10;
};
static inline UnkAttackEntry_b0162* GetEntry_b0162(CAttack* attack, u32 value) {
    return reinterpret_cast<UnkAttackEntry_b0162* (*)(UnkAttackTarget_A2_b68*, u32)>(&fn_71003646b0)(
        reinterpret_cast<UnkAttackTarget_A2_b68*>(attack->field_0x20), value);
}

struct UnkAttackAI_b0162 {
    u8 pad_0x0[0x1780];
    union {
        UnkIdTextData_b0162* id;
    };
    u32 index;  // 0x1788
};
struct UnkAttackActor_b0162 {
    u8 pad_0x0[0x240];
    UnkAttackAI_b0162* ai;
};
struct UnkAttackHolder_b0162 {
    u8 pad_0x0[0x10];
    UnkAttackActor_b0162* actor;
};

s32 fn_71010864e0(lua_State*) {
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    if (UnkAttackHolder_b0162* holder = *reinterpret_cast<UnkAttackHolder_b0162**>(manager)) {
        if (UnkAttackActor_b0162* actor = holder->actor) {
            if (UnkAttackAI_b0162* ai = actor->ai) {
                u32 index = 0;
                if (reinterpret_cast<void*>(ai->id) != &g_unk_7101d47108->empty) {
                    ++ai->id->reads;
                    const char* text = ai->id->text;
                    CAttack* attack = fn_7101105148(reinterpret_cast<UnkAttackListSelf_b0143*>(ai),
                                                    text ? text : g_unk_710162e2ed);
                    if (attack) {
                        for (s32 i = 0; i < fn_71003623a4(attack); ++i) {
                            UnkAttackEntry_b0162* entry = GetEntry_b0162(attack, i);
                            if (entry && entry->field_0x10) {
                                index = i;
                                break;
                            }
                        }
                    }
                }
                ai->index = index;
            }
        }
    }
    return 0;
}

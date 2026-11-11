// Unit 2666 Lua bindings (batch ovb0160): each reads its arguments from the script stack (fn_71010c0270)
// and hands them, as string ids, string keys or hashes, to the game manager, the sound, music or
// effect singletons (created on first use) or the object a name finds. Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/UnkNamedRegistry.h"
#include "base/UnkVector.h"
#include "base/file/nx/CDeviceNX.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "base/snd/CSoundSystem.h"
#include "game/CCollisionMaterialCacheComponent.h"
#include "game/CDrop.h"
#include "game/CFXComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2669.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU52.h"
#include "game/UnkTU92.h"
#include "game/UnkXformNode.h"
#include "game/malloc.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

struct lua_State;
int fn_7100fbc4e0(lua_State* L, void* arg);

// fn_7100fbc4e0 hands back the number of results it pushed.
static inline s32 PushObject(lua_State* L, void* arg) {
    return fn_7100fbc4e0(L, arg);
}

bool fn_7100dfc344(UnkGameManager* manager, const char* const& a, const char* const& b, const base::global::CRntString& name,
                   bool flag);

// fn_7100dfc344 answers with a flag; its other user declares it without one.
static inline bool RecordSnapshot(UnkGameManager* manager, const char* const& a, const char* const& b,
                                  const base::global::CRntString& name, bool flag) {
    return reinterpret_cast<bool (*)(UnkGameManager*, const char* const&, const char* const&,
                                     const base::global::CRntString&, bool)>(fn_7100dfc344)(manager, a, b, name, flag);
}

s32 fn_7101078b20(lua_State* L) {
    const char* a;
    const char* b;
    const char* c;
    bool flag;
    bool ok = false;
    if (fn_71010c0270(L, "sssb", &a, &b, &c, &flag)) {
        UnkGameManager* manager = g_unk_7101d47310;
        base::UnkNamedEntry ka;
        fn_710007e704(&ka, a);
        base::UnkNamedEntry kb;
        fn_710007e704(&kb, b);
        base::UnkNamedEntry kc;
        fn_710007e704(&kc, c);
        ok = RecordSnapshot(manager, reinterpret_cast<const char* const&>(ka), reinterpret_cast<const char* const&>(kb),
                            reinterpret_cast<const CRntString&>(kc), flag);
        fn_710007ecd4(reinterpret_cast<CRntString*>(&kc));
        fn_710007ecd4(reinterpret_cast<CRntString*>(&kb));
        fn_710007ecd4(reinterpret_cast<CRntString*>(&ka));
    }
    return fn_71010c0b40(L, "b", ok);
}

struct UnkGuiHolder_b0159;
UnkGuiHolder_b0159* fn_7101126814(const CStrId* id);

struct UnkLuaSub_b0160 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};
struct UnkLuaTarget_b0160 {
    u8 pad_0x0[0x40];
    UnkLuaSub_b0160* field_0x40;
};

s32 fn_710107cc00(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    UnkLuaTarget_b0160* target;
    {
        CStrId id(name, true);
        target = reinterpret_cast<UnkLuaTarget_b0160*>(fn_7101126814(&id));
    }
    if (target && target->field_0x40 && target->field_0x40->field_0x10)
        return PushObject(L, target->field_0x40->field_0x10);
    return 0;
}


struct UnkSpawnGroupEach_ovb0065;
using UnkSpawnEachFn_ovb0065 = void (UnkSpawnGroupEach_ovb0065::*)(void*);
void fn_710111c3d8(UnkSpawnGroupEach_ovb0065* self, UnkSpawnEachFn_ovb0065 fn);
void fn_710111c470(UnkSpawnGroupEach_ovb0065* self, UnkSpawnEachFn_ovb0065 fn);
void fn_710111c508(UnkSpawnGroupEach_ovb0065* self, void* object);

// A pointer to a member function from its two words (the function, or a virtual slot's offset with the flag).
static inline UnkSpawnEachFn_ovb0065 MemberPointer(u64 function, u64 adjust) {
    u64 words[2] = {function, adjust};
    UnkSpawnEachFn_ovb0065 fn;
    __builtin_memcpy(&fn, words, sizeof(fn));
    return fn;
}

static inline void* FindByName(UnkGameManager* manager, const char* name, u32 kind) {
    u64 hash = fn_7100001570(name, __builtin_strlen(name));
    alignas(8) u32 flags = kind;
    return fn_7100deba8c(manager, &hash, &flags);
}

s32 fn_7101085230(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    UnkGameManager* manager = g_unk_7101d47310;
    void* object = FindByName(manager, name, g_unk_71727bebb8);
    if (object) {
        if (void* spawner = fn_71007c04f0(object)) {
            UnkSpawnGroupEach_ovb0065* each = static_cast<UnkSpawnGroupEach_ovb0065*>(spawner);
            fn_710111c3d8(each, MemberPointer(reinterpret_cast<u64>(&fn_710111c508), 0));
            fn_710111c470(each, MemberPointer(reinterpret_cast<u64>(&fn_710111c508), 0));
            return 0;
        }
    }
    return 1;
}

struct UnkLuaObjRef_b0160 {
    u8 pad_0x0[0x10];
    void* obj;
};
struct UnkLuaWorld_b0160 {
    u8 pad_0x0[0xd63f40];
    UnkLuaObjRef_b0160* field_0xd63f40;
};
void fn_7100b4333c(void* obj, const CStrId* id);

s32 fn_71010956b0(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        UnkLuaObjRef_b0160* ref = static_cast<UnkLuaWorld_b0160*>(g_unk_7101d47310->field_0x2228)->field_0xd63f40;
        void* obj = ref ? ref->obj : nullptr;
        CStrId id(name, true);
        fn_7100b4333c(obj, &id);
    }
    return 0;
}


void* fn_7100deb7a8(UnkGameManager* self, const CStrId* id);
void* fn_7100deb914(UnkGameManager* self, const CStrId* id);

s32 fn_710107ece0(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    UnkGameManager* manager = g_unk_7101d47310;
    void* found;
    {
        CStrId id(name, true);
        found = fn_7100deb7a8(manager, &id);
    }
    if (found)
        return PushObject(L, found);
    return 0;
}

s32 fn_710107edb0(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    UnkGameManager* manager = g_unk_7101d47310;
    void* found;
    {
        CStrId id(name, true);
        found = fn_7100deb914(manager, &id);
    }
    if (found)
        return PushObject(L, found);
    return 0;
}

extern void* g_unk_717275c380;
void fn_710101eee8(void* self, const CFilePathStrId* path, bool flag, u32 mode, f32 a, f32 b, f32 c);
void fn_7100fc8dec(void* self, const CFilePathStrId* path, bool flag, u32 mode, f32 a, f32 b, f32 c);

s32 fn_710108c890(lua_State* L) {
    const char* path;
    bool flag = false;
    f32 a;
    f32 b;
    f32 c;
    if (fn_71010c0270(L, "sffbf", &path, &a, &b, &flag, &c)) {
        void* fx = g_unk_71727b85d0;
        CFilePathStrId id(path, true);
        fn_710101eee8(fx, &id, flag, 0, a, b, c);
    }
    return 0;
}

s32 fn_710108c9b0(lua_State* L) {
    const char* path;
    bool flag = false;
    f32 a;
    f32 b;
    f32 c;
    if (fn_71010c0270(L, "sffbf", &path, &a, &b, &flag, &c)) {
        void* fx = g_unk_717275c380;
        CFilePathStrId id(path, true);
        fn_7100fc8dec(fx, &id, flag, 0, a, b, c);
    }
    return 0;
}

struct UnkLuaNode_b0160;
UnkLuaNode_b0160* fn_71008a3288(const CStrId* id);
UnkVec3f fn_71008a32f8(UnkLuaNode_b0160* node);
UnkVec3f fn_71008a3314(UnkLuaNode_b0160* node);

s32 fn_710107e940(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    UnkLuaNode_b0160* node;
    {
        CStrId id(name, true);
        node = fn_71008a3288(&id);
    }
    if (!node)
        return 0;
    UnkVec3f v = fn_71008a32f8(node);
    return fn_71010c0b40(L, "v", &v);
}

s32 fn_710107ea10(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    UnkLuaNode_b0160* node;
    {
        CStrId id(name, true);
        node = fn_71008a3288(&id);
    }
    if (!node)
        return 0;
    UnkVec3f v = fn_71008a3314(node);
    return fn_71010c0b40(L, "v", &v);
}

struct UnkCamCtrl_ovb5;
UnkCamCtrl_ovb5* fn_7100df2404(UnkGameManager* manager, const CStrId& id);

s32 fn_710107d200(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name) == 1) {
        UnkGameManager* manager = g_unk_7101d47310;
        CStrId id(name, true);
        return fn_71010c0b40(L, "b", fn_7100df2404(manager, id) != nullptr);
    }
    return 0;
}

struct UnkStatsObject;
extern UnkStatsObject* g_unk_7101d47100;
void fn_7100132248(UnkStatsObject* self, const CStrId& name, f32 value);

s32 fn_710107fad0(lua_State* L) {
    union Slot {
        Slot() {}
        ~Slot() {}
        CStrId id;
    } slot;
    const char* name;
    f32 value;
    if (fn_71010c0270(L, "sf", &name, &value)) {
        UnkStatsObject* single = g_unk_7101d47100;
        if (!single) {
            single = static_cast<UnkStatsObject*>(fn_71000839a0(0x78, 0x3f));
            fn_7100131f48(single);
            g_unk_7101d47100 = single;
        }
        const char* n = name;
        f32 v = value;
        new (&slot.id) CStrId(n, true);
        fn_7100132248(single, slot.id, v);
        slot.id.~CStrId();
    }
    return 0;
}

struct UnkSoundMgr;
extern UnkSoundMgr* g_unk_7101d4d820;
struct UnkMgr;
struct UnkHandle;
void fn_7100108a84(UnkMgr*, UnkHandle*, f32);

s32 fn_710108d4f0(lua_State* L) {
    const char* path = nullptr;
    if (fn_71010c0270(L, "|s", &path)) {
        const char* text = path;
        UnkSoundMgr* sound = g_unk_7101d4d820;
        if (text) {
            CFilePathStrId id(text, true);
            fn_7100108a84(reinterpret_cast<UnkMgr*>(sound), reinterpret_cast<UnkHandle*>(&id), 0.2f);
        } else {
            fn_7100108b0c(sound, 1, 0.2f);
        }
    }
    return 0;
}


struct UnkWorld;
struct UnkRinkaControl_ruc;
struct UnkStruct_710024fa0c;
void fn_710114fe8c(UnkWorld*, u64, u32, u32);

s32 fn_710108fc70(lua_State* L) {
    const char* target;
    const char* effect;
    bool first;
    bool second;
    bool third;
    if (fn_71010c0270(L, "ssbbb", &target, &effect, &first, &second, &third)) {
        UnkGameManager* manager = g_unk_7101d47310;
        if (void* object = FindByName(manager, target, g_unk_71727bebb8)) {
            if (UnkStruct_710024fa0c* world = fn_7100798eec(object))
                fn_710114fe8c(reinterpret_cast<UnkWorld*>(world), fn_7100001570(effect, __builtin_strlen(effect)), first, second);
        }
    }
    return 0;
}

s32 fn_710108fd50(lua_State* L) {
    const char* target;
    const char* effect;
    bool first;
    bool second = false;
    if (fn_71010c0270(L, "ssbb", &target, &effect, &first, &second)) {
        UnkGameManager* manager = g_unk_7101d47310;
        if (void* object = FindByName(manager, target, g_unk_71727bebb8)) {
            if (UnkStruct_710024fa0c* world = fn_7100798eec(object))
                fn_710114ffb8(reinterpret_cast<UnkRinkaControl_ruc*>(world),
                              reinterpret_cast<void*>(fn_7100001570(effect, __builtin_strlen(effect))), first, second);
        }
    }
    return 0;
}


struct UnkLuaPlaced_b0160 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0(const UnkVec3f* v, bool flag);
    virtual void vf_0xd8(const UnkVec3f* v, bool flag);
};

s32 fn_710107cf40(lua_State* L) {
    const char* name;
    UnkVec3f first(0.0f, 0.0f, 0.0f);
    UnkVec3f second(0.0f, 0.0f, 0.0f);
    bool flag;
    if (fn_71010c0270(L, "svvb", &name, &first, &second, &flag)) {
        UnkGameManager* manager = g_unk_7101d47310;
        if (auto* object = static_cast<UnkLuaPlaced_b0160*>(FindByName(manager, name, g_unk_71727bebb8))) {
            object->vf_0xd0(&first, flag);
            object->vf_0xd8(&second, flag);
        }
    }
    return 0;
}


extern void* g_unk_7101d47368;
struct UnkEnvironmentManager;
void fn_7100d8e32c(UnkEnvironmentManager*, const CStrId*, bool, bool);

extern void* g_unk_7101d47360;
void fn_7100ee1f34(void* self);
void fn_7100eea3fc(void* self, const base::global::CStrId* id, bool a, bool b);
void fn_7100eea334(void* self, const base::global::CStrId* id, bool a, bool b);

s32 fn_7101083ac0(lua_State* L) {
    const char* name;
    bool flag = false;
    if (fn_71010c0ad0(L, 0, 1, "s|b", &name, &flag)) {
        void* mgr = g_unk_7101d47368;
        if (!mgr) {
            mgr = fn_71000839a0(0x4c8, 0x3f);
            fn_7100d8ced8(mgr);
            g_unk_7101d47368 = mgr;
        }
        CStrId id(name, true);
        fn_7100d8e32c(static_cast<UnkEnvironmentManager*>(mgr), &id, flag, false);
    }
    return 0;
}

s32 fn_7101083480(lua_State* L) {
    const char* name;
    bool first;
    bool second = false;
    if (fn_71010c0ad0(L, 0, 1, "sb|b", &name, &first, &second)) {
        void* mgr = g_unk_7101d47360;
        if (!mgr) {
            mgr = fn_71000839a0(0xae8, 0x3f);
            fn_7100ee1f34(mgr);
            g_unk_7101d47360 = mgr;
        }
        CStrId id(name, true);
        fn_7100eea3fc(mgr, &id, first, second);
    }
    return 0;
}

s32 fn_7101083560(lua_State* L) {
    const char* name;
    bool first;
    bool second = false;
    if (fn_71010c0ad0(L, 0, 1, "sb|b", &name, &first, &second)) {
        void* mgr = g_unk_7101d47360;
        if (!mgr) {
            mgr = fn_71000839a0(0xae8, 0x3f);
            fn_7100ee1f34(mgr);
            g_unk_7101d47360 = mgr;
        }
        CStrId id(name, true);
        fn_7100eea334(mgr, &id, first, second);
    }
    return 0;
}

struct UnkGameManagerIdTable {
    u8 pad_0x0[0xd80];
    CStrId field_0xd80;
    u8 pad_0xd88[0x2578 - 0xd88];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

s32 fn_7101083390(lua_State* L) {
    bool first;
    bool second = false;
    if (fn_71010c0ad0(L, 0, 1, "b|b", &first, &second)) {
        void* mgr = g_unk_7101d47360;
        if (!mgr) {
            mgr = fn_71000839a0(0xae8, 0x3f);
            fn_7100ee1f34(mgr);
            g_unk_7101d47360 = mgr;
        }
        fn_7100eea3fc(mgr, &GameManagerIdTable().field_0xd80, first, second);
    }
    return 0;
}


struct UnkLuaPart_b0160;
struct UnkLuaOwner_b0160 {
    u8 pad_0x0[0x1b8];
    UnkLuaPart_b0160* field_0x1b8;
};
bool fn_71011603a4(UnkLuaPart_b0160* part, f32 value);
bool fn_7101160214(UnkLuaPart_b0160* part, f32 value);
extern void* g_unk_7101d47330;
void fn_71011991c4(void* self, UnkReflObject* object);
void fn_7101198450(void* self, UnkReflObject* object);
UnkWeakTarget* fn_7100debbc8(UnkGameManager* manager, const u64* key, const u32* kind);

static inline UnkLuaOwner_b0160* FindById(UnkGameManager* manager, const CStrId& id, u32 kind) {
    u64 key = *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(id.data) + 0x30);
    alignas(8) u32 flags = kind;
    return reinterpret_cast<UnkLuaOwner_b0160*>(fn_7100debbc8(manager, &key, &flags));
}

s32 fn_7101080750(lua_State* L) {
    const char* name;
    f32 value;
    if (fn_71010c0270(L, "sf", &name, &value)) {
        UnkGameManager* manager = g_unk_7101d47310;
        UnkLuaOwner_b0160* owner;
        {
            CStrId id(name, true);
            owner = FindById(manager, id, g_unk_71727bebb8);
        }
        if (owner && owner->field_0x1b8)
            fn_71011603a4(owner->field_0x1b8, value);
    }
    return 0;
}

s32 fn_7101080830(lua_State* L) {
    const char* name;
    f32 value;
    if (fn_71010c0270(L, "sf", &name, &value)) {
        UnkGameManager* manager = g_unk_7101d47310;
        UnkLuaOwner_b0160* owner;
        {
            CStrId id(name, true);
            owner = FindById(manager, id, g_unk_71727bebb8);
        }
        if (owner && owner->field_0x1b8)
            fn_7101160214(owner->field_0x1b8, value);
    }
    return 0;
}

s32 fn_7101087fd0(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        UnkGameManager* manager = g_unk_7101d47310;
        UnkLuaOwner_b0160* owner;
        {
            CStrId id(name, true);
            owner = FindById(manager, id, g_unk_71727bebb8);
        }
        if (owner)
            fn_71011991c4(g_unk_7101d47330, reinterpret_cast<UnkReflObject*>(owner));
    }
    return 0;
}

s32 fn_7101088170(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        UnkGameManager* manager = g_unk_7101d47310;
        UnkLuaOwner_b0160* owner;
        {
            CStrId id(name, true);
            owner = FindById(manager, id, g_unk_71727bebb8);
        }
        if (owner)
            fn_7101198450(g_unk_7101d47330, reinterpret_cast<UnkReflObject*>(owner));
    }
    return 0;
}

struct UnkMgr_0xf18;
extern void* g_unk_71726bc560;
void fn_7100fdbd84(UnkMgr_0xf18* self, u32 flag);
u32 fn_71010976c0(const char* name);

// The music manager, created on first use.
static inline UnkMgr_0xf18* MusicManager_b0160() {
    void* instance = g_unk_71726bc560;
    if (!instance) {
        instance = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(instance);
        g_unk_71726bc560 = instance;
    }
    return static_cast<UnkMgr_0xf18*>(instance);
}

s32 fn_7101092070(lua_State* L) {
    const char* name;
    bool flag = false;
    if (fn_71010c0270(L, "s|b", &name, &flag)) {
        fn_7100fdbd84(MusicManager_b0160(), 0);
        fn_7100fdb9e0(MusicManager_b0160(), fn_71010976c0(name), 0, -1.0f, 0.5f);
        fn_7100fdbd84(MusicManager_b0160(), flag);
    }
    return 0;
}

struct UnkEngineObject_0xf0;
extern UnkEngineObject_0xf0* g_unk_7101d47338;
void fn_7100075ec8(UnkEngineObject_0xf0* self, s32 index, const CRntString* a, const CRntString* b,
                   const CRntString* c, const CRntString* d, u32 e, u32 f);

s32 fn_71010931b0(lua_State* L) {
    const char* first;
    const char* second;
    const char* third;
    const char* fourth;
    if (fn_71010c0270(L, "ssss", &first, &second, &third, &fourth)) {
        UnkEngineObject_0xf0* mgr = g_unk_7101d47338;
        if (!mgr) {
            mgr = static_cast<UnkEngineObject_0xf0*>(fn_71000839a0(0xf0, 0x3f));
            fn_7100075db8(mgr);
            g_unk_7101d47338 = mgr;
        }
        CRntString a(first);
        CRntString b(second);
        CRntString c(third);
        CRntString d(fourth);
        fn_7100075ec8(mgr, -1, &a, &b, &c, &d, 0, 0);
    }
    return 0;
}

struct UnkInventory;
struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
bool fn_71011428bc(UnkInventory* inventory, const UnkQualifiedId& item);
void fn_7101142a28(UnkInventory* inventory, const UnkQualifiedId& item, bool a, bool b, bool c, f32 amount,
                   f32 extra);

struct UnkLuaPlayer_b0160 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;  // 0x270
};

static inline const UnkQualifiedId& GetQualifiedId_b0160(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

s32 fn_71010935b0(lua_State*) {
    auto* player = reinterpret_cast<UnkLuaPlayer_b0160*>(fn_7100dec788(g_unk_7101d47310));
    if (player && player->inventory) {
        UnkInventory* inventory = player->inventory;
        bool has = fn_71011428bc(inventory, GetQualifiedId_b0160(g_unk_717275a958));
        fn_7101142a28(inventory, GetQualifiedId_b0160(g_unk_717275a958), true, true, true, has ? 0.0f : 1.0f, 0.0f);
    }
    return 0;
}

// A list of strings (nodes linked through +0x30), emptied when it goes.
struct UnkLuaNameList_b0160 {
    void** vtable;
    u32 count;                      // 0x8
    u8* head;                       // 0x10
    u8* tail;                       // 0x18
    base::UnkAllocator* allocator;  // 0x20
    u32 field_0x28;

    UnkLuaNameList_b0160() : vtable(g_unk_7101a1a070 + 2), count(0), head(nullptr), tail(nullptr) {
        allocator = fn_7100083990();
        field_0x28 = 0x13;
    }
    ~UnkLuaNameList_b0160() {
        vtable = g_unk_7101a1a070 + 2;
        for (u8* node = head; node;) {
            u8* next = *reinterpret_cast<u8**>(node + 0x30);
            fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(node));
            allocator->vf_0x48(node);
            node = next;
        }
    }
};

struct UnkScriptManager_ovb0085;
void fn_710109dc90(UnkScriptManager_ovb0085* self, UnkLuaNameList_b0160* out);

s32 fn_7101077770(lua_State* L) {
    UnkLuaNameList_b0160 names;
    UnkScriptManager_ovb0085* finder = g_unk_71727bed90;
    if (!finder) {
        finder = static_cast<UnkScriptManager_ovb0085*>(fn_71000839a0(0x88, 0x3f));
        fn_710109d810(finder);
        g_unk_71727bed90 = finder;
    }
    fn_710109dc90(finder, &names);
    return fn_71010c0b40(L, "b", names.count != 0);
}

struct UnkEnumInfo;

UnkReflValue fn_7100fbdfd0(lua_State* L, s32 index);

struct UnkLuaReadScratch_b49a6 {
    u64 words[6];
};
bool fn_7100fc22a0(lua_State* L, s32 index, UnkTypedRef* value, u32 mode, UnkLuaReadScratch_b49a6* scratch);

// The value's storage: through field_0x0 when flagged or when it doesn't fit in 8 bytes, else inline.
static inline u64 TypeSize(const CType* type) {
    return *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(type) + 0x18);
}
static inline void* ValuePtr(UnkReflValue& v) {
    const CType* type = reinterpret_cast<const CType*>(v.type);
    return type ? ((v.field_0x14 == 1 || TypeSize(type) > 8) ? v.field_0x0 : &v.field_0x0) : nullptr;
}

s32 fn_7101095ee0(lua_State* L) {
    u32 mode;
    UnkReflValue value = fn_7100fbdfd0(L, 1);
    if (value.type) {
        UnkTypedRef ref;
        {
            ref.type = reinterpret_cast<const CType*>(&g_unk_7101d67c10);
            ref.ptr = &mode;
            UnkLuaReadScratch_b49a6 scratch = {};
            fn_7100fc22a0(L, 2, &ref, 2, &scratch);
        }
        ref.type = reinterpret_cast<const CType*>(value.type);
        ref.ptr = ValuePtr(value);
        UnkLuaReadScratch_b49a6 scratch = {};
        fn_7100fc22a0(L, 3, &ref, mode, &scratch);
    }
    fn_71000a1c60(&value);
    return 0;
}

// Unit 2666 Lua bindings (fourth batch): each reads its arguments from the script stack and looks an object up by
// name (or reaches a manager singleton), then hands it string ids, or fills a property table the script gets
// back. Names and layouts are placeholders.
#include <string.h>
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CActorComponent.h"
#include "game/CBlackboard.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/UnkStringPool.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/CDrop.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU2768.h"
#include "game/UnkTU341.h"
#include "game/UnkTU604.h"
#include "game/nn/time.h"
#include "types.h"

using base::global::CStrId;
using unk_prop_bag::UnkPropBag;

struct lua_State;

// The object the game manager knows under a name (the name's CRC64 and the kind g_unk_71727bebb8).
static inline void* FindByName(UnkGameManager* manager, const char* name) {
    u32 kind_value = g_unk_71727bebb8;
    u64 hash;
    alignas(8) u32 kind;
    hash = fn_7100001570(name, __builtin_strlen(name));
    kind = kind_value;
    return fn_7100deba8c(manager, &hash, &kind);
}

// A string id's pool hash (the u64 at +0x30 of its entry).
struct UnkStrIdHash_b0164 {
    u8 pad_0x0[0x30];
    u64 hash;
};
struct UnkWeakTarget;

// A handle made from a string id (fn_7100ae986c).
struct UnkStrIdHandle_b0164 {
    CStrId id;
    void* field_0x8;
    UnkStrIdHandle_b0164(const CStrId& id);
};

// A temporary list of objects (vtable g_unk_7101a32680, nodes from the default allocator with flags 0x13).

struct UnkList_b0164 {
    struct Node {
        void* value;
        Node* next;
    };
    char* vtable;
    u32 count;
    Node* head;
    Node* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    UnkList_b0164() {
        vtable = reinterpret_cast<char*>(g_unk_7101a32680) + 0x10;
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = fn_7100083990();
        flags = 0x13;
    }
    ~UnkList_b0164() {
        vtable = reinterpret_cast<char*>(g_unk_7101a32680) + 0x10;
        Node* n = head;
        if (!n)
            return;
        while (n) {
            Node* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
    }
};

struct UnkEntityA_b0164 {
    u8 pad_0x0[0x270];
    void* field_0x270;
};
struct UnkInventory;

s32 fn_710108f570(lua_State* L) {
    alignas(8) u32 kind;
    const char* first;
    const char* second;
    if (!fn_71010c0270(L, "ss", &first, &second))
        return 0;
    UnkEntityA_b0164* entity;
    {
        UnkGameManager* manager = g_unk_7101d47310;
        u64 key;
        CStrId a(first, true);
        u32 k = g_unk_71727bebb8;
        key = reinterpret_cast<UnkStrIdHash_b0164*>(a.data)->hash;
        kind = k;
        entity = reinterpret_cast<UnkEntityA_b0164*>(fn_7100debbc8(manager, &key, &kind));
    }
    if (!entity)
        return 1;
    void* object = entity->field_0x270;
    if (!object)
        return 1;
    CStrId b(second, true);
    UnkStrIdHandle_b0164 handle(b);
    fn_7101142a28(static_cast<UnkInventory*>(object), reinterpret_cast<const UnkQualifiedId&>(handle), true, true, true, 1.0f,
                  0.0f);
    return 0;
}

s32 fn_710108f050(lua_State* L) {
    u64 key;
    alignas(8) u32 kind;
    const char* first;
    const char* second;
    if (!fn_71010c0270(L, "ss", &first, &second))
        return 0;
    CStrId a(first, true);
    CStrId b(second, true);
    UnkStrIdHandle_b0164 handle(b);
    UnkGameManager* manager = g_unk_7101d47310;
    u32 k = g_unk_71727bebb8;
    key = reinterpret_cast<UnkStrIdHash_b0164*>(a.data)->hash;
    kind = k;
    s32 result = 0;
    if (UnkEntityA_b0164* entity = reinterpret_cast<UnkEntityA_b0164*>(fn_7100debbc8(manager, &key, &kind))) {
        if (entity->field_0x270)
            result = fn_71010c0b40(L, "f", (double)fn_7101142928(static_cast<UnkInventory*>(entity->field_0x270), reinterpret_cast<const UnkQualifiedId&>(handle)));
    }
    return result;
}

struct UnkCompEntry_b0164 {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* object;  // 0x10
};
struct UnkCompOwner_b0164 {
    u8 pad_0x0[0xa0];
    UnkCompEntry_b0164** entries;  // 0xa0
    u8 pad_0xa8[8];
    u32 count;  // 0xb0
};

s32 fn_710107d680(lua_State* L) {
    UnkCompOwner_b0164* owner = nullptr;
    {
        const char* name;
        if (fn_71010c0270(L, "s", &name))
            owner = static_cast<UnkCompOwner_b0164*>(FindByName(g_unk_7101d47310, name));
    }
    if (owner) {
        UnkPropBag bag;
        if (owner->count) {
            UnkCompEntry_b0164** it = owner->entries;
            UnkCompEntry_b0164** end = it + owner->count;
            for (; it != end; ++it) {
                UnkCompEntry_b0164* entry = *it;
                if (!entry)
                    continue;
                base::reflection::UnkReflObject* object = entry->object;
                if (!object)
                    continue;
                if (!fn_710009d50c(object->vf_0x0(), fn_71002525c0()))
                    continue;
                CStrId* key = reinterpret_cast<CStrId*>(reinterpret_cast<u8*>(object) + 8);
                bool flag = fn_710024fbc8(entry);
                unk_prop_bag::fn_710050f4ac(&bag, *key, flag);
            }
        }
        return fn_71010c0b40(L, "t", &bag);
    }
    return 0;
}

struct UnkPlayer;

struct UnkEnergyHolder_b0164;
struct UnkEnergy_b0164 {
    u8 pad_0x0[0x40];
    f32 max_energy;      // 0x40
    f32 current_energy;  // 0x44
};
UnkEnergy_b0164* fn_7100df55c8(UnkGameManager* self);

s32 fn_710107ba70(lua_State* L) {
    if (fn_7100dec788(g_unk_7101d47310)) {
        UnkPropBag bag;
        if (UnkEnergy_b0164* energy = fn_7100df55c8(g_unk_7101d47310)) {
            unk_prop_bag::fn_710033187c(&bag, CStrId("CurrentEnergy", true), energy->current_energy);
            unk_prop_bag::fn_710033187c(&bag, CStrId("MaxEnergy", true), energy->max_energy);
        }
        return fn_71010c0b40(L, "t", &bag);
    }
    return 0;
}

s32 fn_710108f3d0(lua_State* L) {
    alignas(8) u32 kind;
    const char* first;
    const char* second;
    if (!fn_71010c0270(L, "ss", &first, &second))
        return 0;
    UnkEntityA_b0164* entity;
    {
        UnkGameManager* manager = g_unk_7101d47310;
        u64 key;
        CStrId a(first, true);
        u32 k = g_unk_71727bebb8;
        key = reinterpret_cast<UnkStrIdHash_b0164*>(a.data)->hash;
        kind = k;
        entity = reinterpret_cast<UnkEntityA_b0164*>(fn_7100debbc8(manager, &key, &kind));
    }
    if (!entity)
        return 1;
    void* object = entity->field_0x270;
    if (!object)
        return 1;
    CStrId b(second, true);
    UnkStrIdHandle_b0164 handle(b);
    return fn_71010c0b40(L, "b", fn_71011428bc(static_cast<UnkInventory*>(object), reinterpret_cast<const UnkQualifiedId&>(handle)));
}

struct UnkLuaVec3_b0164 {
    f32 x, y, z;
    UnkLuaVec3_b0164() : x(0.0f), y(0.0f), z(0.0f) {}
};
void fn_7100df2dd4(UnkGameManager* self, const CStrId& a, u8 b1, u8 b2, u8 b3, u8 b4, u8 b5, const CStrId& c,
                   const CStrId& d, const UnkLuaVec3_b0164& v);

s32 fn_71010888b0(lua_State* L) {
    const char* s1;
    const char* s2;
    const char* s3;
    u8 b1, b2, b3, b4, b5;
    UnkLuaVec3_b0164 v;
    if (fn_71010c0270(L, "sbbbbbssfff", &s1, &b1, &b2, &b3, &b4, &b5, &s2, &s3, &v.x, &v.y, &v.z)) {
        UnkGameManager* manager = g_unk_7101d47310;
        CStrId id1(s1, true);
        u8 c5 = b5, c4 = b4, c3 = b3, c2 = b2, c1 = b1;
        CStrId id2(s2, true);
        CStrId id3(s3, true);
        fn_7100df2dd4(manager, id1, c1, c2, c3, c4, c5, id2, id3, v);
    }
    return 0;
}

struct UnkSoundHolder;
struct UnkSoundGroup_b0164 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual bool vf_0x28();
    virtual bool vf_0x30(u32 mask, u32 flags);
};

static inline UnkSoundHolder* SoundHolder_b0164() {
    if (!g_unk_7101d47340) {
        UnkSoundHolder* p = static_cast<UnkSoundHolder*>(fn_71000839a0(0x48, 0x3f));
        fn_710006cd9c(p);
        g_unk_7101d47340 = p;
    }
    return g_unk_7101d47340;
}

s32 fn_71010943a0(lua_State* L) {
    UnkSoundGroup_b0164* group = *reinterpret_cast<UnkSoundGroup_b0164**>(SoundHolder_b0164());
    if (!group || !group->vf_0x28())
        return 0;
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    u32 mask;
    if (!strcmp(name, "L1"))
        mask = 0x200;
    else if (!strcmp(name, "L2"))
        mask = 0x200000;
    else if (!strcmp(name, "R1"))
        mask = 0x100;
    else if (!strcmp(name, "R2"))
        mask = 0x100000;
    else if (!strcmp(name, "CROSS"))
        mask = 2;
    else if (!strcmp(name, "TRIANGLE"))
        mask = 0x400;
    else if (!strcmp(name, "SQUARE"))
        mask = 0x800;
    else
        mask = strcmp(name, "CIRCLE") == 0;
    return fn_71010c0b40(L, "b", group->vf_0x30(mask, 0xfd));
}

using base::reflection::UnkReflObject;

struct UnkCameraCtrl;
void fn_7101043f74(UnkCameraCtrl* component, UnkPropBag* bag);

s32 fn_710107d820(lua_State* L) {
    UnkReflObject* component = nullptr;
    {
        const char* first;
        const char* second;
        if (fn_71010c0270(L, "ss", &first, &second)) {
            if (void* entity = FindByName(g_unk_7101d47310, first)) {
                CStrId id(second, true);
                component = fn_710023341c(entity, id);
                if (component)
                    component = fn_710009d50c(component->vf_0x0(), fn_710024e3c8()) ? component : nullptr;
            }
        }
    }
    if (component) {
        UnkPropBag bag;
        fn_7101043f74(reinterpret_cast<UnkCameraCtrl*>(component), &bag);
        return fn_71010c0b40(L, "t", &bag);
    }
    return 0;
}

// A message string id (copied from another, released in two steps).
struct UnkMlogText_b0164 : base::global::UnkStrIdBase {
    __attribute__((always_inline)) UnkMlogText_b0164(const CStrId& other) : UnkStrIdBase(other) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    __attribute__((always_inline)) ~UnkMlogText_b0164() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkMlogMessage_b0164 {
    u32 kind;
    UnkMlogText_b0164 text;
    base::UnkVector<base::UnkStrIdMember> args;

    __attribute__((always_inline)) UnkMlogMessage_b0164(const CStrId& t) : kind(0), text(t), args(0xc) {}
    __attribute__((always_inline)) ~UnkMlogMessage_b0164() { args.Free(); }
};

struct UnkMlogMessage_b42a1;
void fn_7100e00cc0(UnkGameManager* self, u8 flag);

s32 fn_7101081c50(lua_State* L) {
    u8 flag;
    if (fn_71010c0270(L, "b", &flag)) {
        fn_7100e00cc0(g_unk_7101d47310, flag);
        if (!flag) {
            UnkMlogMessage_b0164 message(CStrId("#MLOG_COOLDOWN_EVENT", true));
            fn_7100456b7c(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x2128,
                          reinterpret_cast<const UnkMlogMessage_b42a1*>(&message));
        }
    }
    return 0;
}

struct UnkComponent_71011199cc;
struct UnkList_71011199cc;
void fn_710111b230(UnkComponent_71011199cc* self, UnkList_71011199cc* out);

s32 fn_7101084b60(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    void* entity = FindByName(g_unk_7101d47310, name);
    if (!entity)
        return 1;
    void* component = fn_71007c04f0(entity);
    if (!component)
        return 1;
    UnkList_b0164 list;
    fn_710111b230(static_cast<UnkComponent_71011199cc*>(component), reinterpret_cast<UnkList_71011199cc*>(&list));
    s32 result;
    {
        UnkPropBag bag;
        for (UnkList_b0164::Node* node = list.head; node; node = node->next) {
            CStrId* key = reinterpret_cast<CStrId*>(static_cast<u8*>(node->value) + 0x30);
            unk_prop_bag::fn_710050e4dc(&bag, *key, unk_prop_bag::UseName(reinterpret_cast<unk_prop_bag::UnkName* const&>(key->data)));
        }
        result = fn_71010c0b40(L, "t", &bag);
    }
    return result;
}

struct UnkUserDataOwner;
extern CStrId g_unk_7172a01968[2] __attribute__((visibility("hidden")));

s32 fn_71010798e0(lua_State* L) {
    bool first = false;
    bool second = false;
    UnkUserDataOwner* provider = g_unk_7101d47308;
    if (!provider) {
        provider = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(provider);
        g_unk_7101d47308 = provider;
    }
    if (CBlackboard* blackboard = fn_710120384c(provider)) {
        {
            CStrId id("USER_DATA", true);
            if (CBlackboard::CSection* section = fn_7101047270(blackboard, id))
                fn_710006d11c(section, &g_unk_7172a01968[0], &first);
        }
        {
            CStrId id("USER_DATA", true);
            if (CBlackboard::CSection* section = fn_7101047270(blackboard, id))
                fn_710006d11c(section, &g_unk_7172a01968[1], &second);
        }
    }
    UnkPropBag bag;
    unk_prop_bag::fn_710050f4ac(&bag, g_unk_7172a01968[0], first);
    unk_prop_bag::fn_710050f4ac(&bag, g_unk_7172a01968[1], second);
    return fn_71010c0b40(L, "t", &bag);
}

using base::global::CRntString;

struct UnkMode_b0164 {
    u8 pad_0x0[8];
    char name[1];  // 0x8
};

s32 fn_7101082690(lua_State* L) {
    const char* s1 = nullptr;
    const char* s2 = nullptr;
    const char* s3 = nullptr;
    const char* s4 = nullptr;
    s32 count = 1;
    if (fn_71010c0270(L, "ssssi", &s1, &s2, &s3, &s4, &count)) {
        UnkGameManager* manager = g_unk_7101d47310;
        bool in_game = false;
        if (UnkMode_b0164* mode = static_cast<UnkMode_b0164*>(manager->field_0x498))
            in_game = !fn_710007f240(mode->name, "INGAME");
        if (!in_game) {
            manager = g_unk_7101d47310;
            CRntString ingame("INGAME");
            fn_7100deac9c(manager, ingame);
        }
        manager = g_unk_7101d47310;
        CRntString a(s1);
        CRntString b(s2);
        CRntString c(s3);
        CStrId d(s4, true);
        u32 n = count;
        CRntString e(g_unk_710162e2ed);
        // The count goes in as a word (the declaration above has the flag a caller passes as a constant).
        auto start = reinterpret_cast<void (*)(UnkGameManager*, const CRntString&, const CRntString&,
                                               const CRntString&, const CStrId&, u32, const CRntString&,
                                               const CStrId&, u32, void*, f32)>(fn_7100df28d0);
        start(manager, a, b, c, d, n, e, g_unk_7101d470e0, 0, nullptr, 0.0f);
    }
    return 0;
}

// The string data's use count, a union member (no type-based alias info, so the id is reloaded after
// the increment).
struct UnkStringUse_b0164 {
    u8 pad_0x0[0x44];
    union {
        u32 use_count;
    };
};
static inline const CRntString* UseStr(const CStrId& id) {
    ++reinterpret_cast<UnkStringUse_b0164*>(id.data)->use_count;
    return reinterpret_cast<const CRntString*>(reinterpret_cast<char*>(id.data) + 0x10);
}

struct UnkCameraHolder_b0164 {
    u8 pad_0x0[0xc28];
    bool field_0xc28;
    u8 pad_0xc29[3];
    u32 field_0xc2c;
};

s32 fn_710108df00(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        if (UnkCameraHolder_b0164* holder = static_cast<UnkCameraHolder_b0164*>(fn_7100defcec(g_unk_7101d47310))) {
            if (fn_710007f140(UseStr(GameManagerIdTable().field_0x1a90), name)) {
                holder->field_0xc28 = true;
                holder->field_0xc2c = 1;
            } else if (fn_710007f140(UseStr(GameManagerIdTable().field_0x1b18), name)) {
                holder->field_0xc28 = true;
                holder->field_0xc2c = 2;
            } else if (fn_710007f140(UseStr(GameManagerIdTable().field_0xd80), name)) {
                holder->field_0xc28 = true;
                holder->field_0xc2c = 0;
            }
        }
    }
    return 0;
}

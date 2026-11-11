// Unit 2666 Lua bindings: each plays an impact effect on the player: a damage-request-like record
// (fn_71008c0c74) with an effect id, a static id and a direction pair is sent to the player's
// component at +0x1b8 (virtual 0x188). Names and layouts are placeholders.
#include <string.h>

#include "base/UnkRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/file/UnkFileDevice.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU1643.h"
#include "game/UnkTU2201.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2676.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU2685.h"
#include "game/UnkTU2728.h"
#include "game/UnkTU315.h"
#include "game/UnkTU426.h"
#include "game/UnkTU785.h"
#include "game/lua_lib.h"
#include "game/msapi/res/CRenderRes.h"
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
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStrId;

struct lua_State;
struct UnkPlayer;
struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;

UnkPlayer* fn_7100dec788(UnkGameManager* self);

extern CStrId g_unk_71726dda50;

struct UnkImpactRequest_b0163 {
    u8 pad_0x0[0x18];
    bool field_0x18;
    u8 pad_0x19[0x30];
    bool field_0x49;
    u8 pad_0x4a[2];
    f32 dir_x;  // 0x4c
    f32 dir_y;  // 0x50
    u8 pad_0x54[0x4c];
    u8 effect_raw[8];  // 0xa0: a CStrId
    u8 pad_0xa8[0x10];
    u8 id_raw[8];  // 0xb8: a CStrId
    u8 pad_0xc0[0x40];

    UnkStrId& effect() { return *reinterpret_cast<UnkStrId*>(effect_raw); }
    CStrId& id() { return *reinterpret_cast<CStrId*>(id_raw); }

    UnkImpactRequest_b0163() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkImpactRequest_b0163() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};

struct UnkPlayerTarget_b0163 {
    void** vtable;
    void vf_0x188(UnkImpactRequest_b0163* request) {
        reinterpret_cast<void (*)(UnkPlayerTarget_b0163*, UnkImpactRequest_b0163*)>(vtable[0x188 / 8])(this,
                                                                                                     request);
    }
};

struct UnkPlayerImpact_b0163 {
    u8 pad_0x0[0x1b8];
    UnkPlayerTarget_b0163* target;  // 0x1b8
};

#define IMPACT_BODY(ID, NAME, DX, DY)                                                            \
    UnkPlayerImpact_b0163* player = reinterpret_cast<UnkPlayerImpact_b0163*>(                    \
        fn_7100dec788(g_unk_7101d47310));                                                        \
    if (player->target) {                                                                        \
        UnkImpactRequest_b0163 request;                                                          \
        request.field_0x18 = true;                                                               \
        request.field_0x49 = true;                                                               \
        request.dir_x = DX;                                                                      \
        request.dir_y = DY;                                                                      \
        request.id() = ID;                                                                         \
        request.effect() = UnkStrId(NAME, true);                                                     \
        player->target->vf_0x188(&request);                                                      \
    }                                                                                            \
    return 0;

s32 fn_7101096540(lua_State* L) {
    IMPACT_BODY(g_unk_71726dda50, "emmywave_impact", 1.0f, 1.0f)
}

s32 fn_71010963e0(lua_State* L) {
    IMPACT_BODY(g_unk_71726dda50, "emmywave_impact", -1.0f, 1.0f)
}

s32 fn_7101096800(lua_State* L) {
    IMPACT_BODY(*reinterpret_cast<CStrId*>(&g_unk_71726dbf68), "frozen_impact", 1.0f, 1.0f)
}

s32 fn_71010966a0(lua_State* L) {
    IMPACT_BODY(*reinterpret_cast<CStrId*>(&g_unk_71726dbf68), "frozen_impact", -1.0f, 1.0f)
}

#include <new>

#include "base/global/CFilePathStrId.h"

using base::global::CFilePathStrId;

struct UnkMaskedId {
    union {
        CStrId id;
    };
    u8 pad_0x8[0x18];
    UnkMaskedId();
    explicit UnkMaskedId(u32 v);
    ~UnkMaskedId() { id.~CStrId(); }
};
void fn_7100c93aec(UnkMaskedId* self, u32 v);
inline UnkMaskedId::UnkMaskedId() { fn_7100c939dc(this); }
inline UnkMaskedId::UnkMaskedId(u32 v) { fn_7100c93aec(this, v); }
struct UnkPackManager_b0163;
struct UnkPackRefTarget_b0163 {
    void** vtable;
    s32 ref_count;  // 0x8
};
struct UnkPackRef_b0163 {
    UnkPackRefTarget_b0163* ptr;
    ~UnkPackRef_b0163() {
        if (UnkPackRefTarget_b0163* p = ptr) {
            __atomic_fetch_sub(&p->ref_count, 1, __ATOMIC_SEQ_CST);
            if (__atomic_load_n(&p->ref_count, __ATOMIC_SEQ_CST) == 0)
                reinterpret_cast<void (*)(UnkPackRefTarget_b0163*)>(p->vtable[0x10 / 8])(p);
        }
    }
};
struct UnkPackLoader_b0163;

UnkPackRef_b0163 fn_7101197008(UnkPackManager_b0163* self, const CFilePathStrId& path, const UnkMaskedId& id,
                               bool flag);
void fn_7101192ae8(UnkPackLoader_b0163* self);
extern void* g_unk_7101d47330;

s32 fn_7101088470(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        {
            UnkPackManager_b0163* manager = static_cast<UnkPackManager_b0163*>(g_unk_7101d47330);
            union Slot {
                Slot() {}
                ~Slot() {}
                UnkPackRef_b0163 ref;
            } slot;
            CFilePathStrId path(name, true);
            UnkMaskedId id;
            new (&slot.ref) UnkPackRef_b0163(fn_7101197008(manager, path, id, false));
            slot.ref.~UnkPackRef_b0163();
        }
        UnkPackLoader_b0163* loader = static_cast<UnkPackLoader_b0163*>(g_unk_7172758b20);
        if (!loader) {
            loader = static_cast<UnkPackLoader_b0163*>(fn_71000839a0(0x40, 0x3f));
            fn_7101192628(loader);
            g_unk_7172758b20 = loader;
        }
        fn_7101192ae8(loader);
    }
    return 0;
}

s32 fn_7101088a60(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        {
            UnkPackManager_b0163* manager = static_cast<UnkPackManager_b0163*>(g_unk_7101d47330);
            union Slot {
                Slot() {}
                ~Slot() {}
                UnkPackRef_b0163 ref;
            } slot;
            CFilePathStrId path(name, true);
            UnkMaskedId id;
            new (&slot.ref) UnkPackRef_b0163(fn_7101197008(manager, path, id, false));
            slot.ref.~UnkPackRef_b0163();
        }
        UnkPackLoader_b0163* loader = static_cast<UnkPackLoader_b0163*>(g_unk_7172758b20);
        if (!loader) {
            loader = static_cast<UnkPackLoader_b0163*>(fn_71000839a0(0x40, 0x3f));
            fn_7101192628(loader);
            g_unk_7172758b20 = loader;
        }
        fn_7101192ae8(loader);
    }
    return 0;
}

void fn_7100b5fd6c(void* object, base::global::UnkStringData** name, void* block, bool a, bool b);

struct UnkGuiEntry_b0163 {
    u8 pad_0x0[0x10];
    void* composition;  // 0x10
};

s32 fn_7101086800(lua_State* L) {
    bool result = false;
    if (fn_7100deac84(g_unk_7101d47310)) {
        const char* first = nullptr;
        u8 flag;
        const char* second = nullptr;
        if (fn_71010c0270(L, "sbs", &first, &flag, &second)) {
            UnkGuiEntry_b0163* entry = static_cast<UnkGuiEntry_b0163*>(g_unk_7101d47310->field_0x2258);
            void* object = entry ? entry->composition : nullptr;
            CStrId a(first, true);
            CStrId b(second, true);
            auto show = reinterpret_cast<bool (*)(void*, base::global::UnkStringData**, void*, bool, bool)>(
                fn_7100b5fd6c);
            result = show(object, &a.data, &b, true, false);
        }
    }
    return fn_71010c0b40(L, "b", result);
}

extern base::file::UnkFileDevice* g_unk_7101d4d828;
struct UnkSoundMgr;
extern UnkSoundMgr* g_unk_7101d4d820;
bool fn_7100089864(base::file::UnkFileDevice* self, const CFilePathStrId* path);
void fn_710008987c(base::file::UnkFileDevice* self, const CFilePathStrId* path, u32 flags);
void fn_7100107b74(UnkSoundMgr* self, const CFilePathStrId* path);
void fn_7100108c94(UnkSoundMgr* self, const CFilePathStrId* path);

static inline base::file::UnkFileDevice* FileDevice_b0163() {
    if (!g_unk_7101d4d828) {
        auto* device = static_cast<base::file::UnkFileDevice*>(fn_71000839a0(0x1348, 0x3f));
        fn_71000893a8(device);
        g_unk_7101d4d828 = device;
    }
    return g_unk_7101d4d828;
}

s32 fn_7101092e90(lua_State* L) {
    const char* name;
    bool result = false;
    if (fn_71010c0270(L, "s", &name)) {
        base::file::UnkFileDevice* device = FileDevice_b0163();
        bool loaded;
        {
            CFilePathStrId id(name, true);
            loaded = fn_7100089864(device, &id);
        }
        UnkSoundMgr* sound = g_unk_7101d4d820;
        CFilePathStrId id(name, true);
        auto request = reinterpret_cast<bool (*)(UnkSoundMgr*, const CFilePathStrId*)>(fn_7100107b74);
        result = loaded & request(sound, &id);
    }
    return fn_71010c0b40(L, "b", result);
}

s32 fn_7101092ff0(lua_State* L) {
    const char* name;
    bool flag;
    bool result = false;
    if (fn_71010c0270(L, "sb", &name, &flag)) {
        base::file::UnkFileDevice* device = FileDevice_b0163();
        {
            CFilePathStrId id(name, true);
            auto load = reinterpret_cast<bool (*)(base::file::UnkFileDevice*, const CFilePathStrId*, u32)>(
                fn_710008987c);
            result = load(device, &id, flag);
        }
        UnkSoundMgr* sound = g_unk_7101d4d820;
        CFilePathStrId id(name, true);
        fn_7100108c94(sound, &id);
    }
    return fn_71010c0b40(L, "b", result);
}

int lua_type(lua_State* L, int idx);
void* fn_7100deab70(UnkGameManager* self, const CRntString& name);
void fn_7100deac9c(UnkGameManager* self, const CRntString& name);
void fn_7100da62a4(void* self, const CFilePathStrId* path, bool value);

s32 fn_7101088320(lua_State* L) {
    const char* name;
    if (fn_71010c0ad0(L, 0, 1, "s", &name)) {
        bool value = true;
        if (lua_type(L, 2) == 1)
            value = fn_71010a4900(L, 2) != 0;
        void* loading = fn_7100deab70(g_unk_7101d47310, CRntString("LOADING"));
        if (loading) {
            CFilePathStrId id(name, true);
            fn_7100da62a4(loading, &id, value);
        }
        UnkGameManager* manager = g_unk_7101d47310;
        if (manager->field_0x498 != loading)
            fn_7100deac9c(manager, CRntString("LOADING"));
    }
    return 0;
}

void fn_7100df2a6c(UnkGameManager* self, const CRntString& a, const CRntString& b, const CRntString& c,
                   const CStrId& d, s32 value);

struct UnkModeHolder_b0163 {
    u8 pad_0x0[8];
    char name[1];  // 0x8
};

s32 fn_7101082860(lua_State* L) {
    const char* first = nullptr;
    const char* second = nullptr;
    const char* third = nullptr;
    const char* fourth = nullptr;
    s32 value = 1;
    if (fn_71010c0270(L, "ssssi", &first, &second, &third, &fourth, &value)) {
        UnkGameManager* manager = g_unk_7101d47310;
        UnkModeHolder_b0163* mode = static_cast<UnkModeHolder_b0163*>(manager->field_0x498);
        if (!mode || fn_710007f240(mode->name, "PROLOGUE")) {
            manager = g_unk_7101d47310;
            fn_7100deac9c(manager, CRntString("PROLOGUE"));
        }
        manager = g_unk_7101d47310;
        CRntString a(first);
        CRntString b(second);
        CRntString c(third);
        CStrId d(fourth, true);
        fn_7100df2a6c(manager, a, b, c, d, value);
    }
    return 0;
}

struct UnkLoadingMask {
    u32 bits;
    UnkLoadingMask() : bits(0) {}
    UnkLoadingMask(u32 b) : bits(b) {}
    UnkLoadingMask(const UnkLoadingMask& o) : bits(o.bits) {}
};
extern void* g_unk_71726bc560;
s32 fn_7101097578(const char* name);

struct UnkMaskStaticZero_b0163 {
    u32 value;
    UnkMaskStaticZero_b0163() : value(0) {}
};

inline const UnkMaskStaticZero_b0163& MaskNone_b0163() {
    static UnkMaskStaticZero_b0163 s_value asm("g_unk_71726c4090");
    return s_value;
}

inline u32 MaskAll_b0163() {
    static u32 s_mask asm("g_unk_71726c4080") = ~MaskNone_b0163().value & 0x1ff;
    return s_mask;
}

s32 fn_7101091420(lua_State* L) {
    bool flag = false;
    const char* name = nullptr;
    if (fn_71010c0ad0(L, 0, 1, "b|s", &flag, &name)) {
        u32 bits = name ? 1u << fn_7101097578(name) : MaskAll_b0163();
        void* manager = g_unk_71726bc560;
        if (!manager) {
            manager = fn_71000839a0(0xf18, 0x3f);
            fn_7100fd3804(manager);
            g_unk_71726bc560 = manager;
        }
        fn_7100fd7b44(manager, UnkLoadingMask(flag ? 4 : 2), UnkLoadingMask(bits), -1.0f);
    }
    return 0;
}

// A typed value as the section setter takes it (as in src/game/CAIComponent/CAIComponent_StoreFloat_b49.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);
CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);

struct UnkStrIdHandle {
    union {
        CStrId id;
    };
    void* field_0x8;
    UnkStrIdHandle() {}
    ~UnkStrIdHandle() { id.~CStrId(); }
};
void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);

s32 fn_7101081f90(lua_State* L) {
    UnkTypedValue v;
    const char* name;
    bool value;
    if (fn_71010c0270(L, "sb", &name, &value)) {
        UnkGameManager* manager = g_unk_7101d47310;
        UnkGameManagerSub_0x608* owner = manager->field_0x608;
        CBlackboard* blackboard = &manager->field_0x968;
        UnkStrIdHandle key;
        {
            CStrId id(name, true);
            fn_7100ae986c(&key, id);
        }
        CStrId section_id = fn_71011a2fbc(owner);
        if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section_id)) {
            if (!manager->field_0x968.field_0xe8) {
                v.type = fn_710000d228();
                v.value = &value;
                fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&key), &v);
            }
        }
    }
    return 0;
}

s32 fn_7101082100(lua_State* L) {
    const char* key_name;
    UnkTypedValue v;
    const char* section_name;
    bool value = false;
    if (fn_71010c0270(L, "ssb", &section_name, &key_name, &value)) {
        UnkGameManager* manager = g_unk_7101d47310;
        CBlackboard* blackboard = &manager->field_0x968;
        UnkStrIdHandle key;
        {
            CStrId id(key_name, true);
            fn_7100ae986c(&key, id);
        }
        CStrId section_id(section_name, true);
        if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section_id)) {
            if (!manager->field_0x968.field_0xe8) {
                v.type = fn_710000d228();
                v.value = &value;
                fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&key), &v);
            }
        }
    }
    return 0;
}


struct UnkGun_ovb0091;
struct UnkPlayer;
struct UnkVec {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};
UnkPlayer* fn_7100dec788(UnkGameManager*);
void fn_7100352b14(void* body_field, UnkActor* owner, u32 flag);
bool fn_710083e250(UnkGun_ovb0091* self, f32 value, bool flag);
void fn_7100df6d3c(UnkGameManager* self, void* settings, f32 a, f32 b);
void fn_7100e038b0(UnkGameManager* dispatcher, void* key, UnkVec* vec, u64 arg);
struct UnkGuiHolder_b0159;
UnkGuiHolder_b0159* fn_7101126814(const CStrId* id);

struct UnkSlomoSettings {
    u8 pad_0x0[0x10];
    u8 body[0x34];
    union {
        s32 ref_count;  // 0x44
    };
};
struct UnkSlomoRef_b0163 {
    void* field_0x0;
    UnkSlomoSettings* settings;  // 0x8
};
extern UnkSlomoRef_b0163 g_unk_7172a01958 __attribute__((visibility("hidden")));

struct UnkHolderTarget_b0163 {
    u8 pad_0x0[0x10];
    UnkActor* field_0x10;
};
struct UnkGuiHolder_b0159 {
    u8 pad_0x0[0x40];
    UnkHolderTarget_b0163* field_0x40;
};
struct UnkGunState_b0163 {
    u8 pad_0x0[0x134];
    s32 field_0x134;
};
struct UnkPlayerView_b0163 {
    u8 pad_0x0[0x2a8];
    UnkGunState_b0163* field_0x2a8;
    void* field_0x2b0;
};

s32 fn_7101094230(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    UnkPlayerView_b0163* player = reinterpret_cast<UnkPlayerView_b0163*>(fn_7100dec788(g_unk_7101d47310));
    if (player) {
        void* body = player->field_0x2b0;
        if (body) {
            UnkGuiHolder_b0159* holder;
            {
                CStrId id(name, true);
                holder = fn_7101126814(&id);
            }
            if (holder && holder->field_0x40 && holder->field_0x40->field_0x10)
                fn_7100352b14(body, holder->field_0x40->field_0x10, 0);
            if (UnkGunState_b0163* gun = player->field_0x2a8) {
                if (gun->field_0x134 != 2)
                    fn_710083e250(reinterpret_cast<UnkGun_ovb0091*>(gun), 0.8f, false);
            }
        }
    }
    UnkSlomoSettings* settings = g_unk_7172a01958.settings;
    UnkGameManager* manager = g_unk_7101d47310;
    ++settings->ref_count;
    fn_7100df6d3c(manager, settings->body, 0.01f, 0.33f);
    UnkVec direction;
    direction.field_0x0 = 0xbf800000;
    direction.field_0x4 = 0;
    direction.field_0x8 = 0;
    fn_7100e038b0(g_unk_7101d47310, &g_unk_7172a01958, &direction, 0);
    return 0;
}

extern void* g_unk_7101d47360;
void fn_7100ee1f34(void* self);
// A bit mask passed by value (as src/game/CAnimatedCameraCtrl/CAnimatedCameraCtrl_Lifecycle_ovb0101.cpp).
struct UnkCamMask_b0101 {
    u32 bits;
    UnkCamMask_b0101() : bits(0) {}
    UnkCamMask_b0101(u32 b) : bits(b) {}
    UnkCamMask_b0101(const UnkCamMask_b0101& o) : bits(o.bits) {}
};
void fn_7100eed3a4(void* manager, const CStrId* id, UnkCamMask_b0101 mask, bool flag);
void fn_7100eed4b0(void* manager, const CStrId* id, UnkCamMask_b0101 mask, bool enable, bool instant);

struct UnkSubareaStaticZero_b0163 {
    u32 value;
    UnkSubareaStaticZero_b0163() : value(0) {}
};

inline const UnkSubareaStaticZero_b0163& UnkSubareaStaticZero() {
    static UnkSubareaStaticZero_b0163 s_value asm("g_unk_7172719980");
    return s_value;
}

inline u32 UnkSubareaAllMask() {
    static u32 s_mask asm("g_unk_7172719970") = ~UnkSubareaStaticZero().value & 0x1ff;
    return s_mask;
}

static inline void* SubareaManager_b0163() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = manager;
    }
    return manager;
}

s32 fn_7101083e40(lua_State* L) {
    const char* name;
    bool enable;
    if (fn_71010c0270(L, "sb", &name, &enable)) {
        void* manager = SubareaManager_b0163();
        CStrId id(name, true);
        fn_7100eed3a4(manager, &id, UnkCamMask_b0101(UnkSubareaAllMask()), enable);
    }
    return 0;
}

s32 fn_7101083c30(lua_State* L) {
    const char* name;
    bool enable;
    bool instant;
    if (fn_71010c0270(L, "sbb", &name, &enable, &instant)) {
        void* manager = SubareaManager_b0163();
        CStrId id(name, true);
        fn_7100eed4b0(manager, &id, UnkCamMask_b0101(UnkSubareaAllMask()), enable, instant);
    }
    return 0;
}


struct UnkStrPair_0x10 {
    void* a;
    void* b;
};

struct UnkRcTarget_b0163 {
    void** vtable;
    s32 ref_count;  // 0x8
};
struct UnkRc_b0163 {
    UnkRcTarget_b0163* ptr;
    UnkRc_b0163() : ptr(nullptr) {}
    UnkRc_b0163(UnkRc_b0163&& o) : ptr(o.ptr) { o.ptr = nullptr; }
    UnkRc_b0163& operator=(const UnkRc_b0163& other) {
        UnkRcTarget_b0163* p = other.ptr;
        if (ptr != p) {
            if (p)
                __atomic_fetch_add(&p->ref_count, 1, __ATOMIC_SEQ_CST);
            if (ptr)
                Release(ptr);
            ptr = p;
        }
        return *this;
    }
    ~UnkRc_b0163() {
        if (ptr)
            Release(ptr);
    }
    static void Release(UnkRcTarget_b0163* p) {
        __atomic_fetch_sub(&p->ref_count, 1, __ATOMIC_SEQ_CST);
        if (__atomic_load_n(&p->ref_count, __ATOMIC_SEQ_CST) == 0)
            reinterpret_cast<void (*)(UnkRcTarget_b0163*)>(p->vtable[0x10 / 8])(p);
    }
};
struct UnkRcBinding_b0163 {
    void* field_0x0;
    UnkRc_b0163* field_0x8;
};

UnkRc_b0163 fn_710021cc9c(UnkPlayer* self, const UnkStrPair_0x10* path, u32 a, bool b);
void* fn_710021dee4();

s32 fn_7101095780(lua_State* L) {
    UnkRc_b0163 ref;
    const char* name;
    bool first = false;
    bool second = false;
    if (fn_71010c0270(L, "ibb", &name, &first, &second)) {
        UnkPlayer* player = g_unk_716e554378;
        union Slot {
            Slot() {}
            ~Slot() {}
            UnkRc_b0163 made;
        } slot;
        UnkStrPair_0x10 pair;
        fn_710008d680(&pair, name);
        new (&slot.made) UnkRc_b0163(fn_710021cc9c(player, &pair, first ^ 1, second));
        ref = slot.made;
        slot.made.~UnkRc_b0163();
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&pair));
    }
    UnkRcBinding_b0163 binding = {fn_710021dee4(), &ref};
    return fn_7100fbd320(L, &binding, 0, 2);
}

inline void* FindByName_b0163(UnkGameManager* manager, const char* name) {
    u32 kind_value = g_unk_71727bebb8;
    u64 key = fn_7100001570(name, strlen(name));
    alignas(8) u32 kind = kind_value;
    return fn_7100deba8c(manager, &key, &kind);
}

struct UnkCommand_b0163 {
    u8 pad_0x0[0x38];
    base::global::CRntFile file;  // 0x38
    u8 pad_0x70[0x10];
};
void fn_71010c7284(UnkCommand_b0163* self, const CStrId* id, const CRntString* text, bool flag, void* extra);
void fn_71010bf250(lua_State* L, s32 kind, const char* name, base::global::CRntFile* file);
void fn_7100233a38(void* object, UnkCommand_b0163* command);

s32 fn_710107d380(lua_State* L) {
    const char* object_name;
    const char* id_name;
    const char* text_name;
    bool flag = false;
    const char* command_name;
    fn_71010c0ad0(L, 0, 1, "sssbs", &object_name, &id_name, &text_name, &flag, &command_name);
    void* object = FindByName_b0163(g_unk_7101d47310, object_name);
    if (!object)
        return 0;
    union Slot {
        Slot() {}
        ~Slot() {}
        UnkCommand_b0163 command;
    } slot;
    {
        CStrId id(id_name, true);
        CRntString text(text_name);
        fn_71010c7284(&slot.command, &id, &text, flag, nullptr);
    }
    fn_71010bf250(L, 5, command_name, &slot.command.file);
    fn_710007c00c(&slot.command.file, 0, 0);
    fn_7100233a38(object, &slot.command);
    fn_71010c7328(&slot.command);
    return 0;
}

s32 fn_7101082270(lua_State* L) {
    const char* name;
    bool value = false;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    UnkGameManager* manager = g_unk_7101d47310;
    UnkGameManagerSub_0x608* owner = manager->field_0x608;
    CBlackboard* blackboard = &manager->field_0x968;
    UnkStrIdHandle key;
    {
        CStrId id(name, true);
        fn_7100ae986c(&key, id);
    }
    {
        CStrId section_id = fn_71011a2fbc(owner);
        if (CBlackboard::CSection* s = fn_7101047270(blackboard, section_id))
            fn_710006d11c(s, &key.id, &value);
    }
    return fn_71010c0b40(L, "b", value);
}

s32 fn_7101088bc0(lua_State* L) {
    const char* name;
    s32 frames;
    if (fn_71010c0270(L, "si", &name, &frames)) {
        {
            UnkPackManager_b0163* manager = static_cast<UnkPackManager_b0163*>(g_unk_7101d47330);
            union Slot {
                Slot() {}
                ~Slot() {}
                UnkPackRef_b0163 ref;
            } slot;
            CFilePathStrId path(name, true);
            UnkMaskedId id(0u);
            bool flag = (f32)frames / 30.0f != 0.0f;
            new (&slot.ref) UnkPackRef_b0163(fn_7101197008(manager, path, id, flag));
            slot.ref.~UnkPackRef_b0163();
        }
        UnkPackLoader_b0163* loader = static_cast<UnkPackLoader_b0163*>(g_unk_7172758b20);
        if (!loader) {
            loader = static_cast<UnkPackLoader_b0163*>(fn_71000839a0(0x40, 0x3f));
            fn_7101192628(loader);
            g_unk_7172758b20 = loader;
        }
        fn_7101192ae8(loader);
    }
    return 0;
}


struct UnkResourceManager;
bool fn_71002f70b0(UnkResourceManager* self, const CStrId* first, const CStrId* second);
void fn_7101097030();

struct UnkEndGameNode_b0163 {
    s64 field_0x0;
    void* field_0x8;
    void (*field_0x10)();
    UnkEndGameNode_b0163() : field_0x0(-1), field_0x8(this), field_0x10(fn_7101097030) {}
};

struct UnkManagerView_b0163 {
    u8 pad_0x0[0x5b8];
    void* field_0x5b8;
};

inline UnkEndGameNode_b0163& EndGameNode_b0163() {
    static UnkEndGameNode_b0163 s_node asm("g_unk_7172a018f0");
    return s_node;
}

bool fn_7101096d68(s32 first, s32 second) {
    UnkEndGameNode_b0163& s_node = EndGameNode_b0163();
    *reinterpret_cast<u32*>(&s_node.field_0x0) = first;
    *(reinterpret_cast<u32*>(&s_node.field_0x0) + 1) = second;
    bool result;
    {
        UnkResourceManager* manager = g_unk_71726ae8a8;
        CStrId a("pt_official", true);
        CStrId b("endgame", true);
        result = fn_71002f70b0(manager, &a, &b);
    }
    reinterpret_cast<UnkManagerView_b0163*>(g_unk_7101d47310)->field_0x5b8 = &s_node.field_0x8;
    return result;
}

extern UnkRegistry_0x70* g_unk_7101d4b5a0;
bool fn_71000878f0(UnkRegistry_0x70* self, const UnkStrPair_0x10* path, u32 flags, void* out);
void fn_7100df9728(UnkGameManager* self);
void fn_7100133020();

struct UnkPairNode_b0163 {
    UnkStrPair_0x10 pair;
    UnkPairNode_b0163* next;  // 0x10
};

struct UnkPairList_b0163 {
    char* vtable;
    u32 count;
    UnkPairNode_b0163* head;
    UnkPairNode_b0163* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    UnkPairList_b0163()
        : vtable(reinterpret_cast<char*>(g_unk_7101a202e0) + 0x10), count(0), head(nullptr), tail(nullptr), allocator(fn_7100083990()),
          flags(0x13) {}
    ~UnkPairList_b0163() {
        vtable = reinterpret_cast<char*>(g_unk_7101a202e0) + 0x10;
        for (UnkPairNode_b0163* node = head; node;) {
            UnkPairNode_b0163* next = node->next;
            fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(node));
            allocator->vf_0x48(node);
            node = next;
        }
    }
};

struct UnkRegistryNotify_b0163 {
    void** vtable;
    void Notify(UnkPairNode_b0163* node, bool flag) {
        reinterpret_cast<void (*)(UnkRegistryNotify_b0163*, UnkPairNode_b0163*, bool)>(vtable[0x58 / 8])(this, node,
                                                                                                         flag);
    }
};

static inline UnkRegistry_0x70* Registry_b0163() {
    UnkRegistry_0x70* reg = g_unk_7101d4b5a0;
    if (!reg) {
        reg = static_cast<UnkRegistry_0x70*>(fn_71000839a0(0x70, 0x3f));
        fn_7100087388(reg);
        g_unk_7101d4b5a0 = reg;
    }
    return reg;
}

s32 fn_7101079770(lua_State*) {
    UnkPairList_b0163 list;
    {
        UnkRegistry_0x70* registry = Registry_b0163();
        UnkStrPair_0x10 prefix;
        fn_710008d680(&prefix, "savedata:");
        fn_71000878f0(registry, &prefix, 0, &list);
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&prefix));
    }
    for (UnkPairNode_b0163* node = list.head; node; node = node->next)
        reinterpret_cast<UnkRegistryNotify_b0163*>(Registry_b0163())->Notify(node, true);
    fn_7100df9728(g_unk_7101d47310);
    fn_7100000790("slots.SetAllDirty", g_unk_710162e2ed);
    reinterpret_cast<u8*>(g_unk_7101d47310)[0x1aa8] = 1;
    fn_7100133020();
    return 0;
}

// Starts of two states. fn_7100b2de48: sets a parameter (by hash) to 1 on the global object at 0x7101d47300,
// creates the object at 0x2c0 when missing and makes it current (0x280), resets three settings
// of the global object, and unless the game manager's object at 0x498 is already in place
// (vf_0x68), sets it up (vf_0x70) and records two flags, applying each (fn_71011cdc30) that is
// set. Finally the global at 0x7101d47340 (created on first use) gets 0xfd.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::reflection::CClass;

struct UnkParamTarget;
struct UnkWatchSet {
    u8 unk_0x0[0x80];
};

// A child of a composition: a count behind the pointer at 0x30, and through vf_0x1f8 an object
// whose watch set sits at 0x18.
struct UnkCountRef {
    s32 count;
};
struct UnkWatched {
    u8 unk_0x0[0x18];
    UnkWatchSet set;
};
struct UnkGuiChild {
    void** vtable;
    u8 unk_0x8[0x28];
    UnkCountRef** field_0x30;

    UnkWatched* Watched() { return reinterpret_cast<UnkWatched* (*)(UnkGuiChild*)>(vtable[0x1f8 / 8])(this); }
};

// A GUI item. Only vf_0x0 (the reflected type) and vf_0x1e8 (an event by name) are used.
struct UnkComposition {
    void** vtable;
    u8 unk_0x8[0xac];
    bool field_0xb4;
    bool flag;

    const CClass* Type() { return reinterpret_cast<const CClass* (*)(UnkComposition*)>(vtable[0])(this); }
    void SendEvent(const char* name, bool flag) {
        reinterpret_cast<void (*)(UnkComposition*, const char*, bool)>(vtable[0x1e8 / 8])(this, name, flag);
    }
};
struct UnkFlagRef {
    u8 unk_0x0[0x10];
    UnkComposition* holder;
};
struct UnkPlacedObj {
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
    virtual bool vf_0x68();
    virtual void vf_0x70();
};
struct UnkGameManager {
    u8 unk_0x0[0x498];
    UnkPlacedObj* placed;
    u8 unk_0x4a0[0x1d88];
    struct UnkWorld* field_0x2228;
    u8 unk_0x2230[0x28];
    UnkFlagRef* field_0x2258;
    u8 unk_0x2260[0x10];
    UnkFlagRef* field_0x2270;
};
struct UnkObject {
    void** vtable;

    const CClass* Type() { return reinterpret_cast<const CClass* (*)(UnkObject*)>(vtable[0])(this); }
};
struct UnkObjRef {
    u8 unk_0x0[0x10];
    UnkObject* obj;
};
struct UnkLogFlagsA {
    u8 unk_0x0[0x320];
    bool field_0x320;
};
struct UnkLogFlagsB {
    u8 unk_0x0[0x262];
    bool field_0x262;
    bool field_0x263;
};
struct UnkMissionLog : UnkObject {
    u8 unk_0x8[0x230];
    UnkLogFlagsB* field_0x238;
    UnkLogFlagsA* field_0x240;

    // Read by address arithmetic: the original keeps the paths where the log is null.
    UnkLogFlagsB* B() { return *reinterpret_cast<UnkLogFlagsB**>(reinterpret_cast<u64>(this) + 0x238); }
    UnkLogFlagsA* A() { return *reinterpret_cast<UnkLogFlagsA**>(reinterpret_cast<u64>(this) + 0x240); }
};
struct UnkSoundGroup;
struct UnkSoundHolder {
    UnkSoundGroup* group;
};

struct UnkStateOwner {
    u8 unk_0x0[0x248];
    UnkWatchSet* watch;
    u8 unk_0x250[0x10];
    UnkWatchSet* field_0x260;
    u8 unk_0x268[0x10];
    bool field_0x278;
    bool field_0x279;
    u8 unk_0x27a[0x6];
    UnkComposition* current;
    UnkComposition* field_0x288;
    UnkComposition* field_0x290;
    u8 unk_0x298[0x8];
    UnkComposition* field_0x2a0;
    UnkComposition* field_0x2a8;
    UnkComposition* field_0x2b0;
    u8 unk_0x2b8[0x8];
    UnkComposition* field_0x2c0;
    UnkComposition* field_0x2c8;
    UnkComposition* field_0x2d0;
    f32 field_0x2d8;
    bool field_0x2dc;
    bool field_0x2dd;
    bool field_0x2de;
    u8 unk_0x2df[0x1];
    s32 field_0x2e0;
    s32 field_0x2e4;
    s32 field_0x2e8;
    u8 unk_0x2ec[0x94];
    struct UnkStateTable* field_0x380;
    u8 unk_0x388[0x108];
    UnkObjRef* field_0x490;

    UnkObject* MissionMenu() const { return field_0x490 ? field_0x490->obj : nullptr; }
};

extern UnkParamTarget* g_unk_7101d47300;
extern UnkGameManager* g_unk_7101d47310;
extern UnkSoundHolder* g_unk_7101d47340;
struct UnkModeFlags {
    u8 unk_0x0[0xb8];
    bool field_0xb8;
};
extern UnkModeFlags* g_unk_71726bc570;
struct UnkStateSink;
struct UnkStateKey;
extern UnkStateSink* g_unk_717275c380;
extern UnkStateKey g_unk_71729f0178;
extern UnkStateKey g_unk_71729f0180;
void fn_7100b2d0a4(UnkStateOwner* self);
void fn_7100fc9d78(UnkStateSink* sink, UnkStateKey* key, void* state);
struct UnkGuiRoot {
    u8 unk_0x0[0x230];
    struct UnkObjRef* field_0x230;
    u8 unk_0x238[0xa70];
    u8 field_0xca8[1];
};
struct UnkGuiKey;
extern UnkGuiRoot* g_unk_717271e8f8;
extern UnkGuiKey g_unk_7172762fd8 __attribute__((visibility("hidden")));
void fn_710002fd10(void* list, UnkGuiKey* key);
void fn_710002ffb4(void* list, UnkGuiKey* key);
void fn_71004f3cf0(UnkWatchSet* set, UnkWatchSet** slot);
void fn_71004f3ea4(UnkWatchSet* set, UnkWatchSet** slot);
const CClass* fn_7100af0954();  // GUI::CAmiiboComposition::refl_GetClass
const CClass* fn_7100b54a10();  // GUI::CMissionLog::refl_GetClass
void fn_7100e9b464(void* target, bool value);
void fn_7100e9b1f8(void* target, bool value);
u64 fn_7100001570(const char* text, u64 length);
void fn_7100066a50(void* text, const char32_t* value);
void fn_7100066e44(void* text);
void fn_7100b2cb44(UnkStateOwner* self);
UnkComposition* fn_71011d98ec(UnkComposition* composition, u64 id);
const CClass* fn_71011e8dfc();  // GUI::CLabel::refl_GetClass
void fn_71011ec468(UnkComposition* label, void* text, bool value);
void fn_71011b6bbc(UnkGuiRoot* root, void* object, bool value);
void fn_7100af1bfc(UnkComposition* composition);
void fn_71011cdce8(UnkComposition* composition, s32 value);
void fn_71011cf2a4(UnkComposition* composition, s32 value);
UnkGuiChild* fn_71011d94d4(UnkComposition* composition, const CStrId& name);
void fn_7100b2c6a0(UnkStateOwner* self);
void fn_7100b2c784(UnkStateOwner* self);
void* fn_71000839a0(u64 size, u32 flags);
void fn_710006cd9c(UnkSoundHolder* holder);
void fn_710006d2a0(UnkSoundGroup* group, u32 id);
void fn_710006d4d0(UnkSoundGroup* group, u32 id);
void fn_7100219294(UnkParamTarget* target, u64 hash, const f32* value, u32 mode);
void fn_7100219cb0(UnkParamTarget* target, bool a, bool b);
void fn_7100219d4c(UnkParamTarget* target, bool a, bool b);
void fn_7100219da4(UnkParamTarget* target, bool a, bool b, u32 mode);
void fn_7100b2cc40(UnkStateOwner* self);
void fn_71011cdc30(UnkComposition* item, bool value);

// The flags are stored as plain bytes: the original reloads the handle after the store.
static inline void StoreFlag(bool* field, bool value) { *reinterpret_cast<u8*>(field) = value; }

static inline UnkComposition* GetHolder(UnkFlagRef* ref) { return ref ? ref->holder : nullptr; }

static inline UnkMissionLog* GetMissionLog(UnkObjRef* ref) {
    UnkObject* obj = ref ? ref->obj : nullptr;
    if (!obj)
        return nullptr;
    return fn_710009d50c(obj->Type(), fn_7100b54a10()) ? static_cast<UnkMissionLog*>(obj) : nullptr;
}

// Sends a sound id to the global sound holder (created on first use).
template <void (*Play)(UnkSoundGroup*, u32) = fn_710006d2a0>
__attribute__((always_inline)) static inline void PlaySound(u32 id) {
    UnkSoundHolder* holder = g_unk_7101d47340;
    if (!holder) {
        holder = static_cast<UnkSoundHolder*>(fn_71000839a0(0x48, 0x3f));
        fn_710006cd9c(holder);
        g_unk_7101d47340 = holder;
    }
    if (holder->group)
        Play(holder->group, id);
}

// Resets three settings of the global object.
__attribute__((always_inline)) static inline void ResetParams() {
    fn_7100219d4c(g_unk_7101d47300, false, false);
    fn_7100219cb0(g_unk_7101d47300, false, false);
    fn_7100219da4(g_unk_7101d47300, true, true, 2);
}

// Records a flag of the game manager and applies it when set.
template <UnkFlagRef* UnkGameManager::*Ref>
__attribute__((always_inline)) static inline void RecordFlag(bool* field) {
    UnkGameManager* manager = g_unk_7101d47310;
    bool flag = (manager->*Ref)->holder->flag;
    StoreFlag(field, flag);
    if (flag)
        fn_71011cdc30(GetHolder(manager->*Ref), false);
}

void fn_7100b2de48(UnkStateOwner* self) {
    f32 value = 1.0f;
    fn_7100219294(g_unk_7101d47300, 0xede73d1953baf654, &value, 2);
    self->field_0x2e4 = 1;
    if (!self->field_0x2c0)
        fn_7100b2cc40(self);
    self->current = self->field_0x2c0;
    ResetParams();
    UnkPlacedObj* placed = g_unk_7101d47310->placed;
    if (!placed->vf_0x68()) {
        placed->vf_0x70();
        RecordFlag<&UnkGameManager::field_0x2258>(&self->field_0x2dd);
        RecordFlag<&UnkGameManager::field_0x2270>(&self->field_0x2de);
    }
    PlaySound(0xfd);
}

// Start of the other state: the object at 0x2c8 (while the global flag at 0xb8 is set) or the
// one at 0x290 becomes current, and once the manager's object is set up it is also handed to
// the global at 0x717275c380.
void fn_7100b2d278(UnkStateOwner* self) {
    if (g_unk_71726bc570->field_0xb8) {
        if (!self->field_0x2c8)
            fn_7100b2c784(self);
        self->current = self->field_0x2c8;
    } else {
        if (!self->field_0x290)
            fn_7100b2c6a0(self);
        self->current = self->field_0x290;
    }
    self->field_0x2e4 = 1;
    ResetParams();
    UnkPlacedObj* placed = g_unk_7101d47310->placed;
    if (!placed->vf_0x68()) {
        placed->vf_0x70();
        fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0178, self->current);
        RecordFlag<&UnkGameManager::field_0x2258>(&self->field_0x2dd);
        RecordFlag<&UnkGameManager::field_0x2270>(&self->field_0x2de);
    }
}

// End of the states: unless the state at 0x2e8 is 3 to 6, the object at 0x2c8 or 0x290 (by
// the same global flag) is kept at 0x288, the current object is handed on unless 0x2dc is set,
// and fn_7100b2d0a4 runs. Two settings of the global object are then set back, and when
// 0x279 is set and the manager's object is in place it is reset (vf_0x70) and the recorded
// flags are undone.
// Undoes the recorded flags once the manager's object is in place again (vf_0x68), after
// resetting it (vf_0x70); only when 0x279 is set.
__attribute__((always_inline)) static inline void UndoFlags(UnkStateOwner* self) {
    if (!self->field_0x279)
        return;
    UnkPlacedObj* placed = g_unk_7101d47310->placed;
    if (placed->vf_0x68()) {
        placed->vf_0x70();
        if (self->field_0x2dd)
            fn_71011cdc30(GetHolder(g_unk_7101d47310->field_0x2258), true);
        if (self->field_0x2de)
            fn_71011cdc30(GetHolder(g_unk_7101d47310->field_0x2270), true);
    }
}

void fn_7100b2d3e8(UnkStateOwner* self) {
    if ((u32)(self->field_0x2e8 - 3) >= 4) {
        self->field_0x288 = !g_unk_71726bc570->field_0xb8 ? self->field_0x290 : self->field_0x2c8;
        if (!self->field_0x2dc)
            fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0180, self->current);
        fn_7100b2d0a4(self);
    }
    fn_7100219d4c(g_unk_7101d47300, true, false);
    fn_7100219cb0(g_unk_7101d47300, true, false);
    UndoFlags(self);
}

// End of the mission log state: the manager's object at 0x2228 is told 0x279; when the mission
// log (the object behind 0x490) has either of its two flags set, both are set to whether the
// state at 0x2e8 is not 1 and its second flag at 0x263 is set. With 0x279 set the owner is
// handed on and the recorded flags are undone; the two settings are set back last.
void fn_7100b2dcac(UnkStateOwner* self) {
    fn_7100e9b464(g_unk_7101d47310->field_0x2228, self->field_0x279);
    UnkMissionLog* log = GetMissionLog(self->field_0x490);
    if (log->A()->field_0x320 || log->B()->field_0x262) {
        bool value = self->field_0x2e8 != 1;
        log->A()->field_0x320 = value;
        log->B()->field_0x262 = value;
        log->B()->field_0x263 = true;
    }
    if (self->field_0x279) {
        fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0180, self);
        UndoFlags(self);
    }
    fn_7100219d4c(g_unk_7101d47300, true, false);
    fn_7100219cb0(g_unk_7101d47300, true, false);
}

// Hides the current menu: "OnHideToGame" in state 1, else "OnHide" (an amiibo composition in
// mode 5 is passed to fn_7100af1bfc first). When its "Content" child holds at least two
// entries and nothing is watched yet, its object is watched at 0x248 and 0x278 is set;
// otherwise fn_71011cdce8 runs on it.
void fn_7100b2d0a4(UnkStateOwner* self) {
    fn_71011cf2a4(self->current, 0);
    if (self->field_0x2e8 == 1) {
        self->current->SendEvent("OnHideToGame", false);
    } else {
        if (self->field_0x2e0 == 5) {
            UnkComposition* composition = self->current;
            fn_7100af1bfc(composition && fn_710009d50c(composition->Type(), fn_7100af0954()) ? composition
                                                                                           : nullptr);
        }
        self->current->SendEvent("OnHide", false);
    }
    fn_710002ffb4(g_unk_717271e8f8->field_0xca8, &g_unk_7172762fd8);
    UnkGuiChild* content = fn_71011d94d4(self->current, CStrId("Content", true));
    if (content && content->field_0x30 && (*content->field_0x30)->count >= 2 && !self->watch) {
        fn_710002fd10(g_unk_717271e8f8->field_0xca8, &g_unk_7172762fd8);
        self->field_0x278 = true;
        UnkWatched* watched = content->Watched();
        if (self->watch) {
            fn_71004f3ea4(self->watch, &self->watch);
            self->watch = nullptr;
        }
        self->watch = &watched->set;
        fn_71004f3cf0(&watched->set, &self->watch);
        return;
    }
    fn_71011cdce8(self->current, 0);
}

// Start of the mission log state: unless already in mode 1 the parameter is set (-1 from mode
// 8, else 1); the flags are recorded as in fn_7100b2de48, the owner is passed to
// fn_71011b6bbc, the settings are reset and the sound global gets 0xfd; then the manager's
// object at 0x2228 is told 0x279, and with 0x279 set the owner is handed on.
void fn_7100b2dad0(UnkStateOwner* self) {
    s32 mode = self->field_0x2e4;
    if (mode != 1) {
        if (mode == 8) {
            f32 value = -1.0f;
            fn_7100219294(g_unk_7101d47300, 0xede73d1953baf654, &value, 2);
        } else {
            f32 value = 1.0f;
            fn_7100219294(g_unk_7101d47300, 0xede73d1953baf654, &value, 2);
        }
        self->field_0x2e4 = 1;
    }
    UnkPlacedObj* placed = g_unk_7101d47310->placed;
    if (!placed->vf_0x68()) {
        placed->vf_0x70();
        RecordFlag<&UnkGameManager::field_0x2258>(&self->field_0x2dd);
        RecordFlag<&UnkGameManager::field_0x2270>(&self->field_0x2de);
    }
    fn_71011b6bbc(g_unk_717271e8f8, self, false);
    ResetParams();
    PlaySound(0xfd);
    fn_7100e9b1f8(g_unk_7101d47310->field_0x2228, self->field_0x279);
    if (self->field_0x279)
        fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0178, self);
}

// A child of a composition found by name, if it is a label.
static inline UnkComposition* FindLabel(UnkComposition* composition, const char* name) {
    if (!composition)
        return nullptr;
    UnkComposition* child = fn_71011d98ec(composition, fn_7100001570(name, __builtin_strlen(name)));
    if (!child)
        return nullptr;
    return fn_710009d50c(child->Type(), fn_71011e8dfc()) ? child : nullptr;
}

// A text string built from UTF-32 text.
struct UnkText {
    u8 unk_0x0[0x20];

    UnkText(const char32_t* text) { fn_7100066a50(this, text); }
    ~UnkText() { fn_7100066e44(this); }
};

struct UnkLanguage;
extern UnkLanguage* g_unk_7101d47358;
void fn_710119c020(UnkLanguage* language);
s32 fn_710119c9f4(UnkLanguage* language);

// Start of the controls menu: mode 2 (boss rush, the global flag at 0xb8) or 4; the controls
// menu (0x2b0, created when missing) becomes current, its "Label-Plus" label gets the plus
// button text, and it is switched to Japanese or the default language. Unless the menu's 0xb4
// is set it is handed on; the settings are reset.
// NON_MATCHING(minor): the original tests the menu again after storing it on the path where it
// already exists (Clang folds that test).
void fn_7100b2d868(UnkStateOwner* self) {
    self->field_0x2e4 = g_unk_71726bc570->field_0xb8 ? 2 : 4;
    if (UnkComposition* menu = self->field_0x2b0) {
        self->current = menu;
    } else {
        fn_7100b2cb44(self);
        self->current = self->field_0x2b0;
    }
    if (UnkComposition* label = FindLabel(self->current, "Label-Plus")) {
        {
            fn_71011cdce8(label, 0);
            fn_71011cdc30(label, false);
            if (g_unk_71726bc570->field_0xb8) {
                UnkText text(U"#GUI_BOSSRUSH_CONTROLS_LABEL_PLUS");
                fn_71011ec468(label, &text, false);
            } else {
                UnkText text(U"#GUI_CONTROLS_LABEL_PLUS");
                fn_71011ec468(label, &text, false);
            }
            fn_71011cdc30(label, true);
            fn_71011cdce8(label, 1);
        }
    }
    UnkLanguage* language = g_unk_7101d47358;
    if (!language) {
        language = static_cast<UnkLanguage*>(fn_71000839a0(0x2068, 0x3f));
        fn_710119c020(language);
        g_unk_7101d47358 = language;
    }
    bool japanese = fn_710119c9f4(language) == 10;
    UnkComposition* menu = self->field_0x2b0;
    auto send = reinterpret_cast<void (*)(UnkComposition*, const char*, bool)>(menu->vtable[0x1e8 / 8]);
    send(menu, japanese ? "ToJapanese" : "DefaultLang", false);
    if (!self->current->field_0xb4)
        fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0178, self->current);
    ResetParams();
}

struct UnkSamusMenu : UnkObject {
    u8 unk_0x8[0x228];
    CStrId field_0x230;
};
extern CStrId g_unk_7101d470e0;
const CClass* fn_7100b65dc0();  // GUI::CSamusMenuComposition::refl_GetClass
void fn_7100e9b4b0(void* target);

// End of the Samus menu state: the menu's id at 0x230 is reset to the global id, the menu is
// kept at 0x288, the mission log is updated as in fn_7100b2dcac and the menu hidden
// (fn_7100b2d0a4); the settings are set back, and with 0x279 set the manager's object at 0x2228
// is told, the manager's object at 0x498 is reset once in place, the owner is handed on and the
// recorded flags are undone.
void fn_7100b2dfc4(UnkStateOwner* self) {
    UnkObject* menu = reinterpret_cast<UnkObject*>(self->field_0x2c0);
    UnkSamusMenu* samus =
        fn_710009d50c(menu->Type(), fn_7100b65dc0()) ? static_cast<UnkSamusMenu*>(menu) : nullptr;
    samus->field_0x230 = g_unk_7101d470e0;
    self->field_0x288 = self->field_0x2c0;
    UnkMissionLog* log = GetMissionLog(self->field_0x490);
    if (log->A()->field_0x320 || log->B()->field_0x262) {
        bool value = self->field_0x2e8 != 1;
        log->A()->field_0x320 = value;
        log->B()->field_0x262 = value;
        log->B()->field_0x263 = true;
    }
    fn_7100b2d0a4(self);
    fn_7100219d4c(g_unk_7101d47300, true, false);
    fn_7100219cb0(g_unk_7101d47300, true, false);
    if (!self->field_0x279)
        return;
    fn_7100e9b4b0(g_unk_7101d47310->field_0x2228);
    UnkPlacedObj* placed = g_unk_7101d47310->placed;
    if (placed && placed->vf_0x68())
        placed->vf_0x70();
    fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0180, self);
    if (self->field_0x2dd)
        fn_71011cdc30(GetHolder(g_unk_7101d47310->field_0x2258), true);
    if (self->field_0x2de)
        fn_71011cdc30(GetHolder(g_unk_7101d47310->field_0x2270), true);
}

// A string with 0x80 bytes of inline storage (as in fn_7100ae9a10).
struct UnkInlineString {
    u8 str[0x30];
    void** vtable;
    u8 buffer[0x80];
    u64 field_0xb8;
    u32 field_0xc0;
};
extern void* g_unk_7101a1b750[];
void fn_710007e6e4(UnkInlineString* self, void* allocator);  // string constructor
void fn_710007e77c(UnkInlineString* self, const char* str);  // assign
void fn_710007ed40(UnkInlineString* self);
void fn_710007ecd4(UnkInlineString* self);

struct UnkPathIds;
void fn_710001f074(UnkPathIds* out, const UnkInlineString* path);

// A string id and a file-path id, filled in from a path string by fn_710001f074 (the members
// start out unset) and destroyed in reverse order.
struct UnkPathIds {
    union {
        CStrId id;
    };
    union {
        base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    };

    explicit UnkPathIds(const UnkInlineString& source) { fn_710001f074(this, &source); }
    ~UnkPathIds() {
        path.~UnkStrIdMemberOf();
        id.~CStrId();
    }
};

// A reference-counted resource (count at 0x54, freed by fn_710001737c).
void fn_710001737c(struct UnkResource* resource);
struct UnkResource {
    u8 unk_0x0[0x54];
    s32 ref_count;
};
struct UnkResourceHandle {
    UnkResource* ptr;

    ~UnkResourceHandle() {
        if (ptr && --ptr->ref_count == 0)
            fn_710001737c(ptr);
    }
};
struct UnkResourceManager;
extern UnkResourceManager* g_unk_71726ae8a8;
UnkResourceHandle fn_71002f7048(UnkResourceManager* manager, const UnkPathIds& ids);
void fn_7100b2c58c(UnkStateOwner* self);

// Start of the playthrough state: one setting is changed, the menu at 0x2d0 (created when
// missing) becomes current in mode -1, sound 0xfd is played, and the official playthrough's
// data is requested (and the handle dropped).
void fn_7100b2cebc(UnkStateOwner* self) {
    fn_7100219da4(g_unk_7101d47300, false, true, 2);
    if (!self->field_0x2d0)
        fn_7100b2c58c(self);
    self->current = self->field_0x2d0;
    self->field_0x2e4 = -1;
    PlaySound<fn_710006d4d0>(0xfd);
    UnkInlineString path;
    fn_710007e6e4(&path, &path.vtable);
    path.vtable = &g_unk_7101a1b750[2];
    path.field_0xb8 = 0;
    path.field_0xc0 = 0;
    fn_710007e77c(&path, "playdata:playthroughs/pt_official/pt_official");
    fn_71002f7048(g_unk_71726ae8a8, UnkPathIds(path));
    fn_710007ed40(&path);
    fn_710007ecd4(&path);
}

void fn_7100b2c964(UnkStateOwner* self);
void fn_7100b2ca48(UnkStateOwner* self);

// Start of a submenu state (the system menu at 0x2a0 or the amiibo menu at 0x2a8): mode 2, the
// menu (created when missing) becomes current and is handed on unless its 0xb4 is set, and the
// settings are reset.
template <UnkComposition* UnkStateOwner::*Menu, void (*Create)(UnkStateOwner*)>
__attribute__((always_inline)) static inline void StartSubmenu(UnkStateOwner* self) {
    self->field_0x2e4 = 2;
    if (!(self->*Menu))
        Create(self);
    self->current = self->*Menu;
    if (!self->current->field_0xb4)
        fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0178, self->current);
    ResetParams();
}

void fn_7100b2d62c(UnkStateOwner* self) { StartSubmenu<&UnkStateOwner::field_0x2a0, fn_7100b2c964>(self); }
void fn_7100b2d754(UnkStateOwner* self) { StartSubmenu<&UnkStateOwner::field_0x2a8, fn_7100b2ca48>(self); }

// End of the system menu state: unless in state 6 the menu is kept at 0x288, the current
// object handed on (unless 0x2dc is set) and hidden; two settings are set back.
void fn_7100b2d6d0(UnkStateOwner* self) {
    if (self->field_0x2e8 != 6) {
        self->field_0x288 = self->field_0x2a0;
        if (!self->field_0x2dc)
            fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0180, self->current);
        fn_7100b2d0a4(self);
    }
    fn_7100219d4c(g_unk_7101d47300, true, false);
    fn_7100219cb0(g_unk_7101d47300, true, false);
}

void fn_7100b2cd24(UnkStateOwner* self);

// The mission log, its menu created first when there is none.
UnkMissionLog* fn_7100b2e338(UnkStateOwner* self) {
    UnkObjRef* ref = self->field_0x490;
    if (!ref || !ref->obj)
        fn_7100b2cd24(self);
    return GetMissionLog(self->field_0x490);
}

// Start of the mission log state from the game: the parameter is set to -1 (mode 1), the
// mission log becomes current, sound 0xfd is played and the settings are reset.
void fn_7100b2e208(UnkStateOwner* self) {
    f32 value = -1.0f;
    fn_7100219294(g_unk_7101d47300, 0xede73d1953baf654, &value, 2);
    self->field_0x2e4 = 1;
    self->current = reinterpret_cast<UnkComposition*>(fn_7100b2e338(self));
    PlaySound(0xfd);
    ResetParams();
}

// End of the mission log state: the mission menu is kept at 0x288, the log's flags are set
// (as in fn_7100b2dcac, unconditionally) and the menu hidden; the rest follows fn_7100b2dfc4.
void fn_7100b2e3b8(UnkStateOwner* self) {
    // Stored as a plain pointer: the original reloads the menu afterwards.
    *reinterpret_cast<UnkObject**>(&self->field_0x288) = self->MissionMenu();
    UnkMissionLog* log = GetMissionLog(self->field_0x490);
    bool value = self->field_0x2e8 != 1;
    log->A()->field_0x320 = value;
    log->B()->field_0x262 = value;
    log->B()->field_0x263 = true;
    fn_7100b2d0a4(self);
    fn_7100219d4c(g_unk_7101d47300, true, false);
    fn_7100219cb0(g_unk_7101d47300, true, false);
    if (!self->field_0x279)
        return;
    fn_7100e9b4b0(g_unk_7101d47310->field_0x2228);
    UnkPlacedObj* placed = g_unk_7101d47310->placed;
    if (placed && placed->vf_0x68())
        placed->vf_0x70();
    fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0180, self);
    if (self->field_0x2dd)
        fn_71011cdc30(GetHolder(g_unk_7101d47310->field_0x2258), true);
    if (self->field_0x2de)
        fn_71011cdc30(GetHolder(g_unk_7101d47310->field_0x2270), true);
}

// The constructor. The base part is built by fn_71011d8d24; three callbacks (owner and
// function) follow at 0x230, the state pointers are cleared, six id pairs take the global
// default ids, an array (flags 0xf) and two more callbacks follow, and the member at 0x3d8 is
// built by fn_710002f4e4.
void fn_71011d8d24(void* self);
void fn_710002f4e4(void* self);
void fn_710002f67c(void* self);
void fn_71011d8e90(void* self);
extern void* g_unk_7101a9b670[];
void fn_7100b31440();
void fn_7100b313bc();
void fn_7100b313c4();
void fn_7100b31394();
void fn_7100b31464();

struct UnkMenuStatesBase {
    void** vtable;
    u8 unk_0x8[0x180];
    u8 field_0x188;
    u8 unk_0x189[0xa7];

    UnkMenuStatesBase() { fn_71011d8d24(this); }
    ~UnkMenuStatesBase() { fn_71011d8e90(this); }
};

struct UnkMenuStatesVtable : UnkMenuStatesBase {
    UnkMenuStatesVtable() { vtable = &g_unk_7101a9b670[2]; }
};

// A callback (owner and function) and the set it is registered with, which its destructor
// leaves through `Unregister`.
template <void (*Unregister)(void*, void**), void (*Register)(void*, void**) = nullptr>
struct UnkCallback {
    void* set;
    void* owner;
    void (*fn)();

    UnkCallback(void* owner, void (*fn)()) : set(nullptr), owner(owner), fn(fn) {}
    ~UnkCallback() {
        if (set) {
            Unregister(set, &set);
            set = nullptr;
        }
        fn = nullptr;
        owner = nullptr;
    }

    // Moves the callback to the set at `Offset` in `owner`.
    template <u32 Offset>
    void Watch(void* owner) {
        if (set) {
            Unregister(set, &set);
            set = nullptr;
        }
        set = static_cast<char*>(owner) + Offset;
        Register(set, &set);
    }
};
void fn_71004567dc(void* set, void** slot);
void fn_71004f3cf0(void* set, void** slot);
void fn_7100b31900(void* set, void** slot);
void fn_71004f3ea4(void* set, void** slot);
void fn_7100b31770(void* set, void** slot);

// Destroyed out of line (fn_7100026f70; the unions keep the members' own destructors out).
struct UnkIdPair {
    union {
        CStrId id = g_unk_7101d470e0;
    };
    union {
        CFilePathStrId path = g_unk_7101d470e8;
    };
    u64 field_0x10 = 0;

    ~UnkIdPair();
};
void fn_7100026f70(UnkIdPair* pair);
inline UnkIdPair::~UnkIdPair() { fn_7100026f70(this); }

// An array of pointers whose destructor frees its storage.
struct UnkPtrArray : base::UnkVector<void*> {
    using base::UnkVector<void*>::UnkVector;
    ~UnkPtrArray() { Free(); }
};

// A reference-counted handle released through vf_0x10.
struct UnkHandleObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};
struct UnkHandle {
    UnkHandleObj* ptr = nullptr;

    ~UnkHandle() {
        if (UnkHandleObj* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkMenuStates : UnkMenuStatesVtable {
    UnkCallback<fn_71004567dc> field_0x230{this, fn_7100b31440};
    UnkCallback<fn_71004f3ea4> field_0x248{this, fn_7100b313bc};
    UnkCallback<fn_71004f3ea4> field_0x260{this, fn_7100b313c4};
    bool field_0x278 = false;
    void* field_0x280 = nullptr;
    void* field_0x288 = nullptr;
    void* field_0x290 = nullptr;
    void* field_0x298 = nullptr;
    void* field_0x2a0 = nullptr;
    void* field_0x2a8 = nullptr;
    void* field_0x2b0 = nullptr;
    void* field_0x2b8 = nullptr;
    void* field_0x2c0 = nullptr;
    void* field_0x2c8 = nullptr;
    void* field_0x2d0 = nullptr;
    f32 field_0x2d8 = -1.0f;
    bool field_0x2dc = false;
    u8 unk_0x2dd[0x3];
    s32 field_0x2e0;
    s32 field_0x2e4;
    s32 field_0x2e8;
    u8 unk_0x2ec[0x4];
    UnkIdPair field_0x2f0;
    UnkIdPair field_0x308;
    UnkIdPair field_0x320;
    UnkIdPair field_0x338;
    UnkIdPair field_0x350;
    UnkIdPair field_0x368;
    UnkPtrArray field_0x380{0xf};
    UnkCallback<fn_71004f3ea4, fn_71004f3cf0> field_0x3a8{this, fn_7100b31394};
    UnkCallback<fn_7100b31770, fn_7100b31900> field_0x3c0{this, fn_7100b31464};
    struct UnkMember3d8 {
        u8 unk_0x0[0xb0];

        UnkMember3d8() { fn_710002f4e4(this); }
        ~UnkMember3d8() { fn_710002f67c(this); }
    } field_0x3d8;
    UnkHandle field_0x488;
    UnkHandle field_0x490;

    __attribute__((always_inline)) UnkMenuStates() {}
    __attribute__((always_inline)) ~UnkMenuStates() {
        vtable = &g_unk_7101a9b670[2];
        field_0x380.count = 0;
    }
};

void fn_7100b2e54c(UnkMenuStates* self) { new (self) UnkMenuStates; }

void fn_7100b2e7c4(UnkMenuStates* self) { self->~UnkMenuStates(); }

struct UnkWorld {
    u8 unk_0x0[0xd63f40];
    UnkObjRef* field_0xd63f40;
};
struct UnkInputEvent {
    u8 unk_0x0[0x1a];
    bool handled;
};
const CClass* fn_7100b5e368();  // GUI::CPopUpComposition::refl_GetClass
bool fn_710118a8d0(UnkPlacedObj* placed);
bool fn_7100b44a3c(UnkObject* obj);
void fn_7100b2eaac(UnkStateOwner* self, s32 state, bool a, bool b);

// Input while no pop-up is shown: from states 8 and 9 (or 7, unless fn_7100b44a3c holds for
// the world's object at 0xd63f40) back to state 1; from state 1, once the manager's object
// allows it (fn_710118a8d0), to state 8 (boss rush) or 7.
void fn_7100b2e978(UnkStateOwner* self, const UnkInputEvent* event) {
    if (event->handled)
        return;
    UnkObjRef* popup_ref = g_unk_717271e8f8->field_0x230;
    UnkObject* popup = popup_ref ? popup_ref->obj : nullptr;
    if (popup && fn_710009d50c(popup->Type(), fn_7100b5e368()))
        return;
    switch (self->field_0x2e0) {
    case 1: {
        UnkPlacedObj* placed = g_unk_7101d47310->placed;
        if (!placed || !fn_710118a8d0(placed))
            return;
        if (g_unk_71726bc570->field_0xb8)
            fn_7100b2eaac(self, 8, false, true);
        else
            fn_7100b2eaac(self, 7, true, true);
        return;
    }
    case 7: {
        UnkObjRef* ref = g_unk_7101d47310->field_0x2228->field_0xd63f40;
        if (fn_7100b44a3c(ref ? ref->obj : nullptr))
            return;
        fn_7100b2eaac(self, 1, true, true);
        return;
    }
    case 8:
    case 9:
        fn_7100b2eaac(self, 1, true, true);
        return;
    }
}

// The state table (0x380) gives each state's enter and exit functions.
// Three member functions per state (enter, exit and one more); zeroed and copied as raw bytes
// (the original reloads the count and the array around each element).
struct UnkStateFns {
    void (UnkStateOwner::*enter)();
    void (UnkStateOwner::*exit)();
    void (UnkStateOwner::*field_0x20)();

    UnkStateFns() { __builtin_memset(this, 0, sizeof(*this)); }
    UnkStateFns(UnkStateFns&& other) { __builtin_memcpy(this, &other, sizeof(*this)); }
};
struct UnkStateTable;

// The table's entry for a state, growing the table when needed (base::UnkVector::GrowAt).
__attribute__((flatten)) UnkStateFns* fn_7100b2f03c(UnkStateTable** table, s32 state) {
    return &reinterpret_cast<base::UnkVector<UnkStateFns>*>(table)->GrowAt(state);
}
struct UnkHighlight {
    u8 unk_0x0[0x2e1];
    bool field_0x2e1;
};

// Requests state `state`; when `now` is set the current state (0 to 9) is left, and unless
// that set 0x278 the new state becomes current and is entered: its menu gets
// "Background_Highlight" and, outside state 7, is shown ("OnShow", or "OnShowFromGame" when
// coming from the game) unless 0xb4 is set. A "Content" child holding at least two entries
// is then watched at 0x260 if nothing is; otherwise the menu is activated and handed to
// fn_71011b6bbc unless the highlight's 0x2e1 is clear.
// NON_MATCHING(minor): register allocation around the "OnShow" event call (x8/x10 swapped).
void fn_7100b2eaac(UnkStateOwner* self, s32 state, bool from_game, bool now) {
    self->field_0x2e8 = state;
    if (!now)
        return;
    self->field_0x279 = from_game;
    self->field_0x278 = false;
    self->field_0x288 = nullptr;
    if ((u32)self->field_0x2e0 <= 9)
        (self->*fn_7100b2f03c(&self->field_0x380, self->field_0x2e0)->exit)();
    if (self->field_0x278)
        return;
    s32 next = self->field_0x2e8;
    self->field_0x2e4 = self->field_0x2e0;
    self->field_0x2e0 = next;
    if ((u32)next > 9)
        return;
    (self->*fn_7100b2f03c(&self->field_0x380, next)->enter)();
    self->current->SendEvent("Background_Highlight", false);
    fn_710002ffb4(g_unk_717271e8f8->field_0xca8, &g_unk_7172762fd8);
    if (self->field_0x2e0 == 7)
        return;
    if (!self->current->field_0xb4) {
        UnkComposition* menu = self->current;
        void** vtable = menu->vtable;
        const char* name = !self->field_0x279 ? "OnShow" : "OnShowFromGame";
        reinterpret_cast<void (*)(UnkComposition*, const char*, bool)>(vtable[0x1e8 / 8])(menu, name, false);
        fn_71011cdce8(self->current, 1);
        UnkGuiChild* content = fn_71011d94d4(self->current, CStrId("Content", true));
        if (content && content->field_0x30 && (*content->field_0x30)->count >= 2 && !self->field_0x260) {
            fn_710002fd10(g_unk_717271e8f8->field_0xca8, &g_unk_7172762fd8);
            UnkWatched* watched = content->Watched();
            if (self->field_0x260) {
                fn_71004f3ea4(self->field_0x260, &self->field_0x260);
                self->field_0x260 = nullptr;
            }
            self->field_0x260 = &watched->set;
            fn_71004f3cf0(&watched->set, &self->field_0x260);
            return;
        }
    }
    fn_71011cf2a4(self->current, 1);
    UnkFlagRef* ref = g_unk_7101d47310->field_0x2258;
    UnkComposition* highlight = GetHolder(ref);
    if (highlight && !reinterpret_cast<UnkHighlight*>(highlight)->field_0x2e1)
        return;
    fn_71011b6bbc(g_unk_717271e8f8, self->current, false);
}

extern UnkGuiKey g_unk_7172762fe0 __attribute__((visibility("hidden")));
bool fn_7100b41e5c(UnkObject* obj);

// Page switching between states 7, 8 and 9 (outside boss rush, while the world's object at
// 0xd63f40 allows it): from state 7 to `Next`, from `Prev` back to 7, each with a sound and a
// 0.12 delay at 0x2d8.
template <s32 Prev, s32 Next>
__attribute__((always_inline)) static inline void SwitchPage(UnkStateOwner* self, const UnkInputEvent* event) {
    if (event->handled)
        return;
    UnkObjRef* ref = g_unk_7101d47310->field_0x2228->field_0xd63f40;
    if (!ref)
        return;
    UnkObject* obj = ref->obj;
    if (!obj || fn_7100b41e5c(obj) || fn_7100b44a3c(obj))
        return;
    if (g_unk_71726bc570->field_0xb8)
        return;
    switch (self->field_0x2e0) {
    case 7:
        fn_710002fd10(g_unk_717271e8f8->field_0xca8, &g_unk_7172762fe0);
        self->field_0x2d8 = 0.12f;
        fn_7100b2eaac(self, Next, false, true);
        return;
    case Prev:
        fn_710002fd10(g_unk_717271e8f8->field_0xca8, &g_unk_7172762fe0);
        self->field_0x2d8 = 0.12f;
        fn_7100b2eaac(self, 7, false, true);
        return;
    }
}

void fn_7100b2ed84(UnkStateOwner* self, const UnkInputEvent* event) { SwitchPage<9, 8>(self, event); }
void fn_7100b2ee88(UnkStateOwner* self, const UnkInputEvent* event) { SwitchPage<8, 9>(self, event); }

// A string id and a file-path id copied from globals for an assignment (fn_71000267f8), the
// path released as a two-step member.
struct UnkIdPairValue {
    union {
        CStrId id;
    };
    union {
        CFilePathStrId path_init;
        base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    };

    UnkIdPairValue(const CStrId& a, const CFilePathStrId& b) : id(a) { new (&path_init) CFilePathStrId(b); }
    ~UnkIdPairValue() {
        path.~UnkStrIdMemberOf();
        id.~CStrId();
    }
};
void fn_71000267f8(UnkIdPair* pair, const UnkIdPairValue& value);
void fn_71011d931c(UnkMenuStates* self);
void fn_7100b2f6b0(UnkMenuStates* self);
void fn_7100b2c2bc(UnkMenuStates* self);
void fn_7100b2cd24(UnkMenuStates* self);
extern CStrId g_unk_71727b9d30;
extern CFilePathStrId g_unk_7172762ff0 __attribute__((visibility("hidden")));
extern CFilePathStrId g_unk_7172762ff8 __attribute__((visibility("hidden")));
extern CFilePathStrId g_unk_7172762fe8 __attribute__((visibility("hidden")));
extern CFilePathStrId g_unk_7172763000 __attribute__((visibility("hidden")));

// Initialisation: after the base part (fn_71011d931c) four id pairs are set, fn_7100b2f6b0
// runs, the delay and states are reset (state 1 next), the object is activated once (0x188,
// vf_0xa0), the two callbacks are moved to the manager's sets at 0x24f8 and 0x2128, and the
// power description and mission menu are set up.
void fn_7100b2f250(UnkMenuStates* self) {
    fn_71011d931c(self);
    fn_71000267f8(&self->field_0x2f0, UnkIdPairValue(g_unk_71727b9d30, g_unk_7172762ff0));
    fn_71000267f8(&self->field_0x308, UnkIdPairValue(g_unk_71727b9d30, g_unk_7172762ff8));
    fn_71000267f8(&self->field_0x368, UnkIdPairValue(g_unk_71727b9d30, g_unk_7172762fe8));
    fn_71000267f8(&self->field_0x320, UnkIdPairValue(g_unk_71727b9d30, g_unk_7172763000));
    fn_7100b2f6b0(self);
    self->field_0x2e0 = -1;
    self->field_0x2e4 = -1;
    self->field_0x2e8 = 1;
    self->field_0x2d8 = -1.0f;
    if (self->field_0x188 != 1) {
        self->field_0x188 = 1;
        reinterpret_cast<void (*)(UnkMenuStates*)>(self->vtable[0xa0 / 8])(self);
    }
    self->field_0x3a8.Watch<0x24f8>(g_unk_7101d47310);
    self->field_0x3c0.Watch<0x2128>(g_unk_7101d47310);
    fn_7100b2c2bc(self);
    fn_7100b2cd24(self);
}

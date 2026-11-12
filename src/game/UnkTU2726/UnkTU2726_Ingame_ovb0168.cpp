// Unit 2726 (gui/scripts/msemenu_ingame.lua): members of the in-game menu root. fn_710118984c and
// fn_71011897c4 drive the highlight flag of the display object the handle at +0x100 holds;
// fn_710118fd30 registers a display object with the GUI root (as fn_7100affe40,
// src/game/GUI/CDebugMenuEntryItemRenderer/CDebugMenuEntryItemRenderer_Layout_ovb0112.cpp);
// fn_710118a4c0 shows the quit prompt; fn_7101187e2c mounts the in-game and GUI packs;
// fn_710118a8d0 says whether the menu may open; fn_7101187c30 applies the saved rumble setting.
// Names and layouts are placeholders.
#include "../../game_s7/UnkPropBagS7.h"
#include "base/UnkAtExit.h"
#include "base/UnkVector.h"
#include "base/file/UnkFileDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPool.h"
#include "base/input/UnkRumbleManager.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CActorComponent.h"
#include "game/CBlackboard.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU0.h"
#include "game/UnkTU23.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2656.h"
#include "game/UnkTU2726.h"
#include "game/UnkTU2730.h"
#include "game/UnkTU2768.h"
#include "game/UnkTU39.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101d47330;
void fn_7101198654(void* manager);
bool fn_7101198674(void* manager);

struct UnkIngameMenu_ovb0168;

struct UnkIngameMenu_ovb0168 {
    u8 pad_0x0[0x100];
    UnkWeakHandle* field_0x100;  // the highlighted object's handle
    u8 pad_0x108[0xc8];
    u8 field_0x1d0[8];
};
static_assert(__builtin_offsetof(UnkIngameMenu_ovb0168, field_0x1d0) == 0x1d0, "layout");

void fn_710118f9b8(void* list, UnkIngameMenu_ovb0168** self);

static inline UnkGuiDisplayObject* Highlighted(UnkIngameMenu_ovb0168* self) {
    return reinterpret_cast<UnkGuiDisplayObject*>(self->field_0x100->target);
}

void fn_710118984c(UnkIngameMenu_ovb0168* self) {
    if (Highlighted(self)->field_0xb4) {
        fn_7101198654(g_unk_7101d47330);
        UnkIngameMenu_ovb0168* slot = self;
        fn_710118f9b8(self->field_0x1d0, &slot);
        UnkWeakHandle* handle = self->field_0x100;
        fn_71011cdce8(handle ? reinterpret_cast<UnkGuiDisplayObject*>(handle->target) : nullptr, false);
    }
}

// Sets the highlight to the opposite of its state when `on`, clears it otherwise.
static inline void Apply(UnkIngameMenu_ovb0168* self, bool on) {
    UnkGuiDisplayObject* object = Highlighted(self);
    bool flag = object->field_0xb4;
    if (on)
        fn_71011cdce8(object, !flag);
    else if (flag)
        fn_71011cdce8(object, false);
}

void fn_71011897c4(UnkIngameMenu_ovb0168* self) {
    bool on = fn_710002fb70(self->pad_0x108);
    if (on || Highlighted(self)->field_0xb4) {
        Apply(self, on);
        return;
    }
    Apply(self, fn_7101198674(g_unk_7101d47330));
}

using base::global::CStrId;
using unk_prop_bag::UnkPropBag;

bool fn_71011ceac0(UnkGuiDisplayObject* self);
bool fn_71011ceb34(UnkGuiDisplayObject* self);
void fn_71011b5598(UnkGuiRoot* root, UnkGuiDisplayObject* object);
void fn_71011cd424(UnkGuiDisplayObject* self, UnkGuiDisplayObject* parent);

struct UnkNamedObject_ovb0168 {
    UnkGuiDisplayObject* Object() { return reinterpret_cast<UnkGuiDisplayObject*>(this); }
    u8 pad_0x0[0x130];
    CStrId id;  // 0x130
    u8 pad_0x138[0x4c];
    bool field_0x184;
    u8 pad_0x185[0x2];
    bool field_0x187;
    bool field_0x188;
};

void fn_710118fd30(UnkGuiRoot* root, UnkWeakTarget* object, const CStrId& name, UnkPropBag* bag) {
    UnkNamedObject_ovb0168* item = reinterpret_cast<UnkNamedObject_ovb0168*>(object);
    if (!item)
        return;
    item->id = name;
    if (bag)
        fn_71011cd424(item->Object(), reinterpret_cast<UnkGuiDisplayObject*>(bag));
    if (!item->field_0x187 &&
        (item->field_0x188 || item->field_0x184 ||
         (fn_71011cde14(item->Object())->field_0x3c &&
          (fn_71011ceac0(item->Object()) || fn_71011ceb34(item->Object())))))
        fn_71011b5598(root, item->Object());
}

using base::global::CStrId;

struct UnkPromptHolder_ovb0168 {
    u8 pad_0x0[0x10];
    UnkGuiDisplayObject* prompt;  // 0x10
};

struct UnkGameManagerPrompt_ovb0168 {
    u8 pad_0x0[0x2258];
    UnkPromptHolder_ovb0168* holder;
};

struct UnkModeFlags {
    u8 unk_0x0[0xb8];
    bool field_0xb8;
};

struct UnkPromptCallback_ovb0168 {
    void* owner;
    void (*fn)(void*);
};

struct UnkPromptParams_ovb0168;
extern UnkModeFlags* g_unk_71726bc570;
extern char g_unk_7172a0b890[] __attribute__((visibility("hidden")));
void fn_710118fd28(void*);
void fn_710118fd2c(void*);
struct UnkMapCallback_ovb0090;
struct UnkGuiComposition_ovb0090;
void fn_7100b5fde0(UnkGuiComposition_ovb0090* gui, const CStrId* text, const CStrId* accept, const CStrId* back,
                   const UnkMapCallback_ovb0090* on_accept, const UnkMapCallback_ovb0090* on_back, u32 mode,
                   f32 time);

struct UnkIngameMenu_ovb0168b;

void fn_710118a4c0(UnkIngameMenu_ovb0168b* self) {
    UnkPromptHolder_ovb0168* holder = reinterpret_cast<UnkGameManagerPrompt_ovb0168*>(g_unk_7101d47310)->holder;
    if (!holder)
        return;
    UnkGuiDisplayObject* prompt = holder->prompt;
    if (!prompt)
        return;
    fn_71011cdce8(prompt, true);
    CStrId text(g_unk_71726bc570->field_0xb8 ? "#GUI_BOSSRUSH_PROMPT_QUIT_BOSSRUSH" : "#GUI_OPTIONS_PROMPT_QUIT", true);
    UnkPromptCallback_ovb0168 yes = {self, fn_710118fd28};
    UnkPromptCallback_ovb0168 no = {self, fn_710118fd2c};
    fn_7100b5fde0(reinterpret_cast<UnkGuiComposition_ovb0090*>(prompt), &text,
                  reinterpret_cast<const CStrId*>(g_unk_7172a0b890 + 8), reinterpret_cast<const CStrId*>(g_unk_7172a0b890),
                  reinterpret_cast<const UnkMapCallback_ovb0090*>(&yes),
                  reinterpret_cast<const UnkMapCallback_ovb0090*>(&no), 0, 1.0f);
}

using base::global::CFilePathStrId;

extern base::file::UnkFileDevice* g_unk_7101d4d828;
struct UnkSoundMgr;

void fn_710008987c(base::file::UnkFileDevice* self, const CFilePathStrId* path, u32 flags);
void fn_7100108c94(UnkSoundMgr* self, const CFilePathStrId* path);

void fn_7101187e2c() {
    base::file::UnkFileDevice* device = g_unk_7101d4d828;
    if (!device) {
        device = static_cast<base::file::UnkFileDevice*>(fn_71000839a0(0x1348, 0x3f));
        fn_71000893a8(device);
        g_unk_7101d4d828 = device;
    }
    {
        CFilePathStrId path("packs/gui/ingame.pkg", true);
        fn_710008987c(device, &path, 0);
    }
    UnkSoundMgr* sound = g_unk_7101d4d820;
    {
        CFilePathStrId path("packs/gui/gui.pkg", true);
        fn_7100108c94(sound, &path);
    }
}

using base::global::CStrId;

struct UnkPlacedObj;
struct UnkPlayer;

extern void* g_unk_7101d47330;
bool fn_71011b3838(UnkGuiRoot* root);
bool fn_71008723f4(void* object, const CStrId* id);
extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

struct UnkMenuFlag_ovb0168 {
    u8 pad_0x0[0x50];
    u8 field_0x50;
};

struct UnkPlayerSub_ovb0168 {
    u8 pad_0x0[0x70];
    s32 state;  // 0x70
};

struct UnkPlayerInfo_ovb0168 {
    u8 pad_0x0[0x1c0];
    UnkPlayerSub_ovb0168* field_0x1c0;
    u8 pad_0x1c8[0x10];
    void* field_0x1d8;
};

struct UnkHostState_ovb0168 {
    u8 pad_0x0[0xb14];
    u8 field_0xb14;
};

struct UnkGuardedId_ovb0168 {
    CStrId id;
    u64 guard;
};
extern UnkGuardedId_ovb0168 g_unk_7172a0b900 __attribute__((visibility("hidden")));

bool fn_710118a8d0(UnkPlacedObj* placed) {
    if (reinterpret_cast<UnkMenuFlag_ovb0168*>(placed)->field_0x50)
        return false;
    if (fn_71011b3838(g_unk_717271e8f8))
        return false;
    UnkGameManager* manager = g_unk_7101d47310;
    if (void* cameras = fn_7100defcec(manager)) {
        if (UnkCameraCtrl* camera = fn_71010e2080(cameras)) {
            if (fn_710009d50c(camera->vf_0x0(), fn_71010e7f90()))
                return false;
        }
    }
    char* state = reinterpret_cast<char*>(g_unk_7101d47310) + 0x1040;
    if (fn_7101053a7c(state) || fn_7101053a0c(state))
        return false;
    if (UnkPlayerInfo_ovb0168* player = reinterpret_cast<UnkPlayerInfo_ovb0168*>(fn_7100dec788(g_unk_7101d47310))) {
        void* object = player->field_0x1d8;
        if (object) {
            if (!fn_710024fbc8(object))
                return false;
            if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_7172a0b900.guard), __ATOMIC_ACQUIRE) & 1), 0)) {
                if (__cxa_guard_acquire(&g_unk_7172a0b900.guard)) {
                    new (&g_unk_7172a0b900.id) CStrId("EmmyAbilityObtained", true);
                    fn_7100000250(fn_710000051c, &g_unk_7172a0b900.id, g_unk_7101a19000);
                    __cxa_guard_release(&g_unk_7172a0b900.guard);
                }
            }
            if (fn_71008723f4(object, &g_unk_7172a0b900.id))
                return false;
        }
        if (player->field_0x1c0 && player->field_0x1c0->state == 2)
            return false;
    }
    if (!fn_7101196fd8(g_unk_7101d47330) && !reinterpret_cast<UnkHostState_ovb0168*>(g_unk_7101d47330)->field_0xb14)
        return true;
    return false;
}

using base::global::CStrId;
using base::input::UnkRumbleManager;

struct UnkUserDataOwner;
bool fn_71010474f4(CBlackboard* self, const CStrId& section, const CStrId& name);
extern UnkRumbleManager* g_unk_7101d47348;
void fn_7100074e3c(UnkRumbleManager* self, bool enabled);

void fn_7101187c30() {
    UnkUserDataOwner* owner = g_unk_7101d47308;
    if (!owner) {
        owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(owner);
        g_unk_7101d47308 = owner;
    }
    CBlackboard* blackboard = fn_710120384c(owner);
    bool known;
    {
        CStrId section("USER_DATA", true);
        CStrId name("RumbleTunable", true);
        known = fn_71010474f4(blackboard, section, name);
    }
    if (!known)
        return;
    bool enabled = true;
    {
        CStrId section("USER_DATA", true);
        CStrId name("RumbleTunable", true);
        if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
            fn_710006d11c(s, &name, &enabled);
    }
    UnkRumbleManager* manager = g_unk_7101d47348;
    if (!manager) {
        manager = static_cast<UnkRumbleManager*>(fn_71000839a0(0x2210, 0x3f));
        fn_71000733e4(manager);
        g_unk_7101d47348 = manager;
    }
    fn_7100074e3c(manager, enabled);
}

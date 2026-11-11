// GUI::CLoadCheckPointButton's unit (tools/tu_map.py): asks "#GUI_OPTIONS_PROMPT_LOAD_CHECKPOINT"
// through the GUI composition (the game manager's +0x2258, its +0x10) with accept and back texts and
// two callbacks on the button (fn_7100b33ee0, fn_7100b33ee4), as CMapAcquisitionComponent's
// save-station question (src/game/CMapAcquisitionComponent/CMapAcquisitionComponent_Question_ovb0090.cpp).
// fn_7100b33d28: with the in-game menu (a GUI::CIngameMenu) marked busy (+0x2dc), sends it a kind-0x12
// GUI event for the button (slot 0xb8) and moves it to state 1; then the lazily created singleton
// g_unk_7172763578 gets fn_7100e19954 and the game manager fn_7100dff93c. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CIngameMenu.h"
#include "game/GUI/CLoadCheckPointButton.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiEvent.h"
#include "game/UnkTU2559.h"
#include "types.h"

using base::global::CStrId;

struct UnkLoadCheckPointButton_ovb0118;
void fn_7100b33ee0(UnkLoadCheckPointButton_ovb0118* self);
void fn_7100b33ee4(UnkLoadCheckPointButton_ovb0118* self);

// A callback: an object and a function to call on it.
struct UnkButtonCallback_ovb0118 {
    UnkLoadCheckPointButton_ovb0118* object;
    void (*function)(UnkLoadCheckPointButton_ovb0118*);
};

struct UnkGuiComposition_ovb0090;
struct UnkMapCallback_ovb0090;
void fn_7100b5fde0(UnkGuiComposition_ovb0090* gui, const CStrId* text, const CStrId* accept, const CStrId* back,
                   const UnkMapCallback_ovb0090* on_accept, const UnkMapCallback_ovb0090* on_back, u32 mode,
                   f32 time);

struct UnkGuiRef_ovb0118 {
    u8 pad_0x0[0x10];
    UnkGuiComposition_ovb0090* composition;  // 0x10
};

void fn_7100b33bcc(UnkLoadCheckPointButton_ovb0118* self) {
    UnkGuiRef_ovb0118* ref = static_cast<UnkGuiRef_ovb0118*>(g_unk_7101d47310->field_0x2258);
    if (!ref)
        return;
    UnkGuiComposition_ovb0090* gui = ref->composition;
    if (!gui)
        return;
    CStrId text("#GUI_OPTIONS_PROMPT_LOAD_CHECKPOINT", true);
    CStrId back("#GUI_B_BACK", true);
    CStrId accept("#GUI_A_ACCEPT", true);
    UnkButtonCallback_ovb0118 accept_callback = {self, fn_7100b33ee0};
    UnkButtonCallback_ovb0118 back_callback = {self, fn_7100b33ee4};
    fn_7100b5fde0(gui, &text, &accept, &back, reinterpret_cast<const UnkMapCallback_ovb0090*>(&accept_callback),
                  reinterpret_cast<const UnkMapCallback_ovb0090*>(&back_callback), 0, -1.0f);
}

// The in-game menu, as this unit sees it.
struct UnkLoadCheckPointMenu_ovb0118 {
    void** vtable;
    u8 pad_0x8[0x2d4];
    bool busy;  // 0x2dc

    void vf_0xb8(UnkGuiEvent* event) {
        reinterpret_cast<void (*)(UnkLoadCheckPointMenu_ovb0118*, UnkGuiEvent*)>(vtable[0xb8 / 8])(this, event);
    }
};

struct UnkStateOwner;
void fn_7100b30d70(UnkLoadCheckPointMenu_ovb0118* menu, bool flag);

// A lazily created singleton (0x28 bytes).
struct UnkLoadCheckPointSingleton_ovb0118;
void fn_7100dff93c(UnkGameManager* manager);

void fn_7100b33d28(UnkLoadCheckPointButton_ovb0118* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    if (UnkIngameMenu* found = fn_7100deac84(manager)) {
        base::reflection::UnkReflObject* object = reinterpret_cast<base::reflection::UnkReflObject*>(found);
        UnkLoadCheckPointMenu_ovb0118* menu =
            fn_710009d50c(object->vf_0x0(), fn_7100b2c064()) ? reinterpret_cast<UnkLoadCheckPointMenu_ovb0118*>(object)
                                                            : nullptr;
        menu->busy = true;
        UnkGuiEvent event;
        s32 kind = 0x12;
        fn_710120235c(&event, reinterpret_cast<UnkGuiDisplayObject*>(self), &kind, nullptr);
        menu->vf_0xb8(&event);
        fn_7100b30d70(menu, true);
        fn_7100b2eaac(reinterpret_cast<UnkStateOwner*>(menu), 1, true, true);
        menu->busy = false;
        fn_7101202488(&event);
    }
    UnkLoadCheckPointSingleton_ovb0118* singleton = g_unk_7172763578;
    if (!singleton) {
        singleton = static_cast<UnkLoadCheckPointSingleton_ovb0118*>(fn_71000839a0(0x28, 0x3f));
        fn_7100e17cc8(singleton);
        g_unk_7172763578 = singleton;
    }
    fn_7100e19954(singleton, true);
    fn_7100dff93c(g_unk_7101d47310);
}

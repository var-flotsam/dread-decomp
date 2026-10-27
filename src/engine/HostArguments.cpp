// The first translation unit of main (tools/tu_map.py: unit 0), apart from nnMain's
// (src/engine/nnMain.cpp): the host argument walk nnMain starts with, the script call and the
// debug start-up step nnMain runs after the main loop, and the game's main function
// (fn_7100000980), which nnMain hands to the main loop.
//
// The host argument walk reads every argument after the program's name and keeps none (the
// release build's option parsing is compiled out). Its parameters are unused; nnMain passes 0 and
// null.

#include <nn/os.h>

#include "types.h"

void fn_71000005d0(s32, char**) {
    const s32 argc = nn::os::GetHostArgc();
    for (s32 i = 1; i < argc; ++i)
        nn::os::GetHostArgv();
}

// ---------------------------------------------------------------------------------------------
// The same unit's script call (fn_7100000790, also used by the start-up steps of other units) and
// the debug start-up step that uses it (fn_7100000610).

#include <stdarg.h>

struct lua_State;
extern lua_State* g_unk_7172a01d18;
s32 fn_71010c0bb0(lua_State* L, const char* str);
s32 fn_71010bf400(lua_State* L, const char* fmt, va_list args);
s32 fn_71010c0d80(lua_State* L, s32 a, char* buf, u32 size, void* x, void* y);

// Calls the script function `name` with the arguments `fmt` describes; whether it returned 1
// (the same as fn_7100e01f08 in src/game/CGameManager.cpp).
bool fn_7100000790(const char* name, const char* fmt, ...) {
    if (!name)
        return false;
    if (fn_71010c0bb0(g_unk_7172a01d18, name) == 0)
        return false;
    va_list args;
    va_start(args, fmt);
    s32 r = fn_71010bf400(g_unk_7172a01d18, fmt, args);
    char buf[0x100];
    bool result = fn_71010c0d80(g_unk_7172a01d18, r, buf, 0x100, nullptr, nullptr) == 1;
    va_end(args);
    return result;
}

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"

using base::global::CRntString;
using base::global::CStrId;

// The debug start-up settings in base::global's .bss: two flags, then four strings.
struct UnkDebugStartStrings {
    CRntString field_0x0;
    CRntString field_0x30;
    CRntString field_0x60;  // passed on as a string id
    CRntString field_0x90;
};
extern u8 g_unk_7101d473c0 __attribute__((visibility("hidden")));  // give all items and power-ups
extern u8 g_unk_7101d473c1 __attribute__((visibility("hidden")));  // start in game
extern UnkDebugStartStrings g_unk_7101d473c8 __attribute__((visibility("hidden")));

// A string id built from a string out of line (fn_7100000850), released like CStrId.
struct UnkStartStrId {
    union {
        CStrId id;
    };

    explicit UnkStartStrId(const CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkStartStrId() { id.~CStrId(); }
};

// The game manager's start-up steps (unit 2553, CGameManager).
void fn_7100deac9c(UnkGameManager* manager, const CRntString& mode);
void fn_7100df28d0(UnkGameManager* manager, const CRntString& a, const CRntString& b,
                   const CRntString& c, const CStrId& id, bool flag, const CRntString& d,
                   const CStrId& e, u32 f, void* g, f32 h);
void fn_7100decdfc(UnkGameManager* manager, f32 time);
void fn_7100dffd48(UnkGameManager* manager);

// The debug start-up step: with the start-in-game flag, enters the "INGAME" mode around the
// script that initialises the game blackboard, starts the game from the debug strings and runs
// the manager's next step; with the other flag, the script that gives every item and power-up.
void fn_7100000610() {
    if (g_unk_7101d473c1) {
        fn_7100deac9c(g_unk_7101d47310, CRntString("INGAME"));
        fn_7100000790("Init.InitGameBlackboard", "");
        fn_7100deac9c(g_unk_7101d47310, CRntString("INGAME"));
        fn_7100df28d0(g_unk_7101d47310, g_unk_7101d473c8.field_0x0, g_unk_7101d473c8.field_0x30,
                      g_unk_7101d473c8.field_0x90, UnkStartStrId(g_unk_7101d473c8.field_0x60).id,
                      true, CRntString(""), g_unk_7101d470e0, 0, nullptr, 0.0f);
        fn_7100decdfc(g_unk_7101d47310, 0.0f);
    }
    if (g_unk_7101d473c0) {
        fn_7100000790("msemenu.GainAllItemsAndPowerUps", "");
        fn_7100dffd48(g_unk_7101d47310);
    }
}

// ---------------------------------------------------------------------------------------------
// The game's main function (fn_7100000980), which nnMain hands to the main loop: the engine's
// start-up from a settings record and the root path "./", the render manager, fn_7100adfb20 (the
// end of the start-up logo), the "MAINMENU" mode, srand(0), and a first render pass parameter.

#include <stdlib.h>

#include "base/global/CFilePathStrId.h"
#include "game/UnkRenderManager.h"

using base::global::CFilePathStrId;
using base::global::UnkStrIdInline;

extern u32 g_unk_7101a20300;  // a word in .data (reached through the GOT); the first thing set
// Two more of the debug start-up settings in base::global's .bss (after g_unk_7101d473c0 and
// g_unk_7101d473c1 above); what they select is unknown.
extern u8 g_unk_7101d473c2 __attribute__((visibility("hidden")));
extern u8 g_unk_7101d473c4 __attribute__((visibility("hidden")));

// The settings record's functions. fn_7100001714 (a tail call of fn_71010c2080, which tail-calls
// fn_7100132920) and fn_7100001718 belong to unit 2 (tools/tu_map.py) and are taken PC-relatively,
// so they are hidden; the other two are taken through the GOT.
void fn_710109c880();
void fn_7100015fc0(const CFilePathStrId& root);
void fn_7100001714() __attribute__((visibility("hidden")));
void fn_7100001718() __attribute__((visibility("hidden")));

// A callback and the object it is for (none here).
struct UnkStartupCallback {
    void* object;
    void (*function)();
};

// What fn_7100015c70 starts the engine from: it copies +0x0 and the two callbacks into objects it
// creates, calls +0x10 with the root path and passes +0x3b on; +0x8 and +0x38-0x3a are not read
// there.
struct UnkStartupSettings {
    u64 field_0x0;
    void (*field_0x8)();
    void (*field_0x10)(const CFilePathStrId& root);
    UnkStartupCallback field_0x18;
    UnkStartupCallback field_0x28;
    bool field_0x38;
    bool field_0x39;
    u8 field_0x3a;
    bool field_0x3b;
};
static_assert(sizeof(UnkStartupSettings) == 0x40, "UnkStartupSettings layout");

// What the render manager is created and set up from (fn_71002182c0, with the root path):
// fn_71010c2050 fills the two words and the flags at +0xc and +0xd from a global record of the
// same shape (g_unk_7172a02cc8).
struct UnkRenderSetup {
    u32 field_0x0;
    u32 field_0x4;
    f32 field_0x8;  // an aspect ratio (16:9)
    bool field_0xc;
    bool field_0xd;
    u8 field_0xe;
    bool field_0xf;
    bool field_0x10;
};

// Two flags fn_7100adfb20 takes with the root path.
struct UnkStartupFlags {
    bool field_0x0;
    u8 field_0x1;
};

// The paths are passed by value (a copy the caller makes and destroys), except fn_71002182c0's.
void fn_7100015c70(const UnkStartupSettings* settings, CFilePathStrId root);
void fn_71010c2050(u32* a, u32* b, bool* c, bool* d);
void fn_71002182c0(const UnkRenderSetup* setup, const CFilePathStrId* root);
// The flag is passed as 1; the callee stores and passes on 1 whatever it gets.
void fn_7100218a1c(UnkRenderManager* self, bool flag);
// Among other things, ends the console's start-up logo (nn::oe::FinishStartupLogo,
// port/runtime/include/nn/oe.h).
void fn_7100adfb20(const UnkStartupFlags* flags, CFilePathStrId root);
void fn_7100001a50();  // empty

// The order of the settings stores is the one that gives the original's schedule.
void fn_7100000980() {
    UnkStartupSettings settings;
    g_unk_7101a20300 = 0xe5de7;
    settings.field_0x0 = 0;
    settings.field_0x10 = fn_7100015fc0;
    settings.field_0x38 = true;
    settings.field_0x39 = true;
    settings.field_0x3a = g_unk_7101d473c4;
    settings.field_0x8 = fn_710109c880;
    settings.field_0x18 = {nullptr, fn_7100001714};
    settings.field_0x28 = {nullptr, fn_7100001718};
    settings.field_0x3b = false;
    fn_7100015c70(&settings, CFilePathStrId(UnkStrIdInline(), "./", true));

    UnkRenderSetup setup;
    setup.field_0x0 = 0;
    setup.field_0x4 = 0;
    setup.field_0x8 = 16.0f / 9.0f;
    setup.field_0xc = false;
    setup.field_0xd = false;
    setup.field_0xe = true;
    setup.field_0xf = false;
    setup.field_0x10 = false;
    fn_71010c2050(&setup.field_0x0, &setup.field_0x4, &setup.field_0xc, &setup.field_0xd);
    setup.field_0x8 = 16.0f / 9.0f;  // stored again after the call
    setup.field_0xe = g_unk_7101d473c2;
    fn_71002182c0(&setup, &g_unk_7101d470e8);
    fn_7100218a1c(g_unk_7101d47300, true);

    UnkStartupFlags flags = {false, g_unk_7101d473c2};
    fn_7100adfb20(&flags, g_unk_7101d470e8);

    fn_7100deac9c(g_unk_7101d47310, CRntString("MAINMENU"));
    srand(0);
    fn_7100001a50();
    const f32 one = 1.0f;
    fn_7100219294(g_unk_7101d47300, 0x478f99b643ea9c4c, &one, 0);
}

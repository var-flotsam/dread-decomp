// The host argument walk nnMain starts with (src/engine/nnMain.cpp): reads every argument after
// the program's name and keeps none (the release build's option parsing is compiled out). Its
// parameters are unused; nnMain passes 0 and null. In the first translation unit of main
// (tools/tu_map.py: unit 0), apart from nnMain's.

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

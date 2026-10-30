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

// ---------------------------------------------------------------------------------------------
// The shutdown callback nnMain passes to fn_7100132e90 (which calls it before the shutdown
// fn_7100132730): the scene state's shutdown step, then fn_71000810b0 with 100.

// The scene-state singleton (0x150 bytes, created on first use; src/game/UnkLoadingScreen.cpp's).
struct UnkStruct_0x128;
extern UnkStruct_0x128* g_unk_7101d47320;
void fn_7100ef3494(UnkStruct_0x128* self);
void fn_7100f00600(UnkStruct_0x128* self);
void* fn_71000810b0(u32 value);

static inline UnkStruct_0x128* SceneState() {
    UnkStruct_0x128* s = g_unk_7101d47320;
    if (!s) {
        s = static_cast<UnkStruct_0x128*>(fn_71000839a0(0x150, 0x3f));
        fn_7100ef3494(s);
        g_unk_7101d47320 = s;
    }
    return s;
}

void fn_7100000cf0() {
    fn_7100f00600(SceneState());
    fn_71000810b0(100);
}

// ---------------------------------------------------------------------------------------------
// The per-frame system callback nnMain registers (fn_7100000d40; the notification loop
// fn_71001327c0 calls it every frame): the frame time, the engine's and the game's per-frame
// updates (each singleton created on first use), the frame statistics, the player's position and
// morph ball state for the renderer, the render frame, and a periodic step.

#include "base/UnkResKey.h"
#include "base/input/UnkRumbleManager.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkTunableRegistry.h"
#include "game/UnkItemProperties.h"

using base::input::UnkRumbleManager;
using base::reflection::UnkTunableRegistry;

// Creates the object on first use: storage from the default allocator, built by its constructor
// function (as src/engine/UnkEngineStart.cpp's).
template <class T, u64 Size, void (*Ctor)(T*)>
__attribute__((always_inline)) static inline T* Singleton(T*& instance) {
    if (!instance) {
        T* p = static_cast<T*>(fn_71000839a0(Size, 0x3f));
        Ctor(p);
        instance = p;
    }
    return instance;
}

// The frame timer (src/engine/UnkEngineStart.cpp): the frame time at +0x30 and the frame rate it
// aims at at +0x40.
struct UnkGfxFrameSource;
extern UnkGfxFrameSource* g_unk_7101d47328;
struct UnkFrameTimer {
    u8 unk_0x0[0x30];
    f32 frame_time;
    u8 unk_0x34[0xc];
    f32 target_rate;
};
static inline UnkFrameTimer* FrameTimer() {
    return reinterpret_cast<UnkFrameTimer*>(g_unk_7101d47328);
}
// Advances the timer by a frame, the game manager's +0x6a4 being the time scale; the time passed
// since the last frame.
void fn_7100080cb8(UnkFrameTimer* timer, f32 scale);
f32 fn_7100081020(UnkFrameTimer* timer);

// The GUI root (src/game/CCutscene.cpp): whether it runs the frame time, and the time it gives.
extern void* g_unk_7101d47330;
struct UnkGuiRootView {
    u8 unk_0x0[0x58];
    void* field_0x58;
};
bool fn_710119a320(void* root);
f32 fn_710119a3d0(void* root);
void fn_71011994e0(void* root, bool value);

// The engine's singletons (src/engine/UnkEngineStart.cpp) and their per-frame updates.
struct UnkEngineObject_0xf0;
extern UnkEngineObject_0xf0* g_unk_7101d47338;
void fn_7100075db8(UnkEngineObject_0xf0* self);
void fn_7100075e64(UnkEngineObject_0xf0* self, f32 dt);
struct UnkSoundHolder;
extern UnkSoundHolder* g_unk_7101d47340;
void fn_710006cd9c(UnkSoundHolder* holder);
void fn_710006ce38(UnkSoundHolder* holder, f32 dt);
extern UnkRumbleManager* g_unk_7101d47348;
void fn_7100073af8(UnkRumbleManager* self, f32 dt);
void fn_710013dee8(UnkTunableRegistry* self);
// The language singleton (src/game/UnkLanguage.cpp); its per-frame step is empty.
struct UnkLanguage;
extern UnkLanguage* g_unk_7101d47358;
void fn_710119c020(UnkLanguage* language);
void fn_710119d31c(UnkLanguage* language);
// The game's singletons: the sub-area manager (0xae8 bytes), the environment manager (0x4c8), two
// more objects (0x150 and 0x60 bytes) and the loading screen (0x220).
extern void* g_unk_7101d47360;
extern void* g_unk_7101d47368;
void fn_7100ee1f34(void* self);
void fn_7100d8ced8(void* self);
void fn_7100ee49f0(void* self, f32 dt);
void fn_7100d8ef04(void* self, f32 dt);
struct UnkGameObject_0x150;
extern UnkGameObject_0x150* g_unk_7101d47370;
void fn_7100dde0c8(UnkGameObject_0x150* self);
void fn_7100dde21c(UnkGameObject_0x150* self, f32 dt);
struct UnkGameObject_0x60;
extern UnkGameObject_0x60* g_unk_7101d47378;
void fn_7100dde718(UnkGameObject_0x60* self);
void fn_7100dde7a0(UnkGameObject_0x60* self, f32 dt);
struct UnkLoadingScreen;
extern UnkLoadingScreen* g_unk_7101d47380;
void fn_7100e0f89c(UnkLoadingScreen* self);
void fn_7100e134a0(UnkLoadingScreen* self, f32 dt);
void fn_7100f00780(UnkStruct_0x128* self, f32 dt);
// The statistics object (0x78 bytes) the device's memory size is reported to, in KiB.
struct UnkStatsObject;
extern UnkStatsObject* g_unk_7101d47100;
void fn_7100131f48(UnkStatsObject* self);
void fn_7100132248(UnkStatsObject* self, const CStrId& name, f32 value);

// The render manager's frame steps (game/UnkRenderManager.h).
void fn_7100219dcc(UnkRenderManager* self);
bool fn_7100219c4c(UnkRenderManager* self);
void fn_7100219db4(UnkRenderManager* self, f32 a, f32 b, f32 c, bool d);
bool fn_7100219970(UnkRenderManager* self);
void fn_71002192c8(UnkRenderManager* self, f32 dt);
void fn_71002194a4(UnkRenderManager* self);
void fn_71002194e8(UnkRenderManager* self);
// Sets the pass parameter the hash names to a colour.
void fn_710021a0b4(UnkRenderManager* self, const u64* hash, const base::math::CVector4D* value);
extern const base::math::CVector4D g_unk_71016477b4;  // (0, 0, 0, 0)
extern const base::math::CVector4D g_unk_71016477c4;  // (1, 1, 1, 1)

// The game manager's per-frame steps (game/UnkGameManager.h).
bool fn_7100ded930(UnkGameManager* self);
bool fn_7100decc5c(UnkGameManager* self);
struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
struct UnkInventory;
bool fn_71011428bc(UnkInventory* inventory, const UnkQualifiedId& item);
struct UnkFramePlayer {
    u8 unk_0x0[0x44];
    f32 x;
    u8 unk_0x48[0xc];
    f32 y;
    u8 unk_0x58[0xc];
    f32 z;
    u8 unk_0x68[0x208];
    UnkInventory* inventory;
};
// A position built from its three values (all read before any is stored).
struct UnkFramePosition {
    f32 x;
    f32 y;
    f32 z;
    UnkFramePosition(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
};
// The object behind the game manager's +0x498: whether the render frame runs while paused.
struct UnkFramePauseView {
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
    virtual bool vf_0x60();
};

// The default allocator's interface as far as the memory size (slot 0xa0).
struct UnkSizedAllocator {
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
    virtual u64 vf_0xa0();
};

void fn_7100132c30();
void fn_7100132c60();
void fn_710109d0a0();
void fn_7100083980(f32 dt);
void fn_7100001a60(f32 dt);
void fn_71010e537c(void* object);
void fn_71000ebad0(UnkResRegistry* self);

// The frame statistics: a counter, the time accumulated over the last second and its frame count,
// the time to the next periodic step, and whether the player had the morph ball last frame.
extern u32 g_unk_7101d47390 __attribute__((visibility("hidden")));
struct UnkFrameRate {
    f32 time;
    u32 frames;
};
extern UnkFrameRate g_unk_7101d47398 __attribute__((visibility("hidden")));
extern f32 g_unk_7101d47394 __attribute__((visibility("hidden")));
extern bool g_unk_7101d47388 __attribute__((visibility("hidden")));

// NON_MATCHING(minor): the original returns 1 (`mov w0, #1` before the epilogue; nnMain declares the
// callback void, and its only caller, fn_71001327c0, ignores the result), and it zeroes the frame
// rate pair through a fresh adrp instead of the pair's address register (the same 64-bit store).
void fn_7100000d40() {
    fn_7100132c30();
    fn_7100219dcc(g_unk_7101d47300);
    if (!fn_7100deabe4(g_unk_7101d47310))
        FrameTimer()->target_rate = fn_7100219c4c(g_unk_7101d47300) ? 30.0f : 60.0f;
    f32 dt;
    if (fn_710119a320(g_unk_7101d47330)) {
        dt = fn_710119a3d0(g_unk_7101d47330);
    } else {
        fn_7100080cb8(FrameTimer(), *reinterpret_cast<f32*>(reinterpret_cast<char*>(g_unk_7101d47310) + 0x6a4));
        dt = FrameTimer()->frame_time;
    }
    bool paused = fn_710119a320(g_unk_7101d47330);
    if (dt < 0.0001f && paused)
        return;
    fn_7100083980(dt);
    fn_7100075e64(Singleton<UnkEngineObject_0xf0, 0xf0, fn_7100075db8>(g_unk_7101d47338), dt);
    fn_710006ce38(Singleton<UnkSoundHolder, 0x48, fn_710006cd9c>(g_unk_7101d47340), dt);
    fn_7100073af8(Singleton<UnkRumbleManager, 0x2210, fn_71000733e4>(g_unk_7101d47348), dt);
    fn_7100219db4(g_unk_7101d47300, 0.0f, 0.0f, 0.0f, false);
    fn_710013dee8(Singleton<UnkTunableRegistry, 0x58, fn_710013ac14>(g_unk_7101d47350));
    fn_710119d31c(Singleton<UnkLanguage, 0x2068, fn_710119c020>(g_unk_7101d47358));
    fn_7100ee49f0(Singleton<void, 0xae8, fn_7100ee1f34>(g_unk_7101d47360), dt);
    fn_7100d8ef04(Singleton<void, 0x4c8, fn_7100d8ced8>(g_unk_7101d47368), dt);
    fn_7100dde21c(Singleton<UnkGameObject_0x150, 0x150, fn_7100dde0c8>(g_unk_7101d47370), dt);
    fn_7100dde7a0(Singleton<UnkGameObject_0x60, 0x60, fn_7100dde718>(g_unk_7101d47378), dt);
    if (fn_7100ded930(g_unk_7101d47310))
        return;
    Singleton<UnkSoundHolder, 0x48, fn_710006cd9c>(g_unk_7101d47340);
    fn_7100e134a0(Singleton<UnkLoadingScreen, 0x220, fn_7100e0f89c>(g_unk_7101d47380), dt);
    fn_71011994e0(g_unk_7101d47330, false);
    fn_7100decdfc(g_unk_7101d47310, dt);
    fn_7100001a60(dt);
    fn_7100f00780(SceneState(), dt);
    ++g_unk_7101d47390;

    f32 elapsed = fn_7100081020(FrameTimer());
    UnkStatsObject* stats = Singleton<UnkStatsObject, 0x78, fn_7100131f48>(g_unk_7101d47100);
    f32 kib = (f32)reinterpret_cast<UnkSizedAllocator*>(fn_7100083990())->vf_0xa0() * (1.0f / 1024.0f);
    fn_7100132248(stats, CStrId("DEVICE_SIZE", true), kib);
    g_unk_7101d47398.time = elapsed + g_unk_7101d47398.time;
    ++g_unk_7101d47398.frames;
    if (g_unk_7101d47398.time > 1.0f)
        g_unk_7101d47398 = {};

    if (UnkFramePlayer* player = reinterpret_cast<UnkFramePlayer*>(fn_7100dec788(g_unk_7101d47310))) {
        UnkFramePosition pos(player->x, player->y, player->z);
        fn_7100219294(g_unk_7101d47300, 0xee51cf856cc66bdc, &pos.x, 0);
        fn_7100219294(g_unk_7101d47300, 0xee51cf856cc66bdc, &pos.x, 3);
        // The object fn_7100defcec returns is fn_71010e537c's argument (x0 on the console).
        void* object = fn_7100defcec(g_unk_7101d47310);
        if (object && !static_cast<UnkGuiRootView*>(g_unk_7101d47330)->field_0x58)
            fn_71010e537c(object);
        bool morph_ball = false;
        if (UnkInventory* inventory = player->inventory) {
            UnkQualifiedId& qid = ItemQualifiedId(g_unk_717275a8d8);
            if ((u64)qid.id.data == (u64)g_unk_7101d47108 + 0x10)
                fn_7100ae9a10(&qid, ItemSource(g_unk_717275a8d8), g_unk_7101d470e0);
            morph_ball = fn_71011428bc(inventory, qid);
        }
        if (morph_ball != g_unk_7101d47388) {
            g_unk_7101d47388 = morph_ball;
            static const u64 s_hash = 0xce8cd9d5c167d2a1;
            fn_710021a0b4(g_unk_7101d47300, &s_hash, morph_ball ? &g_unk_71016477c4 : &g_unk_71016477b4);
        }
    }

    if (!fn_7100219970(g_unk_7101d47300)) {
        f32 t = 0.0f;
        if (!fn_7100decc5c(g_unk_7101d47310) ||
            static_cast<UnkFramePauseView*>(g_unk_7101d47310->field_0x498)->vf_0x60())
            t = dt;
        fn_71002192c8(g_unk_7101d47300, t);
        fn_71002194a4(g_unk_7101d47300);
        fn_7100132c60();
        fn_71002194e8(g_unk_7101d47300);
    }
    g_unk_7101d47394 = dt + g_unk_7101d47394;
    if (g_unk_7101d47394 >= 0.0f) {
        g_unk_7101d47394 = 0.0f;
        fn_710109d0a0();
    }
    fn_71000ebad0(g_unk_7101d472f8);
}


#include "base/UnkAtExit.h"

// The unit's static initialiser (fn_7100001390): the global empty string id g_unk_7101d470e0
// (base/global/CStrId.h), set to the pool's empty entry behind the guard word after it (an inline
// variable's: the low byte is tested, the word set to 1), and destroyed by fn_710000051c at exit.
extern u64 g_unk_7101d470f0;

void fn_7100001390() {
    if (!(*reinterpret_cast<u8*>(&g_unk_7101d470f0) & 1)) {
        g_unk_7101d470e0.data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        g_unk_7101d470e0.data = &g_unk_7101d47108->empty;
        fn_7100000250(fn_710000051c, &g_unk_7101d470e0, g_unk_7101a19000);
        g_unk_7101d470f0 = 1;
    }
}

#include "game/UnkStartPoint.h"

// The start record's destructor (game/UnkStartPoint.h), which unit 2's initialiser registers.
void fn_7100000584(UnkStartPoint* self) {
    fn_710007ecd4(&self->field_0xc8);
    fn_710007ecd4(&self->start_point);
    fn_710007ecd4(&self->character);
    fn_710007ecd4(&self->field_0x38);
    fn_710007ecd4(&self->field_0x8);
}

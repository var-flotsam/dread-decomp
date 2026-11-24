// Unit 2666 Lua bindings: each reads its arguments from the script stack and forwards them to the game
// manager, the render manager or the sound manager. Names and layouts are placeholders.
#include "base/UnkNamedRegistry.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/snd/CSoundSystem.h"
#include "game/UnkGameManager.h"
#include "game/UnkProfileSaveArg.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU23.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2656.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU53.h"
#include "types.h"

using base::global::CRntString;

struct lua_State;
struct UnkSoundMgr;
struct UnkVec;
struct UnkSoundSystemMasks_ovb0074;
struct UnkObserverEntryMap_7100ca79f0;

// Reads an int and a string argument and does nothing with them.
s32 fn_71010947e0(lua_State* L) {
    s32 number = 0;
    const char* text;
    fn_71010c0270(L, "is", &number, &text);
    return 0;
}

namespace lua_bind {
s32 LogWarn(lua_State* L) {
    s32 number = 0;
    const char* text;
    fn_71010c0270(L, "is", &number, &text);
    return 0;
}

s32 LogError(lua_State* L) {
    s32 number = 0;
    const char* text;
    fn_71010c0270(L, "is", &number, &text);
    return 0;
}

s32 SetPadVibrationFor(lua_State* L) {
    f32 a, b, c;
    return !fn_71010c0270(L, "fff", &a, &b, &c);
}
}  // namespace lua_bind

struct UnkFlagHolder_ovb0156 {
    u8 pad_0x0[0x5d0];
    u8* field_0x5d0;
};

namespace lua_bind {
s32 IsGameModeChangePending(lua_State* L) {
    UnkGameManager* manager = g_unk_7101d47310;
    u8* flag = reinterpret_cast<UnkFlagHolder_ovb0156*>(manager)->field_0x5d0;
    if (flag)
        return fn_71010c0b40(L, "b", *flag != 0);
    return fn_71010c0b40(L, "b", false);
}

s32 IsSFXEnabled(lua_State* L) {
    u8* block = reinterpret_cast<u8*>(g_unk_7101d4d820) + 0x400910;
    return fn_71010c0b40(L, "b", block[0] ? block[6] : 0);
}

s32 SetSlomo(lua_State* L) {
    f32 value;
    if (fn_71010c0270(L, "f", &value))
        fn_7100df6c78(g_unk_7101d47310, value, 0.0f);
    return 0;
}

s32 SetTotalPlayTime(lua_State* L) {
    f32 value = 0.0f;
    if (fn_71010c0270(L, "f", &value))
        fn_7100dffc3c(g_unk_7101d47310, value);
    return 0;
}
}  // namespace lua_bind

struct UnkCounterHolder_ovb0156 {
    u8 pad_0x0[0x48];
    s32 field_0x48;
};
UnkCounterHolder_ovb0156* fn_7100e02920(UnkGameManager* self);

namespace lua_bind {
s32 DecrementNavMeshGeneratorDebugBreakableTileIndex(lua_State*) {
    s32 count = fn_7100e02920(g_unk_7101d47310)->field_0x48;
    s32 next = count >= 0 ? count - 1 : count;
    fn_7100e02920(g_unk_7101d47310)->field_0x48 = next;
    return 0;
}

s32 EnableSceneDraw(lua_State* L) {
    s32 layer;
    bool flag;
    if (fn_71010c0270(L, "bi", &flag, &layer))
        fn_7100219d4c(g_unk_7101d47300, flag, layer);
    return 0;
}
}  // namespace lua_bind

bool fn_7101096d68(s32 a, s32 b);

namespace lua_bind {
s32 LoadGameEnding(lua_State* L) {
    s32 a = -1;
    s32 b = -1;
    return fn_71010c0270(L, "ii", &a, &b) && fn_7101096d68(a, b);
}
}  // namespace lua_bind

void fn_710010805c(UnkSoundMgr* self, f32 v);
void fn_710021a088(UnkRenderManager* self, f32 v);

namespace lua_bind {
s32 SetGlobalPitch(lua_State* L) {
    f32 value = 1.0f;
    if (fn_71010c0270(L, "f", &value))
        fn_710010805c(g_unk_7101d4d820, value);
    return 0;
}

s32 SetSafeFarPlaneFactor(lua_State* L) {
    f32 value = -1.0f;
    if (fn_71010c0270(L, "f", &value))
        fn_710021a088(g_unk_7101d47300, value);
    return 0;
}
}  // namespace lua_bind

struct UnkSceneInner_ovb0156 {
    u8 pad_0x0[0x10];
    const char* field_0x10;
    u8 pad_0x18[0x2c];
    s32 field_0x44;
};
struct UnkSceneHolder_ovb0156 {
    u8 pad_0x0[0x18];
    UnkSceneInner_ovb0156* field_0x18;
};

namespace lua_bind {
s32 GetScenarioID(lua_State* L) {
    UnkSceneHolder_ovb0156* holder = reinterpret_cast<UnkSceneHolder_ovb0156*>(g_unk_7101d47310->field_0x608);
    if (!holder)
        return 0;
    // Counts the use through an untyped store so the inner pointer is read again afterwards.
    s32 uses = holder->field_0x18->field_0x44 + 1;
    holder->field_0x18->field_0x44 = uses;
    const char* name = holder->field_0x18->field_0x10;
    return fn_71010c0b40(L, "s", name ? name : g_unk_710162e2ed);
}
}  // namespace lua_bind

s32 fn_7100108c84(UnkSoundMgr* self);
void fn_7100108c8c(UnkSoundMgr* self, s32 mode);

namespace lua_bind {
s32 SwitchSoundMode(lua_State*) {
    s32 mode = fn_7100108c84(g_unk_7101d4d820);
    if (mode == 1)
        fn_7100108c8c(g_unk_7101d4d820, 2);
    else if (mode == 2)
        fn_7100108c8c(g_unk_7101d4d820, 1);
    return 0;
}

s32 FadeOutStream(lua_State* L) {
    f32 time;
    if (fn_71010c0270(L, "f", &time))
        fn_7100108ae8(g_unk_7101d4d820, 1, time, 0.0f, false);
    return 0;
}
}  // namespace lua_bind

bool fn_7100108c14(UnkSoundMgr* self);
s32 fn_7100108c1c(UnkSoundMgr* self);

namespace lua_bind {
s32 GetReverbConfig(lua_State* L) {
    s32 value = 0;
    if (fn_7100108c14(g_unk_7101d4d820))
        value = fn_7100108c1c(g_unk_7101d4d820) + 1;
    return fn_71010c0b40(L, "i", value);
}

s32 SetEnvironmentVolume(lua_State* L) {
    f32 delay;
    if (fn_71010c0270(L, "f", &delay))
        fn_7100108ae8(g_unk_7101d4d820, 5, 0.25f, delay, true);
    return 0;
}

s32 SaveProfile(lua_State* L) {
    u8 flag;
    if (fn_71010c0270(L, "b", &flag)) {
        u32 bits = flag << 1;
        UnkGameManager* manager = g_unk_7101d47310;
        UnkProfileSaveArg arg = {nullptr, nullptr};
        fn_7100df99f8(manager, bits, &arg, 0);
    }
    return 0;
}
}  // namespace lua_bind

void CGameManager_SetSceneBrightness(void* self, u32 index, s32 level);

namespace lua_bind {
s32 SetSceneBrightness(lua_State* L) {
    s32 index = 0;
    s32 level = 0;
    if (fn_71010c0270(L, "ii", &index, &level))
        CGameManager_SetSceneBrightness(g_unk_7101d47310, index, level);
    return 0;
}

s32 PauseSounds(lua_State* L) {
    bool on = false;
    if (fn_71010c0270(L, "b", &on)) {
        UnkSoundSystemMasks_ovb0074* sound = reinterpret_cast<UnkSoundSystemMasks_ovb0074*>(g_unk_7101d4d820);
        u32 mask = 1;
        fn_7100108464(sound, on, &mask);
    }
    return 0;
}

s32 SetSFXMuted(lua_State* L) {
    bool on = true;
    if (fn_71010c0270(L, "b", &on)) {
        u32 mask = 1;
        fn_71001082a8(reinterpret_cast<UnkSoundSystemMasks_ovb0074*>(g_unk_7101d4d820), on, &mask);
    }
    return 0;
}
}  // namespace lua_bind

void fn_7100df6cd0(UnkGameManager* self, f32 a, f32 b);

namespace lua_bind {
s32 SetSlomoFor(lua_State* L) {
    f32 a = 1.0f;
    f32 b = 1.0f;
    if (fn_71010c0270(L, "ff", &a, &b))
        fn_7100df6cd0(g_unk_7101d47310, a, b);
    return 0;
}
}  // namespace lua_bind

void CBreakableTileManager_ResetTileGroup(UnkObserverEntryMap_7100ca79f0* manager, u32 id, bool flag);

namespace lua_bind {
s32 ResetTileGroup(lua_State* L) {
    s32 id = 0;
    bool flag = false;
    if (fn_71010c0270(L, "ib", &id, &flag))
        CBreakableTileManager_ResetTileGroup(reinterpret_cast<UnkObserverEntryMap_7100ca79f0*>(g_unk_7101d47310->field_0x1af8), id, flag);
    return 0;
}
}  // namespace lua_bind

struct UnkGaugeHolder_ovb0156 {
    u8 pad_0x0[0x2318];
    f32 field_0x2318;
};

namespace lua_bind {
s32 SetSoundListenerFactor(lua_State* L) {
    f32 value = 0.0f;
    if (fn_71010c0270(L, "f", &value)) {
        UnkGaugeHolder_ovb0156* holder = reinterpret_cast<UnkGaugeHolder_ovb0156*>(g_unk_7101d47310);
        f32 clamped = !(value <= 0.0f) ? (!(value >= 1.0f) ? value : 1.0f) : 0.0f;
        holder->field_0x2318 = clamped;
    }
    return 0;
}
}  // namespace lua_bind

s32 fn_7101090c60(lua_State* L) {
    f32 t;
    const char* modifiers;
    if (!fn_71010c0270(L, "fs", &t, &modifiers))
        return 0;
    return fn_71010c0b40(L, "f", (f64)fn_7100076020(t, modifiers));
}

struct UnkPos_ovb0156 {
    f32 x, y, z;
    UnkPos_ovb0156() : x(0.0f), y(0.0f), z(0.0f) {}
};

namespace lua_bind {
s32 FadeOut(lua_State* L) {
    f32 duration;
    if (fn_71010c0270(L, "f", &duration)) {
        u8* effects = g_unk_7101d47310->field_0x1040;
        UnkPos_ovb0156 origin;
        fn_7101053850(effects, reinterpret_cast<UnkVec*>(&origin), 1, duration, 1.0f);
    }
    return 0;
}

s32 ChangeGameMode(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name)) {
        UnkGameManager* manager = g_unk_7101d47310;
        base::UnkNamedEntry key;
        CRntString_ConstructFromCString(&key, name);
        fn_7100deac9c(manager, *reinterpret_cast<CRntString*>(&key));
        CRntString_Destroy(reinterpret_cast<CRntString*>(&key));
    }
    return 0;
}
}  // namespace lua_bind

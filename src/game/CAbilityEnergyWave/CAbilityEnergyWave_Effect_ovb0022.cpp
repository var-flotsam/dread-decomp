// CAbilityEnergyWave's unit (tools/tu_map.py): turns an effect of a world (the player's by default) on or
// off. Names and layouts are placeholders.
#include <new>
#include "base/UnkNamedRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbility.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CFXComponent.h"
#include "game/CGrabComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU1428.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::UnkReflValue;

struct UnkSelf;
struct UnkOwner;
struct UnkObject;
struct UnkComponent;
struct UnkPlayer;
struct UnkWorld;
struct UnkEntry;
struct UnkLifeComponent;
struct UnkSamusGunComponent;
struct UnkScriptCaller;
struct UnkSmLockon_A1_b64;
struct UnkSmAbility3_A2_b63;
struct UnkAbilityC_A2_b69;
struct UnkStruct_71002fda50;
struct UnkWaveChild_A2_b72;
struct UnkStateObject;
struct UnkEnergyWaveCompA_b49;
struct UnkEnergyWaveCompB_b49;
struct UnkEnergyWaveObject_b49;
struct UnkEnergyWaveFxComp_b49;

extern char g_unk_7101648d28[];
extern const u64 g_unk_7101648d18;

// The id table (fn_7100031894 builds it; as src/game/CEmmyAIComponent/CEmmyAIComponent_IsStateId_b56.cpp).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0xf18];
    CStrId field_0xf18;
    u8 pad_0xf20[0x2578 - 0xf20];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}


struct UnkPart;
UnkPart* fn_7101152b90(UnkWorld* world, u64 hash);
bool fn_710114e668(UnkObject*, u8);
void fn_710114fe8c(UnkWorld* world, u64 hash, u32 a, u32 b);
void fn_71002078e4(void* fade, f32 target, f32 from, f32 to, bool a, bool b);

struct UnkEnergyWavePart_ovb22 {
    u8 pad_0x0[0xb];
    u8 kind;  // 0xb
    u8 pad_0xc[0x1c];
    u8 fade[0xf9];  // 0x28
    bool active;    // 0x121
};

struct UnkEnergyWavePlayer_ovb22 {
    u8 pad_0x0[0x280];
    UnkObject* world;  // 0x280
};

// Turns the effect `hash` of `world` (the player's by default) on (switching the first part off and
// fading the second in) or off.
void fn_7100301600(void* self, u64 hash, bool on, UnkObject* world) {
    if (!world)
        world = reinterpret_cast<UnkEnergyWavePlayer_ovb22*>(fn_7100defcd4(g_unk_7101d47310))->world;
    if (!on) {
        fn_710114fe8c(reinterpret_cast<UnkWorld*>(world), hash, 0, 0);
        return;
    }
    auto* part = reinterpret_cast<UnkEnergyWavePart_ovb22*>(fn_7101152b90(reinterpret_cast<UnkWorld*>(world), hash));
    if (part && fn_710114e668(world, part->kind) && part->active)
        fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(part), false);
    part = reinterpret_cast<UnkEnergyWavePart_ovb22*>(fn_7101152b90(reinterpret_cast<UnkWorld*>(world), hash));
    if (part && fn_710114e680(world, part->kind) && part->active)
        fn_71002078e4(part->fade, 0.15f, 0.0f, 1.0f, true, true);
}

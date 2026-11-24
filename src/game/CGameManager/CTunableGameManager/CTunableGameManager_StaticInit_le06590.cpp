// Static initialiser of CGameManager::CTunableGameManager's unit (tools/tu_map.py): the header ids, the four
// game-manager tunables, the SLoadScenarioRequest::EMode and EDifficultyMode enums, then the unit's string
// ids, file paths and strings (Samus' character classes and packs, the save-data keys). Names and layouts
// are placeholders.
#include "../../../game_5821e4/UnkStaticInit.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

using namespace unk_static_init;
using base::global::CRntString;
#include "base/reflection/TypeObject.h"
namespace SLoadScenarioRequest { enum class EMode : u32; }
template <> base::reflection::CType base::reflection::TypeObject<SLoadScenarioRequest::EMode>::object asm("g_unk_71727be938");
enum class EDifficultyMode : u32;
template <> base::reflection::CType base::reflection::TypeObject<EDifficultyMode>::object asm("g_unk_71727bea58");

struct UnkStatics_le06590 {
    HeaderIds ids;
    u8 pad_0x18[0x20];
    UnkStrIdBase samus;                  // 0x38
    UnkStrIdBase samus_zero_low;         // 0x40
    UnkStrIdBase samus_deadsuit;         // 0x48
    UnkFilePathStrIdBase deadsuit_class;  // 0x50
    UnkFilePathStrIdBase common_pack;    // 0x58
    UnkFilePathStrIdBase power_pack;     // 0x60
    UnkFilePathStrIdBase varia_pack;     // 0x68
    UnkFilePathStrIdBase gravity_pack;   // 0x70
    UnkFilePathStrIdBase hyper_pack;     // 0x78
    UnkStrIdBase ghost_aura;             // 0x80
    UnkStrIdBase not_supported;          // 0x88
    UnkStrIdBase magnetic;               // 0x90
    u8 pad_0x98[0x8];
    UnkStrIdBase easy_completed;         // 0xa0
    UnkStrIdBase normal_completed;       // 0xa8
    UnkStrIdBase hard_completed;         // 0xb0
    UnkStrIdBase expert_completed;       // 0xb8
    UnkStrIdBase show_easy_popup;        // 0xc0
    UnkStrIdBase show_boss_rush_popup;   // 0xc8
    UnkStrIdBase show_dread_rush_popup;  // 0xd0
    UnkStrIdBase user_data;              // 0xd8
    u8 pad_0xe0[0x60];
    f32 field_0x140;
    f32 field_0x144;
    bool field_0x148;
    u8 pad_0x149[0x57];
    CRntString samus_zero;               // 0x1a0
    CRntString samus_zero_parts;         // 0x1d0
    u8 pad_0x200[0x60];
    u8 game_manager_entry[8];            // 0x260
    u8 pad_0x268[0x130];
    u8 graphic_settings_entry[8];        // 0x398
    u8 pad_0x3a0[0x130];
    u8 music_volume_entry[8];            // 0x4d0
    u8 pad_0x4d8[0x130];
    u8 enemies_entry[8];                 // 0x608
};
static_assert(__builtin_offsetof(UnkStatics_le06590, field_0x140) == 0x140, "layout");
static_assert(__builtin_offsetof(UnkStatics_le06590, samus_zero) == 0x1a0, "layout");
static_assert(__builtin_offsetof(UnkStatics_le06590, game_manager_entry) == 0x260, "layout");
static_assert(__builtin_offsetof(UnkStatics_le06590, enemies_entry) == 0x608, "layout");
extern UnkStatics_le06590 g_unk_71727bed98 __attribute__((visibility("hidden")));

// The graphic settings tunable, as far as its default gamma.
struct UnkGraphicSettingsTunable_le06590 : UnkTunable {
    u8 pad_0x8[0x1c];
    f32 gamma;  // 0x24
};

// As src/game/GUI/CCustomMarkersDialog/fn_7100afe1d0.cpp defines it.
struct UnkRefPtr;
struct UnkRefPtrArray {
    UnkRefPtr* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};
void fn_7100de49cc(UnkRefPtrArray* self);

extern char g_unk_71727be868[];
extern UnkTunable g_unk_71727be870;
extern void* g_unk_7101aa99e0[];
extern char g_unk_71727be8a1[];
extern void* g_unk_7101aa9a10[];
extern char g_unk_71727be8d1[];
extern UnkTunable g_unk_71727be8d8;
extern void* g_unk_7101aa9a40[];
extern char g_unk_71727be901[];
extern UnkTunable g_unk_71727be908;
extern void* g_unk_7101aa9a70[];

void fn_7100de441c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100de42d0();
extern char g_unk_71727bea50[];
void fn_7100de47a8(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100de459c();
extern char g_unk_71727beb70[];
extern UnkConversion g_unk_71727beb78;
extern UnkConversion g_unk_71727beb98;
void fn_7100e04e68() __attribute__((visibility("hidden")));
void fn_7100e04e74() __attribute__((visibility("hidden")));

void fn_7100e06590() {
    UnkStatics_le06590& s = g_unk_71727bed98;
    InitHeaderIds(&s.ids);
    s.field_0x140 = -1.0f;
    s.field_0x144 = -1.0f;
    s.field_0x148 = true;

    fn_7100de2780();
    fn_710009ec00(g_unk_71727be868, s.game_manager_entry);
    InitTunable(&g_unk_71727be870, fn_7100136ecc, g_unk_7101aa99e0, fn_7100075104);
    fn_7100de30dc();
    fn_710009ec00(g_unk_71727be8a1, s.graphic_settings_entry);
    UnkGraphicSettingsTunable_le06590& graphics = *reinterpret_cast<UnkGraphicSettingsTunable_le06590*>(g_unk_71727be8a8);
    fn_7100136ecc(&graphics);
    graphics.vtable = g_unk_7101aa9a10 + 2;
    graphics.gamma = 2.2f;
    fn_7100000250(fn_7100075104, &graphics, g_unk_7101a19000);
    fn_7100de368c();
    fn_710009ec00(g_unk_71727be8d1, s.music_volume_entry);
    InitTunable(&g_unk_71727be8d8, fn_7100136ecc, g_unk_7101aa9a40, fn_7100075104);
    fn_7100de3d24();
    fn_710009ec00(g_unk_71727be901, s.enemies_entry);
    InitTunable(&g_unk_71727be908, fn_7100136ecc, g_unk_7101aa9a70, fn_7100075104);

    UnkEnumType* mode = reinterpret_cast<UnkEnumType*>(&base::reflection::TypeObject<SLoadScenarioRequest::EMode>::object);
    {
        CStrId name("SLoadScenarioRequest::EMode", true);
        fn_7100de441c(mode, name, fn_7100de42d0, nullptr);
    }
    fn_7100000250(CEnumType_Destroy, mode, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727bea50, mode);
    InitEnum(reinterpret_cast<UnkEnumType*>(&base::reflection::TypeObject<EDifficultyMode>::object), "EDifficultyMode", fn_7100de47a8, fn_7100de459c,
             g_unk_71727beb70, &g_unk_71727beb78, fn_7100e04e68, &g_unk_71727beb98, fn_7100e04e74);

    g_unk_71727bebb8 = 6;
    InitId(&s.samus, "samus");
    InitId(&s.samus_zero_low, "samuszerolow");
    InitId(&s.samus_deadsuit, "samus_deadsuit");
    InitPathId(&s.deadsuit_class, "actors/props/samus_deadsuit/charclasses/samus_deadsuit.bmsad");
    new (&s.samus_zero) CRntString("samuszero");
    fn_7100000250(reinterpret_cast<void (*)(void*)>(CRntString_Destroy), &s.samus_zero, g_unk_7101a19000);
    new (&s.samus_zero_parts) CRntString("samuszeroparts");
    fn_7100000250(reinterpret_cast<void (*)(void*)>(CRntString_Destroy), &s.samus_zero_parts, g_unk_7101a19000);
    InitPathId(&s.common_pack, "packs/players/common.pkg");
    InitPathId(&s.power_pack, "packs/players/power.pkg");
    InitPathId(&s.varia_pack, "packs/players/varia.pkg");
    InitPathId(&s.gravity_pack, "packs/players/gravity.pkg");
    InitPathId(&s.hyper_pack, "packs/players/hyper.pkg");
    InitId(&s.ghost_aura, "GhostAura");
    InitId(&s.not_supported, "CharacterNotSupported");
    InitId(&s.magnetic, "Magnetic");

    UnkRefPtrArray& array = reinterpret_cast<UnkRefPtrArray&>(g_unk_71727bebc0);
    array.step = 10;
    array.field_0x14 = true;
    array.data = nullptr;
    array.count = 0;
    array.capacity = 0;
    array.flags = 15;
    array.allocator = GetDefaultAllocator();
    fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_7100de49cc), &array, g_unk_7101a19000);

    InitId(&s.easy_completed, "EasyModeCompleted");
    InitId(&s.normal_completed, "NormalModeCompleted");
    InitId(&s.hard_completed, "HardModeCompleted");
    InitId(&s.expert_completed, "ExpertModeCompleted");
    InitId(&s.show_easy_popup, "ShowEasyModePopUp");
    InitId(&s.show_boss_rush_popup, "ShowBossRushPopUp");
    InitId(&s.show_dread_rush_popup, "ShowDreadRushPopUp");
    InitId(&s.user_data, "USER_DATA");
}

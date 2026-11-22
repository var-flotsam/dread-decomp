// Static initialiser of CLogicDroppedItemManager::CTunableDroppableBillboard's unit: the header ids, the pickup
// effect and its sound presets, the drops' ids, the billboard animations' ids, the billboard tunable, the
// drops' textures and materials, and three empty sound handles. Names and layouts are placeholders.
#include "../../../game_5821e4/UnkStaticInit.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CLogicDroppedItemManager/CTunableDroppableBillboard.h"
#include "game/UnkDroppableTunable.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2561.h"
#include "game/UnkTU2562.h"
#include "game/UnkTU57.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using namespace unk_static_init;
using base::global::CRntString;

struct UnkStatics_le1dde0 {
    HeaderIds ids;
    UnkFilePathStrIdBase pick_effect;      // 0x18
    UnkStrIdBase dissolve;                 // 0x20
    u8 pad_0x28[0x8];
    UnkFilePathStrIdBase textures[2];      // 0x30
    UnkFilePathStrIdBase ammo_material;    // 0x40
    UnkFilePathStrIdBase glow_material;    // 0x48
    UnkFilePathStrIdBase minix_material;   // 0x50
    UnkFilePathStrIdBase life_material;    // 0x58
    f32 field_0x60;
    f32 field_0x64;
    bool field_0x68;
    u8 pad_0x69[0x7];
    CRntString enemy_presets;              // 0x70
    CRntString gui_presets;                // 0xa0
    CRntString small_suffix;               // 0xd0
    CRntString medium_suffix;              // 0x100
    CRntString big_suffix;                 // 0x130
    u8 billboard_entry[8];                 // 0x160
};
static_assert(__builtin_offsetof(UnkStatics_le1dde0, field_0x60) == 0x60, "layout");
static_assert(__builtin_offsetof(UnkStatics_le1dde0, billboard_entry) == 0x160, "layout");
extern UnkStatics_le1dde0 g_unk_71727c1358 __attribute__((visibility("hidden")));

void fn_7100e1dbc0(void*) __attribute__((visibility("hidden")));
extern char g_unk_71727c10c0[];
extern UnkSoundHandle g_unk_71727c1328;
extern UnkSoundHandle g_unk_71727c1338;
extern UnkSoundHandle g_unk_71727c1348;

static inline void InitString_le1dde0(CRntString* string, const char* text) {
    new (string) CRntString(text);
    fn_7100000250(reinterpret_cast<void (*)(void*)>(CRntString_Destroy), string, g_unk_7101a19000);
}

void fn_7100e1dde0() {
    UnkStatics_le1dde0& s = g_unk_71727c1358;
    InitHeaderIds(&s.ids);
    s.field_0x60 = -1.0f;
    s.field_0x64 = -1.0f;
    s.field_0x68 = true;
    InitPathId(&s.pick_effect, "system/fx/generic/onpick.bcptl");
    InitId(&s.dissolve, "dissolve");
    InitString_le1dde0(&s.enemy_presets, "audiopreset:system/snd/presets/enemies/");
    InitString_le1dde0(&s.gui_presets, "audiopreset:system/snd/presets/gui/");
    InitString_le1dde0(&s.small_suffix, "_small");
    InitString_le1dde0(&s.medium_suffix, "_medium");
    InitString_le1dde0(&s.big_suffix, "_big");

    InitId(&g_unk_71727c1048, "LIFE");
    InitId(&g_unk_71727c1050, "LIFEBIG");
    InitId(&g_unk_71727c1058, "MISSILE");
    InitId(&g_unk_71727c1060, "MISSILEBIG");
    InitId(&g_unk_71727c1068, "POWERBOMB");
    InitId(&g_unk_71727c1070, "POWERBOMBBIG");
    InitId(&g_unk_71727c1078, "MINIX");
    InitId(&g_unk_71727c1080, "relax");

    new (&g_unk_71727c1088[0]) CStrId(g_unk_710162e2ed, true);
    new (&g_unk_71727c1088[1]) CStrId("MoveUpDown", true);
    new (&g_unk_71727c1088[2]) CStrId("Spin", true);
    new (&g_unk_71727c1088[3]) CStrId("Scl", true);
    new (&g_unk_71727c1088[4]) CStrId("Sclfadein", true);
    new (&g_unk_71727c1088[5]) CStrId("SpinScl", true);
    new (&g_unk_71727c1088[6]) CStrId("X-Parasite", true);
    fn_7100000250(fn_7100e1dbc0, nullptr, g_unk_7101a19000);

    fn_7100e1e360();
    fn_710009ec00(g_unk_71727c10c0, s.billboard_entry);

    UnkDroppableTunable& tunable = g_unk_71727c10c8;
    fn_7100136ecc(&tunable);
    tunable.vtable = g_unk_7101aa9cc8 + 2;
    fn_710007e6c4(&tunable.sMiniXAbsorbSmallGroupPresetName);
    fn_710007e6c4(&tunable.sMiniXAbsorbMediumGroupPresetName);
    fn_710007e6c4(&tunable.sMiniXAbsorbLargeGroupPresetName);
    fn_710007e6c4(&tunable.sMiniXFlyPresetName);
    fn_710007e6c4(&tunable.sDropAbsorbLifePresetName);
    fn_710007e6c4(&tunable.sDropAbsorbLifeBigPresetName);
    fn_710007e6c4(&tunable.sDropAbsorbMissilePresetName);
    fn_710007e6c4(&tunable.sDropAbsorbPowerBombPresetName);
    fn_710007e6c4(&tunable.sAbsorbingSoundPresetName);
    fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_7100e20ba0), &tunable, g_unk_7101a19000);

    InitPathId(&s.textures[0], "system/fx/textures/droppables01.bctex");
    InitPathId(&s.textures[1], "system/fx/textures/droppables00.bctex");
    InitPathId(&s.ammo_material, "actors/items/item_ammo/imats/item_ammo.bsmat");
    InitPathId(&s.glow_material, "actors/items/item_ammo/imats/item_glow.bsmat");
    InitPathId(&s.minix_material, "actors/items/item_minix/imats/item_minix.bsmat");
    InitPathId(&s.life_material, "actors/items/drop_life/fx/imats/drop_life.bsmat");

    g_unk_71727c1328 = g_unk_7108b6c460;
    g_unk_71727c1338 = g_unk_7108b6c460;
    g_unk_71727c1348 = g_unk_7108b6c460;
}

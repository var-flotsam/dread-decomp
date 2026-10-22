// Static initialiser of the unit of the CAutsniperShootAttack::ELaserState, CAutsniperShootAttack::EAutsniperState.
#include "../game_s7/UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_71003d8910 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkFilePathStrIdBase field_0x38;
    UnkFilePathStrIdBase field_0x40;
    UnkFilePathStrIdBase field_0x48;
    UnkFilePathStrIdBase field_0x50;
    UnkStrIdBase field_0x58;
    UnkStrIdBase field_0x60;
    u8 pad_0x68[0x10];
    u8 field_0x78[8];
};
extern UnkStatics_71003d8910 g_unk_71726bf7f8 __attribute__((visibility("hidden")));
extern char g_unk_71726bf530[];
void* fn_71003d2cd4();  // CAutsniperShootAttack::refl_GetClass
extern UnkEnumType g_unk_71726bf538;
void fn_71003d32d0(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71003d2f44();
extern char g_unk_71726bf650[];
extern UnkConversion g_unk_71726bf658;
extern UnkConversion g_unk_71726bf678;
void fn_71003d8448() __attribute__((visibility("hidden")));
void fn_71003d8454() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726bf698;
void fn_71003d35fc(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71003d3450();
extern char g_unk_71726bf7b0[];
extern UnkConversion g_unk_71726bf7b8;
extern UnkConversion g_unk_71726bf7d8;
void fn_71003d8460() __attribute__((visibility("hidden")));
void fn_71003d846c() __attribute__((visibility("hidden")));

void fn_71003d8910() {
    InitHeaderIds(&g_unk_71726bf7f8.ids);
    fn_71003d2cd4();
    fn_710009ec00(g_unk_71726bf530, g_unk_71726bf7f8.field_0x78);
    InitEnum(&g_unk_71726bf538, "CAutsniperShootAttack::ELaserState", fn_71003d32d0, fn_71003d2f44, g_unk_71726bf650, &g_unk_71726bf658, fn_71003d8448, &g_unk_71726bf678, fn_71003d8454);
    InitEnum(&g_unk_71726bf698, "CAutsniperShootAttack::EAutsniperState", fn_71003d35fc, fn_71003d3450, g_unk_71726bf7b0, &g_unk_71726bf7b8, fn_71003d8460, &g_unk_71726bf7d8, fn_71003d846c);
    InitId(&g_unk_71726bf7f8.field_0x20, "CannonMixer");
    InitId(&g_unk_71726bf7f8.field_0x28, "Aim");
    InitId(&g_unk_71726bf7f8.field_0x30, "Shoot");
    InitPathId(&g_unk_71726bf7f8.field_0x38, "actors/autsniper/autsniper_loop_charge_laser.wav");
    InitPathId(&g_unk_71726bf7f8.field_0x40, "actors/autsniper/walk_slowly_loop_01.wav");
    InitPathId(&g_unk_71726bf7f8.field_0x48, "actors/characters/autsniper/fx/autimpact.bcptl");
    InitPathId(&g_unk_71726bf7f8.field_0x50, "actors/characters/autsniper/fx/autlaser.bcptl");
    InitId(&g_unk_71726bf7f8.field_0x58, "Shoot");
    InitId(&g_unk_71726bf7f8.field_0x60, "AutsniperLaserDamageID");
}

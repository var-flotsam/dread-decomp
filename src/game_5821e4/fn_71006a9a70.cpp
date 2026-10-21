// Static initialiser of the unit of the CShakernautPiercingLaserAttack::EState, CShakernautPiercingLaserAttack::ESide enums.
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_71006a9a70 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    UnkStrIdBase field_0x40;
    UnkStrIdBase field_0x48;
    UnkStrIdBase field_0x50;
    UnkStrIdBase field_0x58;
    UnkStrIdBase field_0x60;
    UnkStrIdBase field_0x68;
    UnkStrIdBase field_0x70;
    UnkStrIdBase field_0x78;
    UnkStrIdBase field_0x80;
    UnkStrIdBase field_0x88;
    UnkStrIdBase field_0x90;
    UnkStrIdBase field_0x98;
    UnkStrIdBase field_0xa0;
    UnkFilePathStrIdBase field_0xa8;
    u8 pad_0xb0[0x20];
    u8 field_0xd0[8];
};
extern UnkStatics_71006a9a70 g_unk_7172705c50 __attribute__((visibility("hidden")));
void* fn_71006a6330();  // CShakernautPiercingLaserAttack::refl_GetClass
extern char g_unk_7172705988[];
extern UnkEnumType g_unk_7172705990;
void fn_71006a6938(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71006a65ac();
extern char g_unk_7172705aa8[];
extern UnkConversion g_unk_7172705ab0;
extern UnkConversion g_unk_7172705ad0;
void fn_71006a93a0() __attribute__((visibility("hidden")));
void fn_71006a93ac() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_7172705af0;
void fn_71006a6c64(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71006a6ab8();
extern char g_unk_7172705c08[];
extern UnkConversion g_unk_7172705c10;
extern UnkConversion g_unk_7172705c30;
void fn_71006a93b8() __attribute__((visibility("hidden")));
void fn_71006a93c4() __attribute__((visibility("hidden")));

void fn_71006a9a70() {
    InitHeaderIds(&g_unk_7172705c50.ids);
    fn_71006a6330();
    fn_710009ec00(g_unk_7172705988, g_unk_7172705c50.field_0xd0);
    InitEnum(&g_unk_7172705990, "CShakernautPiercingLaserAttack::EState", fn_71006a6938, fn_71006a65ac,
             g_unk_7172705aa8, &g_unk_7172705ab0, fn_71006a93a0, &g_unk_7172705ad0, fn_71006a93ac);
    InitEnum(&g_unk_7172705af0, "CShakernautPiercingLaserAttack::ESide", fn_71006a6c64, fn_71006a6ab8,
             g_unk_7172705c08, &g_unk_7172705c10, fn_71006a93b8, &g_unk_7172705c30, fn_71006a93c4);
    InitId(&g_unk_7172705c50.field_0x20, "right_attack_piercinglaser_relaxtoattack");
    InitId(&g_unk_7172705c50.field_0x28, "left_attack_piercinglaser_relaxtoattack");
    InitId(&g_unk_7172705c50.field_0x30, "attack_piercinglaser_chargeloop");
    InitId(&g_unk_7172705c50.field_0x38, "attack_piercinglaser_shot");
    InitId(&g_unk_7172705c50.field_0x40, "attack_piercinglaser_shotloop");
    InitId(&g_unk_7172705c50.field_0x48, "attack_piercinglaser_endtorelax");
    InitId(&g_unk_7172705c50.field_0x50, "attack_piercinglaser_endturn");
    InitId(&g_unk_7172705c50.field_0x58, "attack_piercinglaser_relocate");
    InitId(&g_unk_7172705c50.field_0x60, "ChargeAim");
    InitId(&g_unk_7172705c50.field_0x68, "ChargeLoopAim");
    InitId(&g_unk_7172705c50.field_0x70, "ShotAim");
    InitId(&g_unk_7172705c50.field_0x78, "ShotloopAim");
    InitId(&g_unk_7172705c50.field_0x80, "EyeLaserDamageID");
    InitId(&g_unk_7172705c50.field_0x88, "left_");
    InitId(&g_unk_7172705c50.field_0x90, "right_");
    InitId(&g_unk_7172705c50.field_0x98, "relax");
    InitId(&g_unk_7172705c50.field_0xa0, "floorslide");
    InitPathId(&g_unk_7172705c50.field_0xa8, "actors/characters/shakernaut/fx/piercinglaser_impact.bcptl");
}

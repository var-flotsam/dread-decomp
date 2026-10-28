// Static initialiser of the unit of the CShakernautDoubleGroundShockAttack::EState, CShakernautDoubleGroundShockAttack::ESide enums.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71006a5da0 {
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
    UnkStrIdBase field_0xa8;
    u8 field_0xb0[8];
};
extern UnkStatics_71006a5da0 g_unk_71727057a0 __attribute__((visibility("hidden")));
extern char g_unk_71727054d8[];
extern UnkEnumType g_unk_71727054e0;
void fn_71006a3d0c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71006a37a0();
extern char g_unk_71727055f8[];
extern UnkConversion g_unk_7172705600;
extern UnkConversion g_unk_7172705620;
void fn_71006a5780() __attribute__((visibility("hidden")));
void fn_71006a578c() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_7172705640;
void fn_71006a4038(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71006a3e8c();
extern char g_unk_7172705758[];
extern UnkConversion g_unk_7172705760;
extern UnkConversion g_unk_7172705780;
void fn_71006a5798() __attribute__((visibility("hidden")));
void fn_71006a57a4() __attribute__((visibility("hidden")));

void fn_71006a5da0() {
    InitHeaderIds(&g_unk_71727057a0.ids);
    fn_71006a3524();
    fn_710009ec00(g_unk_71727054d8, g_unk_71727057a0.field_0xb0);
    InitEnum(&g_unk_71727054e0, "CShakernautDoubleGroundShockAttack::EState", fn_71006a3d0c, fn_71006a37a0,
             g_unk_71727055f8, &g_unk_7172705600, fn_71006a5780, &g_unk_7172705620, fn_71006a578c);
    InitEnum(&g_unk_7172705640, "CShakernautDoubleGroundShockAttack::ESide", fn_71006a4038, fn_71006a3e8c,
             g_unk_7172705758, &g_unk_7172705760, fn_71006a5798, &g_unk_7172705780, fn_71006a57a4);
    InitId(&g_unk_71727057a0.field_0x20, "left_attack_groundshock_relaxtoattack");
    InitId(&g_unk_71727057a0.field_0x28, "right_attack_groundshock_relaxtoattack");
    InitId(&g_unk_71727057a0.field_0x30, "right_lasertogroundshock_init");
    InitId(&g_unk_71727057a0.field_0x38, "left_lasertogroundshock_init");
    InitId(&g_unk_71727057a0.field_0x40, "left_");
    InitId(&g_unk_71727057a0.field_0x48, "right_");
    InitId(&g_unk_71727057a0.field_0x50, "attack_groundshock_shotinit_firstarm");
    InitId(&g_unk_71727057a0.field_0x58, "attack_groundshock_shotinit_secondarm");
    InitId(&g_unk_71727057a0.field_0x60, "attack_groundshock_chargeloop_firstarm");
    InitId(&g_unk_71727057a0.field_0x68, "attack_groundshock_chargeloop_secondarm");
    InitId(&g_unk_71727057a0.field_0x70, "attack_groundshock_shot_firstarm");
    InitId(&g_unk_71727057a0.field_0x78, "attack_groundshock_shot_secondarm");
    InitId(&g_unk_71727057a0.field_0x80, "attack_groundshock_shotend_firstarm");
    InitId(&g_unk_71727057a0.field_0x88, "attack_groundshock_shotend_secondarm");
    InitId(&g_unk_71727057a0.field_0x90, "attack_groundshock_changearm");
    InitId(&g_unk_71727057a0.field_0x98, "attack_groundshock_endtorelax");
    InitId(&g_unk_71727057a0.field_0xa0, "attack_groundshock_turn_secondarm");
    InitId(&g_unk_71727057a0.field_0xa8, "relax");
}

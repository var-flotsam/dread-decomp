// Static initialiser of a reflected unit: the header ids, then the unit's own statics and
// registrations (helpers in UnkStaticInitS7.h). Layouts are placeholders.
#include "../../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71004d57f0 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
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
    u8 pad_0x78[0x28];
    u8 field_0xa0[8];
    u8 pad_0xa8[0x18];
    u8 field_0xc0[8];
    u8 pad_0xc8[0x18];
    u8 field_0xe0[8];
    u8 pad_0xe8[0x18];
    u8 field_0x100[8];
    u8 pad_0x108[0x18];
    u8 field_0x120[8];
    u8 pad_0x128[0x18];
    u8 field_0x140[8];
    u8 pad_0x148[0x18];
    u8 field_0x160[8];
    u8 pad_0x168[0x18];
    u8 field_0x180[8];
};
extern UnkStatics_71004d57f0 g_unk_71726d86d8 __attribute__((visibility("hidden")));
void fn_71004cee70(void* self, const UnkStrIdBase* ids, u32 count);
void fn_71004cf1d4(void* self);  // its destructor, as registered
extern char g_unk_71726d86d0[];

void fn_71004d57f0() {
    InitHeaderIds(&g_unk_71726d86d8.ids);
    InitId(&g_unk_71726d86d8.field_0x18, "head");
    InitId(&g_unk_71726d86d8.field_0x20, "head_mlm");
    InitId(&g_unk_71726d86d8.field_0x28, "head_grab");
    InitId(&g_unk_71726d86d8.field_0x30, "CooldownXBoss");
    [] {
        UnkStrIdTmp ids[] = {UnkStrIdTmp("WeakPoint_LeftUp", true), UnkStrIdTmp("WeakPoint_LeftDown", true), UnkStrIdTmp("WeakPoint_RightUp", true), UnkStrIdTmp("WeakPoint_RightDown", true)};
        fn_71004cee70(g_unk_71726d86d8.field_0xa0, ids, 4);
    }();
    __cxa_atexit(fn_71004cf1d4, g_unk_71726d86d8.field_0xa0, g_unk_7101a19000);
    [] {
        UnkStrIdTmp ids[] = {UnkStrIdTmp("DC_L_Hand", true), UnkStrIdTmp("DC_L_Foot", true), UnkStrIdTmp("DC_R_Hand", true), UnkStrIdTmp("DC_R_Foot", true)};
        fn_71004cee70(g_unk_71726d86d8.field_0xc0, ids, 4);
    }();
    __cxa_atexit(fn_71004cf1d4, g_unk_71726d86d8.field_0xc0, g_unk_7101a19000);
    [] {
        UnkStrIdTmp ids[] = {UnkStrIdTmp("L01_Arm_Hand_WeakPoint_02_MESH", true), UnkStrIdTmp("L02_Arm_Hand_WeakPoint_02_MESH", true), UnkStrIdTmp("R01_Arm_Hand_WeakPoint_02_MESH", true), UnkStrIdTmp("R02_Arm_Hand_WeakPoint_02_MESH", true)};
        fn_71004cee70(g_unk_71726d86d8.field_0xe0, ids, 4);
    }();
    __cxa_atexit(fn_71004cf1d4, g_unk_71726d86d8.field_0xe0, g_unk_7101a19000);
    [] {
        UnkStrIdTmp ids[] = {UnkStrIdTmp("L01_Arm", true), UnkStrIdTmp("L02_Arm", true), UnkStrIdTmp("R01_Arm", true), UnkStrIdTmp("R02_Arm", true)};
        fn_71004cee70(g_unk_71726d86d8.field_0x100, ids, 4);
    }();
    __cxa_atexit(fn_71004cf1d4, g_unk_71726d86d8.field_0x100, g_unk_7101a19000);
    [] {
        UnkStrIdTmp ids[] = {UnkStrIdTmp("L01_Arm_FX", true), UnkStrIdTmp("L02_Arm_FX", true), UnkStrIdTmp("R01_Arm_FX", true), UnkStrIdTmp("R02_Arm_FX", true)};
        fn_71004cee70(g_unk_71726d86d8.field_0x120, ids, 4);
    }();
    __cxa_atexit(fn_71004cf1d4, g_unk_71726d86d8.field_0x120, g_unk_7101a19000);
    [] {
        UnkStrIdTmp ids[] = {UnkStrIdTmp("L01_ArmScales", true), UnkStrIdTmp("L02_ArmScales", true), UnkStrIdTmp("R01_ArmScales", true), UnkStrIdTmp("R02_ArmScales", true)};
        fn_71004cee70(g_unk_71726d86d8.field_0x140, ids, 4);
    }();
    __cxa_atexit(fn_71004cf1d4, g_unk_71726d86d8.field_0x140, g_unk_7101a19000);
    [] {
        UnkStrIdTmp ids[] = {UnkStrIdTmp("onimpact_weakpoint_leftup", true), UnkStrIdTmp("onimpact_weakpoint_leftdown", true), UnkStrIdTmp("onimpact_weakpoint_rightup", true), UnkStrIdTmp("onimpact_weakpoint_rightdown", true)};
        fn_71004cee70(g_unk_71726d86d8.field_0x160, ids, 4);
    }();
    __cxa_atexit(fn_71004cf1d4, g_unk_71726d86d8.field_0x160, g_unk_7101a19000);
    InitId(&g_unk_71726d86d8.field_0x38, "cooldownxbossweakpoint");
    InitId(&g_unk_71726d86d8.field_0x40, "Mixer01");
    InitId(&g_unk_71726d86d8.field_0x48, "Mixer02");
    InitId(&g_unk_71726d86d8.field_0x50, "Mixer03");
    InitId(&g_unk_71726d86d8.field_0x58, "Mixer04");
    InitId(&g_unk_71726d86d8.field_0x60, "ShootingGrabDamage");
    InitId(&g_unk_71726d86d8.field_0x68, "attack_windtunnel_tired2reaper");
    InitId(&g_unk_71726d86d8.field_0x70, "fan_cooldown_000");
    fn_71004cf318();
    fn_710009ec00(g_unk_71726d86d0, g_unk_71726d86d8.field_0x180);
}

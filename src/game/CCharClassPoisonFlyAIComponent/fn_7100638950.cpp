// Static initialiser of the unit of the CCharClassPoisonFlyAIComponent::ESubspecies enum.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUProperty.h"

using namespace unk_static_init;

struct UnkStatics_7100638950 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    u8 field_0x28[8];
    u8 pad_0x30[0x130];
    u8 field_0x160[8];
};
extern UnkStatics_7100638950 g_unk_71726fa2e0 __attribute__((visibility("hidden")));
extern char g_unk_71726fa0f0[];
extern UnkTunable g_unk_71726fa0f8;
extern void* g_unk_7101a584e0[];
void fn_7100075104(void* self);  // its destructor, as registered
extern UnkEnumType g_unk_71726fa160;
void fn_7100638090(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100637f44();
extern char g_unk_71726fa278[];
extern UnkConversion g_unk_71726fa280;
extern UnkConversion g_unk_71726fa2a0;
void fn_71006387b8() __attribute__((visibility("hidden")));
void fn_71006387c4() __attribute__((visibility("hidden")));
extern char g_unk_71726fa2c0[];
extern UnkStrIdBase g_unk_71726fa2c8;
extern UnkStrIdBase g_unk_71726fa2d0;
extern UnkStrIdBase g_unk_71726fa2d8;

void fn_7100638950() {
    InitHeaderIds(&g_unk_71726fa2e0.ids);
    fn_71006367c4();
    fn_710009ec00(g_unk_71726fa0f0, g_unk_71726fa2e0.field_0x28);
    InitTunable(&g_unk_71726fa0f8, fn_7100136ecc, g_unk_7101a584e0, fn_7100075104);
    InitEnum(&g_unk_71726fa160, "CCharClassPoisonFlyAIComponent::ESubspecies", fn_7100638090, fn_7100637f44,
             g_unk_71726fa278, &g_unk_71726fa280, fn_71006387b8, &g_unk_71726fa2a0, fn_71006387c4);
    fn_7100638214();
    fn_710009ec00(g_unk_71726fa2c0, g_unk_71726fa2e0.field_0x160);
    InitId(&g_unk_71726fa2c8, "chaseturn");
    InitId(&g_unk_71726fa2d0, "fleeturn");
    InitId(&g_unk_71726fa2d8, "patrolturn");
}

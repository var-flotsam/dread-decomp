// Static initialiser of the unit of the EForceOpticCamouflageMode enum.
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_7100690990 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    u8 pad_0x28[0x10];
    u8 field_0x38[8];
    u8 pad_0x40[0x130];
    u8 field_0x170[8];
};
extern UnkStatics_7100690990 g_unk_7172703f08 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_7172703d20;
void fn_710068de8c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710068dce0();
extern char g_unk_7172703e38[];
extern UnkConversion g_unk_7172703e40;
extern UnkConversion g_unk_7172703e60;
void fn_71006907c4() __attribute__((visibility("hidden")));
void fn_71006907d0() __attribute__((visibility("hidden")));
void* fn_710068e010();  // CCharClassScorpiusAIComponent::CTunableCharClassScorpiusAIComponent::refl_GetClass
extern char g_unk_7172703e80[];
extern UnkTunable g_unk_7172703e88;
void fn_7100136ecc(UnkTunable* self);  // the base constructor
extern void* g_unk_7101a60f28[];
void fn_7100075104(void* self);  // its destructor, as registered
void* fn_710068f63c();  // CCharClassScorpiusAIComponent::refl_GetClass
extern char g_unk_7172703ee9[];
extern UnkStrIdBase g_unk_7172703ef0;
extern UnkStrIdBase g_unk_7172703ef8;
extern UnkStrIdBase g_unk_7172703f00;

void fn_7100690990() {
    InitHeaderIds(&g_unk_7172703f08.ids);
    InitId(&g_unk_7172703f08.field_0x18, "Life|CTunableCharClassLifeComponent");
    InitId(&g_unk_7172703f08.field_0x20, "fLifeScorpiusStage1");
    InitEnum(&g_unk_7172703d20, "EForceOpticCamouflageMode", fn_710068de8c, fn_710068dce0,
             g_unk_7172703e38, &g_unk_7172703e40, fn_71006907c4, &g_unk_7172703e60, fn_71006907d0);
    fn_710068e010();
    fn_710009ec00(g_unk_7172703e80, g_unk_7172703f08.field_0x38);
    InitTunable(&g_unk_7172703e88, fn_7100136ecc, g_unk_7101a60f28, fn_7100075104);
    fn_710068f63c();
    fn_710009ec00(g_unk_7172703ee9, g_unk_7172703f08.field_0x170);
    InitId(&g_unk_7172703ef0, "STAGE_1");
    InitId(&g_unk_7172703ef8, "STAGE_2");
    InitId(&g_unk_7172703f00, "STAGE_3");
}

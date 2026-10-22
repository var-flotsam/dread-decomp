// Static initialiser of the unit of the CCharClassVulkranAIComponent::EDir, CCharClassVulkranAIComponent::ESubspecies enums.
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_71006dae40 {
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
    u8 field_0x78[8];
};
extern UnkStatics_71006dae40 g_unk_717270a458 __attribute__((visibility("hidden")));
void* fn_71006d85b4();  // CCharClassVulkranAIComponent::refl_GetClass
extern char g_unk_717270a190[];
extern UnkEnumType g_unk_717270a198;
void fn_71006da1f8(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71006d9fec();
extern char g_unk_717270a2b0[];
extern UnkConversion g_unk_717270a2b8;
extern UnkConversion g_unk_717270a2d8;
void fn_71006daccc() __attribute__((visibility("hidden")));
void fn_71006dacd8() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_717270a2f8;
void fn_71006da4c4(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71006da378();
extern char g_unk_717270a410[];
extern UnkConversion g_unk_717270a418;
extern UnkConversion g_unk_717270a438;
void fn_71006dace4() __attribute__((visibility("hidden")));
void fn_71006dacf0() __attribute__((visibility("hidden")));

void fn_71006dae40() {
    InitHeaderIds(&g_unk_717270a458.ids);
    fn_71006d85b4();
    fn_710009ec00(g_unk_717270a190, g_unk_717270a458.field_0x78);
    InitEnum(&g_unk_717270a198, "CCharClassVulkranAIComponent::EDir", fn_71006da1f8, fn_71006d9fec,
             g_unk_717270a2b0, &g_unk_717270a2b8, fn_71006daccc, &g_unk_717270a2d8, fn_71006dacd8);
    InitEnum(&g_unk_717270a2f8, "CCharClassVulkranAIComponent::ESubspecies", fn_71006da4c4, fn_71006da378,
             g_unk_717270a410, &g_unk_717270a418, fn_71006dace4, &g_unk_717270a438, fn_71006dacf0);
    InitId(&g_unk_717270a458.field_0x20, "turn2walk");
    InitId(&g_unk_717270a458.field_0x28, "burrowloop");
    InitId(&g_unk_717270a458.field_0x30, "burrow2relaxprepare");
    InitId(&g_unk_717270a458.field_0x38, "burrow2relax");
    InitId(&g_unk_717270a458.field_0x40, "burrow2relaxturn");
    InitId(&g_unk_717270a458.field_0x48, "patrol2combat");
    InitId(&g_unk_717270a458.field_0x50, "patrol2combatturn");
    InitId(&g_unk_717270a458.field_0x58, "patrol2chase");
    InitId(&g_unk_717270a458.field_0x60, "patrol2chaseturn");
    InitId(&g_unk_717270a458.field_0x68, "combat2patrol");
    InitId(&g_unk_717270a458.field_0x70, "chase2patrol");
}

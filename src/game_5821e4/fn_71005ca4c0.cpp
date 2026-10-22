// Static initialiser of the unit of the CCharClassInfesterAIComponent::ESubspecies enum.
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_71005ca4c0 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_71005ca4c0 g_unk_71726ec2b8 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726ec150;
void fn_71005c9acc(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71005c9980();
extern char g_unk_71726ec268[];
extern UnkConversion g_unk_71726ec270;
extern UnkConversion g_unk_71726ec290;
void fn_71005ca3c4() __attribute__((visibility("hidden")));
void fn_71005ca3d0() __attribute__((visibility("hidden")));
void* fn_71005c9c50();  // CCharClassInfesterAIComponent::refl_GetClass
extern char g_unk_71726ec2b0[];

void fn_71005ca4c0() {
    InitHeaderIds(&g_unk_71726ec2b8.ids);
    InitEnum(&g_unk_71726ec150, "CCharClassInfesterAIComponent::ESubspecies", fn_71005c9acc, fn_71005c9980,
             g_unk_71726ec268, &g_unk_71726ec270, fn_71005ca3c4, &g_unk_71726ec290, fn_71005ca3d0);
    fn_71005c9c50();
    fn_710009ec00(g_unk_71726ec2b0, g_unk_71726ec2b8.field_0x20);
}

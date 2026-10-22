// Static initialiser of the unit of the CInfesterShootAttack::EState enum.
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_71005c7830 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    u8 field_0x38[8];
};
extern UnkStatics_71005c7830 g_unk_71726ebbc0 __attribute__((visibility("hidden")));
void* fn_71005c5ddc();  // CInfesterShootAttack::refl_GetClass
extern char g_unk_71726eba58[];
extern UnkEnumType g_unk_71726eba60;
void fn_71005c6204(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71005c6058();
extern char g_unk_71726ebb78[];
extern UnkConversion g_unk_71726ebb80;
extern UnkConversion g_unk_71726ebba0;
void fn_71005c73a8() __attribute__((visibility("hidden")));
void fn_71005c73b4() __attribute__((visibility("hidden")));

void fn_71005c7830() {
    InitHeaderIds(&g_unk_71726ebbc0.ids);
    fn_71005c5ddc();
    fn_710009ec00(g_unk_71726eba58, g_unk_71726ebbc0.field_0x38);
    InitEnum(&g_unk_71726eba60, "CInfesterShootAttack::EState", fn_71005c6204, fn_71005c6058,
             g_unk_71726ebb78, &g_unk_71726ebb80, fn_71005c73a8, &g_unk_71726ebba0, fn_71005c73b4);
    InitId(&g_unk_71726ebbc0.field_0x20, "shoot");
    InitId(&g_unk_71726ebbc0.field_0x28, "turn_shoot");
    InitId(&g_unk_71726ebbc0.field_0x30, "relax");
}

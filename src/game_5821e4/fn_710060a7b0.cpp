// Static initialiser of the unit of the CCharClassNailongThornsAttack::EDepthornAttackType enum.
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_710060a7b0 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_710060a7b0 g_unk_71726f4a48 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726f48e0;
void fn_71006098b4(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100609708();
extern char g_unk_71726f49f8[];
extern UnkConversion g_unk_71726f4a00;
extern UnkConversion g_unk_71726f4a20;
void fn_710060a6e4() __attribute__((visibility("hidden")));
void fn_710060a6f0() __attribute__((visibility("hidden")));
void* fn_7100609a38();  // CCharClassNailongThornsAttack::refl_GetClass
extern char g_unk_71726f4a40[];

void fn_710060a7b0() {
    InitHeaderIds(&g_unk_71726f4a48.ids);
    InitEnum(&g_unk_71726f48e0, "CCharClassNailongThornsAttack::EDepthornAttackType", fn_71006098b4, fn_7100609708,
             g_unk_71726f49f8, &g_unk_71726f4a00, fn_710060a6e4, &g_unk_71726f4a20, fn_710060a6f0);
    fn_7100609a38();
    fn_710009ec00(g_unk_71726f4a40, g_unk_71726f4a48.field_0x20);
}

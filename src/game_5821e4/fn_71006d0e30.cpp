// Static initialiser of the unit of the CTakumakuDashAttack::EState enum.
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_71006d0e30 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_71006d0e30 g_unk_7172709448 __attribute__((visibility("hidden")));
void* fn_71006ce0a4();  // CTakumakuDashAttack::refl_GetClass
extern char g_unk_71727092e0[];
extern UnkEnumType g_unk_71727092e8;
void fn_71006ce58c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71006ce320();
extern char g_unk_7172709400[];
extern UnkConversion g_unk_7172709408;
extern UnkConversion g_unk_7172709428;
void fn_71006d0858() __attribute__((visibility("hidden")));
void fn_71006d0864() __attribute__((visibility("hidden")));

void fn_71006d0e30() {
    InitHeaderIds(&g_unk_7172709448.ids);
    fn_71006ce0a4();
    fn_710009ec00(g_unk_71727092e0, g_unk_7172709448.field_0x20);
    InitEnum(&g_unk_71727092e8, "CTakumakuDashAttack::EState", fn_71006ce58c, fn_71006ce320,
             g_unk_7172709400, &g_unk_7172709408, fn_71006d0858, &g_unk_7172709428, fn_71006d0864);
}

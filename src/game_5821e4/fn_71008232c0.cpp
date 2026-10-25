// Static initialiser of a reflected component's unit.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71008232c0 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_71008232c0 g_unk_7172723668 __attribute__((visibility("hidden")));
extern UnkStrIdBase g_unk_71727235c8;
extern UnkProperty g_unk_71727235d0;
extern UnkProperty* g_unk_71726db788;
extern void* g_unk_7101a75830[];
void fn_71008230c8(UnkProperty* p);
void fn_7100821f40(void* p);
extern UnkProperty g_unk_71727235f8;
extern char g_unk_7172723620[];

void fn_71008232c0() {
    InitHeaderIds(&g_unk_7172723668.ids);
    InitId(&g_unk_71727235c8, "GRAPPLE");
    InitProperty(&g_unk_71727235d0, g_unk_7101a75830, g_unk_71726db788, "Used", TypeOf<fn_710000d228>(),
                 fn_71008230c8, fn_7100821f40);
    InitProperty(&g_unk_71727235f8, g_unk_7101a75830, g_unk_71726db788, "Position", TypeOf<fn_71000246e4>(),
                 fn_71008230c8, fn_7100821f40);
    fn_7100821fbc();
    fn_710009ec00(g_unk_7172723620, g_unk_7172723668.field_0x20);
}

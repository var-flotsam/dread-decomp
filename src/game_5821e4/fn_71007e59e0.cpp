// Static initialiser of a reflected component's unit.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71007e59e0 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    u8 field_0x28[8];
    u8 pad_0x30[0x130];
    u8 field_0x160[8];
};
extern UnkStatics_71007e59e0 g_unk_717271f3d0 __attribute__((visibility("hidden")));
extern UnkStrIdBase g_unk_717271f348;
extern UnkProperty g_unk_717271f350;
extern UnkProperty* g_unk_71726db788;
extern void* g_unk_7101a73580[];
void fn_71007e53d0(UnkProperty* p);
void fn_71007dff10(void* p);
extern char g_unk_717271f378[];
extern char g_unk_717271f379[];

void fn_71007e59e0() {
    InitHeaderIds(&g_unk_717271f3d0.ids);
    InitId(&g_unk_717271f348, "EVENTSCENARIO");
    InitProperty(&g_unk_717271f350, g_unk_7101a73580, g_unk_71726db788, "EventFinished", TypeOf<fn_710000d228>(),
                 fn_71007e53d0, fn_71007dff10);
    fn_71007dff8c();
    fn_710009ec00(g_unk_717271f378, g_unk_717271f3d0.field_0x28);
    fn_71007e020c();
    fn_710009ec00(g_unk_717271f379, g_unk_717271f3d0.field_0x160);
}

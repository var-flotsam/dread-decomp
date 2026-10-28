// Static initialiser of a reflected component's unit.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71007dcd50 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    u8 field_0x28[8];
    u8 pad_0x30[0x130];
    u8 field_0x160[8];
};
extern UnkStatics_71007dcd50 g_unk_717271ec80 __attribute__((visibility("hidden")));
extern UnkStrIdBase g_unk_717271ebb8;
extern UnkProperty g_unk_717271ebc0;
extern void* g_unk_7101a72f00[];
void fn_71007dca34(UnkProperty* p);
void fn_71007db81c(void* p);
extern UnkProperty g_unk_717271ebe8;
extern UnkProperty g_unk_717271ec10;
extern char g_unk_717271ec38[];
extern char g_unk_717271ec39[];

void fn_71007dcd50() {
    InitHeaderIds(&g_unk_717271ec80.ids);
    InitId(&g_unk_717271ebb8, "EVENTPROP");
    InitProperty(&g_unk_717271ebc0, g_unk_7101a72f00, g_unk_71726db788, "EventPending", TypeOf<fn_710000d228>(),
                 fn_71007dca34, fn_71007db81c);
    InitProperty(&g_unk_717271ebe8, g_unk_7101a72f00, g_unk_71726db788, "EventLaunched", TypeOf<fn_710000d228>(),
                 fn_71007dca34, fn_71007db81c);
    InitProperty(&g_unk_717271ec10, g_unk_7101a72f00, g_unk_71726db788, "EventFinished", TypeOf<fn_710000d228>(),
                 fn_71007dca34, fn_71007db81c);
    fn_71007db898();
    fn_710009ec00(g_unk_717271ec38, g_unk_717271ec80.field_0x28);
    fn_71007dbb18();
    fn_710009ec00(g_unk_717271ec39, g_unk_717271ec80.field_0x160);
}

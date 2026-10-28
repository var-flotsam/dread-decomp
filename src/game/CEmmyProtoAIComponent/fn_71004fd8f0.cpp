// Static initialiser of the unit of CEmmyProtoAIComponent: its string ids, a property and the
// class registration.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"

using namespace unk_static_init;

struct UnkStatics_71004fd8f0 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    u8 pad_0x40[0x90];
    u8 field_0xd0[8];
};
extern UnkStatics_71004fd8f0 g_unk_71726dd1e0 __attribute__((visibility("hidden")));
extern UnkPropertyEx<> g_unk_71726dd158;
extern void* g_unk_7101a45fe8[];
extern void* g_unk_7101a45f90[];
void fn_71004fa3b8(void* p);
extern char g_unk_71726dd198[];

void fn_71004fd8f0() {
    InitHeaderIds(&g_unk_71726dd1e0.ids);
    InitId(&g_unk_71726dd1e0.field_0x18, "floorslide");
    InitId(&g_unk_71726dd1e0.field_0x20, "protoemmyai");
    InitId(&g_unk_71726dd1e0.field_0x28, "Stretched");
    InitId(&g_unk_71726dd1e0.field_0x30, "EmmyProtoAIComponent");
    InitId(&g_unk_71726dd1e0.field_0x38, "EmmyProtoAIComponent");
    InitPropertyEx(&g_unk_71726dd158, g_unk_7101a45fe8, g_unk_71726db788, g_unk_7101a45f90, fn_71002ef5bc,
                  "EMMY_PROTO_GRAB_TUTO_LAUNCHED", TypeOf<fn_710000d228>(), fn_71004fd2cc, fn_71004fa3b8);
    fn_71004fa488();
    fn_710009ec00(g_unk_71726dd198, g_unk_71726dd1e0.field_0xd0);
}

// Copies of fn_71004d7d80 (src/game_s2/fn_71004d7d80.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_7100952a60.
// Static initialiser of the unit of the reflected types.
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71004d7d80 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    u8 pad_0x28[0x8];
    u8 field_0x30[8];
};
extern UnkStatics_71004d7d80 g_unk_7172739dd0 __attribute__((visibility("hidden")));
extern char g_unk_7172739dc8[];

void fn_7100952a60() {
    InitHeaderIds(&g_unk_7172739dd0.ids);
    InitClassId(&g_unk_7172739dd0.field_0x18, "MOVABLE_PLATFORM");
    InitId(&g_unk_7172739dd0.field_0x20, "MovablePlatform");
    fn_7100951424();
    fn_710009ec00(g_unk_7172739dc8, g_unk_7172739dd0.field_0x30);
}


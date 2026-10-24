// Static initialiser of CCubeMapComponent's unit.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71007a10c0 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    u8 pad_0x28[0x20];
    u8 field_0x48[8];
};
extern UnkStatics_71007a10c0 g_unk_7172718d68 __attribute__((visibility("hidden")));
extern char g_unk_7172718d54[];

void fn_71007a10c0() {
    InitHeaderIds(&g_unk_7172718d68.ids);
    fn_710079d3b0();
    fn_710009ec00(g_unk_7172718d54, g_unk_7172718d68.field_0x48);
    InitClassId(&g_unk_7172718d68.field_0x20, "CCubeMapComponent");
}

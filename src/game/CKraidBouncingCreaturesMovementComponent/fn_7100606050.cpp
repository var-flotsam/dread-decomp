// Static initialiser of CKraidBouncingCreaturesMovementComponent's unit.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_7100606050 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    u64 field_0x20;
    u8 field_0x28[8];
};
extern UnkStatics_7100606050 g_unk_71726f41b8 __attribute__((visibility("hidden")));
extern char g_unk_71726f41b0[];

void fn_7100606050() {
    InitHeaderIds(&g_unk_71726f41b8.ids);
    InitClassId(&g_unk_71726f41b8.field_0x18, "CKraidBouncingCreaturesMovementComponent");
    fn_7100604f54();
    fn_710009ec00(g_unk_71726f41b0, g_unk_71726f41b8.field_0x28);
}

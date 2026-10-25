// Static initialiser of QuetzoaMultiTargetProjectileMovementComponent's unit.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71006422e0 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    u8 pad_0x20[0x8];
    u8 field_0x28[8];
};
extern UnkStatics_71006422e0 g_unk_71726fae00 __attribute__((visibility("hidden")));
extern char g_unk_71726fadf8[];

void fn_71006422e0() {
    InitHeaderIds(&g_unk_71726fae00.ids);
    InitClassId(&g_unk_71726fae00.field_0x18, "QuetzoaMultiTargetProjectileMovementComponent");
    fn_71006411c4();
    fn_710009ec00(g_unk_71726fadf8, g_unk_71726fae00.field_0x28);
}

// Static initialiser of NailongThornMovementComponent's unit.
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_7100623af0 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    u8 pad_0x30[0x8];
    u8 field_0x38[8];
};
extern UnkStatics_7100623af0 g_unk_71726f8118 __attribute__((visibility("hidden")));
void* fn_7100622d34();  // CNailongThornMovementComponent::refl_GetClass
extern char g_unk_71726f8100[];

void fn_7100623af0() {
    InitHeaderIds(&g_unk_71726f8118.ids);
    InitClassId(&g_unk_71726f8118.field_0x18, "NailongThornMovementComponent");
    InitId(&g_unk_71726f8118.field_0x20, "fDamageNailongThorn");
    InitId(&g_unk_71726f8118.field_0x28, "fDamageDepthornBall");
    fn_7100622d34();
    fn_710009ec00(g_unk_71726f8100, g_unk_71726f8118.field_0x38);
}

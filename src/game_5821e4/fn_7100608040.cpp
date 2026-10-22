// Static initialiser of CKraidShockerSplashMovementComponent's unit.
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_7100608040 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    u8 pad_0x30[0x8];
    u8 field_0x38[8];
};
extern UnkStatics_7100608040 g_unk_71726f4498 __attribute__((visibility("hidden")));
void* fn_7100606dbc();  // CKraidShockerSplashMovementComponent::refl_GetClass
extern char g_unk_71726f4490[];

void fn_7100608040() {
    InitHeaderIds(&g_unk_71726f4498.ids);
    InitId(&g_unk_71726f4498.field_0x18, "floorshockwave");
    InitId(&g_unk_71726f4498.field_0x20, "fDamageKraidShockerSplash");
    InitClassId(&g_unk_71726f4498.field_0x28, "CKraidShockerSplashMovementComponent");
    fn_7100606dbc();
    fn_710009ec00(g_unk_71726f4490, g_unk_71726f4498.field_0x38);
}

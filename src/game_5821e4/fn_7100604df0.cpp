// Static initialiser of CKraidAcidBlobsMovementComponent's unit.
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_7100604df0 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    u8 pad_0x28[0x8];
    u8 field_0x30[8];
};
extern UnkStatics_7100604df0 g_unk_71726f4048 __attribute__((visibility("hidden")));
void* fn_71006043e4();  // CKraidAcidBlobsMovementComponent::refl_GetClass
extern char g_unk_71726f4040[];

void fn_7100604df0() {
    InitHeaderIds(&g_unk_71726f4048.ids);
    InitClassId(&g_unk_71726f4048.field_0x18, "CKraidAcidBlobsMovementComponent");
    InitId(&g_unk_71726f4048.field_0x20, "fDamageKraidAcidBlobs");
    fn_71006043e4();
    fn_710009ec00(g_unk_71726f4040, g_unk_71726f4048.field_0x30);
}

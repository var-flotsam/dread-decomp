// Static initialiser of CKraidSpikeMovablePlatformComponent's unit.
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_7100609090 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    u8 pad_0x28[0x8];
    u8 field_0x30[8];
};
extern UnkStatics_7100609090 g_unk_71726f4610 __attribute__((visibility("hidden")));
void* fn_71006081c4();  // CKraidSpikeMovablePlatformComponent::refl_GetClass
extern char g_unk_71726f4608[];

void fn_7100609090() {
    InitHeaderIds(&g_unk_71726f4610.ids);
    InitId(&g_unk_71726f4610.field_0x18, "fDamageKraidSpikes");
    InitClassId(&g_unk_71726f4610.field_0x20, "CKraidSpikeMovablePlatformComponent");
    fn_71006081c4();
    fn_710009ec00(g_unk_71726f4608, g_unk_71726f4610.field_0x30);
}

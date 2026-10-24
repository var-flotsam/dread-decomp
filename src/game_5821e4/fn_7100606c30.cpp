// Static initialiser of CKraidNailMovementComponent's unit.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_7100606c30 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    u8 pad_0x30[0x8];
    u8 field_0x38[8];
};
extern UnkStatics_7100606c30 g_unk_71726f4320 __attribute__((visibility("hidden")));
extern char g_unk_71726f4318[];

void fn_7100606c30() {
    InitHeaderIds(&g_unk_71726f4320.ids);
    InitId(&g_unk_71726f4320.field_0x18, "fDamageKraidSpinningNails");
    InitClassId(&g_unk_71726f4320.field_0x20, "CKraidNailMovementComponent");
    InitId(&g_unk_71726f4320.field_0x28, "kraid");
    fn_7100606184();
    fn_710009ec00(g_unk_71726f4318, g_unk_71726f4320.field_0x38);
}

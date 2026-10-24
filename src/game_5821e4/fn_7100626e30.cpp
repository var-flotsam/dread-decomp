// Static initialiser of NailuggerAcidBallMovementComponent's unit.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_7100626e30 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    u8 pad_0x28[0x8];
    u8 field_0x30[8];
};
extern UnkStatics_7100626e30 g_unk_71726f8818 __attribute__((visibility("hidden")));
extern char g_unk_71726f8810[];

void fn_7100626e30() {
    InitHeaderIds(&g_unk_71726f8818.ids);
    InitClassId(&g_unk_71726f8818.field_0x18, "NailuggerAcidBallMovementComponent");
    InitId(&g_unk_71726f8818.field_0x20, "fDamageNailuggerSpit");
    fn_7100626184();
    fn_710009ec00(g_unk_71726f8810, g_unk_71726f8818.field_0x30);
}

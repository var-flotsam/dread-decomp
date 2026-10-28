// Static initialiser of the unit of the EJumpType.
#include "../../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71003bbc20 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    u8 pad_0x30[0x10];
    u8 field_0x40[8];
};
extern UnkStatics_71003bbc20 g_unk_71726bd8b0 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726bd748;
void fn_71003badfc(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71003bacb0();
extern char g_unk_71726bd860[];
extern UnkConversion g_unk_71726bd868;
extern UnkConversion g_unk_71726bd888;
void fn_71003bbaf4() __attribute__((visibility("hidden")));
void fn_71003bbb00() __attribute__((visibility("hidden")));
extern char g_unk_71726bd8a8[];

void fn_71003bbc20() {
    InitHeaderIds(&g_unk_71726bd8b0.ids);
    InitEnum(&g_unk_71726bd748, "EJumpType", fn_71003badfc, fn_71003bacb0,
             g_unk_71726bd860, &g_unk_71726bd868, fn_71003bbaf4, &g_unk_71726bd888, fn_71003bbb00);
    fn_71003baf80();
    fn_710009ec00(g_unk_71726bd8a8, g_unk_71726bd8b0.field_0x40);
    InitId(&g_unk_71726bd8b0.field_0x20, "HurtboxHidden");
    InitId(&g_unk_71726bd8b0.field_0x28, "Hurtbox");
}

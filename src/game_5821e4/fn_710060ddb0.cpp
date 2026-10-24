// Static initialiser of the unit of the CNailongThornsAttack::EState enum.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_710060ddb0 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    UnkStrIdBase field_0x40;
    UnkStrIdBase field_0x48;
    UnkStrIdBase field_0x50;
    u8 field_0x58[8];
};
extern UnkStatics_710060ddb0 g_unk_71726f4d08 __attribute__((visibility("hidden")));
extern char g_unk_71726f4ba0[];
extern UnkEnumType g_unk_71726f4ba8;
void fn_710060aeb0(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710060abe4();
extern char g_unk_71726f4cc0[];
extern UnkConversion g_unk_71726f4cc8;
extern UnkConversion g_unk_71726f4ce8;
void fn_710060d870() __attribute__((visibility("hidden")));
void fn_710060d87c() __attribute__((visibility("hidden")));

void fn_710060ddb0() {
    InitHeaderIds(&g_unk_71726f4d08.ids);
    fn_710060a968();
    fn_710009ec00(g_unk_71726f4ba0, g_unk_71726f4d08.field_0x58);
    InitEnum(&g_unk_71726f4ba8, "CNailongThornsAttack::EState", fn_710060aeb0, fn_710060abe4,
             g_unk_71726f4cc0, &g_unk_71726f4cc8, fn_710060d870, &g_unk_71726f4ce8, fn_710060d87c);
    InitId(&g_unk_71726f4d08.field_0x20, "attack_charge_loop");
    InitId(&g_unk_71726f4d08.field_0x28, "attack_release");
    InitId(&g_unk_71726f4d08.field_0x30, "attack_release_end");
    InitId(&g_unk_71726f4d08.field_0x38, "attack_release_relax");
    InitId(&g_unk_71726f4d08.field_0x40, "attack_release_relax2charge_loop");
    InitId(&g_unk_71726f4d08.field_0x48, "relax");
    InitId(&g_unk_71726f4d08.field_0x50, "damage");
}

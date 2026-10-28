// Static initialiser of a reflected component's unit.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71007b4d80 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    UnkStrIdBase field_0x40;
    u8 field_0x48[8];
    u8 pad_0x50[0x130];
    u8 field_0x180[8];
};
extern UnkStatics_71007b4d80 g_unk_717271a290 __attribute__((visibility("hidden")));
extern UnkStrIdBase g_unk_717271a208;
extern UnkProperty g_unk_717271a210;
extern void* g_unk_7101a706d8[];
void fn_71007b4824(UnkProperty* p);
void fn_71007b1bac(void* p);
extern char g_unk_717271a238[];
extern char g_unk_717271a239[];

void fn_71007b4d80() {
    InitHeaderIds(&g_unk_717271a290.ids);
    InitId(&g_unk_717271a208, "EMMY_VALVE");
    InitProperty(&g_unk_717271a210, g_unk_7101a706d8, g_unk_71726db788, "Disabled", TypeOf<fn_710000d228>(),
                 fn_71007b4824, fn_71007b1bac);
    fn_71007b1c28();
    fn_710009ec00(g_unk_717271a238, g_unk_717271a290.field_0x48);
    fn_71007b1ea8();
    fn_710009ec00(g_unk_717271a239, g_unk_717271a290.field_0x180);
    InitId(&g_unk_717271a290.field_0x28, "emmysensor");
    InitId(&g_unk_717271a290.field_0x30, "collision");
    InitId(&g_unk_717271a290.field_0x38, "Default");
    InitId(&g_unk_717271a290.field_0x40, "Limbs");
}

// Static initialiser of a reflected component's unit.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_710082f0f0 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkFilePathStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    u8 field_0x40[8];
};
extern UnkStatics_710082f0f0 g_unk_7172725048 __attribute__((visibility("hidden")));
extern UnkStrIdBase g_unk_7172724fd0;
extern UnkProperty g_unk_7172724fd8;
extern void* g_unk_7101a76fa8[];
void fn_710082ec70(UnkProperty* p);
void fn_710082d53c(void* p);
extern char g_unk_7172725000[];

void fn_710082f0f0() {
    InitHeaderIds(&g_unk_7172725048.ids);
    InitId(&g_unk_7172724fd0, "GRAPPLEPULLOFF");
    InitProperty(&g_unk_7172724fd8, g_unk_7101a76fa8, g_unk_71726db788, "ActivatableObjectLastAnim", TypeOf<fn_7100009658>(),
                 fn_710082ec70, fn_710082d53c);
    fn_710082d5b8();
    fn_710009ec00(g_unk_7172725000, g_unk_7172725048.field_0x40);
    InitPathId(&g_unk_7172725048.field_0x20, "actors/props/grapplepulloffpoint/fx/grapplebreak.bcptl");
    InitId(&g_unk_7172725048.field_0x28, "Collision");
    InitId(&g_unk_7172725048.field_0x30, "Hit");
    InitId(&g_unk_7172725048.field_0x38, "whengrappleispossible");
}

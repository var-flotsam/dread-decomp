// Static initialiser of the unit of the CGroundShockerAttack::EState enum.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71005a17b0 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    u8 field_0x30[8];
};
extern UnkStatics_71005a17b0 g_unk_71726e8028 __attribute__((visibility("hidden")));
extern char g_unk_71726e7ec0[];
extern UnkEnumType g_unk_71726e7ec8;
void fn_71005a0814(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71005a0488();
extern char g_unk_71726e7fe0[];
extern UnkConversion g_unk_71726e7fe8;
extern UnkConversion g_unk_71726e8008;
void fn_71005a1250() __attribute__((visibility("hidden")));
void fn_71005a125c() __attribute__((visibility("hidden")));

void fn_71005a17b0() {
    InitHeaderIds(&g_unk_71726e8028.ids);
    fn_71005a020c();
    fn_710009ec00(g_unk_71726e7ec0, g_unk_71726e8028.field_0x30);
    InitEnum(&g_unk_71726e7ec8, "CGroundShockerAttack::EState", fn_71005a0814, fn_71005a0488,
             g_unk_71726e7fe0, &g_unk_71726e7fe8, fn_71005a1250, &g_unk_71726e8008, fn_71005a125c);
    InitId(&g_unk_71726e8028.field_0x20, "attack_hit_init");
    InitId(&g_unk_71726e8028.field_0x28, "attack_hit_end");
}

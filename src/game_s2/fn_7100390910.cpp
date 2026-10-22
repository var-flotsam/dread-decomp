// Static initialiser of the unit of the behaviortree::CScaredAction::EState.
#include "../game_s7/UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_7100390910 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_7100390910 g_unk_71726b71b8 __attribute__((visibility("hidden")));
void* fn_710038f7d4();  // behaviortree::CScaredAction::refl_GetClass
extern char g_unk_71726b7050[];
extern UnkEnumType g_unk_71726b7058;
void fn_710038fde8(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710038fb1c();
extern char g_unk_71726b7170[];
extern UnkConversion g_unk_71726b7178;
extern UnkConversion g_unk_71726b7198;
void fn_7100390730() __attribute__((visibility("hidden")));
void fn_710039073c() __attribute__((visibility("hidden")));

void fn_7100390910() {
    InitHeaderIds(&g_unk_71726b71b8.ids);
    fn_710038f7d4();
    fn_710009ec00(g_unk_71726b7050, g_unk_71726b71b8.field_0x20);
    InitEnum(&g_unk_71726b7058, "behaviortree::CScaredAction::EState", fn_710038fde8, fn_710038fb1c,
             g_unk_71726b7170, &g_unk_71726b7178, fn_7100390730, &g_unk_71726b7198, fn_710039073c);
}

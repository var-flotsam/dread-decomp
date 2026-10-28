// Static initialiser of the unit of the CCharClassFlockingSwarmControllerComponent::SRotationMode enum.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_7100732a50 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_7100732a50 g_unk_7172710500 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_7172710398;
void fn_71007314ec(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71007313a0();
extern char g_unk_71727104b0[];
extern UnkConversion g_unk_71727104b8;
extern UnkConversion g_unk_71727104d8;
void fn_7100732900() __attribute__((visibility("hidden")));
void fn_710073290c() __attribute__((visibility("hidden")));
extern char g_unk_71727104f8[];

void fn_7100732a50() {
    InitHeaderIds(&g_unk_7172710500.ids);
    InitEnum(&g_unk_7172710398, "CCharClassFlockingSwarmControllerComponent::SRotationMode", fn_71007314ec, fn_71007313a0,
             g_unk_71727104b0, &g_unk_71727104b8, fn_7100732900, &g_unk_71727104d8, fn_710073290c);
    fn_7100731670();
    fn_710009ec00(g_unk_71727104f8, g_unk_7172710500.field_0x20);
}

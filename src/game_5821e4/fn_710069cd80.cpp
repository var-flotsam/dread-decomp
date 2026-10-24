// Static initialiser of the unit of the CScourgeTongueSlashAttack::EState enum.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_710069cd80 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    UnkStrIdBase field_0x40;
    UnkStrIdBase field_0x48;
    u8 field_0x50[8];
};
extern UnkStatics_710069cd80 g_unk_7172704928 __attribute__((visibility("hidden")));
extern char g_unk_71727047c0[];
extern UnkEnumType g_unk_71727047c8;
void fn_710069a57c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710069a250();
extern char g_unk_71727048e0[];
extern UnkConversion g_unk_71727048e8;
extern UnkConversion g_unk_7172704908;
void fn_710069c7cc() __attribute__((visibility("hidden")));
void fn_710069c7d8() __attribute__((visibility("hidden")));

void fn_710069cd80() {
    InitHeaderIds(&g_unk_7172704928.ids);
    fn_7100699fd4();
    fn_710009ec00(g_unk_71727047c0, g_unk_7172704928.field_0x50);
    InitEnum(&g_unk_71727047c8, "CScourgeTongueSlashAttack::EState", fn_710069a57c, fn_710069a250,
             g_unk_71727048e0, &g_unk_71727048e8, fn_710069c7cc, &g_unk_7172704908, fn_710069c7d8);
    InitId(&g_unk_7172704928.field_0x20, "tongue_slash_attack_init");
    InitId(&g_unk_7172704928.field_0x28, "tongue_slash_attack_1");
    InitId(&g_unk_7172704928.field_0x30, "tongue_slash_attack_straight_1");
    InitId(&g_unk_7172704928.field_0x38, "tongue_slash_attack_reversearc_1");
    InitId(&g_unk_7172704928.field_0x40, "TongueBreakableTileBlocker");
    InitId(&g_unk_7172704928.field_0x48, "TongueBreakableTileBlocker");
}

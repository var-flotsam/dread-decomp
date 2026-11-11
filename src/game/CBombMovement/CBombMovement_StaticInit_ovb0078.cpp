// Static initialiser of the CBombMovement unit: the header ids, the class and its tunable's registrations,
// the tunable itself, then the class id ("BombMovement") and the "bomb" id. Layouts are placeholders.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_7100aac720 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    u8 field_0x38[8];
    u8 pad_0x40[0x130];
    u8 field_0x170[8];
};
extern UnkStatics_7100aac720 g_unk_7172753b10 __attribute__((visibility("hidden")));
extern char g_unk_7172753ac8[];
extern char g_unk_7172753ac9[];
extern UnkTunable g_unk_7172753ad0;
extern void* g_unk_7101a94fa8[];

void fn_7100aac720() {
    InitHeaderIds(&g_unk_7172753b10.ids);
    fn_7100aa9174();
    fn_710009ec00(g_unk_7172753ac8, g_unk_7172753b10.field_0x38);
    fn_7100aa94f8();
    fn_710009ec00(g_unk_7172753ac9, g_unk_7172753b10.field_0x170);
    InitTunable(&g_unk_7172753ad0, fn_7100136ecc, g_unk_7101a94fa8, fn_7100075104);
    InitClassId(&g_unk_7172753b10.field_0x28, "BombMovement");
    InitId(&g_unk_7172753b10.field_0x30, "bomb");
}

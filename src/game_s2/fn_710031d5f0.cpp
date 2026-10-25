// Static initialiser of the unit of the CAbilityMultiLockon::SSubState enum.
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_710031d5f0 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    u8 pad_0x40[0xf8];
    u8 field_0x138[8];
    u8 pad_0x140[0x130];
    u8 field_0x270[8];
};
extern UnkStatics_710031d5f0 g_unk_71726af948 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726af748;
void fn_71003164b4(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71003162a8();
extern char g_unk_71726af860[];
extern UnkConversion g_unk_71726af868;
extern UnkConversion g_unk_71726af888;
void fn_710031d8ac() __attribute__((visibility("hidden")));
void fn_710031d8b8() __attribute__((visibility("hidden")));
extern char g_unk_71726af8a8[];
extern char g_unk_71726af8a9[];
extern UnkTunable g_unk_71726af8b0;
void fn_7100136ecc(UnkTunable* self);  // the base constructor
extern void* g_unk_7101a22f60[];
void fn_7100075104(void* self);  // its destructor, as registered

void fn_710031d5f0() {
    InitHeaderIds(&g_unk_71726af948.ids);
    InitEnum(&g_unk_71726af748, "CAbilityMultiLockon::SSubState", fn_71003164b4, fn_71003162a8,
             g_unk_71726af860, &g_unk_71726af868, fn_710031d8ac, &g_unk_71726af888, fn_710031d8b8);
    fn_7100316638();
    fn_710009ec00(g_unk_71726af8a8, g_unk_71726af948.field_0x138);
    fn_71003169f8();
    fn_710009ec00(g_unk_71726af8a9, g_unk_71726af948.field_0x270);
    InitTunableZeroed<0x24, 0x30>(&g_unk_71726af8b0, fn_7100136ecc, g_unk_7101a22f60, fn_7100075104);
    InitId(&g_unk_71726af948.field_0x28, "MultiLockon");
    InitId(&g_unk_71726af948.field_0x30, "LockOnMissile");
    InitId(&g_unk_71726af948.field_0x38, "lockonmissilemarker");
}

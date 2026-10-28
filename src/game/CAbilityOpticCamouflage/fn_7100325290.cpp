// Static initialiser of the unit of the CAbilityOpticCamouflage::ESelectionMode, CAbilityOpticCamouflage::ESelectionInput.
#include "../../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUProperty.h"

using namespace unk_static_init;

struct UnkStatics_7100325290 {
    HeaderIds ids;
    u8 pad_0x18[0x20];
    f32 field_0x38[2];
    bool field_0x40;
    u8 pad_0x41[7];
    u8 field_0x48[8];
    u8 pad_0x50[0x130];
    u8 field_0x180[8];
};
extern UnkStatics_7100325290 g_unk_71726b00b8 __attribute__((visibility("hidden")));
extern char g_unk_71726afcf0[];
extern UnkEnumType g_unk_71726afcf8;
void fn_7100320638(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71003204ec();
extern char g_unk_71726afe10[];
extern UnkConversion g_unk_71726afe18;
extern UnkConversion g_unk_71726afe38;
void fn_7100324a80() __attribute__((visibility("hidden")));
void fn_7100324a8c() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726afe58;
void fn_7100320904(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71003207b8();
extern char g_unk_71726aff70[];
extern UnkConversion g_unk_71726aff78;
extern UnkConversion g_unk_71726aff98;
void fn_7100324a98() __attribute__((visibility("hidden")));
void fn_7100324aa4() __attribute__((visibility("hidden")));
extern char g_unk_71726affb8[];
extern UnkTunable g_unk_71726affc0;
extern void* g_unk_7101a23250[];
void fn_7100075104(void* self);  // its destructor, as registered

void fn_7100325290() {
    InitHeaderIds(&g_unk_71726b00b8.ids);
    g_unk_71726b00b8.field_0x38[0] = -1.0f;
    g_unk_71726b00b8.field_0x38[1] = -1.0f;
    g_unk_71726b00b8.field_0x40 = true;
    fn_7100320270();
    fn_710009ec00(g_unk_71726afcf0, g_unk_71726b00b8.field_0x48);
    InitEnum(&g_unk_71726afcf8, "CAbilityOpticCamouflage::ESelectionMode", fn_7100320638, fn_71003204ec, g_unk_71726afe10, &g_unk_71726afe18, fn_7100324a80, &g_unk_71726afe38, fn_7100324a8c);
    InitEnum(&g_unk_71726afe58, "CAbilityOpticCamouflage::ESelectionInput", fn_7100320904, fn_71003207b8, g_unk_71726aff70, &g_unk_71726aff78, fn_7100324a98, &g_unk_71726aff98, fn_7100324aa4);
    fn_7100320a88();
    fn_710009ec00(g_unk_71726affb8, g_unk_71726b00b8.field_0x180);
    InitTunable(&g_unk_71726affc0, fn_7100136ecc, g_unk_7101a23250, fn_7100075104);
}

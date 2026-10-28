// Static initialiser of the unit of the CCharClassSabotoruAIComponent::ESubspecies enum.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUProperty.h"

using namespace unk_static_init;

struct UnkStatics_7100677670 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    u8 field_0x28[8];
    u8 pad_0x30[0x130];
    u8 field_0x160[8];
};
extern UnkStatics_7100677670 g_unk_7172700860 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71727006c0;
void fn_710067664c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100676500();
extern char g_unk_71727007d8[];
extern UnkConversion g_unk_71727007e0;
extern UnkConversion g_unk_7172700800;
void fn_7100677498() __attribute__((visibility("hidden")));
void fn_71006774a4() __attribute__((visibility("hidden")));
extern char g_unk_7172700820[];
extern char g_unk_7172700821[];
extern UnkTunable g_unk_7172700828;
extern void* g_unk_7101a5e950[];
void fn_7100075104(void* self);  // its destructor, as registered

void fn_7100677670() {
    InitHeaderIds(&g_unk_7172700860.ids);
    InitEnum(&g_unk_71727006c0, "CCharClassSabotoruAIComponent::ESubspecies", fn_710067664c, fn_7100676500,
             g_unk_71727007d8, &g_unk_71727007e0, fn_7100677498, &g_unk_7172700800, fn_71006774a4);
    fn_71006767d0();
    fn_710009ec00(g_unk_7172700820, g_unk_7172700860.field_0x28);
    fn_7100676c10();
    fn_710009ec00(g_unk_7172700821, g_unk_7172700860.field_0x160);
    InitTunable(&g_unk_7172700828, fn_7100136ecc, g_unk_7101a5e950, fn_7100075104);
}

// Static initialiser of the unit of the EShinesparkTravellingDirection, TShinesparkTravellingDirectionFlagSet, ECoolShinesparkSituation, TCoolShinesparkSituation.
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_7100332f40 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    UnkStrIdBase field_0x40;
    UnkStrIdBase field_0x48;
    UnkStrIdBase field_0x50;
    UnkStrIdBase field_0x58;
    u8 field_0x60[8];
    u8 pad_0x68[0x130];
    u8 field_0x198[8];
};
extern UnkStatics_7100332f40 g_unk_71726b0970 __attribute__((visibility("hidden")));
extern char g_unk_71726b0370[];
extern char g_unk_71726b0371[];
extern UnkTunable g_unk_71726b0378;
void fn_7100136ecc(UnkTunable* self);  // the base constructor
extern void* g_unk_7101a23448[];
void fn_7100075104(void* self);  // its destructor, as registered
extern UnkEnumType g_unk_71726b0410;
void fn_7100327688(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71003272fc();
extern char g_unk_71726b0528[];
extern UnkConversion g_unk_71726b0530;
extern UnkConversion g_unk_71726b0550;
void fn_710033181c() __attribute__((visibility("hidden")));
void fn_7100331828() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726b0570;
void fn_7100327808(UnkEnumType* type, const CStrId& name, void* arg);
void fn_710009a368(void* type);  // the type's destructor, as registered
extern char g_unk_71726b0660[];
extern UnkConversion g_unk_71726b0668;
extern UnkConversion g_unk_71726b0688;
void fn_7100331834() __attribute__((visibility("hidden")));
void fn_7100331840() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726b06a8;
void fn_7100327a44(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100327958();
extern char g_unk_71726b07c0[];
extern UnkConversion g_unk_71726b07c8;
extern UnkConversion g_unk_71726b07e8;
void fn_710033184c() __attribute__((visibility("hidden")));
void fn_7100331858() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726b0808;
void fn_7100327bc4(UnkEnumType* type, const CStrId& name, void* arg);
extern char g_unk_71726b08f8[];
extern UnkConversion g_unk_71726b0900;
extern UnkConversion g_unk_71726b0920;
void fn_7100331864() __attribute__((visibility("hidden")));
void fn_7100331870() __attribute__((visibility("hidden")));

void fn_7100332f40() {
    InitHeaderIds(&g_unk_71726b0970.ids);
    fn_71003255a4();
    fn_710009ec00(g_unk_71726b0370, g_unk_71726b0970.field_0x60);
    fn_7100325824();
    fn_710009ec00(g_unk_71726b0371, g_unk_71726b0970.field_0x198);
    InitTunable(&g_unk_71726b0378, fn_7100136ecc, g_unk_7101a23448, fn_7100075104);
    InitEnum(&g_unk_71726b0410, "EShinesparkTravellingDirection", fn_7100327688, fn_71003272fc, g_unk_71726b0528, &g_unk_71726b0530, fn_710033181c, &g_unk_71726b0550, fn_7100331828);
    InitFlagSet(&g_unk_71726b0570, "TShinesparkTravellingDirectionFlagSet", fn_7100327808, fn_710009a368, g_unk_71726b0660, TypeOf<fn_7100005178>, &g_unk_71726b0668, fn_7100331834, &g_unk_71726b0688, fn_7100331840);
    InitEnum(&g_unk_71726b06a8, "ECoolShinesparkSituation", fn_7100327a44, fn_7100327958, g_unk_71726b07c0, &g_unk_71726b07c8, fn_710033184c, &g_unk_71726b07e8, fn_7100331858);
    InitFlagSet(&g_unk_71726b0808, "TCoolShinesparkSituation", fn_7100327bc4, fn_710009a368, g_unk_71726b08f8, TypeOf<fn_7100005178>, &g_unk_71726b0900, fn_7100331864, &g_unk_71726b0920, fn_7100331870);
    InitId(&g_unk_71726b0970.field_0x28, "Shinespark");
    InitId(&g_unk_71726b0970.field_0x30, "CharacterShinespark");
    InitId(&g_unk_71726b0970.field_0x38, "CharacterNotSupported");
    InitId(&g_unk_71726b0970.field_0x40, "Shinespark");
    InitId(&g_unk_71726b0970.field_0x48, "SPIDER_IMPULSE_END");
    InitId(&g_unk_71726b0970.field_0x50, "ShinesparkCollisionAngle");
    InitId(&g_unk_71726b0970.field_0x58, "ShinesparkLaunchAngle");
}

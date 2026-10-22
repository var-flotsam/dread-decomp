// Static initialiser of the unit of the CChozoWarriorXUltimateGrabAttack::SState enum.
#include "../game_s7/UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_710048c680 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    u8 pad_0x28[0x10];
    u8 field_0x38[8];
    u8 pad_0x40[0x130];
    u8 field_0x170[8];
};
extern UnkStatics_710048c680 g_unk_71726cf4d0 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726cf338;
void fn_7100486004(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100485a98();
extern char g_unk_71726cf450[];
extern UnkConversion g_unk_71726cf458;
extern UnkConversion g_unk_71726cf478;
void fn_710048a930() __attribute__((visibility("hidden")));
void fn_710048a93c() __attribute__((visibility("hidden")));
void* fn_7100486188();  // CChozoWarriorXUltimateGrabAttack::refl_GetClass
extern char g_unk_71726cf498[];
void* fn_7100486408();  // CChozoWarriorXUltimateGrabAttack::CTunableChozoWarriorXUltimateGrabAttack::refl_GetClass
extern char g_unk_71726cf499[];
extern UnkTunable g_unk_71726cf4a0;
void fn_7100136ecc(UnkTunable* self);  // the base constructor
extern void* g_unk_7101a3b530[];
void fn_7100075104(void* self);  // its destructor, as registered

void fn_710048c680() {
    InitHeaderIds(&g_unk_71726cf4d0.ids);
    InitId(&g_unk_71726cf4d0.field_0x18, "ugrabsamuswarriorxintroground");
    InitId(&g_unk_71726cf4d0.field_0x20, "ugrabsamuswarriorxintrowall");
    InitEnum(&g_unk_71726cf338, "CChozoWarriorXUltimateGrabAttack::SState", fn_7100486004, fn_7100485a98,
             g_unk_71726cf450, &g_unk_71726cf458, fn_710048a930, &g_unk_71726cf478, fn_710048a93c);
    fn_7100486188();
    fn_710009ec00(g_unk_71726cf498, g_unk_71726cf4d0.field_0x38);
    fn_7100486408();
    fn_710009ec00(g_unk_71726cf499, g_unk_71726cf4d0.field_0x170);
    InitTunable(&g_unk_71726cf4a0, fn_7100136ecc, g_unk_7101a3b530, fn_7100075104);
}

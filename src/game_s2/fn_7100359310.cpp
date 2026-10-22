// Static initialiser of the unit of the EInputAimDir.
#include "../game_s7/UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_7100359310 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    UnkStrIdBase field_0x40;
    u8 pad_0x48[0x180];
    u8 field_0x1c8[8];
    u8 pad_0x1d0[0x130];
    u8 field_0x300[8];
};
extern UnkStatics_7100359310 g_unk_71726b16a0 __attribute__((visibility("hidden")));
void* fn_710033ff68();  // CAimComponent::refl_GetClass
extern char g_unk_71726b1430[];
void* fn_71003412a0();  // CAimComponent::CTunableAim::refl_GetClass
extern char g_unk_71726b1431[];
struct __attribute__((packed)) UnkAimTunable {
    void** vtable;
    u8 pad_0x8[0x1c];
    u64 field_0x24;
    u32 field_0x2c;
    u8 pad_0x30[4];
    u64 field_0x34;
    u32 field_0x3c;
    u8 pad_0x40[4];
    u64 field_0x44;
    u64 field_0x4c;
    u64 field_0x54;
    u8 pad_0x5c[8];
    u64 field_0x64;
    u64 field_0x6c;
    u64 field_0x74;
    u64 field_0x7c;
    u64 field_0x84;
    u64 field_0x8c;
};
extern UnkAimTunable g_unk_71726b1438;
void fn_7100136ecc(UnkAimTunable* self);  // the base constructor
extern void* g_unk_7101a23c28[];
void fn_7100075104(void* self);  // its destructor, as registered
extern UnkEnumType g_unk_71726b1508;
void fn_7100342d64(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100342a38();
extern char g_unk_71726b1620[];
extern UnkConversion g_unk_71726b1628;
extern UnkConversion g_unk_71726b1648;
void fn_7100353968() __attribute__((visibility("hidden")));
void fn_7100353974() __attribute__((visibility("hidden")));

void fn_7100359310() {
    InitHeaderIds(&g_unk_71726b16a0.ids);
    InitId(&g_unk_71726b16a0.field_0x18, "MELEE_COUNTER");
    InitId(&g_unk_71726b16a0.field_0x20, "MELEE");
    InitId(&g_unk_71726b16a0.field_0x28, "diss");
    InitId(&g_unk_71726b16a0.field_0x30, "SPRCHARGE_WEAPON_BOOST");
    InitId(&g_unk_71726b16a0.field_0x38, "SPRSHOT_WEAPON_BOOST");
    InitId(&g_unk_71726b16a0.field_0x40, "AutoLockOn");
    fn_710033ff68();
    fn_710009ec00(g_unk_71726b1430, g_unk_71726b16a0.field_0x1c8);
    fn_71003412a0();
    fn_710009ec00(g_unk_71726b1431, g_unk_71726b16a0.field_0x300);
    {
        UnkAimTunable* t = &g_unk_71726b1438;
        fn_7100136ecc(t);
        t->vtable = g_unk_7101a23c28 + 2;
        t->field_0x24 = 0;
        t->field_0x2c = 0;
        t->field_0x34 = 0;
        t->field_0x3c = 0;
        t->field_0x44 = 0;
        t->field_0x4c = 0;
        t->field_0x54 = 0;
        t->field_0x64 = 0;
        t->field_0x6c = 0;
        t->field_0x74 = 0;
        t->field_0x7c = 0;
        t->field_0x84 = 0;
        t->field_0x8c = 0;
        __cxa_atexit(fn_7100075104, t, g_unk_7101a19000);
    }
    InitEnum(&g_unk_71726b1508, "EInputAimDir", fn_7100342d64, fn_7100342a38,
             g_unk_71726b1620, &g_unk_71726b1628, fn_7100353968, &g_unk_71726b1648, fn_7100353974);
}

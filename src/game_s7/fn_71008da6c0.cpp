// Static initialiser of the unit of CDoorLifeComponent::SState: the header ids, then the unit's
// own statics and registrations (helpers in UnkStaticInitS7.h). Layouts are placeholders.
#include "UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_71008da6c0 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkFilePathStrIdBase field_0x20;
    UnkFilePathStrIdBase field_0x28;
    UnkFilePathStrIdBase field_0x30;
    UnkFilePathStrIdBase field_0x38;
    UnkStrIdBase field_0x40;
    UnkStrIdBase field_0x48;
    UnkStrIdBase field_0x50;
    UnkStrIdBase field_0x58;
    UnkStrIdBase field_0x60;
    UnkStrIdBase field_0x68;
    UnkStrIdBase field_0x70;
    UnkStrIdBase field_0x78;
    UnkStrIdBase field_0x80;
    UnkStrIdBase field_0x88;
    UnkStrIdBase field_0x90;
    UnkStrIdBase field_0x98;
    UnkStrIdBase field_0xa0;
    UnkStrIdBase field_0xa8;
    u8 pad_0xb0[0x340];
    u8 field_0x3f0[8];
    u8 pad_0x3f8[0x130];
    u8 field_0x528[8];
};
extern UnkStatics_71008da6c0 g_unk_7172731708 __attribute__((visibility("hidden")));
struct UnkStruct_7172731438;
extern UnkStrIdBase g_unk_71727310b8;
extern UnkProperty g_unk_71727310c0;
extern UnkProperty* g_unk_71726db788;
extern void* g_unk_7101a7ea58[];
void fn_71008dbddc(void* p);
void fn_71008cac70(void* p);
extern UnkProperty g_unk_71727310e8;
extern UnkProperty g_unk_7172731110;
extern UnkProperty g_unk_7172731138;
extern UnkEnumType g_unk_7172731200;
extern UnkProperty g_unk_7172731160;
extern UnkProperty g_unk_7172731188;
extern UnkProperty g_unk_71727311b0;
extern UnkProperty g_unk_71727311d8;
inline const CStrId& StaticName_71726a47a8() {
    static CStrId s("LIFE", true);
    return s;
}
extern UnkConversion g_unk_7172731320;
extern UnkConversion g_unk_7172731340;
void fn_71008dbb84() __attribute__((visibility("hidden")));
void fn_71008dbb90() __attribute__((visibility("hidden")));
extern char g_unk_7172731318[];
void fn_71008caef4(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71008cace8();
void* fn_71008cb078();
extern char g_unk_7172731360[];
void* fn_71008cd2c8();
extern char g_unk_7172731361[];
extern UnkTunable g_unk_7172731368;
void fn_7100136ecc(UnkTunable* self);  // the base constructor
extern void* g_unk_7101a7eab0[];
void fn_7100075104(void* self);  // its destructor, as registered
struct UnkStruct_71727313a4 {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    f32 field_0x14;
    u64 field_0x18;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
};
extern UnkStruct_71727313a4 g_unk_71727313a4;
struct UnkStruct_71727313d4 {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
};
extern UnkStruct_71727313d4 g_unk_71727313d4;
struct UnkStruct_7172731404 {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    f32 field_0x14;
    u64 field_0x18;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
};
extern UnkStruct_7172731404 g_unk_7172731404;
extern UnkStruct_7172731438 g_unk_7172731438;
void fn_71008ce190(UnkStruct_7172731438* self);  // its constructor
void fn_71008ce35c(void* self);  // its destructor, as registered

// NON_MATCHING(minor): the three float objects' constant stores are scheduled differently (the
// original builds 0.02f from 0.01f with an orr and stores some pairs as two words); the same
// values are stored.
void fn_71008da6c0() {
    InitHeaderIds(&g_unk_7172731708.ids);
    InitId(&g_unk_71727310b8, "DOOR");
    InitProperty(&g_unk_71727310c0, g_unk_7101a7ea58, g_unk_71726db788, "Opened", TypeOf<fn_710000d228>(), fn_71008dbddc, fn_71008cac70);
    InitProperty(&g_unk_71727310e8, g_unk_7101a7ea58, g_unk_71726db788, "Disabled", TypeOf<fn_710000d228>(), fn_71008dbddc, fn_71008cac70);
    InitProperty(&g_unk_7172731110, g_unk_7101a7ea58, g_unk_71726db788, "StayOpened", TypeOf<fn_710000d228>(), fn_71008dbddc, fn_71008cac70);
    InitProperty(&g_unk_7172731138, g_unk_7101a7ea58, g_unk_71726db788, "ChangeWhenVisible", TypeOf<fn_710000d228>(), fn_71008dbddc, fn_71008cac70);
    InitProperty(&g_unk_7172731160, g_unk_7101a7ea58, g_unk_71726db788, "NextState", &g_unk_7172731200, fn_71008dbddc, fn_71008cac70);
    InitProperty(&g_unk_7172731188, g_unk_7101a7ea58, g_unk_71726db788, "ControlledByUsable", TypeOf<fn_710000d228>(), fn_71008dbddc, fn_71008cac70);
    InitProperty(&g_unk_71727311b0, g_unk_7101a7ea58, g_unk_71726db788, "IsWet", TypeOf<fn_710000d228>(), fn_71008dbddc, fn_71008cac70);
    InitProperty(&g_unk_71727311d8, g_unk_7101a7ea58, g_unk_71726db788, "FrozenDuringCooldown", TypeOf<fn_710000d228>(), fn_71008dbddc, fn_71008cac70);
    InitId(&g_unk_7172731708.field_0x18, "door");
    InitPathId(&g_unk_7172731708.field_0x20, "props/genericdoor/mdoor_fireopened.wav");
    InitPathId(&g_unk_7172731708.field_0x28, "props/genericdoor/mdoor_fireclosed.wav");
    InitPathId(&g_unk_7172731708.field_0x30, "actors/props/door/fx/doorchargecharge_heatfx.bcptl");
    InitPathId(&g_unk_7172731708.field_0x38, "actors/props/door/fx/doorchargecharge_heatopenfx.bcptl");
    InitId(&g_unk_7172731708.field_0x40, "areafx");
    InitId(&g_unk_7172731708.field_0x48, "areaopenfx");
    InitClassIdFrom(&g_unk_7172731708.field_0x50, StaticName_71726a47a8());
    InitId(&g_unk_7172731708.field_0x58, "sensor");
    InitId(&g_unk_7172731708.field_0x60, "sensorclose");
    InitId(&g_unk_7172731708.field_0x68, "sensorpresence");
    InitId(&g_unk_7172731708.field_0x70, "closed");
    InitId(&g_unk_7172731708.field_0x78, "opened");
    InitId(&g_unk_7172731708.field_0x80, "frozen");
    InitId(&g_unk_7172731708.field_0x88, "sabotoru_nest_slime_R");
    InitId(&g_unk_7172731708.field_0x90, "sabotoru_nest_slime_L");
    InitId(&g_unk_7172731708.field_0x98, "sabotoru_nest_eggs_R");
    InitId(&g_unk_7172731708.field_0xa0, "sabotoru_nest_eggs_L");
    InitId(&g_unk_7172731708.field_0xa8, "eggs");
    InitEnum(&g_unk_7172731200, "CDoorLifeComponent::SState", fn_71008caef4, fn_71008cace8, g_unk_7172731318, &g_unk_7172731320, fn_71008dbb84, &g_unk_7172731340, fn_71008dbb90);
    fn_71008cb078();
    fn_710009ec00(g_unk_7172731360, g_unk_7172731708.field_0x3f0);
    fn_71008cd2c8();
    fn_710009ec00(g_unk_7172731361, g_unk_7172731708.field_0x528);
    InitTunable(&g_unk_7172731368, fn_7100136ecc, g_unk_7101a7eab0, fn_7100075104);
    g_unk_71727313a4.field_0x0 = 1.0f;
    g_unk_71727313a4.field_0x4 = 0.8f;
    g_unk_71727313a4.field_0x8 = 0.05f;
    g_unk_71727313a4.field_0xc = 1.0f;
    g_unk_71727313a4.field_0x10 = 1.0f;
    g_unk_71727313a4.field_0x14 = 0.0f;
    g_unk_71727313a4.field_0x18 = 0;
    g_unk_71727313a4.field_0x20 = 0.07f;
    g_unk_71727313a4.field_0x24 = 0.02f;
    g_unk_71727313a4.field_0x28 = 0.0f;
    g_unk_71727313a4.field_0x2c = 1.0f;
    g_unk_71727313d4.field_0x0 = 0.001f;
    g_unk_71727313d4.field_0x4 = 0.001f;
    g_unk_71727313d4.field_0x8 = 0.001f;
    g_unk_71727313d4.field_0xc = 0.0f;
    g_unk_71727313d4.field_0x10 = 1.0f;
    g_unk_71727313d4.field_0x14 = 0.0f;
    g_unk_71727313d4.field_0x18 = 0.0f;
    g_unk_71727313d4.field_0x1c = 1.0f;
    g_unk_71727313d4.field_0x20 = 0.003f;
    g_unk_71727313d4.field_0x24 = 0.003f;
    g_unk_71727313d4.field_0x28 = 0.003f;
    g_unk_71727313d4.field_0x2c = 1.0f;
    g_unk_7172731404.field_0x10 = 1.0f;
    g_unk_7172731404.field_0x14 = 0.0f;
    g_unk_7172731404.field_0x18 = 0;
    g_unk_7172731404.field_0x0 = 0.01f;
    g_unk_7172731404.field_0x4 = 0.01f;
    g_unk_7172731404.field_0x8 = 0.01f;
    g_unk_7172731404.field_0xc = 1.0f;
    g_unk_7172731404.field_0x20 = 0.07f;
    g_unk_7172731404.field_0x24 = 0.07f;
    g_unk_7172731404.field_0x28 = 0.07f;
    g_unk_7172731404.field_0x2c = 1.0f;
    fn_71008ce190(&g_unk_7172731438);
    __cxa_atexit(fn_71008ce35c, &g_unk_7172731438, g_unk_7101a19000);
}

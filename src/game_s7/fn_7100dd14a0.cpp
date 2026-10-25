// Static initialiser of the unit of CBossRushManager::SRushRecord,
// base::global::CRntSmallDictionary<int, float>: the header ids, then the unit's own statics and
// registrations (helpers in UnkStaticInitS7.h). Layouts are placeholders.
#include "UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_7100dd14a0 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    u8 pad_0x38[0x18];
    u8 field_0x50[8];
    u8 pad_0x58[0x130];
    u8 field_0x188[8];
    u8 pad_0x190[0x130];
    u8 field_0x2c0[8];
};
extern UnkStatics_7100dd14a0 g_unk_71727bdb68 __attribute__((visibility("hidden")));
extern f32 g_unk_71727bca30;
extern f32 g_unk_7101aa949c;
extern f32 g_unk_7101aa94a0;
extern f32 g_unk_7101aa94a8;
extern f32 g_unk_7101aa94ac;
extern UnkStrIdBase g_unk_71727bca38;
extern UnkEnumType g_unk_71727bcb40;
extern UnkProperty2 g_unk_71727bca40;
extern UnkProperty* g_unk_71726db788;
extern void* g_unk_7101a45fe8[];
extern void* g_unk_7101aa94d8[];
void fn_7100dcfb6c(void* p);
void fn_7100db464c(void* p);
extern UnkProperty2 g_unk_71727bca80;
extern UnkProperty2 g_unk_71727bcac0;
extern UnkProperty2 g_unk_71727bcb00;
void fn_7100db49e4(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
void fn_7100db4718();
extern char g_unk_71727bcc78[];
extern char g_unk_71727bcc79[];
UnkEnumType* fn_7100db4b68();
extern char g_unk_71727bcc7a[];
UnkEnumType* fn_7100db4c44();
extern UnkEnumType g_unk_71727bcc80;
void fn_7100db50a4(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_7101d5e2e0[];
void fn_7100db4d20();
extern char g_unk_71727bcdb8[];
extern char g_unk_71727bcdb9[];
UnkEnumType* fn_7100db5228();
extern UnkEnumType g_unk_71727bcdc0;
void fn_7100db5688(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100db5304();
extern char g_unk_71727bcef8[];
extern char g_unk_71727bcef9[];
UnkEnumType* fn_7100db580c();
extern UnkEnumType g_unk_71727bcf00;
void fn_7100db58e8(UnkEnumType* type, const CStrId& name, void*, void*);
void fn_710009891c(void* type);  // the type's destructor, as registered
extern char g_unk_71727bd070[];
extern char g_unk_71727bd071[];
UnkEnumType* fn_7100db5adc();
extern char g_unk_71727bd072[];
UnkEnumType* fn_7100db5bb8();
extern char g_unk_71727bd073[];
extern char g_unk_71727bd074[];
UnkEnumType* fn_7100db6268();
extern char g_unk_71727bd075[];
UnkEnumType* fn_7100db6344();
extern char g_unk_71727bd076[];
extern char g_unk_71727bd077[];
struct UnkTunable_71727bd078 : UnkTunable {
    u8 pad_0x8[0x30];
    void** field_0x38;  // a member's vtable
    u8 pad_0x40[0x18];
    void** field_0x58;  // a member's vtable
    u8 pad_0x60[0x18];
    void** field_0x78;  // a member's vtable
    u8 pad_0x80[0x18];
    void** field_0x98;  // a member's vtable
    u8 pad_0xa0[0x18];
    void** field_0xb8;  // a member's vtable
    u8 pad_0xc0[0x10];
    void** field_0xd0;  // a member's vtable
    u8 pad_0xd8[0x10];
    void** field_0xe8;  // a member's vtable
    u8 pad_0xf0[0x10];
    void** field_0x100;  // a member's vtable
    u8 pad_0x108[0x10];
    void** field_0x118;  // a member's vtable
    u8 pad_0x120[0x10];
    void** field_0x130;  // a member's vtable
    u8 pad_0x138[0x10];
    void** field_0x148;  // a member's vtable
    u8 pad_0x150[0x10];
    void** field_0x160;  // a member's vtable
    u8 pad_0x168[0x10];
    void** field_0x178;  // a member's vtable
    u8 pad_0x180[0x10];
    void** field_0x190;  // a member's vtable
    u8 pad_0x198[0x10];
    void** field_0x1a8;  // a member's vtable
    u8 pad_0x1b0[0x10];
    void** field_0x1c0;  // a member's vtable
};
extern UnkTunable_71727bd078 g_unk_71727bd078;
void fn_7100136ecc(UnkTunable* self);  // the base constructor
extern void* g_unk_7101aa9530[];
extern void* g_unk_7101aa9560[];
extern void* g_unk_7101aa9588[];
void fn_7100db78b0(void* self);  // its destructor, as registered
extern UnkStrIdBase g_unk_71727bd268;
extern UnkStrIdBase g_unk_71727bd270;
extern UnkStrIdBase g_unk_71727bd278;
extern UnkStrIdBase g_unk_71727bd280;
extern UnkStrIdBase g_unk_71727bd288;
extern UnkStrIdBase g_unk_71727bd290;
extern UnkStrIdBase g_unk_71727bd298;
extern UnkStrIdBase g_unk_71727bd2a0;

void fn_7100dd14a0() {
    InitHeaderIds(&g_unk_71727bdb68.ids);
    InitId(&g_unk_71727bdb68.field_0x18, "MINIMAP");
    InitId(&g_unk_71727bdb68.field_0x20, "BossRushManager");
    InitId(&g_unk_71727bdb68.field_0x28, "PLAYER_INVENTORY");
    InitId(&g_unk_71727bdb68.field_0x30, "energyfield_grabs");
    g_unk_71727bca30 = g_unk_7101aa949c + g_unk_7101aa94a0 + g_unk_7101aa94a8 + g_unk_7101aa94ac;
    InitId(&g_unk_71727bca38, "BOSSRUSH");
    InitProperty2(&g_unk_71727bca40, g_unk_7101a45fe8, g_unk_71726db788, g_unk_7101aa94d8, "BossRushRecord", &g_unk_71727bcb40, fn_7100dcfb6c, fn_7100db464c);
    InitProperty2(&g_unk_71727bca80, g_unk_7101a45fe8, g_unk_71726db788, g_unk_7101aa94d8, "DreadRushRecord", &g_unk_71727bcb40, fn_7100dcfb6c, fn_7100db464c);
    InitProperty2(&g_unk_71727bcac0, g_unk_7101a45fe8, g_unk_71726db788, g_unk_7101aa94d8, "SurvivalRushRecord", TypeOf<fn_7100014768>(), fn_7100dcfb6c, fn_7100db464c);
    InitProperty2(&g_unk_71727bcb00, g_unk_7101a45fe8, g_unk_71726db788, g_unk_7101aa94d8, "SurvivalRushTime", TypeOf<fn_710000abbc>(), fn_7100dcfb6c, fn_7100db464c);
    {
        CStrId n("CBossRushManager::SRushRecord", true);
        fn_7100db49e4(&g_unk_71727bcb40, n, nullptr, nullptr, fn_7100db4718, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727bcb40, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727bcc78, &g_unk_71727bcb40);
    fn_710009ec00(g_unk_71727bcc79, fn_7100db4b68());
    fn_710009ec00(g_unk_71727bcc7a, fn_7100db4c44());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<int, float>>::TKeyElementIterator::value_type", true);
        fn_7100db50a4(&g_unk_71727bcc80, n, g_unk_7101d5e2e0, nullptr, fn_7100db4d20, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727bcc80, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727bcdb8, &g_unk_71727bcc80);
    fn_710009ec00(g_unk_71727bcdb9, fn_7100db5228());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<int, float>>::TKeyElementConstIterator::value_type", true);
        fn_7100db5688(&g_unk_71727bcdc0, n, g_unk_7101d5e2e0, nullptr, fn_7100db5304, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727bcdc0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727bcef8, &g_unk_71727bcdc0);
    fn_710009ec00(g_unk_71727bcef9, fn_7100db580c());
    {
        CStrId n("base::global::CRntSmallDictionary<int, float>", true);
        fn_7100db58e8(&g_unk_71727bcf00, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727bcf00, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727bd070, &g_unk_71727bcf00);
    fn_710009ec00(g_unk_71727bd071, fn_7100db5adc());
    fn_710009ec00(g_unk_71727bd072, fn_7100db5bb8());
    fn_7100db5c98();
    fn_710009ec00(g_unk_71727bd073, g_unk_71727bdb68.field_0x50);
    fn_710009ec00(g_unk_71727bd074, fn_7100db6268());
    fn_710009ec00(g_unk_71727bd075, fn_7100db6344());
    fn_7100db6424();
    fn_710009ec00(g_unk_71727bd076, g_unk_71727bdb68.field_0x188);
    fn_7100db68b4();
    fn_710009ec00(g_unk_71727bd077, g_unk_71727bdb68.field_0x2c0);
    fn_7100136ecc(&g_unk_71727bd078);
    g_unk_71727bd078.vtable = g_unk_7101aa9530 + 2;
    g_unk_71727bd078.field_0x38 = g_unk_7101aa9560 + 2;
    g_unk_71727bd078.field_0x58 = g_unk_7101aa9560 + 2;
    g_unk_71727bd078.field_0x78 = g_unk_7101aa9560 + 2;
    g_unk_71727bd078.field_0x98 = g_unk_7101aa9560 + 2;
    g_unk_71727bd078.field_0xb8 = g_unk_7101aa9588 + 2;
    g_unk_71727bd078.field_0xd0 = g_unk_7101aa9588 + 2;
    g_unk_71727bd078.field_0xe8 = g_unk_7101aa9588 + 2;
    g_unk_71727bd078.field_0x100 = g_unk_7101aa9588 + 2;
    g_unk_71727bd078.field_0x118 = g_unk_7101aa9588 + 2;
    g_unk_71727bd078.field_0x130 = g_unk_7101aa9588 + 2;
    g_unk_71727bd078.field_0x148 = g_unk_7101aa9588 + 2;
    g_unk_71727bd078.field_0x160 = g_unk_7101aa9588 + 2;
    g_unk_71727bd078.field_0x178 = g_unk_7101aa9588 + 2;
    g_unk_71727bd078.field_0x190 = g_unk_7101aa9588 + 2;
    g_unk_71727bd078.field_0x1a8 = g_unk_7101aa9588 + 2;
    g_unk_71727bd078.field_0x1c0 = g_unk_7101aa9588 + 2;
    __cxa_atexit(fn_7100db78b0, &g_unk_71727bd078, g_unk_7101a19000);
    InitId(&g_unk_71727bd268, "ShowBossRushPopUp");
    InitId(&g_unk_71727bd270, "BossRushUnlocked");
    InitId(&g_unk_71727bd278, "BossRushCompleted");
    InitId(&g_unk_71727bd280, "ShowDreadRushPopUp");
    InitId(&g_unk_71727bd288, "DreadRushUnlocked");
    InitId(&g_unk_71727bd290, "DreadRushCompleted");
    InitId(&g_unk_71727bd298, "ShowSurvivalPopUp");
    InitId(&g_unk_71727bd2a0, "SurvivalUnlocked");
}

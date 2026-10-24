// Static initialiser of the unit of the ELavaCarpetState, base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CCooldownXBossLavaCarpetDef>>>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CCooldownXBossLavaCarpetDef>>>::TKeyElementConstIterator::value_type, base::global::CRntVector<std::unique_ptr<CCooldownXBossLavaCarpetDef>>, base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CRntVector<std::unique_ptr<CCooldownXBossLavaCarpetDef>>>>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CRntVector<std::unique_ptr<CCooldownXBossLavaCarpetDef>>>>::TKeyElementConstIterator::value_type, base::global::CRntVector<base::global::CRntVector<std::unique_ptr<CCooldownXBossLavaCarpetDef>>>.
#include "../game_s7/UnkStaticInitS7.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkRntString {
    u8 field_0x0[0x30];
};
void fn_710007e704(UnkRntString* self, const char* str);  // builds one from a string

__attribute__((always_inline)) inline void InitRntString(UnkRntString* s, const char* str) {
    fn_710007e704(s, str);
    __cxa_atexit(reinterpret_cast<void (*)(void*)>(fn_710007ecd4), s, g_unk_7101a19000);
}

struct UnkStatics_71004a9c40 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    u8 pad_0x38[0x8];
    UnkRntString field_0x40;
    u8 field_0x70[8];
    u8 pad_0x78[0x130];
    u8 field_0x1a8[8];
    u8 pad_0x1b0[0x130];
    u8 field_0x2e0[8];
};
extern UnkStatics_71004a9c40 g_unk_71726d3398 __attribute__((visibility("hidden")));
extern char g_unk_71726d1f40[];
extern UnkTunable g_unk_71726d1f48;
void fn_7100136ecc(UnkTunable* self);  // the base constructor
extern void* g_unk_7101a3e038[];
void fn_7100075104(void* self);  // its destructor, as registered
extern UnkEnumType g_unk_71726d1f78;
void fn_71004a2404(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71004a2138();
extern char g_unk_71726d2090[];
extern UnkConversion g_unk_71726d2098;
extern UnkConversion g_unk_71726d20b8;
void fn_71004a52c4() __attribute__((visibility("hidden")));
void fn_71004a52d0() __attribute__((visibility("hidden")));
extern char g_unk_71726d20d8[];
extern char g_unk_71726d20d9[];
UnkEnumType* fn_71004a2b34();
extern char g_unk_71726d20da[];
UnkEnumType* fn_71004a2c10();  // base::reflection::refl_GetType<std::unique_ptr<CCooldownXBossLavaCarpetDef>>
extern char g_unk_71726d20db[];
UnkEnumType* fn_71004a2cec();
extern char g_unk_71726d20dc[];
UnkEnumType* fn_71004a2dc8();
extern UnkEnumType g_unk_71726d20e0;
extern char g_unk_7101d5e2e0[];
void fn_71004a2ea4();
void fn_71004a3228(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
extern char g_unk_71726d2218[];
extern char g_unk_71726d2219[];
UnkEnumType* fn_71004a33ac();
extern UnkEnumType g_unk_71726d2220;
void fn_71004a3488();
void fn_71004a380c(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726d2358[];
extern char g_unk_71726d2359[];
UnkEnumType* fn_71004a3990();
extern UnkEnumType g_unk_71726d2360;
void fn_71004a3a6c(UnkEnumType* type, const CStrId& name, void*, void*);
void fn_710009891c(void* type);  // the type's destructor, as registered
extern char g_unk_71726d24d0[];
extern char g_unk_71726d24d1[];
UnkEnumType* fn_71004a3c4c();
extern char g_unk_71726d24d2[];
UnkEnumType* fn_71004a3d28();
extern UnkEnumType g_unk_71726d24d8;
void fn_71004a3e04();
void fn_71004a4188(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726d2610[];
extern char g_unk_71726d2611[];
UnkEnumType* fn_71004a430c();
extern UnkEnumType g_unk_71726d2618;
void fn_71004a43e8();
void fn_71004a476c(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726d2750[];
extern char g_unk_71726d2751[];
UnkEnumType* fn_71004a48f0();
extern UnkEnumType g_unk_71726d2758;
void fn_71004a49cc(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71726d28c8[];
extern UnkConversion g_unk_71726d28d0;
UnkEnumType* fn_71004a2c10();
void fn_71004a7394() __attribute__((visibility("hidden")));
extern char g_unk_71726d28f0[];

void fn_71004a9c40() {
    InitHeaderIds(&g_unk_71726d3398.ids);
    fn_71004a1a2c();
    fn_710009ec00(g_unk_71726d1f40, g_unk_71726d3398.field_0x70);
    InitTunable(&g_unk_71726d1f48, fn_7100136ecc, g_unk_7101a3e038, fn_7100075104);
    InitEnum(&g_unk_71726d1f78, "ELavaCarpetState", fn_71004a2404, fn_71004a2138, g_unk_71726d2090, &g_unk_71726d2098, fn_71004a52c4, &g_unk_71726d20b8, fn_71004a52d0);
    fn_71004a2588();
    fn_710009ec00(g_unk_71726d20d8, g_unk_71726d3398.field_0x1a8);
    InitId(&g_unk_71726d3398.field_0x28, "attack_lavacarpet_left2right-init");
    InitId(&g_unk_71726d3398.field_0x30, "attack_lavacarpet_right2left-init");
    InitRntString(&g_unk_71726d3398.field_0x40, "LAVA_CARPET");
    fn_710009ec00(g_unk_71726d20d9, fn_71004a2b34());
    fn_710009ec00(g_unk_71726d20da, fn_71004a2c10());
    fn_710009ec00(g_unk_71726d20db, fn_71004a2cec());
    fn_710009ec00(g_unk_71726d20dc, fn_71004a2dc8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CCooldownXBossLavaCarpetDef>>>::TKeyElementIterator::value_type", true);
        fn_71004a3228(&g_unk_71726d20e0, n, g_unk_7101d5e2e0, nullptr, fn_71004a2ea4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726d20e0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d2218, &g_unk_71726d20e0);
    fn_710009ec00(g_unk_71726d2219, fn_71004a33ac());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CCooldownXBossLavaCarpetDef>>>::TKeyElementConstIterator::value_type", true);
        fn_71004a380c(&g_unk_71726d2220, n, g_unk_7101d5e2e0, nullptr, fn_71004a3488, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726d2220, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d2358, &g_unk_71726d2220);
    fn_710009ec00(g_unk_71726d2359, fn_71004a3990());
    {
        CStrId n("base::global::CRntVector<std::unique_ptr<CCooldownXBossLavaCarpetDef>>", true);
        fn_71004a3a6c(&g_unk_71726d2360, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726d2360, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d24d0, &g_unk_71726d2360);
    fn_710009ec00(g_unk_71726d24d1, fn_71004a3c4c());
    fn_710009ec00(g_unk_71726d24d2, fn_71004a3d28());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CRntVector<std::unique_ptr<CCooldownXBossLavaCarpetDef>>>>::TKeyElementIterator::value_type", true);
        fn_71004a4188(&g_unk_71726d24d8, n, g_unk_7101d5e2e0, nullptr, fn_71004a3e04, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726d24d8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d2610, &g_unk_71726d24d8);
    fn_710009ec00(g_unk_71726d2611, fn_71004a430c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CRntVector<std::unique_ptr<CCooldownXBossLavaCarpetDef>>>>::TKeyElementConstIterator::value_type", true);
        fn_71004a476c(&g_unk_71726d2618, n, g_unk_7101d5e2e0, nullptr, fn_71004a43e8, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726d2618, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d2750, &g_unk_71726d2618);
    fn_710009ec00(g_unk_71726d2751, fn_71004a48f0());
    {
        CStrId n("base::global::CRntVector<base::global::CRntVector<std::unique_ptr<CCooldownXBossLavaCarpetDef>>>", true);
        fn_71004a49cc(&g_unk_71726d2758, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726d2758, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d28c8, &g_unk_71726d2758);
    InitConversion(&g_unk_71726d28d0, fn_71004a2c10, fn_71004a2b34, fn_71004a7394);
    fn_71004a4bb4();
    fn_710009ec00(g_unk_71726d28f0, g_unk_71726d3398.field_0x2e0);
}

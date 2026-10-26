// Static initialiser of the unit of the ELavaDropArm, ELavaDropArmPos, base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CCooldownXBossLavaDropsDef>>>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CCooldownXBossLavaDropsDef>>>::TKeyElementConstIterator::value_type, base::global::CRntVector<std::unique_ptr<CCooldownXBossLavaDropsDef>>, base::reflection::CollectionTypeMapper<base::global::CRntVector<CCooldownXBossLavaDropsMovementDef>>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntVector<CCooldownXBossLavaDropsMovementDef>>::TKeyElementConstIterator::value_type, base::global::CRntVector<CCooldownXBossLavaDropsMovementDef>.
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "game/UnkTUProperty.h"

using namespace unk_static_init;

struct UnkStatics_71004b3200 {
    HeaderIds ids;
    u8 pad_0x18[0x20];
    u8 field_0x38[8];
    u8 pad_0x40[0x130];
    u8 field_0x170[8];
    u8 pad_0x178[0x130];
    u8 field_0x2a8[8];
    u8 pad_0x2b0[0x130];
    u8 field_0x3e0[8];
};
extern UnkStatics_71004b3200 g_unk_71726d4d68 __attribute__((visibility("hidden")));
extern char g_unk_71726d37b0[];
extern UnkTunable g_unk_71726d37b8;
extern void* g_unk_7101a3e130[];
void fn_7100075104(void* self);  // its destructor, as registered
extern UnkEnumType g_unk_71726d37e8;
void fn_71004aacc0(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71004aaab4();
extern char g_unk_71726d3900[];
extern UnkConversion g_unk_71726d3908;
extern UnkConversion g_unk_71726d3928;
void fn_71004ae9cc() __attribute__((visibility("hidden")));
void fn_71004ae9d8() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726d3948;
void fn_71004aaf8c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71004aae40();
extern char g_unk_71726d3a60[];
extern UnkConversion g_unk_71726d3a68;
extern UnkConversion g_unk_71726d3a88;
void fn_71004ae9e4() __attribute__((visibility("hidden")));
void fn_71004ae9f0() __attribute__((visibility("hidden")));
extern char g_unk_71726d3aa8[];
extern char g_unk_71726d3aa9[];
extern char g_unk_71726d3aaa[];
UnkEnumType* fn_71004abc74();
extern char g_unk_71726d3aab[];
UnkEnumType* fn_71004abd50();  // base::reflection::refl_GetType<std::unique_ptr<CCooldownXBossLavaDropsDef>>
extern char g_unk_71726d3aac[];
UnkEnumType* fn_71004abe2c();
extern char g_unk_71726d3aad[];
UnkEnumType* fn_71004abf08();
extern UnkEnumType g_unk_71726d3ab0;
extern char g_unk_7101d5e2e0[];
void fn_71004abfe4();
void fn_71004ac368(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726d3be8[];
extern char g_unk_71726d3be9[];
UnkEnumType* fn_71004ac4ec();
extern UnkEnumType g_unk_71726d3bf0;
void fn_71004ac5c8();
void fn_71004ac94c(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726d3d28[];
extern char g_unk_71726d3d29[];
UnkEnumType* fn_71004acad0();
extern UnkEnumType g_unk_71726d3d30;
void fn_71004acbac(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71726d3ea0[];
extern UnkConversion g_unk_71726d3ea8;
UnkEnumType* fn_71004abd50();
void fn_71004afc6c() __attribute__((visibility("hidden")));
extern char g_unk_71726d3ec8[];
UnkEnumType* fn_71004acd8c();
extern char g_unk_71726d3ec9[];
UnkEnumType* fn_71004ace68();
extern UnkEnumType g_unk_71726d3ed0;
void fn_71004acf44();
void fn_71004ad2d4(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726d4008[];
extern char g_unk_71726d4009[];
UnkEnumType* fn_71004ad458();
extern UnkEnumType g_unk_71726d4010;
void fn_71004ad534();
void fn_71004ad8c4(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726d4148[];
extern char g_unk_71726d4149[];
UnkEnumType* fn_71004ada48();
extern UnkEnumType g_unk_71726d4150;
void fn_71004adb24(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71726d42c0[];
extern char g_unk_71726d42c1[];

void fn_71004b3200() {
    InitHeaderIds(&g_unk_71726d4d68.ids);
    fn_71004aa3a8();
    fn_710009ec00(g_unk_71726d37b0, g_unk_71726d4d68.field_0x38);
    InitTunable(&g_unk_71726d37b8, fn_7100136ecc, g_unk_7101a3e130, fn_7100075104);
    InitEnum(&g_unk_71726d37e8, "ELavaDropArm", fn_71004aacc0, fn_71004aaab4, g_unk_71726d3900, &g_unk_71726d3908, fn_71004ae9cc, &g_unk_71726d3928, fn_71004ae9d8);
    InitEnum(&g_unk_71726d3948, "ELavaDropArmPos", fn_71004aaf8c, fn_71004aae40, g_unk_71726d3a60, &g_unk_71726d3a68, fn_71004ae9e4, &g_unk_71726d3a88, fn_71004ae9f0);
    fn_71004ab110();
    fn_710009ec00(g_unk_71726d3aa8, g_unk_71726d4d68.field_0x170);
    fn_71004ab5ac();
    fn_710009ec00(g_unk_71726d3aa9, g_unk_71726d4d68.field_0x2a8);
    fn_710009ec00(g_unk_71726d3aaa, fn_71004abc74());
    fn_710009ec00(g_unk_71726d3aab, fn_71004abd50());
    fn_710009ec00(g_unk_71726d3aac, fn_71004abe2c());
    fn_710009ec00(g_unk_71726d3aad, fn_71004abf08());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CCooldownXBossLavaDropsDef>>>::TKeyElementIterator::value_type", true);
        fn_71004ac368(&g_unk_71726d3ab0, n, g_unk_7101d5e2e0, nullptr, fn_71004abfe4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726d3ab0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d3be8, &g_unk_71726d3ab0);
    fn_710009ec00(g_unk_71726d3be9, fn_71004ac4ec());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CCooldownXBossLavaDropsDef>>>::TKeyElementConstIterator::value_type", true);
        fn_71004ac94c(&g_unk_71726d3bf0, n, g_unk_7101d5e2e0, nullptr, fn_71004ac5c8, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726d3bf0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d3d28, &g_unk_71726d3bf0);
    fn_710009ec00(g_unk_71726d3d29, fn_71004acad0());
    {
        CStrId n("base::global::CRntVector<std::unique_ptr<CCooldownXBossLavaDropsDef>>", true);
        fn_71004acbac(&g_unk_71726d3d30, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726d3d30, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d3ea0, &g_unk_71726d3d30);
    InitConversion(&g_unk_71726d3ea8, fn_71004abd50, fn_71004abc74, fn_71004afc6c);
    fn_710009ec00(g_unk_71726d3ec8, fn_71004acd8c());
    fn_710009ec00(g_unk_71726d3ec9, fn_71004ace68());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CCooldownXBossLavaDropsMovementDef>>::TKeyElementIterator::value_type", true);
        fn_71004ad2d4(&g_unk_71726d3ed0, n, g_unk_7101d5e2e0, nullptr, fn_71004acf44, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726d3ed0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d4008, &g_unk_71726d3ed0);
    fn_710009ec00(g_unk_71726d4009, fn_71004ad458());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CCooldownXBossLavaDropsMovementDef>>::TKeyElementConstIterator::value_type", true);
        fn_71004ad8c4(&g_unk_71726d4010, n, g_unk_7101d5e2e0, nullptr, fn_71004ad534, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726d4010, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d4148, &g_unk_71726d4010);
    fn_710009ec00(g_unk_71726d4149, fn_71004ada48());
    {
        CStrId n("base::global::CRntVector<CCooldownXBossLavaDropsMovementDef>", true);
        fn_71004adb24(&g_unk_71726d4150, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726d4150, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d42c0, &g_unk_71726d4150);
    fn_71004add20();
    fn_710009ec00(g_unk_71726d42c1, g_unk_71726d4d68.field_0x3e0);
}

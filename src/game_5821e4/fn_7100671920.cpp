// Static initialiser of the unit of the CCharClassRodotukAIComponent::SAbsorbConfig::EType, CCharClassRodotukAIComponent::SAbsorbConfig, base::reflection::CollectionTypeMapper<CCharClassRodotukAIComponent::TVAbsorbConfigs>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<CCharClassRodotukAIComponent::TVAbsorbConfigs>::TKeyElementConstIterator::value_type, CCharClassRodotukAIComponent::TVAbsorbConfigs reflected types.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "game/UnkTUProperty.h"

using namespace unk_static_init;

struct UnkStatics_7100671920 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    u8 field_0x28[8];
    u8 pad_0x30[0x130];
    u8 field_0x160[8];
};
extern UnkStatics_7100671920 g_unk_71726ff920 __attribute__((visibility("hidden")));
extern char g_unk_71726fede8[];
extern char g_unk_71726fede9[];
extern UnkTunable g_unk_71726fedf0;
extern void* g_unk_7101a5dc20[];
void fn_7100075104(void* self);  // its destructor, as registered
extern UnkEnumType g_unk_71726fee48;
void fn_710066e054(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710066de48();
extern char g_unk_71726fef60[];
extern UnkConversion g_unk_71726fef68;
extern UnkConversion g_unk_71726fef88;
void fn_710066fc5c() __attribute__((visibility("hidden")));
void fn_710066fc68() __attribute__((visibility("hidden")));
void fn_710066e1d4();
extern UnkEnumType g_unk_71726fefa8;
void fn_710066e9b0(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
extern char g_unk_71726ff0e0[];
extern char g_unk_71726ff0e1[];
void* fn_710066eb34();  // fn_710066eb34
extern char g_unk_71726ff0e2[];
void* fn_710066ec10();  // fn_710066ec10
extern char g_unk_7101d5e2e0[];
void fn_710066ecec();
extern UnkEnumType g_unk_71726ff0e8;
void fn_710066f070(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726ff220[];
extern char g_unk_71726ff221[];
void* fn_710066f1f4();  // fn_710066f1f4
void fn_710066f2d0();
extern UnkEnumType g_unk_71726ff228;
void fn_710066f654(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726ff360[];
extern char g_unk_71726ff361[];
void* fn_710066f7d8();  // fn_710066f7d8
extern UnkEnumType g_unk_71726ff368;
void fn_710066f8b4(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71726ff4d8[];

void fn_7100671920() {
    InitHeaderIds(&g_unk_71726ff920.ids);
    fn_710066cd74();
    fn_710009ec00(g_unk_71726fede8, g_unk_71726ff920.field_0x28);
    fn_710066d22c();
    fn_710009ec00(g_unk_71726fede9, g_unk_71726ff920.field_0x160);
    InitTunable(&g_unk_71726fedf0, fn_7100136ecc, g_unk_7101a5dc20, fn_7100075104);
    InitEnum(&g_unk_71726fee48, "CCharClassRodotukAIComponent::SAbsorbConfig::EType", fn_710066e054, fn_710066de48,
             g_unk_71726fef60, &g_unk_71726fef68, fn_710066fc5c, &g_unk_71726fef88, fn_710066fc68);
    {
        CStrId n("CCharClassRodotukAIComponent::SAbsorbConfig", true);
        fn_710066e9b0(&g_unk_71726fefa8, n, nullptr, nullptr, fn_710066e1d4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726fefa8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726ff0e0, &g_unk_71726fefa8);
    fn_710009ec00(g_unk_71726ff0e1, fn_710066eb34());
    fn_710009ec00(g_unk_71726ff0e2, fn_710066ec10());
    {
        CStrId n("base::reflection::CollectionTypeMapper<CCharClassRodotukAIComponent::TVAbsorbConfigs>::TKeyElementIterator::value_type", true);
        fn_710066f070(&g_unk_71726ff0e8, n, g_unk_7101d5e2e0, nullptr, fn_710066ecec, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726ff0e8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726ff220, &g_unk_71726ff0e8);
    fn_710009ec00(g_unk_71726ff221, fn_710066f1f4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<CCharClassRodotukAIComponent::TVAbsorbConfigs>::TKeyElementConstIterator::value_type", true);
        fn_710066f654(&g_unk_71726ff228, n, g_unk_7101d5e2e0, nullptr, fn_710066f2d0, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726ff228, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726ff360, &g_unk_71726ff228);
    fn_710009ec00(g_unk_71726ff361, fn_710066f7d8());
    {
        CStrId n("CCharClassRodotukAIComponent::TVAbsorbConfigs", true);
        fn_710066f8b4(&g_unk_71726ff368, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726ff368, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726ff4d8, &g_unk_71726ff368);
}

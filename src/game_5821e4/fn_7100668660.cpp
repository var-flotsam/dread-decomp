// Static initialiser of the unit of the CCharClassRodomithonXAIComponent::SFirePillarConfig::EType, CCharClassRodomithonXAIComponent::SFirePillarConfig, base::reflection::CollectionTypeMapper<CCharClassRodomithonXAIComponent::TVFirePillarConfigs>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<CCharClassRodomithonXAIComponent::TVFirePillarConfigs>::TKeyElementConstIterator::value_type, CCharClassRodomithonXAIComponent::TVFirePillarConfigs reflected types.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_7100668660 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    u8 field_0x28[8];
};
extern UnkStatics_7100668660 g_unk_71726fe6f8 __attribute__((visibility("hidden")));
extern char g_unk_71726fdc18[];
extern UnkEnumType g_unk_71726fdc20;
void fn_7100665544(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100665338();
extern char g_unk_71726fdd38[];
extern UnkConversion g_unk_71726fdd40;
extern UnkConversion g_unk_71726fdd60;
void fn_7100666bd0() __attribute__((visibility("hidden")));
void fn_7100666bdc() __attribute__((visibility("hidden")));
void fn_71006656c4();
extern UnkEnumType g_unk_71726fdd80;
void fn_7100665984(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
extern char g_unk_71726fdeb8[];
extern char g_unk_71726fdeb9[];
void* fn_7100665b08();  // fn_7100665b08
extern char g_unk_71726fdeba[];
void* fn_7100665be4();  // fn_7100665be4
extern char g_unk_7101d5e2e0[];
void fn_7100665cc0();
extern UnkEnumType g_unk_71726fdec0;
void fn_7100666044(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726fdff8[];
extern char g_unk_71726fdff9[];
void* fn_71006661c8();  // fn_71006661c8
void fn_71006662a4();
extern UnkEnumType g_unk_71726fe000;
void fn_7100666628(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726fe138[];
extern char g_unk_71726fe139[];
void* fn_71006667ac();  // fn_71006667ac
extern UnkEnumType g_unk_71726fe140;
void fn_7100666888(UnkEnumType* type, const CStrId& name, void*, void*);
void fn_710009891c(void* type);  // the type's destructor, as registered
extern char g_unk_71726fe2b0[];

void fn_7100668660() {
    InitHeaderIds(&g_unk_71726fe6f8.ids);
    fn_7100664e84();
    fn_710009ec00(g_unk_71726fdc18, g_unk_71726fe6f8.field_0x28);
    InitEnum(&g_unk_71726fdc20, "CCharClassRodomithonXAIComponent::SFirePillarConfig::EType", fn_7100665544, fn_7100665338,
             g_unk_71726fdd38, &g_unk_71726fdd40, fn_7100666bd0, &g_unk_71726fdd60, fn_7100666bdc);
    {
        CStrId n("CCharClassRodomithonXAIComponent::SFirePillarConfig", true);
        fn_7100665984(&g_unk_71726fdd80, n, nullptr, nullptr, fn_71006656c4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726fdd80, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726fdeb8, &g_unk_71726fdd80);
    fn_710009ec00(g_unk_71726fdeb9, fn_7100665b08());
    fn_710009ec00(g_unk_71726fdeba, fn_7100665be4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<CCharClassRodomithonXAIComponent::TVFirePillarConfigs>::TKeyElementIterator::value_type", true);
        fn_7100666044(&g_unk_71726fdec0, n, g_unk_7101d5e2e0, nullptr, fn_7100665cc0, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726fdec0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726fdff8, &g_unk_71726fdec0);
    fn_710009ec00(g_unk_71726fdff9, fn_71006661c8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<CCharClassRodomithonXAIComponent::TVFirePillarConfigs>::TKeyElementConstIterator::value_type", true);
        fn_7100666628(&g_unk_71726fe000, n, g_unk_7101d5e2e0, nullptr, fn_71006662a4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726fe000, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726fe138, &g_unk_71726fe000);
    fn_710009ec00(g_unk_71726fe139, fn_71006667ac());
    {
        CStrId n("CCharClassRodomithonXAIComponent::TVFirePillarConfigs", true);
        fn_7100666888(&g_unk_71726fe140, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726fe140, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726fe2b0, &g_unk_71726fe140);
    InitId(&g_unk_71726fe6f8.field_0x20, "fire");
}

// Static initialiser of the unit of the base::reflection::CollectionTypeMapper<base::global::CRntVector<SFXInstanceData, false>>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntVector<SFXInstanceData, false>>::TKeyElementConstIterator::value_type, base::global::CRntVector<SFXInstanceData, false>, SFXInstanceData reflected types.
#include "base/reflection/UnkReflTypeGlobals.h"
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_7100815b00 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_7100815b00 g_unk_7172722eb8 __attribute__((visibility("hidden")));
extern char g_unk_7172722528[];
void* fn_710080fedc();  // fn_710080fedc
extern char g_unk_7172722529[];
void* fn_710080ffb8();  // fn_710080ffb8
void fn_7100810094();
extern UnkEnumType g_unk_7172722530;
void fn_7100810418(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_7172722668[];
extern char g_unk_7172722669[];
void* fn_710081059c();  // fn_710081059c
void fn_7100810678();
extern UnkEnumType g_unk_7172722670;
void fn_71008109fc(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71727227a8[];
extern char g_unk_71727227a9[];
void* fn_7100810b80();  // fn_7100810b80
extern UnkEnumType g_unk_71727227b0;
void fn_7100810c5c(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_7172722920[];
void fn_7100810e40();
extern UnkEnumType g_unk_7172722928;
void fn_71008116b4(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
extern char g_unk_7172722a60[];
extern char g_unk_7172722a61[];

void fn_7100815b00() {
    InitHeaderIds(&g_unk_7172722eb8.ids);
    fn_710009ec00(g_unk_7172722528, fn_710080fedc());
    fn_710009ec00(g_unk_7172722529, fn_710080ffb8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<SFXInstanceData, false>>::TKeyElementIterator::value_type", true);
        fn_7100810418(&g_unk_7172722530, n, &g_unk_7101d5e2e0, nullptr, fn_7100810094, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7172722530, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172722668, &g_unk_7172722530);
    fn_710009ec00(g_unk_7172722669, fn_710081059c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<SFXInstanceData, false>>::TKeyElementConstIterator::value_type", true);
        fn_71008109fc(&g_unk_7172722670, n, &g_unk_7101d5e2e0, nullptr, fn_7100810678, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7172722670, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727227a8, &g_unk_7172722670);
    fn_710009ec00(g_unk_71727227a9, fn_7100810b80());
    {
        CStrId n("base::global::CRntVector<SFXInstanceData, false>", true);
        fn_7100810c5c(&g_unk_71727227b0, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71727227b0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172722920, &g_unk_71727227b0);
    {
        CStrId n("SFXInstanceData", true);
        fn_71008116b4(&g_unk_7172722928, n, nullptr, nullptr, fn_7100810e40, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7172722928, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172722a60, &g_unk_7172722928);
    fn_71008120c8();
    fn_710009ec00(g_unk_7172722a61, g_unk_7172722eb8.field_0x20);
}

// Static initialiser of the unit of the base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<querysystem::CFilter>>>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<querysystem::CFilter>>>::TKeyElementConstIterator::value_type, base::global::CRntVector<std::unique_ptr<querysystem::CFilter>>.
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71002c0f70 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_71002c0f70 g_unk_71726a6828 __attribute__((visibility("hidden")));
extern char g_unk_71726a5eb0[];
UnkEnumType* fn_71002bda60();
extern char g_unk_71726a5eb1[];
UnkEnumType* fn_71002bd4d8();  // base::reflection::refl_GetType<std::unique_ptr<querysystem::CFilter>>
extern char g_unk_71726a5eb2[];
UnkEnumType* fn_71002bdb3c();
extern char g_unk_71726a5eb3[];
UnkEnumType* fn_71002bdc18();
extern UnkEnumType g_unk_71726a5eb8;
extern char g_unk_7101d5e2e0[];
void fn_71002bdcf4();
void fn_71002be078(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
extern char g_unk_71726a5ff0[];
extern char g_unk_71726a5ff1[];
UnkEnumType* fn_71002be1fc();
extern UnkEnumType g_unk_71726a5ff8;
void fn_71002be2d8();
void fn_71002be65c(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726a6130[];
extern char g_unk_71726a6131[];
UnkEnumType* fn_71002be7e0();
extern UnkEnumType g_unk_71726a6138;
void fn_71002be8bc(UnkEnumType* type, const CStrId& name, void*, void*);
void fn_710009891c(void* type);  // the type's destructor, as registered
extern char g_unk_71726a62a8[];
extern UnkConversion g_unk_71726a62b0;
UnkEnumType* fn_71002bd4d8();
void fn_71002bfeac() __attribute__((visibility("hidden")));
extern char g_unk_71726a62d0[];

void fn_71002c0f70() {
    InitHeaderIds(&g_unk_71726a6828.ids);
    fn_710009ec00(g_unk_71726a5eb0, fn_71002bda60());
    fn_710009ec00(g_unk_71726a5eb1, fn_71002bd4d8());
    fn_710009ec00(g_unk_71726a5eb2, fn_71002bdb3c());
    fn_710009ec00(g_unk_71726a5eb3, fn_71002bdc18());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<querysystem::CFilter>>>::TKeyElementIterator::value_type", true);
        fn_71002be078(&g_unk_71726a5eb8, n, g_unk_7101d5e2e0, nullptr, fn_71002bdcf4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726a5eb8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726a5ff0, &g_unk_71726a5eb8);
    fn_710009ec00(g_unk_71726a5ff1, fn_71002be1fc());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<querysystem::CFilter>>>::TKeyElementConstIterator::value_type", true);
        fn_71002be65c(&g_unk_71726a5ff8, n, g_unk_7101d5e2e0, nullptr, fn_71002be2d8, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726a5ff8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726a6130, &g_unk_71726a5ff8);
    fn_710009ec00(g_unk_71726a6131, fn_71002be7e0());
    {
        CStrId n("base::global::CRntVector<std::unique_ptr<querysystem::CFilter>>", true);
        fn_71002be8bc(&g_unk_71726a6138, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726a6138, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726a62a8, &g_unk_71726a6138);
    InitConversion(&g_unk_71726a62b0, fn_71002bd4d8, fn_71002bda60, fn_71002bfeac);
    fn_71002beaa0();
    fn_710009ec00(g_unk_71726a62d0, g_unk_71726a6828.field_0x20);
}

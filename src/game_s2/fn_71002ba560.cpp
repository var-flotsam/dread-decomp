// Static initialiser of the unit of the base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<querysystem::CEvaluator>>>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<querysystem::CEvaluator>>>::TKeyElementConstIterator::value_type, base::global::CRntVector<std::unique_ptr<querysystem::CEvaluator>>.
#include "base/reflection/UnkReflTypeGlobals.h"
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_71002ba560 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_71002ba560 g_unk_71726a56c8 __attribute__((visibility("hidden")));
extern char g_unk_71726a4c40[];
UnkEnumType* fn_71002b6a5c();
extern char g_unk_71726a4c41[];
UnkEnumType* fn_71002b6b38();  // base::reflection::refl_GetType<std::unique_ptr<querysystem::CEvaluator>>
extern char g_unk_71726a4c42[];
UnkEnumType* fn_71002b6c14();
extern char g_unk_71726a4c43[];
UnkEnumType* fn_71002b6cf0();
extern UnkEnumType g_unk_71726a4c48;
void fn_71002b6dcc();
void fn_71002b7150(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71726a4d80[];
extern char g_unk_71726a4d81[];
UnkEnumType* fn_71002b72d4();
extern UnkEnumType g_unk_71726a4d88;
void fn_71002b73b0();
void fn_71002b7734(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71726a4ec0[];
extern char g_unk_71726a4ec1[];
UnkEnumType* fn_71002b78b8();
extern UnkEnumType g_unk_71726a4ec8;
void fn_71002b7994(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71726a5038[];
extern UnkConversion g_unk_71726a5040;
UnkEnumType* fn_71002b6b38();
void fn_71002b93f4() __attribute__((visibility("hidden")));
extern char g_unk_71726a5060[];

void fn_71002ba560() {
    InitHeaderIds(&g_unk_71726a56c8.ids);
    fn_710009ec00(g_unk_71726a4c40, fn_71002b6a5c());
    fn_710009ec00(g_unk_71726a4c41, fn_71002b6b38());
    fn_710009ec00(g_unk_71726a4c42, fn_71002b6c14());
    fn_710009ec00(g_unk_71726a4c43, fn_71002b6cf0());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<querysystem::CEvaluator>>>::TKeyElementIterator::value_type", true);
        fn_71002b7150(&g_unk_71726a4c48, n, &g_unk_7101d5e2e0, nullptr, fn_71002b6dcc, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726a4c48, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726a4d80, &g_unk_71726a4c48);
    fn_710009ec00(g_unk_71726a4d81, fn_71002b72d4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<querysystem::CEvaluator>>>::TKeyElementConstIterator::value_type", true);
        fn_71002b7734(&g_unk_71726a4d88, n, &g_unk_7101d5e2e0, nullptr, fn_71002b73b0, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726a4d88, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726a4ec0, &g_unk_71726a4d88);
    fn_710009ec00(g_unk_71726a4ec1, fn_71002b78b8());
    {
        CStrId n("base::global::CRntVector<std::unique_ptr<querysystem::CEvaluator>>", true);
        fn_71002b7994(&g_unk_71726a4ec8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726a4ec8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726a5038, &g_unk_71726a4ec8);
    InitConversion(&g_unk_71726a5040, fn_71002b6b38, fn_71002b6a5c, fn_71002b93f4);
    fn_71002b7b78();
    fn_710009ec00(g_unk_71726a5060, g_unk_71726a56c8.field_0x20);
}

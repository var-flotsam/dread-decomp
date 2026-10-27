// Static initialiser of the unit of the SBigkranXSpitLaunchPatternStep, base::reflection::CollectionTypeMapper<TBigkranXSpitLaunchPattern>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<TBigkranXSpitLaunchPattern>::TKeyElementConstIterator::value_type, TBigkranXSpitLaunchPattern, SBigkranXSpitLaunchConfig.
#include "base/reflection/UnkReflTypeGlobals.h"
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_71003eed80 {
    HeaderIds ids;
};
extern UnkStatics_71003eed80 g_unk_71726c2418 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726c1960;
void fn_71003eb5d4();
void fn_71003eb864(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71726c1a98[];
extern char g_unk_71726c1a99[];
UnkEnumType* fn_71003eb9e8();
extern char g_unk_71726c1a9a[];
UnkEnumType* fn_71003ebac4();
extern UnkEnumType g_unk_71726c1aa0;
void fn_71003ebba0();
void fn_71003ebf24(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71726c1bd8[];
extern char g_unk_71726c1bd9[];
UnkEnumType* fn_71003ec0a8();
extern UnkEnumType g_unk_71726c1be0;
void fn_71003ec184();
void fn_71003ec508(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71726c1d18[];
extern char g_unk_71726c1d19[];
UnkEnumType* fn_71003ec68c();
extern UnkEnumType g_unk_71726c1d20;
void fn_71003ec768(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71726c1e90[];
extern UnkEnumType g_unk_71726c1e98;
void fn_71003ec95c();
void fn_71003ecb04(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71726c1fd0[];

void fn_71003eed80() {
    InitHeaderIds(&g_unk_71726c2418.ids);
    {
        CStrId n("SBigkranXSpitLaunchPatternStep", true);
        fn_71003eb864(&g_unk_71726c1960, n, nullptr, nullptr, fn_71003eb5d4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726c1960, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726c1a98, &g_unk_71726c1960);
    fn_710009ec00(g_unk_71726c1a99, fn_71003eb9e8());
    fn_710009ec00(g_unk_71726c1a9a, fn_71003ebac4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<TBigkranXSpitLaunchPattern>::TKeyElementIterator::value_type", true);
        fn_71003ebf24(&g_unk_71726c1aa0, n, &g_unk_7101d5e2e0, nullptr, fn_71003ebba0, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726c1aa0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726c1bd8, &g_unk_71726c1aa0);
    fn_710009ec00(g_unk_71726c1bd9, fn_71003ec0a8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<TBigkranXSpitLaunchPattern>::TKeyElementConstIterator::value_type", true);
        fn_71003ec508(&g_unk_71726c1be0, n, &g_unk_7101d5e2e0, nullptr, fn_71003ec184, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726c1be0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726c1d18, &g_unk_71726c1be0);
    fn_710009ec00(g_unk_71726c1d19, fn_71003ec68c());
    {
        CStrId n("TBigkranXSpitLaunchPattern", true);
        fn_71003ec768(&g_unk_71726c1d20, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726c1d20, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726c1e90, &g_unk_71726c1d20);
    {
        CStrId n("SBigkranXSpitLaunchConfig", true);
        fn_71003ecb04(&g_unk_71726c1e98, n, nullptr, nullptr, fn_71003ec95c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726c1e98, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726c1fd0, &g_unk_71726c1e98);
}

// Static initialiser of unit 2601 (CEnvironmentMusicData): the unit's header ids, then the reflected types
// CEnvironmentMusicData::SMusicTransition, its vector and that vector's iterator value types, and
// CEnvironmentMusicData, its dictionary and that dictionary's iterator value types. Names and layouts are
// placeholders.
#include "base/reflection/UnkReflTypeGlobals.h"
#include "../../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_7100f87780 {
    HeaderIds ids;
};
extern UnkStatics_7100f87780 g_unk_71729ea1d8 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71729e8888;
void fn_7100f7deac();
void fn_7100f7e108(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71729e89c0[];
extern char g_unk_71729e89c1[];
UnkEnumType* fn_7100f7e28c();
extern char g_unk_71729e89c2[];
UnkEnumType* fn_7100f7e368();
extern char g_unk_71729e89c3[];
UnkEnumType* fn_7100f7e444();
extern char g_unk_71729e89c4[];
UnkEnumType* fn_7100f7e520();
extern UnkEnumType g_unk_71729e89c8;
void fn_7100f7e5fc();
void fn_7100f7e980(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71729e8b00[];
extern char g_unk_71729e8b01[];
UnkEnumType* fn_7100f7eb04();
extern UnkEnumType g_unk_71729e8b08;
void fn_7100f7ebe0();
void fn_7100f7ef64(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71729e8c40[];
extern char g_unk_71729e8c41[];
UnkEnumType* fn_7100f7f0e8();
extern UnkEnumType g_unk_71729e8c48;
void fn_7100f7f1c4(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71729e8db8[];
extern char g_unk_71729e8db9[];
UnkEnumType* fn_7100f7f3b8();
extern char g_unk_71729e8dba[];
UnkEnumType* fn_7100f7f494();
extern UnkEnumType g_unk_71729e8dc0;
void fn_7100f7f570();
void fn_7100f7fbb4(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71729e8ef8[];
extern char g_unk_71729e8ef9[];
UnkEnumType* fn_7100f7feb4();
extern char g_unk_71729e8efa[];
UnkEnumType* fn_7100f7ff90();
extern char g_unk_71729e8efb[];
UnkEnumType* fn_7100f8006c();
extern char g_unk_71729e8efc[];
UnkEnumType* fn_7100f80148();
extern UnkEnumType g_unk_71729e8f00;
void fn_7100f80224();
void fn_7100f805a8(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71729e9038[];
extern char g_unk_71729e9039[];
UnkEnumType* fn_7100f8072c();
extern UnkEnumType g_unk_71729e9040;
void fn_7100f80808();
void fn_7100f80b8c(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71729e9178[];
extern char g_unk_71729e9179[];
UnkEnumType* fn_7100f80d10();
extern UnkEnumType g_unk_71729e9180;
void fn_7100f80dec(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71729e92f0[];

void fn_7100f87780() {
    InitHeaderIds(&g_unk_71729ea1d8.ids);
    {
        CStrId n("CEnvironmentMusicData::SMusicTransition", true);
        fn_7100f7e108(&g_unk_71729e8888, n, nullptr, nullptr, fn_7100f7deac, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729e8888, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729e89c0, &g_unk_71729e8888);
    fn_710009ec00(g_unk_71729e89c1, fn_7100f7e28c());
    fn_710009ec00(g_unk_71729e89c2, fn_7100f7e368());
    fn_710009ec00(g_unk_71729e89c3, fn_7100f7e444());
    fn_710009ec00(g_unk_71729e89c4, fn_7100f7e520());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CEnvironmentMusicData::SMusicTransition>>::"
                 "TKeyElementIterator::value_type",
                 true);
        fn_7100f7e980(&g_unk_71729e89c8, n, &g_unk_7101d5e2e0, nullptr, fn_7100f7e5fc, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729e89c8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729e8b00, &g_unk_71729e89c8);
    fn_710009ec00(g_unk_71729e8b01, fn_7100f7eb04());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CEnvironmentMusicData::SMusicTransition>>::"
                 "TKeyElementConstIterator::value_type",
                 true);
        fn_7100f7ef64(&g_unk_71729e8b08, n, &g_unk_7101d5e2e0, nullptr, fn_7100f7ebe0, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729e8b08, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729e8c40, &g_unk_71729e8b08);
    fn_710009ec00(g_unk_71729e8c41, fn_7100f7f0e8());
    {
        CStrId n("base::global::CRntVector<CEnvironmentMusicData::SMusicTransition>", true);
        fn_7100f7f1c4(&g_unk_71729e8c48, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71729e8c48, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729e8db8, &g_unk_71729e8c48);
    fn_710009ec00(g_unk_71729e8db9, fn_7100f7f3b8());
    fn_710009ec00(g_unk_71729e8dba, fn_7100f7f494());
    {
        CStrId n("CEnvironmentMusicData", true);
        fn_7100f7fbb4(&g_unk_71729e8dc0, n, nullptr, nullptr, fn_7100f7f570, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729e8dc0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729e8ef8, &g_unk_71729e8dc0);
    fn_710009ec00(g_unk_71729e8ef9, fn_7100f7feb4());
    fn_710009ec00(g_unk_71729e8efa, fn_7100f7ff90());
    fn_710009ec00(g_unk_71729e8efb, fn_7100f8006c());
    fn_710009ec00(g_unk_71729e8efc, fn_7100f80148());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, "
                 "CEnvironmentMusicData>>::TKeyElementIterator::value_type",
                 true);
        fn_7100f805a8(&g_unk_71729e8f00, n, &g_unk_7101d5e2e0, nullptr, fn_7100f80224, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729e8f00, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729e9038, &g_unk_71729e8f00);
    fn_710009ec00(g_unk_71729e9039, fn_7100f8072c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, "
                 "CEnvironmentMusicData>>::TKeyElementConstIterator::value_type",
                 true);
        fn_7100f80b8c(&g_unk_71729e9040, n, &g_unk_7101d5e2e0, nullptr, fn_7100f80808, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729e9040, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729e9178, &g_unk_71729e9040);
    fn_710009ec00(g_unk_71729e9179, fn_7100f80d10());
    {
        CStrId n("base::global::CRntDictionary<base::global::CStrId, CEnvironmentMusicData>", true);
        fn_7100f80dec(&g_unk_71729e9180, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71729e9180, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729e92f0, &g_unk_71729e9180);
}

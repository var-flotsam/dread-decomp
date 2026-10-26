// Static initialiser of the unit of sound::TMusicFile,
// base::global::CRntVector<sound::TMusicFile>, sound::TMusicTrack and others: the header ids, then
// the unit's own statics and registrations (helpers in UnkStaticInitS7.h). Layouts are
// placeholders.
#include "UnkStaticInitS7.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_7101006110 {
    HeaderIds ids;
};
extern UnkStatics_7101006110 g_unk_71729f80d8 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71729f1318;
void fn_7100fe1388(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100fe119c();
extern char g_unk_71729f1450[];
extern char g_unk_71729f1451[];
UnkEnumType* fn_7100fe150c();
extern char g_unk_71729f1452[];
UnkEnumType* fn_7100fe15e8();
extern char g_unk_71729f1453[];
UnkEnumType* fn_7100fe16c4();
extern char g_unk_71729f1454[];
UnkEnumType* fn_7100fe17a0();
extern UnkEnumType g_unk_71729f1458;
void fn_7100fe1c00(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100fe187c();
extern char g_unk_71729f1590[];
extern char g_unk_71729f1591[];
UnkEnumType* fn_7100fe1d84();
extern UnkEnumType g_unk_71729f1598;
void fn_7100fe21e4(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100fe1e60();
extern char g_unk_71729f16d0[];
extern char g_unk_71729f16d1[];
UnkEnumType* fn_7100fe2368();
extern UnkEnumType g_unk_71729f16d8;
void fn_7100fe2444(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71729f1848[];
extern char g_unk_71729f1849[];
UnkEnumType* fn_7100fe2638();
extern char g_unk_71729f184a[];
UnkEnumType* fn_7100fe2714();
extern UnkEnumType g_unk_71729f1850;
void fn_7100fe2c7c(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100fe27f0();
extern char g_unk_71729f1988[];
extern char g_unk_71729f1989[];
UnkEnumType* fn_7100fe2e00();
extern char g_unk_71729f198a[];
UnkEnumType* fn_7100fe2edc();
extern char g_unk_71729f198b[];
UnkEnumType* fn_7100fe2fb8();
extern char g_unk_71729f198c[];
UnkEnumType* fn_7100fe3094();
extern UnkEnumType g_unk_71729f1990;
void fn_7100fe34f4(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100fe3170();
extern char g_unk_71729f1ac8[];
extern char g_unk_71729f1ac9[];
UnkEnumType* fn_7100fe3678();
extern UnkEnumType g_unk_71729f1ad0;
void fn_7100fe3ad8(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100fe3754();
extern char g_unk_71729f1c08[];
extern char g_unk_71729f1c09[];
UnkEnumType* fn_7100fe3c5c();
extern UnkEnumType g_unk_71729f1c10;
void fn_7100fe3d38(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71729f1d80[];
extern char g_unk_71729f1d81[];
UnkEnumType* fn_7100fe3f2c();
extern char g_unk_71729f1d82[];
UnkEnumType* fn_7100fe4008();
extern UnkEnumType g_unk_71729f1d88;
void fn_7100fe41f0(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100fe40e4();
extern char g_unk_71729f1ec0[];
extern char g_unk_71729f1ec1[];
UnkEnumType* fn_7100fe4374();
extern char g_unk_71729f1ec2[];
UnkEnumType* fn_7100fe4450();
extern char g_unk_71729f1ec3[];
UnkEnumType* fn_7100fe452c();
extern char g_unk_71729f1ec4[];
UnkEnumType* fn_7100fe4608();
extern UnkEnumType g_unk_71729f1ec8;
void fn_7100fe4a68(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100fe46e4();
extern char g_unk_71729f2000[];
extern char g_unk_71729f2001[];
UnkEnumType* fn_7100fe4bec();
extern UnkEnumType g_unk_71729f2008;
void fn_7100fe504c(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100fe4cc8();
extern char g_unk_71729f2140[];
extern char g_unk_71729f2141[];
UnkEnumType* fn_7100fe51d0();
extern UnkEnumType g_unk_71729f2148;
void fn_7100fe52ac(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71729f22b8[];
extern char g_unk_71729f22b9[];
UnkEnumType* fn_7100fe54a0();
extern char g_unk_71729f22ba[];
UnkEnumType* fn_7100fe557c();
extern UnkEnumType g_unk_71729f22c0;
void fn_7100fe58d4(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100fe5658();
extern char g_unk_71729f23f8[];
extern char g_unk_71729f23f9[];
UnkEnumType* fn_7100fe5bd4();
extern char g_unk_71729f23fa[];
UnkEnumType* fn_7100fe5cb0();
extern char g_unk_71729f23fb[];
UnkEnumType* fn_7100fe5d8c();
extern char g_unk_71729f23fc[];
UnkEnumType* fn_7100fe5e68();
extern UnkEnumType g_unk_71729f2400;
void fn_7100fe62c8(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100fe5f44();
extern char g_unk_71729f2538[];
extern char g_unk_71729f2539[];
UnkEnumType* fn_7100fe644c();
extern UnkEnumType g_unk_71729f2540;
void fn_7100fe68ac(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100fe6528();
extern char g_unk_71729f2678[];
extern char g_unk_71729f2679[];
UnkEnumType* fn_7100fe6a30();
extern UnkEnumType g_unk_71729f2680;
void fn_7100fe6b0c(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71729f27f0[];
extern char g_unk_71729f27f1[];
UnkEnumType* fn_7100fe6ce0();
extern char g_unk_71729f27f2[];
UnkEnumType* fn_7100fe6dbc();
extern UnkEnumType g_unk_71729f27f8;
void fn_7100fe6fa4(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100fe6e98();
extern char g_unk_71729f2930[];
extern char g_unk_71729f2931[];
UnkEnumType* fn_7100fe7128();
extern char g_unk_71729f2932[];
UnkEnumType* fn_7100fe7204();
extern char g_unk_71729f2933[];
UnkEnumType* fn_7100fe72e0();
extern char g_unk_71729f2934[];
UnkEnumType* fn_7100fe73bc();
extern UnkEnumType g_unk_71729f2938;
void fn_7100fe781c(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100fe7498();
extern char g_unk_71729f2a70[];
extern char g_unk_71729f2a71[];
UnkEnumType* fn_7100fe79a0();
extern UnkEnumType g_unk_71729f2a78;
void fn_7100fe7e00(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100fe7a7c();
extern char g_unk_71729f2bb0[];
extern char g_unk_71729f2bb1[];
UnkEnumType* fn_7100fe7f84();
extern UnkEnumType g_unk_71729f2bb8;
void fn_7100fe8060(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71729f2d28[];
extern char g_unk_71729f2d29[];
UnkEnumType* fn_7100fe8254();
extern char g_unk_71729f2d2a[];
UnkEnumType* fn_7100fe8330();
extern UnkEnumType g_unk_71729f2d30;
void fn_7100fe8518(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100fe840c();
extern char g_unk_71729f2e68[];
extern char g_unk_71729f2e69[];
UnkEnumType* fn_7100fe869c();
extern char g_unk_71729f2e6a[];
UnkEnumType* fn_7100fe8778();
extern char g_unk_71729f2e6b[];
UnkEnumType* fn_7100fe8854();
extern char g_unk_71729f2e6c[];
UnkEnumType* fn_7100fe8930();
extern UnkEnumType g_unk_71729f2e70;
void fn_7100fe8d90(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100fe8a0c();
extern char g_unk_71729f2fa8[];
extern char g_unk_71729f2fa9[];
UnkEnumType* fn_7100fe8f14();
extern UnkEnumType g_unk_71729f2fb0;
void fn_7100fe9374(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100fe8ff0();
extern char g_unk_71729f30e8[];
extern char g_unk_71729f30e9[];
UnkEnumType* fn_7100fe94f8();
extern UnkEnumType g_unk_71729f30f0;
void fn_7100fe95d4(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71729f3260[];
extern char g_unk_71729f3261[];
UnkEnumType* fn_7100fe97a8();
extern char g_unk_71729f3262[];
UnkEnumType* fn_7100fe9884();
extern UnkEnumType g_unk_71729f3268;
void fn_7100fea094(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100fe9960();
extern char g_unk_71729f33a0[];
extern char g_unk_71729f33a1[];
UnkEnumType* fn_7100fea510();
extern char g_unk_71729f33a2[];
UnkEnumType* fn_7100fea5ec();
extern char g_unk_71729f33a3[];
UnkEnumType* fn_7100fea6c8();
extern char g_unk_71729f33a4[];
UnkEnumType* fn_7100fea7a4();
extern UnkEnumType g_unk_71729f33a8;
void fn_7100feac04(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100fea880();
extern char g_unk_71729f34e0[];
extern char g_unk_71729f34e1[];
UnkEnumType* fn_7100fead88();
extern UnkEnumType g_unk_71729f34e8;
void fn_7100feb1e8(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100feae64();
extern char g_unk_71729f3620[];
extern char g_unk_71729f3621[];
UnkEnumType* fn_7100feb36c();
extern UnkEnumType g_unk_71729f3628;
void fn_7100feb448(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71729f3798[];
extern char g_unk_71729f3799[];
UnkEnumType* fn_7100feb61c();
extern char g_unk_71729f379a[];
UnkEnumType* fn_7100feb6f8();
extern UnkEnumType g_unk_71729f37a0;
void fn_7100febf08(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100feb7d4();
extern char g_unk_71729f38d8[];
extern char g_unk_71729f38d9[];
UnkEnumType* fn_7100fec384();
extern char g_unk_71729f38da[];
UnkEnumType* fn_7100fec460();
extern char g_unk_71729f38db[];
UnkEnumType* fn_7100fec53c();
extern char g_unk_71729f38dc[];
UnkEnumType* fn_7100fec618();
extern UnkEnumType g_unk_71729f38e0;
void fn_7100feca78(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100fec6f4();
extern char g_unk_71729f3a18[];
extern char g_unk_71729f3a19[];
UnkEnumType* fn_7100fecbfc();
extern UnkEnumType g_unk_71729f3a20;
void fn_7100fed05c(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100feccd8();
extern char g_unk_71729f3b58[];
extern char g_unk_71729f3b59[];
UnkEnumType* fn_7100fed1e0();
extern UnkEnumType g_unk_71729f3b60;
void fn_7100fed2bc(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71729f3cd0[];
extern char g_unk_71729f3cd1[];
UnkEnumType* fn_7100fed490();
extern char g_unk_71729f3cd2[];
UnkEnumType* fn_7100fed56c();

void fn_7101006110() {
    InitHeaderIds(&g_unk_71729f80d8.ids);
    {
        CStrId n("sound::TMusicFile", true);
        fn_7100fe1388(&g_unk_71729f1318, n, nullptr, nullptr, fn_7100fe119c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f1318, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f1450, &g_unk_71729f1318);
    fn_710009ec00(g_unk_71729f1451, fn_7100fe150c());
    fn_710009ec00(g_unk_71729f1452, fn_7100fe15e8());
    fn_710009ec00(g_unk_71729f1453, fn_7100fe16c4());
    fn_710009ec00(g_unk_71729f1454, fn_7100fe17a0());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<sound::TMusicFile>>::TKeyElementIterator::value_type", true);
        fn_7100fe1c00(&g_unk_71729f1458, n, &g_unk_7101d5e2e0, nullptr, fn_7100fe187c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f1458, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f1590, &g_unk_71729f1458);
    fn_710009ec00(g_unk_71729f1591, fn_7100fe1d84());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<sound::TMusicFile>>::TKeyElementConstIterator::value_type", true);
        fn_7100fe21e4(&g_unk_71729f1598, n, &g_unk_7101d5e2e0, nullptr, fn_7100fe1e60, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f1598, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f16d0, &g_unk_71729f1598);
    fn_710009ec00(g_unk_71729f16d1, fn_7100fe2368());
    {
        CStrId n("base::global::CRntVector<sound::TMusicFile>", true);
        fn_7100fe2444(&g_unk_71729f16d8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71729f16d8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f1848, &g_unk_71729f16d8);
    fn_710009ec00(g_unk_71729f1849, fn_7100fe2638());
    fn_710009ec00(g_unk_71729f184a, fn_7100fe2714());
    {
        CStrId n("sound::TMusicTrack", true);
        fn_7100fe2c7c(&g_unk_71729f1850, n, nullptr, nullptr, fn_7100fe27f0, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f1850, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f1988, &g_unk_71729f1850);
    fn_710009ec00(g_unk_71729f1989, fn_7100fe2e00());
    fn_710009ec00(g_unk_71729f198a, fn_7100fe2edc());
    fn_710009ec00(g_unk_71729f198b, fn_7100fe2fb8());
    fn_710009ec00(g_unk_71729f198c, fn_7100fe3094());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<sound::TMusicTrack>>::TKeyElementIterator::value_type", true);
        fn_7100fe34f4(&g_unk_71729f1990, n, &g_unk_7101d5e2e0, nullptr, fn_7100fe3170, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f1990, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f1ac8, &g_unk_71729f1990);
    fn_710009ec00(g_unk_71729f1ac9, fn_7100fe3678());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<sound::TMusicTrack>>::TKeyElementConstIterator::value_type", true);
        fn_7100fe3ad8(&g_unk_71729f1ad0, n, &g_unk_7101d5e2e0, nullptr, fn_7100fe3754, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f1ad0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f1c08, &g_unk_71729f1ad0);
    fn_710009ec00(g_unk_71729f1c09, fn_7100fe3c5c());
    {
        CStrId n("base::global::CRntVector<sound::TMusicTrack>", true);
        fn_7100fe3d38(&g_unk_71729f1c10, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71729f1c10, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f1d80, &g_unk_71729f1c10);
    fn_710009ec00(g_unk_71729f1d81, fn_7100fe3f2c());
    fn_710009ec00(g_unk_71729f1d82, fn_7100fe4008());
    {
        CStrId n("sound::SStateFadeOut", true);
        fn_7100fe41f0(&g_unk_71729f1d88, n, nullptr, nullptr, fn_7100fe40e4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f1d88, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f1ec0, &g_unk_71729f1d88);
    fn_710009ec00(g_unk_71729f1ec1, fn_7100fe4374());
    fn_710009ec00(g_unk_71729f1ec2, fn_7100fe4450());
    fn_710009ec00(g_unk_71729f1ec3, fn_7100fe452c());
    fn_710009ec00(g_unk_71729f1ec4, fn_7100fe4608());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<sound::SStateFadeOut>>::TKeyElementIterator::value_type", true);
        fn_7100fe4a68(&g_unk_71729f1ec8, n, &g_unk_7101d5e2e0, nullptr, fn_7100fe46e4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f1ec8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f2000, &g_unk_71729f1ec8);
    fn_710009ec00(g_unk_71729f2001, fn_7100fe4bec());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<sound::SStateFadeOut>>::TKeyElementConstIterator::value_type", true);
        fn_7100fe504c(&g_unk_71729f2008, n, &g_unk_7101d5e2e0, nullptr, fn_7100fe4cc8, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f2008, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f2140, &g_unk_71729f2008);
    fn_710009ec00(g_unk_71729f2141, fn_7100fe51d0());
    {
        CStrId n("base::global::CRntVector<sound::SStateFadeOut>", true);
        fn_7100fe52ac(&g_unk_71729f2148, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71729f2148, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f22b8, &g_unk_71729f2148);
    fn_710009ec00(g_unk_71729f22b9, fn_7100fe54a0());
    fn_710009ec00(g_unk_71729f22ba, fn_7100fe557c());
    {
        CStrId n("sound::TScenarioMusicPreset", true);
        fn_7100fe58d4(&g_unk_71729f22c0, n, nullptr, nullptr, fn_7100fe5658, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f22c0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f23f8, &g_unk_71729f22c0);
    fn_710009ec00(g_unk_71729f23f9, fn_7100fe5bd4());
    fn_710009ec00(g_unk_71729f23fa, fn_7100fe5cb0());
    fn_710009ec00(g_unk_71729f23fb, fn_7100fe5d8c());
    fn_710009ec00(g_unk_71729f23fc, fn_7100fe5e68());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, sound::TScenarioMusicPreset>>::TKeyElementIterator::value_type", true);
        fn_7100fe62c8(&g_unk_71729f2400, n, &g_unk_7101d5e2e0, nullptr, fn_7100fe5f44, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f2400, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f2538, &g_unk_71729f2400);
    fn_710009ec00(g_unk_71729f2539, fn_7100fe644c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, sound::TScenarioMusicPreset>>::TKeyElementConstIterator::value_type", true);
        fn_7100fe68ac(&g_unk_71729f2540, n, &g_unk_7101d5e2e0, nullptr, fn_7100fe6528, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f2540, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f2678, &g_unk_71729f2540);
    fn_710009ec00(g_unk_71729f2679, fn_7100fe6a30());
    {
        CStrId n("base::global::CRntDictionary<base::global::CStrId, sound::TScenarioMusicPreset>", true);
        fn_7100fe6b0c(&g_unk_71729f2680, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71729f2680, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f27f0, &g_unk_71729f2680);
    fn_710009ec00(g_unk_71729f27f1, fn_7100fe6ce0());
    fn_710009ec00(g_unk_71729f27f2, fn_7100fe6dbc());
    {
        CStrId n("sound::TBossMusicTrack", true);
        fn_7100fe6fa4(&g_unk_71729f27f8, n, nullptr, nullptr, fn_7100fe6e98, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f27f8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f2930, &g_unk_71729f27f8);
    fn_710009ec00(g_unk_71729f2931, fn_7100fe7128());
    fn_710009ec00(g_unk_71729f2932, fn_7100fe7204());
    fn_710009ec00(g_unk_71729f2933, fn_7100fe72e0());
    fn_710009ec00(g_unk_71729f2934, fn_7100fe73bc());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<sound::TBossMusicTrack>>::TKeyElementIterator::value_type", true);
        fn_7100fe781c(&g_unk_71729f2938, n, &g_unk_7101d5e2e0, nullptr, fn_7100fe7498, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f2938, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f2a70, &g_unk_71729f2938);
    fn_710009ec00(g_unk_71729f2a71, fn_7100fe79a0());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<sound::TBossMusicTrack>>::TKeyElementConstIterator::value_type", true);
        fn_7100fe7e00(&g_unk_71729f2a78, n, &g_unk_7101d5e2e0, nullptr, fn_7100fe7a7c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f2a78, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f2bb0, &g_unk_71729f2a78);
    fn_710009ec00(g_unk_71729f2bb1, fn_7100fe7f84());
    {
        CStrId n("base::global::CRntVector<sound::TBossMusicTrack>", true);
        fn_7100fe8060(&g_unk_71729f2bb8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71729f2bb8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f2d28, &g_unk_71729f2bb8);
    fn_710009ec00(g_unk_71729f2d29, fn_7100fe8254());
    fn_710009ec00(g_unk_71729f2d2a, fn_7100fe8330());
    {
        CStrId n("sound::TBossMusicSubStateConfig", true);
        fn_7100fe8518(&g_unk_71729f2d30, n, nullptr, nullptr, fn_7100fe840c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f2d30, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f2e68, &g_unk_71729f2d30);
    fn_710009ec00(g_unk_71729f2e69, fn_7100fe869c());
    fn_710009ec00(g_unk_71729f2e6a, fn_7100fe8778());
    fn_710009ec00(g_unk_71729f2e6b, fn_7100fe8854());
    fn_710009ec00(g_unk_71729f2e6c, fn_7100fe8930());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, sound::TBossMusicSubStateConfig>>::TKeyElementIterator::value_type", true);
        fn_7100fe8d90(&g_unk_71729f2e70, n, &g_unk_7101d5e2e0, nullptr, fn_7100fe8a0c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f2e70, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f2fa8, &g_unk_71729f2e70);
    fn_710009ec00(g_unk_71729f2fa9, fn_7100fe8f14());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, sound::TBossMusicSubStateConfig>>::TKeyElementConstIterator::value_type", true);
        fn_7100fe9374(&g_unk_71729f2fb0, n, &g_unk_7101d5e2e0, nullptr, fn_7100fe8ff0, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f2fb0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f30e8, &g_unk_71729f2fb0);
    fn_710009ec00(g_unk_71729f30e9, fn_7100fe94f8());
    {
        CStrId n("base::global::CRntDictionary<base::global::CStrId, sound::TBossMusicSubStateConfig>", true);
        fn_7100fe95d4(&g_unk_71729f30f0, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71729f30f0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f3260, &g_unk_71729f30f0);
    fn_710009ec00(g_unk_71729f3261, fn_7100fe97a8());
    fn_710009ec00(g_unk_71729f3262, fn_7100fe9884());
    {
        CStrId n("sound::TBossMusicSpawnGroupConfig", true);
        fn_7100fea094(&g_unk_71729f3268, n, nullptr, nullptr, fn_7100fe9960, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f3268, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f33a0, &g_unk_71729f3268);
    fn_710009ec00(g_unk_71729f33a1, fn_7100fea510());
    fn_710009ec00(g_unk_71729f33a2, fn_7100fea5ec());
    fn_710009ec00(g_unk_71729f33a3, fn_7100fea6c8());
    fn_710009ec00(g_unk_71729f33a4, fn_7100fea7a4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, sound::TBossMusicSpawnGroupConfig>>::TKeyElementIterator::value_type", true);
        fn_7100feac04(&g_unk_71729f33a8, n, &g_unk_7101d5e2e0, nullptr, fn_7100fea880, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f33a8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f34e0, &g_unk_71729f33a8);
    fn_710009ec00(g_unk_71729f34e1, fn_7100fead88());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, sound::TBossMusicSpawnGroupConfig>>::TKeyElementConstIterator::value_type", true);
        fn_7100feb1e8(&g_unk_71729f34e8, n, &g_unk_7101d5e2e0, nullptr, fn_7100feae64, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f34e8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f3620, &g_unk_71729f34e8);
    fn_710009ec00(g_unk_71729f3621, fn_7100feb36c());
    {
        CStrId n("base::global::CRntDictionary<base::global::CStrId, sound::TBossMusicSpawnGroupConfig>", true);
        fn_7100feb448(&g_unk_71729f3628, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71729f3628, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f3798, &g_unk_71729f3628);
    fn_710009ec00(g_unk_71729f3799, fn_7100feb61c());
    fn_710009ec00(g_unk_71729f379a, fn_7100feb6f8());
    {
        CStrId n("sound::TBossMusicPreset", true);
        fn_7100febf08(&g_unk_71729f37a0, n, nullptr, nullptr, fn_7100feb7d4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f37a0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f38d8, &g_unk_71729f37a0);
    fn_710009ec00(g_unk_71729f38d9, fn_7100fec384());
    fn_710009ec00(g_unk_71729f38da, fn_7100fec460());
    fn_710009ec00(g_unk_71729f38db, fn_7100fec53c());
    fn_710009ec00(g_unk_71729f38dc, fn_7100fec618());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, sound::TBossMusicPreset>>::TKeyElementIterator::value_type", true);
        fn_7100feca78(&g_unk_71729f38e0, n, &g_unk_7101d5e2e0, nullptr, fn_7100fec6f4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f38e0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f3a18, &g_unk_71729f38e0);
    fn_710009ec00(g_unk_71729f3a19, fn_7100fecbfc());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, sound::TBossMusicPreset>>::TKeyElementConstIterator::value_type", true);
        fn_7100fed05c(&g_unk_71729f3a20, n, &g_unk_7101d5e2e0, nullptr, fn_7100feccd8, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71729f3a20, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f3b58, &g_unk_71729f3a20);
    fn_710009ec00(g_unk_71729f3b59, fn_7100fed1e0());
    {
        CStrId n("base::global::CRntDictionary<base::global::CStrId, sound::TBossMusicPreset>", true);
        fn_7100fed2bc(&g_unk_71729f3b60, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71729f3b60, g_unk_7101a19000);
    fn_710009ec00(g_unk_71729f3cd0, &g_unk_71729f3b60);
    fn_710009ec00(g_unk_71729f3cd1, fn_7100fed490());
    fn_710009ec00(g_unk_71729f3cd2, fn_7100fed56c());
}

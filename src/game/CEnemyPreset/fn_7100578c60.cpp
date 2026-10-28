// Static initialiser of the unit of the base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CEnemyPreset>>>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CEnemyPreset>>>::TKeyElementConstIterator::value_type, base::global::CRntVector<std::unique_ptr<CEnemyPreset>>.
#include "base/reflection/UnkReflTypeGlobals.h"
#include "../../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_7100578c60 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_7100578c60 g_unk_71726e3ec8 __attribute__((visibility("hidden")));
extern char g_unk_71726e3440[];
UnkEnumType* fn_710057513c();
extern char g_unk_71726e3441[];
UnkEnumType* fn_7100575218();  // base::reflection::refl_GetType<std::unique_ptr<CEnemyPreset>>
extern char g_unk_71726e3442[];
UnkEnumType* fn_71005752f4();
extern char g_unk_71726e3443[];
UnkEnumType* fn_71005753d0();
extern UnkEnumType g_unk_71726e3448;
void fn_71005754ac();
void fn_7100575830(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71726e3580[];
extern char g_unk_71726e3581[];
UnkEnumType* fn_71005759b4();
extern UnkEnumType g_unk_71726e3588;
void fn_7100575a90();
void fn_7100575e14(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71726e36c0[];
extern char g_unk_71726e36c1[];
UnkEnumType* fn_7100575f98();
extern UnkEnumType g_unk_71726e36c8;
void fn_7100576074(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71726e3838[];
extern UnkConversion g_unk_71726e3840;
UnkEnumType* fn_7100575218();
void fn_7100577e50() __attribute__((visibility("hidden")));
extern char g_unk_71726e3860[];

void fn_7100578c60() {
    InitHeaderIds(&g_unk_71726e3ec8.ids);
    fn_710009ec00(g_unk_71726e3440, fn_710057513c());
    fn_710009ec00(g_unk_71726e3441, fn_7100575218());
    fn_710009ec00(g_unk_71726e3442, fn_71005752f4());
    fn_710009ec00(g_unk_71726e3443, fn_71005753d0());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CEnemyPreset>>>::TKeyElementIterator::value_type", true);
        fn_7100575830(&g_unk_71726e3448, n, &g_unk_7101d5e2e0, nullptr, fn_71005754ac, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726e3448, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726e3580, &g_unk_71726e3448);
    fn_710009ec00(g_unk_71726e3581, fn_71005759b4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CEnemyPreset>>>::TKeyElementConstIterator::value_type", true);
        fn_7100575e14(&g_unk_71726e3588, n, &g_unk_7101d5e2e0, nullptr, fn_7100575a90, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726e3588, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726e36c0, &g_unk_71726e3588);
    fn_710009ec00(g_unk_71726e36c1, fn_7100575f98());
    {
        CStrId n("base::global::CRntVector<std::unique_ptr<CEnemyPreset>>", true);
        fn_7100576074(&g_unk_71726e36c8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726e36c8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726e3838, &g_unk_71726e36c8);
    InitConversion(&g_unk_71726e3840, fn_7100575218, fn_710057513c, fn_7100577e50);
    fn_7100576258();
    fn_710009ec00(g_unk_71726e3860, g_unk_71726e3ec8.field_0x20);
}

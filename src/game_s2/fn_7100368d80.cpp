// Static initialiser of the unit of the base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CAttackPreset>>>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CAttackPreset>>>::TKeyElementConstIterator::value_type, base::global::CRntVector<std::unique_ptr<CAttackPreset>>.
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_7100368d80 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_7100368d80 g_unk_71726b30e0 __attribute__((visibility("hidden")));
extern char g_unk_71726b2658[];
UnkEnumType* fn_7100364820();
extern char g_unk_71726b2659[];
UnkEnumType* fn_71003648fc();  // base::reflection::refl_GetType<std::unique_ptr<CAttackPreset>>
extern char g_unk_71726b265a[];
UnkEnumType* fn_71003649d8();
extern char g_unk_71726b265b[];
UnkEnumType* fn_7100364ab4();
extern UnkEnumType g_unk_71726b2660;
extern char g_unk_7101d5e2e0[];
void fn_7100364b90();
void fn_7100364f14(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726b2798[];
extern char g_unk_71726b2799[];
UnkEnumType* fn_7100365098();
extern UnkEnumType g_unk_71726b27a0;
void fn_7100365174();
void fn_71003654f8(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726b28d8[];
extern char g_unk_71726b28d9[];
UnkEnumType* fn_710036567c();
extern UnkEnumType g_unk_71726b28e0;
void fn_7100365758(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71726b2a50[];
extern UnkConversion g_unk_71726b2a58;
UnkEnumType* fn_71003648fc();
void fn_7100367d78() __attribute__((visibility("hidden")));
extern char g_unk_71726b2a78[];

void fn_7100368d80() {
    InitHeaderIds(&g_unk_71726b30e0.ids);
    fn_710009ec00(g_unk_71726b2658, fn_7100364820());
    fn_710009ec00(g_unk_71726b2659, fn_71003648fc());
    fn_710009ec00(g_unk_71726b265a, fn_71003649d8());
    fn_710009ec00(g_unk_71726b265b, fn_7100364ab4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CAttackPreset>>>::TKeyElementIterator::value_type", true);
        fn_7100364f14(&g_unk_71726b2660, n, g_unk_7101d5e2e0, nullptr, fn_7100364b90, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726b2660, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726b2798, &g_unk_71726b2660);
    fn_710009ec00(g_unk_71726b2799, fn_7100365098());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CAttackPreset>>>::TKeyElementConstIterator::value_type", true);
        fn_71003654f8(&g_unk_71726b27a0, n, g_unk_7101d5e2e0, nullptr, fn_7100365174, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726b27a0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726b28d8, &g_unk_71726b27a0);
    fn_710009ec00(g_unk_71726b28d9, fn_710036567c());
    {
        CStrId n("base::global::CRntVector<std::unique_ptr<CAttackPreset>>", true);
        fn_7100365758(&g_unk_71726b28e0, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726b28e0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726b2a50, &g_unk_71726b28e0);
    InitConversion(&g_unk_71726b2a58, fn_71003648fc, fn_7100364820, fn_7100367d78);
    fn_710036593c();
    fn_710009ec00(g_unk_71726b2a78, g_unk_71726b30e0.field_0x20);
}

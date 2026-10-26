// Static initialiser of the unit of
// base::global::CRntVector<std::unique_ptr<CCooldownXBossFireWallDef>>,
// base::global::CRntVector<base::global::CRntVector<std::unique_ptr<CCooldownXBossFireWallDef>>>,
// base::global::CRntVector<CCooldownXBossFireBallDef>: the header ids, then the unit's own statics
// and registrations (helpers in UnkStaticInitS7.h). Layouts are placeholders.
#include "UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "game/UnkTUProperty.h"

using namespace unk_static_init;

struct UnkStatics_71004c0de0 {
    HeaderIds ids;
    u8 pad_0x18[0x20];
    u8 field_0x38[8];
    u8 pad_0x40[0x130];
    u8 field_0x170[8];
    u8 pad_0x178[0x130];
    u8 field_0x2a8[8];
    u8 pad_0x2b0[0x130];
    u8 field_0x3e0[8];
};
extern UnkStatics_71004c0de0 g_unk_71726d7350 __attribute__((visibility("hidden")));
extern char g_unk_71726d5820[];
extern char g_unk_71726d5821[];
extern char g_unk_71726d5822[];
UnkEnumType* fn_71004b5dc0();
extern char g_unk_71726d5823[];
UnkEnumType* fn_71004b5e9c();
extern char g_unk_71726d5824[];
UnkEnumType* fn_71004b5f78();
extern char g_unk_71726d5825[];
UnkEnumType* fn_71004b6054();
extern UnkEnumType g_unk_71726d5828;
void fn_71004b64b4(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_7101d5e2e0[];
void fn_71004b6130();
extern char g_unk_71726d5960[];
extern char g_unk_71726d5961[];
UnkEnumType* fn_71004b6638();
extern UnkEnumType g_unk_71726d5968;
void fn_71004b6a98(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_71004b6714();
extern char g_unk_71726d5aa0[];
extern char g_unk_71726d5aa1[];
UnkEnumType* fn_71004b6c1c();
extern UnkEnumType g_unk_71726d5aa8;
void fn_71004b6cf8(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71726d5c18[];
extern char g_unk_71726d5c19[];
UnkEnumType* fn_71004b6ed8();
extern char g_unk_71726d5c1a[];
UnkEnumType* fn_71004b6fb4();
extern UnkEnumType g_unk_71726d5c20;
void fn_71004b7414(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_71004b7090();
extern char g_unk_71726d5d58[];
extern char g_unk_71726d5d59[];
UnkEnumType* fn_71004b7598();
extern UnkEnumType g_unk_71726d5d60;
void fn_71004b79f8(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_71004b7674();
extern char g_unk_71726d5e98[];
extern char g_unk_71726d5e99[];
UnkEnumType* fn_71004b7b7c();
extern UnkEnumType g_unk_71726d5ea0;
void fn_71004b7c58(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71726d6010[];
extern UnkConversion g_unk_71726d6018;
void fn_71004bc4a4() __attribute__((visibility("hidden")));
extern char g_unk_71726d6038[];
UnkEnumType* fn_71004b7e3c();
extern char g_unk_71726d6039[];
UnkEnumType* fn_71004b7f18();
extern UnkEnumType g_unk_71726d6040;
void fn_71004b8384(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_71004b7ff4();
extern char g_unk_71726d6178[];
extern char g_unk_71726d6179[];
UnkEnumType* fn_71004b8508();
extern UnkEnumType g_unk_71726d6180;
void fn_71004b8974(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_71004b85e4();
extern char g_unk_71726d62b8[];
extern char g_unk_71726d62b9[];
UnkEnumType* fn_71004b8af8();
extern UnkEnumType g_unk_71726d62c0;
void fn_71004b8bd4(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71726d6430[];
extern char g_unk_71726d6431[];
extern UnkTunable g_unk_71726d6438;
extern void* g_unk_7101a3e378[];
void fn_7100075104(void* self);  // its destructor, as registered
extern char g_unk_71726d6469[];

void fn_71004c0de0() {
    InitHeaderIds(&g_unk_71726d7350.ids);
    fn_71004b518c();
    fn_710009ec00(g_unk_71726d5820, g_unk_71726d7350.field_0x38);
    fn_71004b5628();
    fn_710009ec00(g_unk_71726d5821, g_unk_71726d7350.field_0x170);
    fn_710009ec00(g_unk_71726d5822, fn_71004b5dc0());
    fn_710009ec00(g_unk_71726d5823, fn_71004b5e9c());
    fn_710009ec00(g_unk_71726d5824, fn_71004b5f78());
    fn_710009ec00(g_unk_71726d5825, fn_71004b6054());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CCooldownXBossFireWallDef>>>::TKeyElementIterator::value_type", true);
        fn_71004b64b4(&g_unk_71726d5828, n, g_unk_7101d5e2e0, nullptr, fn_71004b6130, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726d5828, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d5960, &g_unk_71726d5828);
    fn_710009ec00(g_unk_71726d5961, fn_71004b6638());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CCooldownXBossFireWallDef>>>::TKeyElementConstIterator::value_type", true);
        fn_71004b6a98(&g_unk_71726d5968, n, g_unk_7101d5e2e0, nullptr, fn_71004b6714, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726d5968, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d5aa0, &g_unk_71726d5968);
    fn_710009ec00(g_unk_71726d5aa1, fn_71004b6c1c());
    {
        CStrId n("base::global::CRntVector<std::unique_ptr<CCooldownXBossFireWallDef>>", true);
        fn_71004b6cf8(&g_unk_71726d5aa8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726d5aa8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d5c18, &g_unk_71726d5aa8);
    fn_710009ec00(g_unk_71726d5c19, fn_71004b6ed8());
    fn_710009ec00(g_unk_71726d5c1a, fn_71004b6fb4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CRntVector<std::unique_ptr<CCooldownXBossFireWallDef>>>>::TKeyElementIterator::value_type", true);
        fn_71004b7414(&g_unk_71726d5c20, n, g_unk_7101d5e2e0, nullptr, fn_71004b7090, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726d5c20, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d5d58, &g_unk_71726d5c20);
    fn_710009ec00(g_unk_71726d5d59, fn_71004b7598());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CRntVector<std::unique_ptr<CCooldownXBossFireWallDef>>>>::TKeyElementConstIterator::value_type", true);
        fn_71004b79f8(&g_unk_71726d5d60, n, g_unk_7101d5e2e0, nullptr, fn_71004b7674, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726d5d60, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d5e98, &g_unk_71726d5d60);
    fn_710009ec00(g_unk_71726d5e99, fn_71004b7b7c());
    {
        CStrId n("base::global::CRntVector<base::global::CRntVector<std::unique_ptr<CCooldownXBossFireWallDef>>>", true);
        fn_71004b7c58(&g_unk_71726d5ea0, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726d5ea0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d6010, &g_unk_71726d5ea0);
    InitConversion(&g_unk_71726d6018, fn_71004b5e9c, fn_71004b5dc0, fn_71004bc4a4);
    fn_710009ec00(g_unk_71726d6038, fn_71004b7e3c());
    fn_710009ec00(g_unk_71726d6039, fn_71004b7f18());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CCooldownXBossFireBallDef>>::TKeyElementIterator::value_type", true);
        fn_71004b8384(&g_unk_71726d6040, n, g_unk_7101d5e2e0, nullptr, fn_71004b7ff4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726d6040, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d6178, &g_unk_71726d6040);
    fn_710009ec00(g_unk_71726d6179, fn_71004b8508());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CCooldownXBossFireBallDef>>::TKeyElementConstIterator::value_type", true);
        fn_71004b8974(&g_unk_71726d6180, n, g_unk_7101d5e2e0, nullptr, fn_71004b85e4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726d6180, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d62b8, &g_unk_71726d6180);
    fn_710009ec00(g_unk_71726d62b9, fn_71004b8af8());
    {
        CStrId n("base::global::CRntVector<CCooldownXBossFireBallDef>", true);
        fn_71004b8bd4(&g_unk_71726d62c0, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726d62c0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726d6430, &g_unk_71726d62c0);
    fn_71004b8dd0();
    fn_710009ec00(g_unk_71726d6431, g_unk_71726d7350.field_0x2a8);
    InitTunable(&g_unk_71726d6438, fn_7100136ecc, g_unk_7101a3e378, fn_7100075104);
    fn_71004b9584();
    fn_710009ec00(g_unk_71726d6469, g_unk_71726d7350.field_0x3e0);
}

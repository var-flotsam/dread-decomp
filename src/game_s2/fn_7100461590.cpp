// Static initialiser of the unit of the base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CChozoRobotSoldierCannonShotPattern>>>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CChozoRobotSoldierCannonShotPattern>>>::TKeyElementConstIterator::value_type, base::global::CRntVector<std::unique_ptr<CChozoRobotSoldierCannonShotPattern>>.
#include "base/reflection/UnkReflTypeGlobals.h"
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "game/UnkTUProperty.h"

using namespace unk_static_init;

struct UnkStatics_7100461590 {
    HeaderIds ids;
    u8 pad_0x18[0x28];
    u8 field_0x40[8];
    u8 pad_0x48[0x130];
    u8 field_0x178[8];
    u8 pad_0x180[0x130];
    u8 field_0x2b0[8];
};
extern UnkStatics_7100461590 g_unk_71726cad28 __attribute__((visibility("hidden")));
extern char g_unk_71726ca260[];
extern char g_unk_71726ca261[];
UnkEnumType* fn_710045d1c0();
extern char g_unk_71726ca262[];
UnkEnumType* fn_710045d29c();  // base::reflection::refl_GetType<std::unique_ptr<CChozoRobotSoldierCannonShotPattern>>
extern char g_unk_71726ca263[];
UnkEnumType* fn_710045d378();
extern char g_unk_71726ca264[];
UnkEnumType* fn_710045d454();
extern UnkEnumType g_unk_71726ca268;
void fn_710045d530();
void fn_710045d8b4(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71726ca3a0[];
extern char g_unk_71726ca3a1[];
UnkEnumType* fn_710045da38();
extern UnkEnumType g_unk_71726ca3a8;
void fn_710045db14();
void fn_710045de98(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71726ca4e0[];
extern char g_unk_71726ca4e1[];
UnkEnumType* fn_710045e01c();
extern UnkEnumType g_unk_71726ca4e8;
void fn_710045e0f8(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71726ca658[];
extern UnkConversion g_unk_71726ca660;
UnkEnumType* fn_710045d29c();
void fn_71004603f0() __attribute__((visibility("hidden")));
extern char g_unk_71726ca680[];
extern UnkTunable g_unk_71726ca688;
extern void* g_unk_7101a38178[];
void fn_7100075104(void* self);  // its destructor, as registered
extern char g_unk_71726ca6c1[];

void fn_7100461590() {
    InitHeaderIds(&g_unk_71726cad28.ids);
    fn_710045cd20();
    fn_710009ec00(g_unk_71726ca260, g_unk_71726cad28.field_0x40);
    fn_710009ec00(g_unk_71726ca261, fn_710045d1c0());
    fn_710009ec00(g_unk_71726ca262, fn_710045d29c());
    fn_710009ec00(g_unk_71726ca263, fn_710045d378());
    fn_710009ec00(g_unk_71726ca264, fn_710045d454());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CChozoRobotSoldierCannonShotPattern>>>::TKeyElementIterator::value_type", true);
        fn_710045d8b4(&g_unk_71726ca268, n, &g_unk_7101d5e2e0, nullptr, fn_710045d530, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726ca268, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726ca3a0, &g_unk_71726ca268);
    fn_710009ec00(g_unk_71726ca3a1, fn_710045da38());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CChozoRobotSoldierCannonShotPattern>>>::TKeyElementConstIterator::value_type", true);
        fn_710045de98(&g_unk_71726ca3a8, n, &g_unk_7101d5e2e0, nullptr, fn_710045db14, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726ca3a8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726ca4e0, &g_unk_71726ca3a8);
    fn_710009ec00(g_unk_71726ca4e1, fn_710045e01c());
    {
        CStrId n("base::global::CRntVector<std::unique_ptr<CChozoRobotSoldierCannonShotPattern>>", true);
        fn_710045e0f8(&g_unk_71726ca4e8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726ca4e8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726ca658, &g_unk_71726ca4e8);
    InitConversion(&g_unk_71726ca660, fn_710045d29c, fn_710045d1c0, fn_71004603f0);
    fn_710045e2dc();
    fn_710009ec00(g_unk_71726ca680, g_unk_71726cad28.field_0x178);
    InitTunable(&g_unk_71726ca688, fn_7100136ecc, g_unk_7101a38178, fn_7100075104);
    fn_710045eb2c();
    fn_710009ec00(g_unk_71726ca6c1, g_unk_71726cad28.field_0x2b0);
}

// Static initialiser of the unit of the base::reflection::CollectionTypeMapper<base::global::CRntVector< std::unique_ptr<CPlaythroughDef::SCheckpointDef> >>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntVector< std::unique_ptr<CPlaythroughDef::SCheckpointDef> >>::TKeyElementConstIterator::value_type, base::global::CRntVector< std::unique_ptr<CPlaythroughDef::SCheckpointDef> >.
#include "base/reflection/UnkReflTypeGlobals.h"
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkFnObj {
    u64 field_0x0[3];
    void (*field_0x18)();
    u64 field_0x20[2];
};

struct UnkStatics_71002f6180 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    UnkFnObj field_0x28;
    UnkFnObj field_0x58;
    UnkFnObj field_0x88;
    u8 field_0xb8[8];
    u8 pad_0xc0[0x130];
    u8 field_0x1f0[8];
};
extern UnkStatics_71002f6180 g_unk_71726ae580 __attribute__((visibility("hidden")));
extern char g_unk_71726ada10[];
extern char g_unk_71726ada11[];
extern char g_unk_71726ada12[];
UnkEnumType* fn_71002f1284();
extern char g_unk_71726ada13[];
UnkEnumType* fn_71002f1360();
extern UnkEnumType g_unk_71726ada18;
void fn_71002f143c();
void fn_71002f17c0(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71726adb50[];
extern char g_unk_71726adb51[];
UnkEnumType* fn_71002f1a20();
extern UnkEnumType g_unk_71726adb58;
void fn_71002f1afc();
void fn_71002f1e80(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71726adc90[];
extern char g_unk_71726adc91[];
UnkEnumType* fn_71002f2004();
extern UnkEnumType g_unk_71726adc98;
void fn_71002f20e0(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71726ade08[];
extern char g_unk_71726ade09[];
UnkEnumType* fn_71002f1944();  // base::reflection::refl_GetType<std::unique_ptr<CPlaythroughDef::SCheckpointDef>>
extern char g_unk_71726ade0a[];
UnkEnumType* fn_71002f22c0();
extern char g_unk_71726ade0b[];
UnkEnumType* fn_71002ee1dc();
extern char g_unk_71726ade0c[];
UnkEnumType* fn_71002f239c();

void fn_71002f6180() {
    InitHeaderIds(&g_unk_71726ae580.ids);
    g_unk_71726ae580.field_0x28 = {{0, 0, 0}, fn_710000cf00, {0, 0}};
    g_unk_71726ae580.field_0x58 = {{0, 0, 0}, fn_710000d470, {0, 0}};
    g_unk_71726ae580.field_0x88 = {{0, 0, 0}, fn_710000d4f4, {0, 0}};
    fn_71002effc4();
    fn_710009ec00(g_unk_71726ada10, g_unk_71726ae580.field_0xb8);
    fn_71002f07e0();
    fn_710009ec00(g_unk_71726ada11, g_unk_71726ae580.field_0x1f0);
    fn_710009ec00(g_unk_71726ada12, fn_71002f1284());
    fn_710009ec00(g_unk_71726ada13, fn_71002f1360());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector< std::unique_ptr<CPlaythroughDef::SCheckpointDef> >>::TKeyElementIterator::value_type", true);
        fn_71002f17c0(&g_unk_71726ada18, n, &g_unk_7101d5e2e0, nullptr, fn_71002f143c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726ada18, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726adb50, &g_unk_71726ada18);
    fn_710009ec00(g_unk_71726adb51, fn_71002f1a20());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector< std::unique_ptr<CPlaythroughDef::SCheckpointDef> >>::TKeyElementConstIterator::value_type", true);
        fn_71002f1e80(&g_unk_71726adb58, n, &g_unk_7101d5e2e0, nullptr, fn_71002f1afc, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726adb58, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726adc90, &g_unk_71726adb58);
    fn_710009ec00(g_unk_71726adc91, fn_71002f2004());
    {
        CStrId n("base::global::CRntVector< std::unique_ptr<CPlaythroughDef::SCheckpointDef> >", true);
        fn_71002f20e0(&g_unk_71726adc98, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726adc98, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726ade08, &g_unk_71726adc98);
    fn_710009ec00(g_unk_71726ade09, fn_71002f1944());
    fn_710009ec00(g_unk_71726ade0a, fn_71002f22c0());
    fn_710009ec00(g_unk_71726ade0b, fn_71002ee1dc());
    fn_710009ec00(g_unk_71726ade0c, fn_71002f239c());
}

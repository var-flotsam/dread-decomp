// Static initialiser of the unit of the base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CPolypFallPattern>>>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CPolypFallPattern>>>::TKeyElementConstIterator::value_type, base::global::CRntVector<std::unique_ptr<CPolypFallPattern>> reflected types.
#include "base/reflection/UnkReflTypeGlobals.h"
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_71005b1e00 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    u8 field_0x28[8];
    u8 pad_0x30[0x130];
    u8 field_0x160[8];
};
extern UnkStatics_71005b1e00 g_unk_71726ea338 __attribute__((visibility("hidden")));
extern char g_unk_71726e98b0[];
extern char g_unk_71726e98b1[];
UnkEnumType* fn_71005adf20();
extern char g_unk_71726e98b2[];
UnkEnumType* fn_71005adffc();  // base::reflection::refl_GetType<std::unique_ptr<CPolypFallPattern>>
extern char g_unk_71726e98b3[];
UnkEnumType* fn_71005ae0d8();
extern char g_unk_71726e98b4[];
UnkEnumType* fn_71005ae1b4();
void fn_71005ae290();
extern UnkEnumType g_unk_71726e98b8;
void fn_71005ae614(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71726e99f0[];
extern char g_unk_71726e99f1[];
UnkEnumType* fn_71005ae798();
void fn_71005ae874();
extern UnkEnumType g_unk_71726e99f8;
void fn_71005aebf8(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_71726e9b30[];
extern char g_unk_71726e9b31[];
UnkEnumType* fn_71005aed7c();
extern UnkEnumType g_unk_71726e9b38;
void fn_71005aee58(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71726e9ca8[];
extern UnkConversion g_unk_71726e9cb0;
void fn_71005b0d08() __attribute__((visibility("hidden")));
extern char g_unk_71726e9cd0[];

void fn_71005b1e00() {
    InitHeaderIds(&g_unk_71726ea338.ids);
    fn_71005ada84();
    fn_710009ec00(g_unk_71726e98b0, g_unk_71726ea338.field_0x28);
    fn_710009ec00(g_unk_71726e98b1, fn_71005adf20());
    fn_710009ec00(g_unk_71726e98b2, fn_71005adffc());
    fn_710009ec00(g_unk_71726e98b3, fn_71005ae0d8());
    fn_710009ec00(g_unk_71726e98b4, fn_71005ae1b4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CPolypFallPattern>>>::TKeyElementIterator::value_type", true);
        fn_71005ae614(&g_unk_71726e98b8, n, &g_unk_7101d5e2e0, nullptr, fn_71005ae290, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726e98b8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726e99f0, &g_unk_71726e98b8);
    fn_710009ec00(g_unk_71726e99f1, fn_71005ae798());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CPolypFallPattern>>>::TKeyElementConstIterator::value_type", true);
        fn_71005aebf8(&g_unk_71726e99f8, n, &g_unk_7101d5e2e0, nullptr, fn_71005ae874, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726e99f8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726e9b30, &g_unk_71726e99f8);
    fn_710009ec00(g_unk_71726e9b31, fn_71005aed7c());
    {
        CStrId n("base::global::CRntVector<std::unique_ptr<CPolypFallPattern>>", true);
        fn_71005aee58(&g_unk_71726e9b38, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726e9b38, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726e9ca8, &g_unk_71726e9b38);
    InitConversion(&g_unk_71726e9cb0, fn_71005adffc, fn_71005adf20, fn_71005b0d08);
    fn_71005af03c();
    fn_710009ec00(g_unk_71726e9cd0, g_unk_71726ea338.field_0x160);
}

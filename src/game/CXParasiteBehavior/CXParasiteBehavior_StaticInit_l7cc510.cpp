// Static initialiser of CXParasiteBehavior's unit: the class, the conversion between its pointer and
// unique_ptr types, the key element value types of the vector of unique_ptrs and the vector itself.
// Names and layouts are placeholders.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_l7cc510 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_l7cc510 g_unk_717271c978 __attribute__((visibility("hidden")));
extern char g_unk_717271bef0[];
extern char g_unk_717271bef1[];
UnkEnumType* fn_71007c9104();
extern char g_unk_717271bef2[];
UnkEnumType* fn_71007c91e0();  // base::reflection::refl_GetType<std::unique_ptr<CXParasiteBehavior>>
extern UnkConversion g_unk_717271bef8;
void fn_71007cb588();
extern char g_unk_717271bf18[];
UnkEnumType* fn_71007c92bc();
extern char g_unk_717271bf19[];
UnkEnumType* fn_71007c9398();
extern UnkEnumType g_unk_717271bf20;
void fn_71007c9474();
void fn_71007c97f8(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_717271c058[];
extern char g_unk_717271c059[];
UnkEnumType* fn_71007c997c();
extern UnkEnumType g_unk_717271c060;
void fn_71007c9a58();
void fn_71007c9ddc(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_717271c198[];
extern char g_unk_717271c199[];
UnkEnumType* fn_71007c9f60();
extern UnkEnumType g_unk_717271c1a0;
void fn_71007ca03c(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_717271c310[];

void fn_71007cc510() {
    InitHeaderIds(&g_unk_717271c978.ids);
    fn_71007c8a94();
    fn_710009ec00(g_unk_717271bef0, g_unk_717271c978.field_0x20);
    fn_710009ec00(g_unk_717271bef1, fn_71007c9104());
    fn_710009ec00(g_unk_717271bef2, fn_71007c91e0());
    InitConversion(&g_unk_717271bef8, fn_71007c9104, fn_71007c91e0, fn_71007cb588);
    fn_710009ec00(g_unk_717271bf18, fn_71007c92bc());
    fn_710009ec00(g_unk_717271bf19, fn_71007c9398());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CXParasiteBehavior>>>::TKeyElementIterator::value_type", true);
        fn_71007c97f8(&g_unk_717271bf20, n, &g_unk_7101d5e2e0, nullptr, fn_71007c9474, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717271bf20, g_unk_7101a19000);
    fn_710009ec00(g_unk_717271c058, &g_unk_717271bf20);
    fn_710009ec00(g_unk_717271c059, fn_71007c997c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CXParasiteBehavior>>>::TKeyElementConstIterator::value_type", true);
        fn_71007c9ddc(&g_unk_717271c060, n, &g_unk_7101d5e2e0, nullptr, fn_71007c9a58, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717271c060, g_unk_7101a19000);
    fn_710009ec00(g_unk_717271c198, &g_unk_717271c060);
    fn_710009ec00(g_unk_717271c199, fn_71007c9f60());
    {
        CStrId n("base::global::CRntVector<std::unique_ptr<CXParasiteBehavior>>", true);
        fn_71007ca03c(&g_unk_717271c1a0, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_717271c1a0, g_unk_7101a19000);
    fn_710009ec00(g_unk_717271c310, &g_unk_717271c1a0);
}

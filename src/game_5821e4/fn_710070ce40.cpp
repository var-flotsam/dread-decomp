// Static initialiser of the unit of the base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CTentacle>>>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CTentacle>>>::TKeyElementConstIterator::value_type, base::global::CRntVector<std::unique_ptr<CTentacle>> reflected types.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_710070ce40 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_710070ce40 g_unk_717270de60 __attribute__((visibility("hidden")));
extern char g_unk_717270d3e0[];
extern char g_unk_717270d3e1[];
UnkEnumType* fn_7100708b74();
extern char g_unk_717270d3e2[];
UnkEnumType* fn_7100708c50();  // base::reflection::refl_GetType<std::unique_ptr<CTentacle>>
extern char g_unk_717270d3e3[];
UnkEnumType* fn_7100708d2c();
extern char g_unk_717270d3e4[];
UnkEnumType* fn_7100708e08();
extern char g_unk_7101d5e2e0[];
void fn_7100708ee4();
extern UnkEnumType g_unk_717270d3e8;
void fn_7100709268(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
extern char g_unk_717270d520[];
extern char g_unk_717270d521[];
UnkEnumType* fn_71007093ec();
void fn_71007094c8();
extern UnkEnumType g_unk_717270d528;
void fn_710070984c(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_717270d660[];
extern char g_unk_717270d661[];
UnkEnumType* fn_71007099d0();
extern UnkEnumType g_unk_717270d668;
void fn_7100709aac(UnkEnumType* type, const CStrId& name, void*, void*);
void fn_710009891c(void* type);  // the type's destructor, as registered
extern char g_unk_717270d7d8[];
extern UnkConversion g_unk_717270d7e0;
void fn_710070ba20() __attribute__((visibility("hidden")));

void fn_710070ce40() {
    InitHeaderIds(&g_unk_717270de60.ids);
    fn_7100708410();
    fn_710009ec00(g_unk_717270d3e0, g_unk_717270de60.field_0x20);
    fn_710009ec00(g_unk_717270d3e1, fn_7100708b74());
    fn_710009ec00(g_unk_717270d3e2, fn_7100708c50());
    fn_710009ec00(g_unk_717270d3e3, fn_7100708d2c());
    fn_710009ec00(g_unk_717270d3e4, fn_7100708e08());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CTentacle>>>::TKeyElementIterator::value_type", true);
        fn_7100709268(&g_unk_717270d3e8, n, g_unk_7101d5e2e0, nullptr, fn_7100708ee4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717270d3e8, g_unk_7101a19000);
    fn_710009ec00(g_unk_717270d520, &g_unk_717270d3e8);
    fn_710009ec00(g_unk_717270d521, fn_71007093ec());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CTentacle>>>::TKeyElementConstIterator::value_type", true);
        fn_710070984c(&g_unk_717270d528, n, g_unk_7101d5e2e0, nullptr, fn_71007094c8, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717270d528, g_unk_7101a19000);
    fn_710009ec00(g_unk_717270d660, &g_unk_717270d528);
    fn_710009ec00(g_unk_717270d661, fn_71007099d0());
    {
        CStrId n("base::global::CRntVector<std::unique_ptr<CTentacle>>", true);
        fn_7100709aac(&g_unk_717270d668, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_717270d668, g_unk_7101a19000);
    fn_710009ec00(g_unk_717270d7d8, &g_unk_717270d668);
    InitConversion(&g_unk_717270d7e0, fn_7100708c50, fn_7100708b74, fn_710070ba20);
}

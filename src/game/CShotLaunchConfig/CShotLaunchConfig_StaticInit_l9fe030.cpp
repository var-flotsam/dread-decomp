// Static initialiser of CShotLaunchConfig's unit: the class, the key element value types of the vector of
// unique_ptrs, the vector itself and the conversion between the unique_ptr and pointer types.
// Names and layouts are placeholders.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_l9fe030 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_l9fe030 g_unk_7172742850 __attribute__((visibility("hidden")));
extern char g_unk_7172741cc0[];
extern char g_unk_7172741cc1[];
UnkEnumType* fn_71009fa92c();
extern char g_unk_7172741cc2[];
UnkEnumType* fn_71009faa08();
extern char g_unk_7172741cc3[];
UnkEnumType* fn_71009faae4();  // base::reflection::refl_GetType<std::unique_ptr<CShotLaunchConfig>>
extern char g_unk_7172741cc4[];
UnkEnumType* fn_71009fabc0();
extern char g_unk_7172741cc5[];
UnkEnumType* fn_71009fac9c();
void fn_71009fad78();
extern UnkEnumType g_unk_7172741cc8;
void fn_71009fb0fc(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_7172741e00[];
extern char g_unk_7172741e01[];
UnkEnumType* fn_71009fb280();
void fn_71009fb35c();
extern UnkEnumType g_unk_7172741e08;
void fn_71009fb6e0(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_7172741f40[];
extern char g_unk_7172741f41[];
UnkEnumType* fn_71009fb864();
extern UnkEnumType g_unk_7172741f48;
void fn_71009fb940(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727420b8[];
extern UnkConversion g_unk_71727420c0;
void fn_71009fd01c() __attribute__((visibility("hidden")));

void CShotLaunchConfig_InitStatics() {
    InitHeaderIds(&g_unk_7172742850.ids);
    fn_71009f98c4();
    fn_710009ec00(g_unk_7172741cc0, g_unk_7172742850.field_0x20);
    fn_710009ec00(g_unk_7172741cc1, fn_71009fa92c());
    fn_710009ec00(g_unk_7172741cc2, fn_71009faa08());
    fn_710009ec00(g_unk_7172741cc3, fn_71009faae4());
    fn_710009ec00(g_unk_7172741cc4, fn_71009fabc0());
    fn_710009ec00(g_unk_7172741cc5, fn_71009fac9c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CShotLaunchConfig>>>::TKeyElementIterator::value_type", true);
        fn_71009fb0fc(&g_unk_7172741cc8, n, &g_unk_7101d5e2e0, nullptr, fn_71009fad78, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7172741cc8, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172741e00, &g_unk_7172741cc8);
    fn_710009ec00(g_unk_7172741e01, fn_71009fb280());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CShotLaunchConfig>>>::TKeyElementConstIterator::value_type", true);
        fn_71009fb6e0(&g_unk_7172741e08, n, &g_unk_7101d5e2e0, nullptr, fn_71009fb35c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7172741e08, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172741f40, &g_unk_7172741e08);
    fn_710009ec00(g_unk_7172741f41, fn_71009fb864());
    {
        CStrId n("base::global::CRntVector<std::unique_ptr<CShotLaunchConfig>>", true);
        fn_71009fb940(&g_unk_7172741f48, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_7172741f48, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727420b8, &g_unk_7172741f48);
    InitConversion(&g_unk_71727420c0, fn_71009faae4, fn_71009fa92c, fn_71009fd01c);
}

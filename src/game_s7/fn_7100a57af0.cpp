// Copies of fn_710070ce40 (src/game_5821e4/fn_710070ce40.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_7100a57af0.
// Static initialiser of the unit of the base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CTentacle>>>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CTentacle>>>::TKeyElementConstIterator::value_type, base::global::CRntVector<std::unique_ptr<CTentacle>> reflected types.
#include "../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_710070ce40 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_710070ce40 g_unk_717274d298 __attribute__((visibility("hidden")));
extern char g_unk_717274c818[];
extern char g_unk_717274c819[];
UnkEnumType* fn_7100708b74();
extern char g_unk_717274c81a[];
UnkEnumType* fn_7100708c50();  // base::reflection::refl_GetType<std::unique_ptr<CTentacle>>
extern char g_unk_717274c81b[];
UnkEnumType* fn_7100708d2c();
extern char g_unk_717274c81c[];
UnkEnumType* fn_7100708e08();
void fn_7100a54cfc();
extern UnkEnumType g_unk_717274c820;
void fn_7100709268(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_717274c958[];
extern char g_unk_717274c959[];
UnkEnumType* fn_71007093ec();
void fn_7100a552e0();
extern UnkEnumType g_unk_717274c960;
void fn_710070984c(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
extern char g_unk_717274ca98[];
extern char g_unk_717274ca99[];
UnkEnumType* fn_71007099d0();
extern UnkEnumType g_unk_717274caa0;
void fn_7100709aac(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_717274cc10[];
extern UnkConversion g_unk_717274cc18;
void fn_7100a56d5c() __attribute__((visibility("hidden")));

UnkEnumType* fn_7100a5498c();
UnkEnumType* fn_7100a54a68();
UnkEnumType* fn_7100a54b44();
UnkEnumType* fn_7100a54c20();
void fn_7100a55080(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
UnkEnumType* fn_7100a55204();
void fn_7100a55664(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
UnkEnumType* fn_7100a557e8();
void fn_7100a558c4(UnkEnumType* type, const CStrId& name, void*, void*);
void fn_7100a57af0() {
    InitHeaderIds(&g_unk_717274d298.ids);
    fn_7100a54724();
    fn_710009ec00(g_unk_717274c818, g_unk_717274d298.field_0x20);
    fn_710009ec00(g_unk_717274c819, fn_7100a5498c());
    fn_710009ec00(g_unk_717274c81a, fn_7100a54a68());
    fn_710009ec00(g_unk_717274c81b, fn_7100a54b44());
    fn_710009ec00(g_unk_717274c81c, fn_7100a54c20());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CTriggerLogicAction>>>::TKeyElementIterator::value_type", true);
        fn_7100a55080(&g_unk_717274c820, n, &g_unk_7101d5e2e0, nullptr, fn_7100a54cfc, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717274c820, g_unk_7101a19000);
    fn_710009ec00(g_unk_717274c958, &g_unk_717274c820);
    fn_710009ec00(g_unk_717274c959, fn_7100a55204());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CTriggerLogicAction>>>::TKeyElementConstIterator::value_type", true);
        fn_7100a55664(&g_unk_717274c960, n, &g_unk_7101d5e2e0, nullptr, fn_7100a552e0, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717274c960, g_unk_7101a19000);
    fn_710009ec00(g_unk_717274ca98, &g_unk_717274c960);
    fn_710009ec00(g_unk_717274ca99, fn_7100a557e8());
    {
        CStrId n("base::global::CRntVector<std::unique_ptr<CTriggerLogicAction>>", true);
        fn_7100a558c4(&g_unk_717274caa0, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_717274caa0, g_unk_7101a19000);
    fn_710009ec00(g_unk_717274cc10, &g_unk_717274caa0);
    InitConversion(&g_unk_717274cc18, fn_7100a54a68, fn_7100a5498c, fn_7100a56d5c);
}

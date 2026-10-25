// Static initialiser of the unit of the darkness::EDarknessPreset enum.
#include "UnkStaticInit.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_710077cd60 {
    HeaderIds ids;
};
extern UnkStatics_710077cd60 g_unk_7172715900 __attribute__((visibility("hidden")));
void fn_710077ca20();
extern UnkEnumType g_unk_71727157e0;
void fn_710077cb6c(UnkEnumType* type, const CStrId& name, void (*)(), void*);
extern char g_unk_71727158f8[];

void fn_710077cd60() {
    InitHeaderIds(&g_unk_7172715900.ids);
    {
        CStrId n("darkness::EDarknessPreset", true);
        fn_710077cb6c(&g_unk_71727157e0, n, fn_710077ca20, nullptr);
    }
    __cxa_atexit(fn_7100099468, &g_unk_71727157e0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727158f8, &g_unk_71727157e0);
}

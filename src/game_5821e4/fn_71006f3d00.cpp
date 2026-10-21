// Static initialiser of the unit of the ENavigablePathType enum.
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_71006f3d00 {
    HeaderIds ids;
};
extern UnkStatics_71006f3d00 g_unk_717270c778 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_717270c618;
void fn_71006f3ae8(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71006f393c();
extern char g_unk_717270c730[];
extern UnkConversion g_unk_717270c738;
extern UnkConversion g_unk_717270c758;
void fn_71006f3c68() __attribute__((visibility("hidden")));
void fn_71006f3c74() __attribute__((visibility("hidden")));

void fn_71006f3d00() {
    InitHeaderIds(&g_unk_717270c778.ids);
    InitEnum(&g_unk_717270c618, "ENavigablePathType", fn_71006f3ae8, fn_71006f393c,
             g_unk_717270c730, &g_unk_717270c738, fn_71006f3c68, &g_unk_717270c758, fn_71006f3c74);
}

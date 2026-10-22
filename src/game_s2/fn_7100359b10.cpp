// Static initialiser of the unit of the EAimDirection.
#include "../game_s7/UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_7100359b10 {
    HeaderIds ids;
};
extern UnkStatics_7100359b10 g_unk_71726b1c38 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726b1ad8;
void fn_71003598f8(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710035962c();
extern char g_unk_71726b1bf0[];
extern UnkConversion g_unk_71726b1bf8;
extern UnkConversion g_unk_71726b1c18;
void fn_7100359a78() __attribute__((visibility("hidden")));
void fn_7100359a84() __attribute__((visibility("hidden")));

void fn_7100359b10() {
    InitHeaderIds(&g_unk_71726b1c38.ids);
    InitEnum(&g_unk_71726b1ad8, "EAimDirection", fn_71003598f8, fn_710035962c,
             g_unk_71726b1bf0, &g_unk_71726b1bf8, fn_7100359a78, &g_unk_71726b1c18, fn_7100359a84);
}

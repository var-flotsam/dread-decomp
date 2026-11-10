// Unit 2608 (tools/tu_map.py, no reliable name): the static initialiser that registers the class
// CEnvironmentVisualPresets (helpers in game/UnkTUInit.h). Names and layouts are placeholders.
#include "game/UnkTUInit.h"

extern UnkTUHeaderFns g_unk_71729ec638 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71729ec3e8;
extern char g_unk_71729ec520;
extern char g_unk_71729ec521;
extern char g_unk_71729ec522;
void fn_7100f9b32c();
void fn_7100f9ba58(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100d8cba8();
UnkReflType* fn_7100f9beb0();

void fn_7100f9cc40() {
    InitHeaderFns(g_unk_71729ec638);
    InitClass(&g_unk_71729ec3e8, "CEnvironmentVisualPresets", fn_7100f9ba58, fn_7100f9b32c);
    fn_710009ec00(&g_unk_71729ec520, &g_unk_71729ec3e8);
    fn_710009ec00(&g_unk_71729ec521, fn_7100d8cba8());
    fn_710009ec00(&g_unk_71729ec522, fn_7100f9beb0());
}

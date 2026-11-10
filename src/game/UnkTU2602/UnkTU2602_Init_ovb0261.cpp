// Unit 2602 (tools/tu_map.py, no reliable name): the static initialiser that registers the class
// CEnvironmentMusicPresets (helpers in game/UnkTUInit.h). Names and layouts are placeholders.
#include "game/UnkTUInit.h"

extern UnkTUHeaderFns g_unk_71729ea440 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71729ea1f0;
extern char g_unk_71729ea328;
extern char g_unk_71729ea329;
extern char g_unk_71729ea32a;
void fn_7100f87e50();
void fn_7100f88c24(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
UnkReflType* fn_7100d8cd60();
UnkReflType* fn_7100f89374();

void fn_7100f8acd0() {
    InitHeaderFns(g_unk_71729ea440);
    InitClass(&g_unk_71729ea1f0, "CEnvironmentMusicPresets", fn_7100f88c24, fn_7100f87e50);
    fn_710009ec00(&g_unk_71729ea328, &g_unk_71729ea1f0);
    fn_710009ec00(&g_unk_71729ea329, fn_7100d8cd60());
    fn_710009ec00(&g_unk_71729ea32a, fn_7100f89374());
}

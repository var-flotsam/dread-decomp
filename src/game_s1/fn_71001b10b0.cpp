// The static initialiser of msapi::res::CProgramResource's TU (helpers in game/UnkTUInit.h): the header, the
// class, and the static "_nx.bshdat" extension id, destroyed at exit.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkTUInit.h"

extern UnkTUHeader g_unk_716e453ed8 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_716e453d90;
extern char g_unk_716e453ec8;
extern CStrId g_unk_716e453ed0;
void fn_71001afb7c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71001afb24();

void fn_71001b10b0() {
    InitHeader(g_unk_716e453ed8);
    InitClass(&g_unk_716e453d90, "msapi::res::CProgramResource", fn_71001afb7c, fn_71001afb24);
    fn_710009ec00(&g_unk_716e453ec8, &g_unk_716e453d90);
    new (&g_unk_716e453ed0) CStrId("_nx.bshdat", true);
    fn_7100000250(fn_710000051c, &g_unk_716e453ed0, &g_unk_7101a19000);
}

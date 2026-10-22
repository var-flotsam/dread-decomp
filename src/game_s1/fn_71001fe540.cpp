// The static initialiser of engine::scene::CSceneManager's TU (helpers in game/UnkTUInit.h): the header, one
// type registered before the class, and the class.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkTUInit.h"

extern UnkTUHeader g_unk_716e598128 __attribute__((visibility("hidden")));
extern char g_unk_716e597ec0;
extern UnkReflType g_unk_716e597ec8;
extern char g_unk_716e598000;
UnkReflType* fn_71001fb9ac();
void fn_71001fbc54(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71001fba88();

void fn_71001fe540() {
    InitHeader(g_unk_716e598128);
    fn_710009ec00(&g_unk_716e597ec0, fn_71001fb9ac());
    InitClass(&g_unk_716e597ec8, "engine::scene::CSceneManager", fn_71001fbc54, fn_71001fba88);
    fn_710009ec00(&g_unk_716e598000, &g_unk_716e597ec8);
}

// The static initialiser of engine::scene::sceneItems::CSceneGroup's unit (helpers in game/UnkTUInit.h): the item
// type enum with its converters, the group's item class and the group class.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkTUInit.h"

extern UnkTUHeader g_unk_716e597e78 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_716e597878;
extern char g_unk_716e597990;
extern UnkConverter g_unk_716e597998;
extern UnkConverter g_unk_716e5979b8;
extern UnkReflType g_unk_716e5979d8;
extern char g_unk_716e597b10;
extern UnkReflType g_unk_716e597b18;
extern char g_unk_716e597c50;
extern char g_unk_716e597c51;
extern char g_unk_716e597c52;
extern char g_unk_716e597c53;
void fn_71001f92a8(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_71001f903c();
void fn_71001f9f48() __attribute__((visibility("hidden")));
void fn_71001f9f54() __attribute__((visibility("hidden")));
void fn_71001f942c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71001f9428();
void fn_71001f9778(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71001f95b0();
UnkReflType* fn_71001f98e0();
UnkReflType* fn_71001ed1a4();
UnkReflType* fn_71001f9ae4();

void fn_71001fa720() {
    InitHeader(g_unk_716e597e78);
    InitEnum(&g_unk_716e597878, "engine::scene::sceneItems::EItemType", fn_71001f92a8, fn_71001f903c);
    fn_710009ec00(&g_unk_716e597990, &g_unk_716e597878);
    InitEnumConverters(&g_unk_716e597878, g_unk_716e597998, fn_71001f9f48, g_unk_716e5979b8, fn_71001f9f54);
    InitClass(&g_unk_716e5979d8, "engine::scene::sceneItems::CSceneGroup::CItem", fn_71001f942c, fn_71001f9428);
    fn_710009ec00(&g_unk_716e597b10, &g_unk_716e5979d8);
    InitClass(&g_unk_716e597b18, "engine::scene::sceneItems::CSceneGroup", fn_71001f9778, fn_71001f95b0);
    fn_710009ec00(&g_unk_716e597c50, &g_unk_716e597b18);
    fn_710009ec00(&g_unk_716e597c51, fn_71001f98e0());
    fn_710009ec00(&g_unk_716e597c52, fn_71001ed1a4());
    fn_710009ec00(&g_unk_716e597c53, fn_71001f9ae4());
}

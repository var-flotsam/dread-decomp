// The static initialiser of engine::scene::CSceneSlot's TU (helpers in game/UnkTUInit.h): the header, the class,
// and the vector of slot pointers with its iterator value types.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkTUInit.h"

extern UnkTUHeader g_unk_716e598cd8 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_716e598140;
extern char g_unk_716e598278;
extern char g_unk_716e598279;
extern char g_unk_716e59827a;
extern char g_unk_716e59827b;
extern UnkReflType g_unk_716e598280;
extern UnkReflType g_unk_7101d5e2e0;
extern char g_unk_716e5983b8;
extern char g_unk_716e5983b9;
extern UnkReflType g_unk_716e5983c0;
extern char g_unk_716e5984f8;
extern char g_unk_716e5984f9;
extern UnkReflType g_unk_716e598500;
extern char g_unk_716e598670;
extern char g_unk_716e598671;
void fn_71001fe894(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71001fe698();
UnkReflType* fn_71001fea10();  // base::reflection::refl_GetType<engine::scene::CSceneSlot*>
UnkReflType* fn_71001feaec();
UnkReflType* fn_71001febc8();
void fn_71001ff028(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71001feca4();
UnkReflType* fn_71001ff1ac();
void fn_71001ff60c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71001ff288();
UnkReflType* fn_71001ff790();
void fn_71001ff86c(UnkReflType* self, const CStrId& name, void* a, void* b);
UnkReflType* fn_71001ffa5c();

void fn_7100205820() {
    InitHeader(g_unk_716e598cd8);
    InitClass(&g_unk_716e598140, "engine::scene::CSceneSlot", fn_71001fe894, fn_71001fe698);
    fn_710009ec00(&g_unk_716e598278, &g_unk_716e598140);
    fn_710009ec00(&g_unk_716e598279, fn_71001fea10());
    fn_710009ec00(&g_unk_716e59827a, fn_71001feaec());
    fn_710009ec00(&g_unk_716e59827b, fn_71001febc8());
    InitClass(&g_unk_716e598280,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<engine::scene::CSceneSlot*>>::"
              "TKeyElementIterator::value_type",
              fn_71001ff028,
              fn_71001feca4,
              &g_unk_7101d5e2e0);
    fn_710009ec00(&g_unk_716e5983b8, &g_unk_716e598280);
    fn_710009ec00(&g_unk_716e5983b9, fn_71001ff1ac());
    InitClass(&g_unk_716e5983c0,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<engine::scene::CSceneSlot*>>::"
              "TKeyElementConstIterator::value_type",
              fn_71001ff60c,
              fn_71001ff288,
              &g_unk_7101d5e2e0);
    fn_710009ec00(&g_unk_716e5984f8, &g_unk_716e5983c0);
    fn_710009ec00(&g_unk_716e5984f9, fn_71001ff790());
    InitVector(&g_unk_716e598500, "base::global::CRntVector<engine::scene::CSceneSlot*>", fn_71001ff86c);
    fn_710009ec00(&g_unk_716e598670, &g_unk_716e598500);
    fn_710009ec00(&g_unk_716e598671, fn_71001ffa5c());
}

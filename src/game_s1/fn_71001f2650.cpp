// The static initialiser of engine::scene::CScene's TU (helpers in game/UnkTUInit.h): the header, the class and
// its scene-group dictionary, with the dictionary's iterator value types.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkTUInit.h"

extern UnkTUHeader g_unk_716e5977e8 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_716e596a30;
extern char g_unk_716e596b68;
extern char g_unk_716e596b69;
extern char g_unk_716e596b6a;
extern char g_unk_716e596b6b;
extern UnkReflType g_unk_716e596b70;
extern UnkReflType g_unk_7101d5e2e0;
extern char g_unk_716e596ca8;
extern char g_unk_716e596ca9;
extern UnkReflType g_unk_716e596cb0;
extern char g_unk_716e596de8;
extern char g_unk_716e596de9;
extern UnkReflType g_unk_716e596df0;
extern char g_unk_716e596f60;
extern char g_unk_716e596f61;
extern char g_unk_716e596f62;
void fn_71001ec8c0(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71001ec8bc();
UnkReflType* fn_71001eca08();  // base::reflection::refl_GetType<engine::scene::CScene*>
UnkReflType* fn_71001ecae4();
UnkReflType* fn_71001ecbc0();
void fn_71001ed020(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71001ecc9c();
UnkReflType* fn_71001ed280();
void fn_71001ed6e0(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71001ed35c();
UnkReflType* fn_71001ed864();
void fn_71001ed940(UnkReflType* self, const CStrId& name, void* a, void* b);
UnkReflType* fn_71001edb10();
UnkReflType* fn_71001edbec();

void fn_71001f2650() {
    InitHeader(g_unk_716e5977e8);
    InitClass(&g_unk_716e596a30, "engine::scene::CScene", fn_71001ec8c0, fn_71001ec8bc);
    fn_710009ec00(&g_unk_716e596b68, &g_unk_716e596a30);
    fn_710009ec00(&g_unk_716e596b69, fn_71001eca08());
    fn_710009ec00(&g_unk_716e596b6a, fn_71001ecae4());
    fn_710009ec00(&g_unk_716e596b6b, fn_71001ecbc0());
    InitClass(&g_unk_716e596b70,
              "base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CName, engine::"
              "scene::sceneItems::CSceneGroup*>>::TKeyElementIterator::value_type",
              fn_71001ed020,
              fn_71001ecc9c,
              &g_unk_7101d5e2e0);
    fn_710009ec00(&g_unk_716e596ca8, &g_unk_716e596b70);
    fn_710009ec00(&g_unk_716e596ca9, fn_71001ed280());
    InitClass(&g_unk_716e596cb0,
              "base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CName, engine::"
              "scene::sceneItems::CSceneGroup*>>::TKeyElementConstIterator::value_type",
              fn_71001ed6e0,
              fn_71001ed35c,
              &g_unk_7101d5e2e0);
    fn_710009ec00(&g_unk_716e596de8, &g_unk_716e596cb0);
    fn_710009ec00(&g_unk_716e596de9, fn_71001ed864());
    InitVector(&g_unk_716e596df0,
               "base::global::CRntDictionary<base::global::CName, engine::scene::sceneItems::CSceneGroup*>",
               fn_71001ed940);
    fn_710009ec00(&g_unk_716e596f60, &g_unk_716e596df0);
    fn_710009ec00(&g_unk_716e596f61, fn_71001edb10());
    fn_710009ec00(&g_unk_716e596f62, fn_71001edbec());
}

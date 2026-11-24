// The static initialiser of CBlackboard's unit (helpers in game/UnkTUInit.h): the header, the section
// class, the section container's iterator value types and the container itself.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/CBlackboard.h"
#include "game/UnkTUInit.h"

// The shared helpers see a reflected type through the placeholder layout (UnkReflType).
static inline UnkReflType* AsUnk(base::reflection::CType* t) { return reinterpret_cast<UnkReflType*>(t); }

extern UnkTUHeaderFnsClass g_unk_71729fd048 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71729fc4b0;
extern char g_unk_71729fc5e8;
extern char g_unk_71729fc5e9;
extern char g_unk_71729fc5ea;
extern char g_unk_71729fc5eb;
extern UnkReflType g_unk_71729fc5f0;
extern char g_unk_71729fc728;
extern char g_unk_71729fc729;
extern UnkReflType g_unk_71729fc730;
extern char g_unk_71729fc868;
extern char g_unk_71729fc869;
extern UnkReflType g_unk_71729fc870;
extern char g_unk_71729fc9e0;
extern char g_unk_71729fc9e1;
extern char g_unk_71729fc9e2;
void fn_7101044c7c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7101044aac();
const base::reflection::CType* fn_71010456bc();  // refl_GetType<...TSectionContainer>::TElementIterator>
const base::reflection::CType* fn_7101045798();  // refl_GetType<...TSectionContainer>::TElementConstIterator>
void fn_7101045bf8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7101045874();
const base::reflection::CType* fn_7101045e58();
void fn_71010462b8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7101045f34();
const base::reflection::CType* fn_710104643c();
void fn_7101046518(UnkReflType* self, const CStrId& name, void* a, void* b);
const base::reflection::CType* fn_710104670c();  // refl_GetType<CBlackboard::TSectionContainer*>

void CBlackboard_InitStatics() {
    InitHeaderFns(g_unk_71729fd048);
    InitClass(&g_unk_71729fc4b0, "CBlackboard::CSection", fn_7101044c7c, fn_7101044aac);
    fn_710009ec00(&g_unk_71729fc5e8, &g_unk_71729fc4b0);
    fn_7101044e3c();  // CBlackboard::refl_GetClass builds the class kept in the header data
    fn_710009ec00(&g_unk_71729fc5e9, &g_unk_71729fd048.type);
    fn_710009ec00(&g_unk_71729fc5ea, refl_TypeOf<fn_71010456bc>());
    fn_710009ec00(&g_unk_71729fc5eb, refl_TypeOf<fn_7101045798>());
    InitClass(&g_unk_71729fc5f0,
              "base::reflection::CollectionTypeMapper<CBlackboard::TSectionContainer>::TKeyElementIterator::value_type",
              fn_7101045bf8, fn_7101045874, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71729fc728, &g_unk_71729fc5f0);
    fn_710009ec00(&g_unk_71729fc729, refl_TypeOf<fn_7101045e58>());
    InitClass(&g_unk_71729fc730,
              "base::reflection::CollectionTypeMapper<CBlackboard::TSectionContainer>::TKeyElementConstIterator::"
              "value_type",
              fn_71010462b8, fn_7101045f34, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71729fc868, &g_unk_71729fc730);
    fn_710009ec00(&g_unk_71729fc869, refl_TypeOf<fn_710104643c>());
    InitVector(&g_unk_71729fc870, "CBlackboard::TSectionContainer", fn_7101046518);
    fn_710009ec00(&g_unk_71729fc9e0, &g_unk_71729fc870);
    fn_710009ec00(&g_unk_71729fc9e1, refl_TypeOf<fn_7101045d7c>());
    fn_710009ec00(&g_unk_71729fc9e2, refl_TypeOf<fn_710104670c>());
}

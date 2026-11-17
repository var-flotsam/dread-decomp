// The static initialiser of CEnvironmentSoundPresets's unit (2605, helpers in game/UnkTUInit.h): the header,
// the class, the sound dictionary's iterator value types and the dictionary itself.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"

// The shared helpers see a reflected type through the placeholder layout (UnkReflType).
static inline UnkReflType* AsUnk(base::reflection::CType* t) { return reinterpret_cast<UnkReflType*>(t); }

extern UnkTUHeaderFns g_unk_71729ec1c8 __attribute__((visibility("hidden")));
extern base::reflection::CType g_unk_71729eb520;
extern char g_unk_71729eb658;
extern char g_unk_71729eb659;
extern char g_unk_71729eb65a;
extern char g_unk_71729eb65b;
extern char g_unk_71729eb65c;
extern UnkReflType g_unk_71729eb660;
extern char g_unk_71729eb798;
extern char g_unk_71729eb799;
extern UnkReflType g_unk_71729eb7a0;
extern char g_unk_71729eb8d8;
extern char g_unk_71729eb8d9;
extern base::reflection::CType g_unk_71729eb8e0;
extern char g_unk_71729eba50;
extern char g_unk_71729eba51;
extern char g_unk_71729eba52;
void fn_7100f93170(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100f92a44();
const base::reflection::CType* fn_7100d8cc84();  // refl_GetType<CEnvironmentSoundPresets*>
const base::reflection::CType* fn_7100f935c8();  // refl_GetType<CEnvironmentSoundPresets const*>
const base::reflection::CType* fn_7100f936a4();  // refl_GetType<...::TElementIterator>
const base::reflection::CType* fn_7100f93780();
void fn_7100f93be0(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100f9385c();
const base::reflection::CType* fn_7100f93d64();
void fn_7100f941c4(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100f93e40();
const base::reflection::CType* fn_7100f94348();
void fn_7100f94424(UnkReflType* self, const CStrId& name, void* a, void* b);
const base::reflection::CType* fn_7100f945f8();  // refl_GetType<CRntDictionary<CStrId, CEnvironmentSoundData>*>
const base::reflection::CType* fn_7100f946d4();  // refl_GetType<CRntDictionary<CStrId, CEnvironmentSoundData> const*>

void fn_7100f973e0() {
    InitHeaderFns(g_unk_71729ec1c8);
    InitClass(AsUnk(&g_unk_71729eb520), "CEnvironmentSoundPresets", fn_7100f93170, fn_7100f92a44);
    fn_710009ec00(&g_unk_71729eb658, &g_unk_71729eb520);
    fn_710009ec00(&g_unk_71729eb659, refl_TypeOf<fn_7100d8cc84>());
    fn_710009ec00(&g_unk_71729eb65a, refl_TypeOf<fn_7100f935c8>());
    fn_710009ec00(&g_unk_71729eb65b, refl_TypeOf<fn_7100f936a4>());
    fn_710009ec00(&g_unk_71729eb65c, refl_TypeOf<fn_7100f93780>());
    InitClass(&g_unk_71729eb660,
              "base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, "
              "CEnvironmentSoundData>>::TKeyElementIterator::value_type",
              fn_7100f93be0, fn_7100f9385c, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71729eb798, &g_unk_71729eb660);
    fn_710009ec00(&g_unk_71729eb799, refl_TypeOf<fn_7100f93d64>());
    InitClass(&g_unk_71729eb7a0,
              "base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, "
              "CEnvironmentSoundData>>::TKeyElementConstIterator::value_type",
              fn_7100f941c4, fn_7100f93e40, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71729eb8d8, &g_unk_71729eb7a0);
    fn_710009ec00(&g_unk_71729eb8d9, refl_TypeOf<fn_7100f94348>());
    InitVector(AsUnk(&g_unk_71729eb8e0), "base::global::CRntDictionary<base::global::CStrId, CEnvironmentSoundData>",
               fn_7100f94424);
    fn_710009ec00(&g_unk_71729eba50, &g_unk_71729eb8e0);
    fn_710009ec00(&g_unk_71729eba51, refl_TypeOf<fn_7100f945f8>());
    fn_710009ec00(&g_unk_71729eba52, refl_TypeOf<fn_7100f946d4>());
}

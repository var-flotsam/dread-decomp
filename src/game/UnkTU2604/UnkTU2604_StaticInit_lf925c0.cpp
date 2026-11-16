// The static initialiser of CEnvironmentSoundData's TU (unit 2604; helpers in game/UnkTUInit.h): the header,
// the SSound and CEnvironmentSoundData classes, and the dictionary of sounds by string id with its iterator
// value types. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"

// The shared helpers see a reflected type through the placeholder layout (UnkReflType).
static inline UnkReflType* AsUnk(base::reflection::CType* t) { return reinterpret_cast<UnkReflType*>(t); }

extern UnkTUHeader g_unk_71729eb508 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71729ea500;
extern char g_unk_71729ea638;
extern char g_unk_71729ea639;
extern char g_unk_71729ea63a;
extern UnkReflType g_unk_71729ea640;
extern char g_unk_71729ea778;
extern char g_unk_71729ea779;
extern char g_unk_71729ea77a;
extern char g_unk_71729ea77b;
extern UnkReflType g_unk_71729ea780;
extern char g_unk_71729ea8b8;
extern char g_unk_71729ea8b9;
extern UnkReflType g_unk_71729ea8c0;
extern char g_unk_71729ea9f8;
extern char g_unk_71729ea9f9;
extern UnkReflType g_unk_71729eaa00;
extern char g_unk_71729eab70;
extern char g_unk_71729eab71;
extern char g_unk_71729eab72;
void fn_7100f8b1d8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100f8af0c();
UnkReflType* fn_7100f8b35c();
UnkReflType* fn_7100f8b438();
void fn_7100f8c0f0(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100f8b514();
UnkReflType* fn_7100f8c864();
// base::reflection::refl_GetType<...CRntDictionary<CStrId, CEnvironmentSoundData::SSound>>::TElementIterator>
UnkReflType* fn_7100f8c940();
// base::reflection::refl_GetType<...CRntDictionary<CStrId, CEnvironmentSoundData::SSound>>::TElementConstIterator>
UnkReflType* fn_7100f8ca1c();
void fn_7100f8ce7c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100f8caf8();
UnkReflType* fn_7100f8d000();
void fn_7100f8d460(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100f8d0dc();
UnkReflType* fn_7100f8d5e4();
// base::reflection::refl_InitType<base::global::CRntDictionary<base::global::CStrId, CEnvironmentSoundData::SSound>>
void fn_7100f8d6c0(UnkReflType* self, const CStrId& name, void* a, void* b);
UnkReflType* fn_7100f8d894();
UnkReflType* fn_7100f8d970();

void fn_7100f925c0() {
    InitHeader(g_unk_71729eb508);
    InitClass(&g_unk_71729ea500, "CEnvironmentSoundData::SSound", fn_7100f8b1d8, fn_7100f8af0c);
    fn_710009ec00(&g_unk_71729ea638, &g_unk_71729ea500);
    fn_710009ec00(&g_unk_71729ea639, fn_7100f8b35c());
    fn_710009ec00(&g_unk_71729ea63a, fn_7100f8b438());
    InitClass(&g_unk_71729ea640, "CEnvironmentSoundData", fn_7100f8c0f0, fn_7100f8b514);
    fn_710009ec00(&g_unk_71729ea778, &g_unk_71729ea640);
    fn_710009ec00(&g_unk_71729ea779, fn_7100f8c864());
    fn_710009ec00(&g_unk_71729ea77a, fn_7100f8c940());
    fn_710009ec00(&g_unk_71729ea77b, fn_7100f8ca1c());
    InitClass(&g_unk_71729ea780,
              "base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, "
              "CEnvironmentSoundData::SSound>>::TKeyElementIterator::value_type",
              fn_7100f8ce7c,
              fn_7100f8caf8,
              AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71729ea8b8, &g_unk_71729ea780);
    fn_710009ec00(&g_unk_71729ea8b9, fn_7100f8d000());
    InitClass(&g_unk_71729ea8c0,
              "base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, "
              "CEnvironmentSoundData::SSound>>::TKeyElementConstIterator::value_type",
              fn_7100f8d460,
              fn_7100f8d0dc,
              AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71729ea9f8, &g_unk_71729ea8c0);
    fn_710009ec00(&g_unk_71729ea9f9, fn_7100f8d5e4());
    InitVector(&g_unk_71729eaa00, "base::global::CRntDictionary<base::global::CStrId, CEnvironmentSoundData::SSound>",
               fn_7100f8d6c0);
    fn_710009ec00(&g_unk_71729eab70, &g_unk_71729eaa00);
    fn_710009ec00(&g_unk_71729eab71, fn_7100f8d894());
    fn_710009ec00(&g_unk_71729eab72, fn_7100f8d970());
}

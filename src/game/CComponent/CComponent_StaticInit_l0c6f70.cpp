// The static initialiser of CComponent's unit (helpers in game/UnkTUInit.h): the class, kept after the header,
// and the dictionary of components by string id with its iterator value types. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"

// The shared helpers see a reflected type through the placeholder layout (UnkReflType).
static inline UnkReflType* AsUnk(base::reflection::CType* t) { return reinterpret_cast<UnkReflType*>(t); }

extern UnkTUHeader g_unk_7172a037e0 __attribute__((visibility("hidden")));
extern char g_unk_7172a02d80;
extern char g_unk_7172a02d81;
extern char g_unk_7172a02d82;
extern char g_unk_7172a02d83;
extern base::reflection::CType g_unk_7172a02d88;
extern char g_unk_7172a02ec0;
extern char g_unk_7172a02ec1;
extern base::reflection::CType g_unk_7172a02ec8;
extern char g_unk_7172a03000;
extern char g_unk_7172a03001;
extern base::reflection::CType g_unk_7172a03008;
extern char g_unk_7172a03178;
extern char g_unk_7172a03179;
const base::reflection::CType* fn_71010c301c();
UnkReflType* fn_71010c30f8();
UnkReflType* fn_71010c31d4();
void fn_71010c3634(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71010c32b0(base::reflection::CClass* cls);
const base::reflection::CType* fn_71010c37b8();
void fn_71010c3c18(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71010c3894(base::reflection::CClass* cls);
UnkReflType* fn_71010c3d9c();
void fn_71010c3e78(UnkReflType* self, const CStrId& name, void* a, void* b);
// base::reflection::refl_GetType<base::global::CRntSmallDictionary<base::global::CStrId, CComponent*> const*>
UnkReflType* fn_71010c406c();

void CComponent_InitStatics() {
    InitHeader(g_unk_7172a037e0);
    fn_71010c23d0();
    fn_710009ec00(&g_unk_7172a02d80, TUType<0x20>(g_unk_7172a037e0));
    fn_710009ec00(&g_unk_7172a02d81, refl_TypeOf<fn_71010c301c>());
    fn_710009ec00(&g_unk_7172a02d82, fn_71010c30f8());
    fn_710009ec00(&g_unk_7172a02d83, fn_71010c31d4());
    InitClass(AsUnk(&g_unk_7172a02d88),
              "base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::"
              "CStrId, CComponent*>>::TKeyElementIterator::value_type",
              fn_71010c3634,
              reinterpret_cast<void (*)()>(fn_71010c32b0),
              AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_7172a02ec0, &g_unk_7172a02d88);
    fn_710009ec00(&g_unk_7172a02ec1, refl_TypeOf<fn_71010c37b8>());
    InitClass(AsUnk(&g_unk_7172a02ec8),
              "base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::"
              "CStrId, CComponent*>>::TKeyElementConstIterator::value_type",
              fn_71010c3c18,
              reinterpret_cast<void (*)()>(fn_71010c3894),
              AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_7172a03000, &g_unk_7172a02ec8);
    fn_710009ec00(&g_unk_7172a03001, fn_71010c3d9c());
    InitVector(AsUnk(&g_unk_7172a03008),
               "base::global::CRntSmallDictionary<base::global::CStrId, CComponent*>",
               fn_71010c3e78);
    fn_710009ec00(&g_unk_7172a03178, &g_unk_7172a03008);
    fn_710009ec00(&g_unk_7172a03179, fn_71010c406c());
}

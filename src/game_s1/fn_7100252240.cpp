// The static initialiser of CActorComponent's unit (helpers in game/UnkTUInit.h): the class, kept after the header,
// and the dictionary of components by string id with its iterator value types.
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"

// The shared helpers see a reflected type through the placeholder layout (UnkReflType).
static inline UnkReflType* AsUnk(base::reflection::CType* t) { return reinterpret_cast<UnkReflType*>(t); }

extern UnkTUHeader g_unk_717269fed8 __attribute__((visibility("hidden")));
extern char g_unk_717269f588;
extern char g_unk_717269f589;
extern char g_unk_717269f58a;
extern char g_unk_717269f58b;
extern UnkReflType g_unk_717269f590;
extern char g_unk_717269f6c8;
extern char g_unk_717269f6c9;
extern UnkReflType g_unk_717269f6d0;
extern char g_unk_717269f808;
extern char g_unk_717269f809;
extern UnkReflType g_unk_717269f810;
extern char g_unk_717269f980;
extern char g_unk_717269f981;
extern char g_unk_717269f982;
UnkReflType* fn_710023e498();  // base::reflection::refl_GetType<CActorComponent*>
UnkReflType* fn_710024e644();
UnkReflType* fn_710024e720();
void fn_710024eb80(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_710024e7fc();
UnkReflType* fn_710024ed04();
void fn_710024f164(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_710024ede0();
UnkReflType* fn_710024f2e8();
void fn_710024f3c4(UnkReflType* self, const CStrId& name, void* a, void* b);
UnkReflType* fn_710024f5b8();
// base::reflection::refl_GetType<base::global::CRntSmallDictionary<base::global::CStrId, CActorComponent*>*>
UnkReflType* fn_7100229060();

void fn_7100252240() {
    InitHeader(g_unk_717269fed8);
    fn_710024e3c8();
    fn_710009ec00(&g_unk_717269f588, TUType<0x20>(g_unk_717269fed8));
    fn_710009ec00(&g_unk_717269f589, fn_710023e498());
    fn_710009ec00(&g_unk_717269f58a, fn_710024e644());
    fn_710009ec00(&g_unk_717269f58b, fn_710024e720());
    InitClass(&g_unk_717269f590,
              "base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::"
              "CStrId, CActorComponent*>>::TKeyElementIterator::value_type",
              fn_710024eb80,
              fn_710024e7fc,
              AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_717269f6c8, &g_unk_717269f590);
    fn_710009ec00(&g_unk_717269f6c9, fn_710024ed04());
    InitClass(&g_unk_717269f6d0,
              "base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::"
              "CStrId, CActorComponent*>>::TKeyElementConstIterator::value_type",
              fn_710024f164,
              fn_710024ede0,
              AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_717269f808, &g_unk_717269f6d0);
    fn_710009ec00(&g_unk_717269f809, fn_710024f2e8());
    InitVector(&g_unk_717269f810,
               "base::global::CRntSmallDictionary<base::global::CStrId, CActorComponent*>",
               fn_710024f3c4);
    fn_710009ec00(&g_unk_717269f980, &g_unk_717269f810);
    fn_710009ec00(&g_unk_717269f981, fn_710024f5b8());
    fn_710009ec00(&g_unk_717269f982, fn_7100229060());
}

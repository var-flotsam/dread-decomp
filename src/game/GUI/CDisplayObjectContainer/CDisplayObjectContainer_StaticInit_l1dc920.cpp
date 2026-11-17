// The static initialiser of GUI::CDisplayObjectContainer's unit (helpers in game/UnkTUInit.h): the
// header, the pointer and iterator types of the container vector, its key-element value types, the
// vector type itself, and the class. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkTU2744.h"
#include "game/UnkTUInit.h"

// The shared helpers see a reflected type through the placeholder layout (UnkReflType).
static inline UnkReflType* AsUnk(base::reflection::CType* t) { return reinterpret_cast<UnkReflType*>(t); }

extern UnkTUHeader g_unk_7172a0f100 __attribute__((visibility("hidden")));
extern char g_unk_7172a0e6a0;
extern char g_unk_7172a0e6a1;
extern char g_unk_7172a0e6a2;
extern char g_unk_7172a0e6a3;
extern char g_unk_7172a0e6a4;
extern UnkReflType g_unk_7172a0e6a8;
extern char g_unk_7172a0e7e0;
extern char g_unk_7172a0e7e1;
extern UnkReflType g_unk_7172a0e7e8;
extern char g_unk_7172a0e920;
extern char g_unk_7172a0e921;
extern UnkReflType g_unk_7172a0e928;
extern char g_unk_7172a0ea98;
extern char g_unk_7172a0ea99;

// base::reflection::refl_GetType<GUI::CDisplayObjectContainer*>
const base::reflection::CType* fn_71011d6a04();
const base::reflection::CType* fn_71011d6ae0();
// The vector's element iterator and element const iterator types.
const base::reflection::CType* fn_71011d6bbc();
const base::reflection::CType* fn_71011d6c98();
// The key-element iterator and key-element const iterator types.
const base::reflection::CType* fn_71011d727c();
const base::reflection::CType* fn_71011d7860();
void fn_71011d70f8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71011d6d74();
void fn_71011d76dc(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71011d7358();
void fn_71011d793c(UnkReflType* self, const CStrId& name, void* a, void* b);

void fn_71011dc920() {
    InitHeader(g_unk_7172a0f100);
    fn_710009ec00(&g_unk_7172a0e6a0, refl_TypeOf<fn_71011d6a04>());
    fn_710009ec00(&g_unk_7172a0e6a1, refl_TypeOf<fn_71011bcf1c>());
    fn_710009ec00(&g_unk_7172a0e6a2, refl_TypeOf<fn_71011d6ae0>());
    fn_710009ec00(&g_unk_7172a0e6a3, refl_TypeOf<fn_71011d6bbc>());
    fn_710009ec00(&g_unk_7172a0e6a4, refl_TypeOf<fn_71011d6c98>());
    InitClass(&g_unk_7172a0e6a8,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<GUI::CDisplayObjectContainer*>>::"
              "TKeyElementIterator::value_type",
              fn_71011d70f8, fn_71011d6d74, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_7172a0e7e0, &g_unk_7172a0e6a8);
    fn_710009ec00(&g_unk_7172a0e7e1, refl_TypeOf<fn_71011d727c>());
    InitClass(&g_unk_7172a0e7e8,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<GUI::CDisplayObjectContainer*>>::"
              "TKeyElementConstIterator::value_type",
              fn_71011d76dc, fn_71011d7358, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_7172a0e920, &g_unk_7172a0e7e8);
    fn_710009ec00(&g_unk_7172a0e921, refl_TypeOf<fn_71011d7860>());
    InitVector(&g_unk_7172a0e928, "base::global::CRntVector<GUI::CDisplayObjectContainer*>", fn_71011d793c);
    fn_710009ec00(&g_unk_7172a0ea98, &g_unk_7172a0e928);
    // The class getter builds the class kept after the header.
    fn_71011d7b84();
    fn_710009ec00(&g_unk_7172a0ea99, g_unk_7172a0f120);
}

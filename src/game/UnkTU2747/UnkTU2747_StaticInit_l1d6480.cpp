// GUI::CDisplayObject's unit (tools/tu_map.py unit 2747): the unit's static initialiser. It builds the header
// data, then registers the pointer types of CDisplayObject, the iterator types and key iterator value types of
// CRntVector<CDisplayObject*> and CPooledList<CDisplayObject*>, the two collection types themselves, a
// converter from CWeakPtr<CDisplayObject> to CDisplayObject* and the class. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflConverter.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"
#include "game/lua_lib.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::CType;

extern UnkTUHeader g_unk_7172a0e548 __attribute__((visibility("hidden")));
extern char g_unk_7172a0e568[] __attribute__((visibility("hidden")));  // CDisplayObject's class

// CDisplayObject*, CWeakPtr<CDisplayObject> and the iterator types of CRntVector<CDisplayObject*>.
extern char g_unk_7172a0d4a8[];
const CType* fn_7100fc1020();  // base::global::CWeakPtr<GUI::CDisplayObject>
extern char g_unk_7172a0d4a9[];
const CType* fn_71011c198c();
extern char g_unk_7172a0d4aa[];
const CType* fn_71011c1a68();
extern char g_unk_7172a0d4ab[];
// The vector's key iterator's value type.
extern base::reflection::CType g_unk_7172a0d4b0;
void fn_71011c1ec8(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_71011c1b44(CClass* cls);
extern char g_unk_7172a0d5e8[];
const CType* fn_71011c204c();
extern char g_unk_7172a0d5e9[];
// The vector's const key iterator's value type.
extern base::reflection::CType g_unk_7172a0d5f0;
void fn_71011c24ac(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_71011c2128(CClass* cls);
extern char g_unk_7172a0d728[];
const CType* fn_71011c2630();
extern char g_unk_7172a0d729[];
// CRntVector<CDisplayObject*> and the iterator types of CPooledList<CDisplayObject*>.
extern base::reflection::CType g_unk_7172a0d730;
void fn_71011c270c(CType* self, const CStrId& name, void* a, void* b);
extern char g_unk_7172a0d8a0[];
const CType* fn_71011c28fc();
extern char g_unk_7172a0d8a1[];
const CType* fn_71011c29d8();
extern char g_unk_7172a0d8a2[];
// The list's key iterator's value type.
extern base::reflection::CType g_unk_7172a0d8a8;
void fn_71011c2e38(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_71011c2ab4(CClass* cls);
extern char g_unk_7172a0d9e0[];
const CType* fn_71011c2fbc();
extern char g_unk_7172a0d9e1[];
// The list's const key iterator's value type.
extern base::reflection::CType g_unk_7172a0d9e8;
void fn_71011c341c(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_71011c3098(CClass* cls);
extern char g_unk_7172a0db20[];
const CType* fn_71011c35a0();
extern char g_unk_7172a0db21[];
// CPooledList<CDisplayObject*>.
extern base::reflection::CType g_unk_7172a0db28;
void fn_71011c367c(CType* self, const CStrId& name, void* a, void* b);
extern char g_unk_7172a0dc98[];
// The converter from CWeakPtr<CDisplayObject> to CDisplayObject*.
extern UnkConverter g_unk_7172a0dca0;
void fn_71011d3494();
extern char g_unk_7172a0dcc0[];

void fn_71011d6480() {
    InitHeader(g_unk_7172a0e548);

    fn_710009ec00(g_unk_7172a0d4a8, const_cast<CType*>(fn_7100fc036c()));
    fn_710009ec00(g_unk_7172a0d4a9, const_cast<CType*>(fn_7100fc1020()));
    fn_710009ec00(g_unk_7172a0d4aa, const_cast<CType*>(fn_71011c198c()));
    fn_710009ec00(g_unk_7172a0d4ab, const_cast<CType*>(fn_71011c1a68()));

    {
        CStrId name("base::reflection::CollectionTypeMapper<base::global::CRntVector<GUI::CDisplayObject*>>::"
                    "TKeyElementIterator::value_type",
                    true);
        fn_71011c1ec8(&g_unk_7172a0d4b0, name, &g_unk_7101d5e2e0, nullptr, fn_71011c1b44, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_7172a0d4b0, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a0d5e8, &g_unk_7172a0d4b0);
    fn_710009ec00(g_unk_7172a0d5e9, const_cast<CType*>(fn_71011c204c()));

    {
        CStrId name("base::reflection::CollectionTypeMapper<base::global::CRntVector<GUI::CDisplayObject*>>::"
                    "TKeyElementConstIterator::value_type",
                    true);
        fn_71011c24ac(&g_unk_7172a0d5f0, name, &g_unk_7101d5e2e0, nullptr, fn_71011c2128, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_7172a0d5f0, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a0d728, &g_unk_7172a0d5f0);
    fn_710009ec00(g_unk_7172a0d729, const_cast<CType*>(fn_71011c2630()));

    {
        CStrId name("base::global::CRntVector<GUI::CDisplayObject*>", true);
        fn_71011c270c(&g_unk_7172a0d730, name, nullptr, nullptr);
    }
    fn_7100000250(fn_710009891c, &g_unk_7172a0d730, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a0d8a0, &g_unk_7172a0d730);
    fn_710009ec00(g_unk_7172a0d8a1, const_cast<CType*>(fn_71011c28fc()));
    fn_710009ec00(g_unk_7172a0d8a2, const_cast<CType*>(fn_71011c29d8()));

    {
        CStrId name("base::reflection::CollectionTypeMapper<base::global::CPooledList<GUI::CDisplayObject*>>::"
                    "TKeyElementIterator::value_type",
                    true);
        fn_71011c2e38(&g_unk_7172a0d8a8, name, &g_unk_7101d5e2e0, nullptr, fn_71011c2ab4, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_7172a0d8a8, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a0d9e0, &g_unk_7172a0d8a8);
    fn_710009ec00(g_unk_7172a0d9e1, const_cast<CType*>(fn_71011c2fbc()));

    {
        CStrId name("base::reflection::CollectionTypeMapper<base::global::CPooledList<GUI::CDisplayObject*>>::"
                    "TKeyElementConstIterator::value_type",
                    true);
        fn_71011c341c(&g_unk_7172a0d9e8, name, &g_unk_7101d5e2e0, nullptr, fn_71011c3098, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_7172a0d9e8, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a0db20, &g_unk_7172a0d9e8);
    fn_710009ec00(g_unk_7172a0db21, const_cast<CType*>(fn_71011c35a0()));

    {
        CStrId name("base::global::CPooledList<GUI::CDisplayObject*>", true);
        fn_71011c367c(&g_unk_7172a0db28, name, nullptr, nullptr);
    }
    fn_7100000250(fn_710009891c, &g_unk_7172a0db28, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a0dc98, &g_unk_7172a0db28);

    InitConverter(g_unk_7172a0dca0, refl_TypeOf<fn_7100fc1020>, refl_TypeOf<fn_7100fc036c>, fn_71011d3494);

    fn_71011c3868();
    fn_710009ec00(g_unk_7172a0dcc0, g_unk_7172a0e568);
}

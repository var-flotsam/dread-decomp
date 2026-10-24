// The static initialiser of the actor layers' unit (helpers in game/UnkTUInit.h): CActorSublayer's dictionary,
// CActorLayer and the array of layers indexed by EActorLayer, with their iterator value types, and the enum with
// its converters to and from the integer types.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkTUInit.h"

extern UnkTUHeader g_unk_71726a1cc0 __attribute__((visibility("hidden")));
extern char g_unk_71726a03a0;
extern char g_unk_71726a03a1;
extern UnkReflType g_unk_71726a03a8;
extern UnkReflType g_unk_7101d5e2e0;
extern char g_unk_71726a04e0;
extern char g_unk_71726a04e1;
extern UnkReflType g_unk_71726a04e8;
extern char g_unk_71726a0620;
extern char g_unk_71726a0621;
extern UnkReflType g_unk_71726a0628;
extern char g_unk_71726a0798;
extern char g_unk_71726a0799;
extern char g_unk_71726a079a;
extern UnkReflType g_unk_71726a07a0;
extern char g_unk_71726a08d8;
extern char g_unk_71726a08d9;
extern char g_unk_71726a08da;
extern char g_unk_71726a08db;
extern char g_unk_71726a08dc;
extern UnkReflType g_unk_71726a08e0;
extern char g_unk_71726a0a18;
extern char g_unk_71726a0a19;
extern UnkReflType g_unk_71726a0a20;
extern char g_unk_71726a0b58;
extern char g_unk_71726a0b59;
extern UnkReflType g_unk_71726a0b60;
extern char g_unk_71726a0cd0;
extern UnkReflType g_unk_71726a0cd8;
extern char g_unk_71726a0df0;
UnkReflType* fn_7100257370();
UnkReflType* fn_710025744c();
void fn_71002578ac(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100257528();
UnkReflType* fn_7100257a30();
void fn_7100257e90(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100257b0c();
UnkReflType* fn_7100258014();
void fn_71002580f0(UnkReflType* self, const CStrId& name, void* a, void* b);
UnkReflType* fn_71002582e8();
UnkReflType* fn_71002583c4();
void fn_7100258fd8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71002584a0();
UnkReflType* fn_71002598b0();
UnkReflType* fn_710025998c();
UnkReflType* fn_7100259a68();
UnkReflType* fn_7100259b44();
void fn_7100259fa4(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100259c20();
UnkReflType* fn_710025a128();
void fn_710025a588(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_710025a204();
UnkReflType* fn_710025a70c();
void fn_710025a7e8(UnkReflType* self, const CStrId& name, void* a, void* b);
void fn_710025ab8c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_710025a9e0();
extern UnkConverter g_unk_71726a0df8;
extern UnkConverter g_unk_71726a0e18;
extern UnkConverter g_unk_71726a0e38;
extern UnkConverter g_unk_71726a0e58;
void fn_710025ef30() __attribute__((visibility("hidden")));
void fn_710025ef3c() __attribute__((visibility("hidden")));
void fn_710025ef48() __attribute__((visibility("hidden")));
void fn_710025ef54() __attribute__((visibility("hidden")));

void fn_7100261530() {
    InitHeader(g_unk_71726a1cc0);
    fn_710009ec00(&g_unk_71726a03a0, fn_7100257370());
    fn_710009ec00(&g_unk_71726a03a1, fn_710025744c());
    InitClass(&g_unk_71726a03a8,
              "base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::"
              "CStrId, CActorSublayer>>::TKeyElementIterator::value_type",
              fn_71002578ac,
              fn_7100257528,
              &g_unk_7101d5e2e0);
    fn_710009ec00(&g_unk_71726a04e0, &g_unk_71726a03a8);
    fn_710009ec00(&g_unk_71726a04e1, fn_7100257a30());
    InitClass(&g_unk_71726a04e8,
              "base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::"
              "CStrId, CActorSublayer>>::TKeyElementConstIterator::value_type",
              fn_7100257e90,
              fn_7100257b0c,
              &g_unk_7101d5e2e0);
    fn_710009ec00(&g_unk_71726a0620, &g_unk_71726a04e8);
    fn_710009ec00(&g_unk_71726a0621, fn_7100258014());
    InitVector(&g_unk_71726a0628,
               "base::global::CRntSmallDictionary<base::global::CStrId, CActorSublayer>",
               fn_71002580f0);
    fn_710009ec00(&g_unk_71726a0798, &g_unk_71726a0628);
    fn_710009ec00(&g_unk_71726a0799, fn_71002582e8());
    fn_710009ec00(&g_unk_71726a079a, fn_71002583c4());
    InitClass(&g_unk_71726a07a0, "CActorLayer", fn_7100258fd8, fn_71002584a0);
    fn_710009ec00(&g_unk_71726a08d8, &g_unk_71726a07a0);
    fn_710009ec00(&g_unk_71726a08d9, fn_71002598b0());
    fn_710009ec00(&g_unk_71726a08da, fn_710025998c());
    fn_710009ec00(&g_unk_71726a08db, fn_7100259a68());
    fn_710009ec00(&g_unk_71726a08dc, fn_7100259b44());
    InitClass(&g_unk_71726a08e0,
              "base::reflection::CollectionTypeMapper<base::global::CArray<CActorLayer, EnumClass<EActorLayer>::"
              "Count, EActorLayer>>::TKeyElementIterator::value_type",
              fn_7100259fa4,
              fn_7100259c20,
              &g_unk_7101d5e2e0);
    fn_710009ec00(&g_unk_71726a0a18, &g_unk_71726a08e0);
    fn_710009ec00(&g_unk_71726a0a19, fn_710025a128());
    InitClass(&g_unk_71726a0a20,
              "base::reflection::CollectionTypeMapper<base::global::CArray<CActorLayer, EnumClass<EActorLayer>::"
              "Count, EActorLayer>>::TKeyElementConstIterator::value_type",
              fn_710025a588,
              fn_710025a204,
              &g_unk_7101d5e2e0);
    fn_710009ec00(&g_unk_71726a0b58, &g_unk_71726a0a20);
    fn_710009ec00(&g_unk_71726a0b59, fn_710025a70c());
    InitVector(&g_unk_71726a0b60,
               "base::global::CArray<CActorLayer, EnumClass<EActorLayer>::Count, EActorLayer>",
               fn_710025a7e8);
    fn_710009ec00(&g_unk_71726a0cd0, &g_unk_71726a0b60);
    InitEnum(&g_unk_71726a0cd8, "EActorLayer", fn_710025ab8c, fn_710025a9e0);
    fn_710009ec00(&g_unk_71726a0df0, &g_unk_71726a0cd8);
    InitEnumConverters(&g_unk_71726a0cd8, g_unk_71726a0df8, fn_710025ef30, g_unk_71726a0e18, fn_710025ef3c);
    InitEnumConverters(&g_unk_71726a0cd8, g_unk_71726a0e38, fn_710025ef48, g_unk_71726a0e58, fn_710025ef54,
                       refl_TypeOf<fn_7100005178>);
}

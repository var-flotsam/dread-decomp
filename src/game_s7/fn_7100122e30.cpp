// Static initialiser of the unit of base::spatial::CEditorSegment, base::spatial::CPolygon2D,
// base::spatial::SSegmentData and others: the header ids, then the unit's own statics and
// registrations (helpers in UnkStaticInitS7.h). Layouts are placeholders.
#include "UnkStaticInitS7.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_7100122e30 {
    HeaderIdsRev ids;
};
extern UnkStatics_7100122e30 g_unk_7108b6fd60 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_7108b6d7b0;
void fn_7100111068(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100110e60();
extern char g_unk_7108b6d8e8[];
extern UnkEnumType g_unk_7108b6d8f0;
void fn_71001122a8(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100111368();
extern char g_unk_7108b6da28[];
extern UnkEnumType g_unk_7108b6da30;
void fn_7100112c58(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100112a1c();
extern char g_unk_7108b6db68[];
extern char g_unk_7108b6db69[];
UnkEnumType* fn_7100112f58();
extern char g_unk_7108b6db6a[];
UnkEnumType* fn_7100113034();
extern char g_unk_7108b6db6b[];
UnkEnumType* fn_7100113110();
extern UnkEnumType g_unk_7108b6db70;
void fn_7100113570(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_71001131ec();
extern char g_unk_7108b6dca8[];
extern char g_unk_7108b6dca9[];
UnkEnumType* fn_71001136f4();
extern UnkEnumType g_unk_7108b6dcb0;
void fn_7100113b54(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_71001137d0();
extern char g_unk_7108b6dde8[];
extern char g_unk_7108b6dde9[];
UnkEnumType* fn_7100113cd8();
extern UnkEnumType g_unk_7108b6ddf0;
void fn_7100113db4(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_7108b6df60[];
extern char g_unk_7108b6df61[];
UnkEnumType* fn_7100113fa8();
extern char g_unk_7108b6df62[];
UnkEnumType* fn_7100114084();
extern UnkEnumType g_unk_7108b6df68;
void fn_71001144e4(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100114160();
extern char g_unk_7108b6e0a0[];
extern char g_unk_7108b6e0a1[];
UnkEnumType* fn_7100114668();
extern UnkEnumType g_unk_7108b6e0a8;
void fn_7100114ac8(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100114744();
extern char g_unk_7108b6e1e0[];
extern char g_unk_7108b6e1e1[];
UnkEnumType* fn_7100114c4c();
extern UnkEnumType g_unk_7108b6e1e8;
void fn_7100114d28(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_7108b6e358[];
extern char g_unk_7108b6e359[];
UnkEnumType* fn_7100114f1c();
extern char g_unk_7108b6e35a[];
UnkEnumType* fn_7100114ff8();
extern UnkEnumType g_unk_7108b6e360;
void fn_7100115458(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_71001150d4();
extern char g_unk_7108b6e498[];
extern char g_unk_7108b6e499[];
UnkEnumType* fn_71001155dc();
extern UnkEnumType g_unk_7108b6e4a0;
void fn_7100115a3c(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_71001156b8();
extern char g_unk_7108b6e5d8[];
extern char g_unk_7108b6e5d9[];
UnkEnumType* fn_7100115bc0();
extern UnkEnumType g_unk_7108b6e5e0;
void fn_7100115c9c(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_7108b6e750[];
extern char g_unk_7108b6e751[];
UnkEnumType* fn_7100115e6c();
extern char g_unk_7108b6e752[];
UnkEnumType* fn_7100115f48();
extern UnkEnumType g_unk_7108b6e758;
void fn_71001163a8(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100116024();
extern char g_unk_7108b6e890[];
extern char g_unk_7108b6e891[];
UnkEnumType* fn_710011652c();
extern UnkEnumType g_unk_7108b6e898;
void fn_710011698c(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100116608();
extern char g_unk_7108b6e9d0[];
extern char g_unk_7108b6e9d1[];
UnkEnumType* fn_7100116b10();
extern UnkEnumType g_unk_7108b6e9d8;
void fn_7100116bec(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_7108b6eb48[];

void fn_7100122e30() {
    InitHeaderIds(&g_unk_7108b6fd60.ids);
    {
        CStrId n("base::spatial::CEditorSegment", true);
        fn_7100111068(&g_unk_7108b6d7b0, n, nullptr, nullptr, fn_7100110e60, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7108b6d7b0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6d8e8, &g_unk_7108b6d7b0);
    {
        CStrId n("base::spatial::CPolygon2D", true);
        fn_71001122a8(&g_unk_7108b6d8f0, n, nullptr, nullptr, fn_7100111368, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7108b6d8f0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6da28, &g_unk_7108b6d8f0);
    {
        CStrId n("base::spatial::SSegmentData", true);
        fn_7100112c58(&g_unk_7108b6da30, n, nullptr, nullptr, fn_7100112a1c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7108b6da30, g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6db68, &g_unk_7108b6da30);
    fn_710009ec00(g_unk_7108b6db69, fn_7100112f58());
    fn_710009ec00(g_unk_7108b6db6a, fn_7100113034());
    fn_710009ec00(g_unk_7108b6db6b, fn_7100113110());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::spatial::CPolygon2D>>::TKeyElementIterator::value_type", true);
        fn_7100113570(&g_unk_7108b6db70, n, &g_unk_7101d5e2e0, nullptr, fn_71001131ec, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7108b6db70, g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6dca8, &g_unk_7108b6db70);
    fn_710009ec00(g_unk_7108b6dca9, fn_71001136f4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::spatial::CPolygon2D>>::TKeyElementConstIterator::value_type", true);
        fn_7100113b54(&g_unk_7108b6dcb0, n, &g_unk_7101d5e2e0, nullptr, fn_71001137d0, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7108b6dcb0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6dde8, &g_unk_7108b6dcb0);
    fn_710009ec00(g_unk_7108b6dde9, fn_7100113cd8());
    {
        CStrId n("base::global::CRntVector<base::spatial::CPolygon2D>", true);
        fn_7100113db4(&g_unk_7108b6ddf0, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_7108b6ddf0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6df60, &g_unk_7108b6ddf0);
    fn_710009ec00(g_unk_7108b6df61, fn_7100113fa8());
    fn_710009ec00(g_unk_7108b6df62, fn_7100114084());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::spatial::SSegmentData>>::TKeyElementIterator::value_type", true);
        fn_71001144e4(&g_unk_7108b6df68, n, &g_unk_7101d5e2e0, nullptr, fn_7100114160, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7108b6df68, g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6e0a0, &g_unk_7108b6df68);
    fn_710009ec00(g_unk_7108b6e0a1, fn_7100114668());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::spatial::SSegmentData>>::TKeyElementConstIterator::value_type", true);
        fn_7100114ac8(&g_unk_7108b6e0a8, n, &g_unk_7101d5e2e0, nullptr, fn_7100114744, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7108b6e0a8, g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6e1e0, &g_unk_7108b6e0a8);
    fn_710009ec00(g_unk_7108b6e1e1, fn_7100114c4c());
    {
        CStrId n("base::global::CRntVector<base::spatial::SSegmentData>", true);
        fn_7100114d28(&g_unk_7108b6e1e8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_7108b6e1e8, g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6e358, &g_unk_7108b6e1e8);
    fn_710009ec00(g_unk_7108b6e359, fn_7100114f1c());
    fn_710009ec00(g_unk_7108b6e35a, fn_7100114ff8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, base::spatial::CPolygon2D *>>::TKeyElementIterator::value_type", true);
        fn_7100115458(&g_unk_7108b6e360, n, &g_unk_7101d5e2e0, nullptr, fn_71001150d4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7108b6e360, g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6e498, &g_unk_7108b6e360);
    fn_710009ec00(g_unk_7108b6e499, fn_71001155dc());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntDictionary<base::global::CStrId, base::spatial::CPolygon2D *>>::TKeyElementConstIterator::value_type", true);
        fn_7100115a3c(&g_unk_7108b6e4a0, n, &g_unk_7101d5e2e0, nullptr, fn_71001156b8, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7108b6e4a0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6e5d8, &g_unk_7108b6e4a0);
    fn_710009ec00(g_unk_7108b6e5d9, fn_7100115bc0());
    {
        CStrId n("base::global::CRntDictionary<base::global::CStrId, base::spatial::CPolygon2D *>", true);
        fn_7100115c9c(&g_unk_7108b6e5e0, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_7108b6e5e0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6e750, &g_unk_7108b6e5e0);
    fn_710009ec00(g_unk_7108b6e751, fn_7100115e6c());
    fn_710009ec00(g_unk_7108b6e752, fn_7100115f48());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::spatial::CEditorSegment>>::TKeyElementIterator::value_type", true);
        fn_71001163a8(&g_unk_7108b6e758, n, &g_unk_7101d5e2e0, nullptr, fn_7100116024, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7108b6e758, g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6e890, &g_unk_7108b6e758);
    fn_710009ec00(g_unk_7108b6e891, fn_710011652c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::spatial::CEditorSegment>>::TKeyElementConstIterator::value_type", true);
        fn_710011698c(&g_unk_7108b6e898, n, &g_unk_7101d5e2e0, nullptr, fn_7100116608, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7108b6e898, g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6e9d0, &g_unk_7108b6e898);
    fn_710009ec00(g_unk_7108b6e9d1, fn_7100116b10());
    {
        CStrId n("base::global::CRntVector<base::spatial::CEditorSegment>", true);
        fn_7100116bec(&g_unk_7108b6e9d8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_7108b6e9d8, g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6eb48, &g_unk_7108b6e9d8);
}

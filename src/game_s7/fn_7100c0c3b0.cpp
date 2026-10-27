// Static initialiser of the unit of CEditorRailSegment, base::global::CRntVector<SCameraRailNode>,
// base::global::CRntVector<SCameraSubRail> and others: the header ids, then the unit's own statics
// and registrations (helpers in UnkStaticInitS7.h). Layouts are placeholders.
#include "UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

struct UnkStatics_7100c0c3b0 {
    HeaderIds ids;
    u8 pad_0x18[0x18];
    u8 field_0x30[8];
    u8 pad_0x38[0x130];
    u8 field_0x168[8];
    u8 pad_0x170[0x130];
    u8 field_0x2a0[8];
};
extern UnkStatics_7100c0c3b0 g_unk_717277cb38 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_717277b150;
void fn_7100bfe508(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100bfe300();
extern char g_unk_717277b288[];
extern char g_unk_717277b289[];
extern char g_unk_717277b28a[];
extern char g_unk_717277b28b[];
extern char g_unk_717277b28c[];
UnkEnumType* fn_7100c009dc();
extern char g_unk_717277b28d[];
UnkEnumType* fn_7100c00ab8();
extern UnkEnumType g_unk_717277b290;
void fn_7100c00f24(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100c00b94();
extern char g_unk_717277b3c8[];
extern char g_unk_717277b3c9[];
UnkEnumType* fn_7100c010a8();
extern UnkEnumType g_unk_717277b3d0;
void fn_7100c01514(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100c01184();
extern char g_unk_717277b508[];
extern char g_unk_717277b509[];
UnkEnumType* fn_7100c01698();
extern UnkEnumType g_unk_717277b510;
void fn_7100c01774(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_717277b680[];
extern char g_unk_717277b681[];
UnkEnumType* fn_7100c0196c();
extern char g_unk_717277b682[];
UnkEnumType* fn_7100c01a48();
extern UnkEnumType g_unk_717277b688;
void fn_7100c01eb4(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100c01b24();
extern char g_unk_717277b7c0[];
extern char g_unk_717277b7c1[];
UnkEnumType* fn_7100c02038();
extern UnkEnumType g_unk_717277b7c8;
void fn_7100c024a4(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100c02114();
extern char g_unk_717277b900[];
extern char g_unk_717277b901[];
UnkEnumType* fn_7100c02628();
extern UnkEnumType g_unk_717277b908;
void fn_7100c02704(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_717277ba78[];
extern char g_unk_717277ba79[];
UnkEnumType* fn_7100c028ec();
extern char g_unk_717277ba7a[];
UnkEnumType* fn_7100c029c8();
extern UnkEnumType g_unk_717277ba80;
void fn_7100c02e28(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100c02aa4();
extern char g_unk_717277bbb8[];
extern char g_unk_717277bbb9[];
UnkEnumType* fn_7100c02fac();
extern UnkEnumType g_unk_717277bbc0;
void fn_7100c0340c(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_7100c03088();
extern char g_unk_717277bcf8[];
extern char g_unk_717277bcf9[];
UnkEnumType* fn_7100c03590();
extern UnkEnumType g_unk_717277bd00;
void fn_7100c0366c(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_717277be70[];

void fn_7100c0c3b0() {
    InitHeaderIds(&g_unk_717277cb38.ids);
    {
        CStrId n("CEditorRailSegment", true);
        fn_7100bfe508(&g_unk_717277b150, n, nullptr, nullptr, fn_7100bfe300, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717277b150, g_unk_7101a19000);
    fn_710009ec00(g_unk_717277b288, &g_unk_717277b150);
    fn_7100bfe80c();
    fn_710009ec00(g_unk_717277b289, g_unk_717277cb38.field_0x30);
    fn_7100bff09c();
    fn_710009ec00(g_unk_717277b28a, g_unk_717277cb38.field_0x168);
    fn_7100c00318();
    fn_710009ec00(g_unk_717277b28b, g_unk_717277cb38.field_0x2a0);
    fn_710009ec00(g_unk_717277b28c, fn_7100c009dc());
    fn_710009ec00(g_unk_717277b28d, fn_7100c00ab8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<SCameraRailNode>>::TKeyElementIterator::value_type", true);
        fn_7100c00f24(&g_unk_717277b290, n, &g_unk_7101d5e2e0, nullptr, fn_7100c00b94, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717277b290, g_unk_7101a19000);
    fn_710009ec00(g_unk_717277b3c8, &g_unk_717277b290);
    fn_710009ec00(g_unk_717277b3c9, fn_7100c010a8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<SCameraRailNode>>::TKeyElementConstIterator::value_type", true);
        fn_7100c01514(&g_unk_717277b3d0, n, &g_unk_7101d5e2e0, nullptr, fn_7100c01184, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717277b3d0, g_unk_7101a19000);
    fn_710009ec00(g_unk_717277b508, &g_unk_717277b3d0);
    fn_710009ec00(g_unk_717277b509, fn_7100c01698());
    {
        CStrId n("base::global::CRntVector<SCameraRailNode>", true);
        fn_7100c01774(&g_unk_717277b510, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_717277b510, g_unk_7101a19000);
    fn_710009ec00(g_unk_717277b680, &g_unk_717277b510);
    fn_710009ec00(g_unk_717277b681, fn_7100c0196c());
    fn_710009ec00(g_unk_717277b682, fn_7100c01a48());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<SCameraSubRail>>::TKeyElementIterator::value_type", true);
        fn_7100c01eb4(&g_unk_717277b688, n, &g_unk_7101d5e2e0, nullptr, fn_7100c01b24, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717277b688, g_unk_7101a19000);
    fn_710009ec00(g_unk_717277b7c0, &g_unk_717277b688);
    fn_710009ec00(g_unk_717277b7c1, fn_7100c02038());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<SCameraSubRail>>::TKeyElementConstIterator::value_type", true);
        fn_7100c024a4(&g_unk_717277b7c8, n, &g_unk_7101d5e2e0, nullptr, fn_7100c02114, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717277b7c8, g_unk_7101a19000);
    fn_710009ec00(g_unk_717277b900, &g_unk_717277b7c8);
    fn_710009ec00(g_unk_717277b901, fn_7100c02628());
    {
        CStrId n("base::global::CRntVector<SCameraSubRail>", true);
        fn_7100c02704(&g_unk_717277b908, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_717277b908, g_unk_7101a19000);
    fn_710009ec00(g_unk_717277ba78, &g_unk_717277b908);
    fn_710009ec00(g_unk_717277ba79, fn_7100c028ec());
    fn_710009ec00(g_unk_717277ba7a, fn_7100c029c8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CEditorRailSegment>>::TKeyElementIterator::value_type", true);
        fn_7100c02e28(&g_unk_717277ba80, n, &g_unk_7101d5e2e0, nullptr, fn_7100c02aa4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717277ba80, g_unk_7101a19000);
    fn_710009ec00(g_unk_717277bbb8, &g_unk_717277ba80);
    fn_710009ec00(g_unk_717277bbb9, fn_7100c02fac());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CEditorRailSegment>>::TKeyElementConstIterator::value_type", true);
        fn_7100c0340c(&g_unk_717277bbc0, n, &g_unk_7101d5e2e0, nullptr, fn_7100c03088, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717277bbc0, g_unk_7101a19000);
    fn_710009ec00(g_unk_717277bcf8, &g_unk_717277bbc0);
    fn_710009ec00(g_unk_717277bcf9, fn_7100c03590());
    {
        CStrId n("base::global::CRntVector<CEditorRailSegment>", true);
        fn_7100c0366c(&g_unk_717277bd00, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_717277bd00, g_unk_7101a19000);
    fn_710009ec00(g_unk_717277be70, &g_unk_717277bd00);
}

// Static initialiser of the unit of
// base::global::CRntVector<std::unique_ptr<CCentralUnitWeightedEdges>>,
// CCentralUnitComponent::ECentralUnitMode,
// base::global::CRntVector<CCentralUnitComponent::SStartPointInfo> and others: the header ids,
// then the unit's own statics and registrations (helpers in UnkStaticInitS7.h). Layouts are
// placeholders.
#include "UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"

using namespace unk_static_init;

struct UnkStatics_710077bce0 {
    HeaderIds2 ids;
    u8 pad_0x20[0x8];
    UnkStrIdBase field_0x28;
    u8 pad_0x30[0x8];
    u8 field_0x38[8];
    u8 pad_0x40[0x130];
    u8 field_0x170[8];
};
extern UnkStatics_710077bce0 g_unk_71727153d8 __attribute__((visibility("hidden")));
extern char g_unk_7172713dd0[];
extern char g_unk_7172713dd1[];
UnkEnumType* fn_710076a7d0();
extern char g_unk_7172713dd2[];
UnkEnumType* fn_710076a8ac();
extern char g_unk_7172713dd3[];
UnkEnumType* fn_710076a988();
extern char g_unk_7172713dd4[];
UnkEnumType* fn_710076aa64();
extern UnkEnumType g_unk_7172713dd8;
void fn_710076aec4(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_7101d5e2e0[];
void fn_710076ab40();
extern char g_unk_7172713f10[];
extern char g_unk_7172713f11[];
UnkEnumType* fn_710076b048();
extern UnkEnumType g_unk_7172713f18;
void fn_710076b4a8(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_710076b124();
extern char g_unk_7172714050[];
extern char g_unk_7172714051[];
UnkEnumType* fn_710076b62c();
extern UnkEnumType g_unk_7172714058;
void fn_710076b708(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727141c8[];
extern UnkConversion g_unk_71727141d0;
void fn_710077768c() __attribute__((visibility("hidden")));
extern UnkStrIdBase g_unk_71727141f0;
extern UnkProperty g_unk_71727141f8;
extern void* g_unk_7101a6edd0[];
void fn_71007798d0(UnkProperty* p);
void fn_710076b8e8(void* p);
extern UnkProperty g_unk_7172714220;
extern UnkConversion g_unk_7172714368;
extern UnkConversion g_unk_7172714388;
void fn_7100777950() __attribute__((visibility("hidden")));
void fn_710077795c() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_7172714248;
extern char g_unk_7172714360[];
void fn_710076bb6c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710076b960();
extern char g_unk_71727143a8[];
UnkEnumType* fn_710076bcec();
extern char g_unk_71727143a9[];
UnkEnumType* fn_710076bdc8();
extern UnkEnumType g_unk_71727143b0;
void fn_710076c228(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_710076bea4();
extern char g_unk_71727144e8[];
extern char g_unk_71727144e9[];
UnkEnumType* fn_710076c3ac();
extern UnkEnumType g_unk_71727144f0;
void fn_710076c80c(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_710076c488();
extern char g_unk_7172714628[];
extern char g_unk_7172714629[];
UnkEnumType* fn_710076c990();
extern UnkEnumType g_unk_7172714630;
void fn_710076ca6c(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71727147a0[];
extern UnkEnumType g_unk_71727147a8;
void fn_710076cdd0(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_710076cc60();
extern char g_unk_71727148e0[];
extern char g_unk_71727148e1[];

void fn_710077bce0() {
    InitHeaderIds(&g_unk_71727153d8.ids);
    fn_710076a0f8();
    fn_710009ec00(g_unk_7172713dd0, g_unk_71727153d8.field_0x38);
    fn_710009ec00(g_unk_7172713dd1, fn_710076a7d0());
    fn_710009ec00(g_unk_7172713dd2, fn_710076a8ac());
    fn_710009ec00(g_unk_7172713dd3, fn_710076a988());
    fn_710009ec00(g_unk_7172713dd4, fn_710076aa64());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CCentralUnitWeightedEdges>>>::TKeyElementIterator::value_type", true);
        fn_710076aec4(&g_unk_7172713dd8, n, g_unk_7101d5e2e0, nullptr, fn_710076ab40, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7172713dd8, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172713f10, &g_unk_7172713dd8);
    fn_710009ec00(g_unk_7172713f11, fn_710076b048());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CCentralUnitWeightedEdges>>>::TKeyElementConstIterator::value_type", true);
        fn_710076b4a8(&g_unk_7172713f18, n, g_unk_7101d5e2e0, nullptr, fn_710076b124, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7172713f18, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172714050, &g_unk_7172713f18);
    fn_710009ec00(g_unk_7172714051, fn_710076b62c());
    {
        CStrId n("base::global::CRntVector<std::unique_ptr<CCentralUnitWeightedEdges>>", true);
        fn_710076b708(&g_unk_7172714058, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_7172714058, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727141c8, &g_unk_7172714058);
    InitConversion(&g_unk_71727141d0, fn_710076a8ac, fn_710076a7d0, fn_710077768c);
    InitId(&g_unk_71727141f0, "CENTRAL_UNIT");
    InitProperty(&g_unk_71727141f8, g_unk_7101a6edd0, g_unk_71726db788, "State", TypeOf<fn_7100014768>(), fn_71007798d0, fn_710076b8e8);
    InitProperty(&g_unk_7172714220, g_unk_7101a6edd0, g_unk_71726db788, "EmmyDisabled", TypeOf<fn_710000d228>(), fn_71007798d0, fn_710076b8e8);
    InitEnum(&g_unk_7172714248, "CCentralUnitComponent::ECentralUnitMode", fn_710076bb6c, fn_710076b960, g_unk_7172714360, &g_unk_7172714368, fn_7100777950, &g_unk_7172714388, fn_710077795c);
    fn_710009ec00(g_unk_71727143a8, fn_710076bcec());
    fn_710009ec00(g_unk_71727143a9, fn_710076bdc8());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CCentralUnitComponent::SStartPointInfo>>::TKeyElementIterator::value_type", true);
        fn_710076c228(&g_unk_71727143b0, n, g_unk_7101d5e2e0, nullptr, fn_710076bea4, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727143b0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727144e8, &g_unk_71727143b0);
    fn_710009ec00(g_unk_71727144e9, fn_710076c3ac());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<CCentralUnitComponent::SStartPointInfo>>::TKeyElementConstIterator::value_type", true);
        fn_710076c80c(&g_unk_71727144f0, n, g_unk_7101d5e2e0, nullptr, fn_710076c488, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727144f0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172714628, &g_unk_71727144f0);
    fn_710009ec00(g_unk_7172714629, fn_710076c990());
    {
        CStrId n("base::global::CRntVector<CCentralUnitComponent::SStartPointInfo>", true);
        fn_710076ca6c(&g_unk_7172714630, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_7172714630, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727147a0, &g_unk_7172714630);
    InitClassId(&g_unk_71727153d8.field_0x28, "CentralUnit");
    {
        CStrId n("CCentralUnitComponent::SStartPointInfo", true);
        fn_710076cdd0(&g_unk_71727147a8, n, nullptr, nullptr, fn_710076cc60, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71727147a8, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727148e0, &g_unk_71727147a8);
    fn_710076cf58();
    fn_710009ec00(g_unk_71727148e1, g_unk_71727153d8.field_0x170);
}

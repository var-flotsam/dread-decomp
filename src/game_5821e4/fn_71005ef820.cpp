// Static initialiser of the unit of the ESpinningNailsState, ESpinningNailsSpeed, base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CKraidSpinningNailsDef>>>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CKraidSpinningNailsDef>>>::TKeyElementConstIterator::value_type, base::global::CRntVector<std::unique_ptr<CKraidSpinningNailsDef>>, base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CRntVector<std::unique_ptr<CKraidSpinningNailsDef>>>>::TKeyElementIterator::value_type, base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CRntVector<std::unique_ptr<CKraidSpinningNailsDef>>>>::TKeyElementConstIterator::value_type, base::global::CRntVector<base::global::CRntVector<std::unique_ptr<CKraidSpinningNailsDef>>> reflected types.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71005ef820 {
    HeaderIds ids;
    u8 pad_0x18[0x18];
    u8 field_0x30[8];
    u8 pad_0x38[0x130];
    u8 field_0x168[8];
    u8 pad_0x170[0x130];
    u8 field_0x2a0[8];
};
extern UnkStatics_71005ef820 g_unk_71726f20c0 __attribute__((visibility("hidden")));
extern char g_unk_71726f0b08[];
extern UnkTunable g_unk_71726f0b10;
void fn_7100136ecc(UnkTunable* self);  // the base constructor
extern void* g_unk_7101a53958[];
void fn_7100075104(void* self);  // its destructor, as registered
extern UnkEnumType g_unk_71726f0b40;
void fn_71005e7e04(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71005e7b38();
extern char g_unk_71726f0c58[];
extern UnkConversion g_unk_71726f0c60;
extern UnkConversion g_unk_71726f0c80;
void fn_71005eae18() __attribute__((visibility("hidden")));
void fn_71005eae24() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726f0ca0;
void fn_71005e8130(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71005e7f84();
extern char g_unk_71726f0db8[];
extern UnkConversion g_unk_71726f0dc0;
extern UnkConversion g_unk_71726f0de0;
void fn_71005eae30() __attribute__((visibility("hidden")));
void fn_71005eae3c() __attribute__((visibility("hidden")));
extern char g_unk_71726f0e00[];
extern char g_unk_71726f0e01[];
UnkEnumType* fn_71005e871c();
extern char g_unk_71726f0e02[];
UnkEnumType* fn_71005e87f8();  // base::reflection::refl_GetType<std::unique_ptr<CKraidSpinningNailsDef>>
extern char g_unk_71726f0e03[];
UnkEnumType* fn_71005e88d4();
extern char g_unk_71726f0e04[];
UnkEnumType* fn_71005e89b0();
extern char g_unk_7101d5e2e0[];
void fn_71005e8a8c();
extern UnkEnumType g_unk_71726f0e08;
void fn_71005e8e10(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
extern char g_unk_71726f0f40[];
extern char g_unk_71726f0f41[];
UnkEnumType* fn_71005e8f94();
void fn_71005e9070();
extern UnkEnumType g_unk_71726f0f48;
void fn_71005e93f4(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726f1080[];
extern char g_unk_71726f1081[];
UnkEnumType* fn_71005e9578();
extern UnkEnumType g_unk_71726f1088;
void fn_71005e9654(UnkEnumType* type, const CStrId& name, void*, void*);
void fn_710009891c(void* type);  // the type's destructor, as registered
extern char g_unk_71726f11f8[];
extern char g_unk_71726f11f9[];
UnkEnumType* fn_71005e9834();
extern char g_unk_71726f11fa[];
UnkEnumType* fn_71005e9910();
void fn_71005e99ec();
extern UnkEnumType g_unk_71726f1200;
void fn_71005e9d70(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726f1338[];
extern char g_unk_71726f1339[];
UnkEnumType* fn_71005e9ef4();
void fn_71005e9fd0();
extern UnkEnumType g_unk_71726f1340;
void fn_71005ea354(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_71726f1478[];
extern char g_unk_71726f1479[];
UnkEnumType* fn_71005ea4d8();
extern UnkEnumType g_unk_71726f1480;
void fn_71005ea5b4(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_71726f15f0[];
extern UnkConversion g_unk_71726f15f8;
void fn_71005ecf00() __attribute__((visibility("hidden")));
extern char g_unk_71726f1618[];

void fn_71005ef820() {
    InitHeaderIds(&g_unk_71726f20c0.ids);
    fn_71005e742c();
    fn_710009ec00(g_unk_71726f0b08, g_unk_71726f20c0.field_0x30);
    InitTunable(&g_unk_71726f0b10, fn_7100136ecc, g_unk_7101a53958, fn_7100075104);
    InitEnum(&g_unk_71726f0b40, "ESpinningNailsState", fn_71005e7e04, fn_71005e7b38,
             g_unk_71726f0c58, &g_unk_71726f0c60, fn_71005eae18, &g_unk_71726f0c80, fn_71005eae24);
    InitEnum(&g_unk_71726f0ca0, "ESpinningNailsSpeed", fn_71005e8130, fn_71005e7f84,
             g_unk_71726f0db8, &g_unk_71726f0dc0, fn_71005eae30, &g_unk_71726f0de0, fn_71005eae3c);
    fn_71005e82b4();
    fn_710009ec00(g_unk_71726f0e00, g_unk_71726f20c0.field_0x168);
    fn_710009ec00(g_unk_71726f0e01, fn_71005e871c());
    fn_710009ec00(g_unk_71726f0e02, fn_71005e87f8());
    fn_710009ec00(g_unk_71726f0e03, fn_71005e88d4());
    fn_710009ec00(g_unk_71726f0e04, fn_71005e89b0());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CKraidSpinningNailsDef>>>::TKeyElementIterator::value_type", true);
        fn_71005e8e10(&g_unk_71726f0e08, n, g_unk_7101d5e2e0, nullptr, fn_71005e8a8c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726f0e08, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726f0f40, &g_unk_71726f0e08);
    fn_710009ec00(g_unk_71726f0f41, fn_71005e8f94());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CKraidSpinningNailsDef>>>::TKeyElementConstIterator::value_type", true);
        fn_71005e93f4(&g_unk_71726f0f48, n, g_unk_7101d5e2e0, nullptr, fn_71005e9070, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726f0f48, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726f1080, &g_unk_71726f0f48);
    fn_710009ec00(g_unk_71726f1081, fn_71005e9578());
    {
        CStrId n("base::global::CRntVector<std::unique_ptr<CKraidSpinningNailsDef>>", true);
        fn_71005e9654(&g_unk_71726f1088, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726f1088, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726f11f8, &g_unk_71726f1088);
    fn_710009ec00(g_unk_71726f11f9, fn_71005e9834());
    fn_710009ec00(g_unk_71726f11fa, fn_71005e9910());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CRntVector<std::unique_ptr<CKraidSpinningNailsDef>>>>::TKeyElementIterator::value_type", true);
        fn_71005e9d70(&g_unk_71726f1200, n, g_unk_7101d5e2e0, nullptr, fn_71005e99ec, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726f1200, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726f1338, &g_unk_71726f1200);
    fn_710009ec00(g_unk_71726f1339, fn_71005e9ef4());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<base::global::CRntVector<std::unique_ptr<CKraidSpinningNailsDef>>>>::TKeyElementConstIterator::value_type", true);
        fn_71005ea354(&g_unk_71726f1340, n, g_unk_7101d5e2e0, nullptr, fn_71005e9fd0, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_71726f1340, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726f1478, &g_unk_71726f1340);
    fn_710009ec00(g_unk_71726f1479, fn_71005ea4d8());
    {
        CStrId n("base::global::CRntVector<base::global::CRntVector<std::unique_ptr<CKraidSpinningNailsDef>>>", true);
        fn_71005ea5b4(&g_unk_71726f1480, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_71726f1480, g_unk_7101a19000);
    fn_710009ec00(g_unk_71726f15f0, &g_unk_71726f1480);
    InitConversion(&g_unk_71726f15f8, fn_71005e87f8, fn_71005e871c, fn_71005ecf00);
    fn_71005ea79c();
    fn_710009ec00(g_unk_71726f1618, g_unk_71726f20c0.field_0x2a0);
}

// Static initialiser of the unit of CTriggerComponent::SActivationCondition,
// base::global::CRntVector<std::unique_ptr<CTriggerComponent::SActivationCondition>>,
// CTriggerComponent::EEvent: the header ids, then the unit's own statics and registrations
// (helpers in UnkStaticInitS7.h). Layouts are placeholders.
#include "UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_7100a64bb0 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    u8 pad_0x30[0x18];
    u8 field_0x48[8];
    u8 pad_0x50[0x130];
    u8 field_0x180[8];
};
extern UnkStatics_7100a64bb0 g_unk_717274e4b8 __attribute__((visibility("hidden")));
extern UnkStrIdBase g_unk_717274d6c0;
extern UnkProperty g_unk_717274d6c8;
extern UnkProperty* g_unk_71726db788;
extern void* g_unk_7101a8f780[];
void fn_7100a63000(UnkProperty* p);
void fn_7100a59a00(void* p);
extern UnkProperty g_unk_717274d6f0;
extern void* g_unk_7101a8f7d8[];
void fn_7100a59a78(void* p);
extern UnkEnumType g_unk_717274d718;
void fn_7100a5a24c(UnkEnumType* type, const CStrId& name, void*, void*, void (*)(), void*);
void fn_7100096604(void* type);  // the type's destructor, as registered
void fn_7100a59af0();
extern char g_unk_717274d850[];
extern char g_unk_717274d851[];
UnkEnumType* fn_7100a5a3c0();
extern char g_unk_717274d852[];
UnkEnumType* fn_7100a5a5c4();
extern char g_unk_717274d853[];
UnkEnumType* fn_7100a5a6a0();
extern char g_unk_717274d854[];
UnkEnumType* fn_7100a5a77c();
extern UnkEnumType g_unk_717274d858;
void fn_7100a5abdc(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
extern char g_unk_7101d5e2e0[];
void fn_7100a5a858();
extern char g_unk_717274d990[];
extern char g_unk_717274d991[];
UnkEnumType* fn_7100a5ad60();
extern UnkEnumType g_unk_717274d998;
void fn_7100a5b1c0(UnkEnumType* type, const CStrId& name, char*, void*, void (*)(), void*);
void fn_7100a5ae3c();
extern char g_unk_717274dad0[];
extern char g_unk_717274dad1[];
UnkEnumType* fn_7100a5b344();
extern UnkEnumType g_unk_717274dad8;
void fn_7100a5b420(UnkEnumType* type, const CStrId& name, void*, void*);
void fn_710009891c(void* type);  // the type's destructor, as registered
extern char g_unk_717274dc48[];
extern UnkConversion g_unk_717274dc50;
void fn_7100a62ed0() __attribute__((visibility("hidden")));
extern UnkConversion g_unk_717274dd90;
extern UnkConversion g_unk_717274ddb0;
void fn_7100a62edc() __attribute__((visibility("hidden")));
void fn_7100a62ee8() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_717274dc70;
extern char g_unk_717274dd88[];
void fn_7100a5b92c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100a5b600();
extern char g_unk_717274ddd0[];
extern char g_unk_717274ddd1[];
extern UnkTunable g_unk_717274ddd8;
void fn_7100136ecc(UnkTunable* self);  // the base constructor
extern void* g_unk_7101a8f830[];
void fn_7100075104(void* self);  // its destructor, as registered

void fn_7100a64bb0() {
    InitHeaderIds(&g_unk_717274e4b8.ids);
    InitId(&g_unk_717274d6c0, "TRIGGER");
    InitProperty(&g_unk_717274d6c8, g_unk_7101a8f780, g_unk_71726db788, "Enabled", TypeOf<fn_710000d228>(), fn_7100a63000, fn_7100a59a00);
    InitProperty(&g_unk_717274d6f0, g_unk_7101a8f7d8, g_unk_71726db788, "ConditionDone", TypeOf<fn_710000d228>(), fn_7100a63000, fn_7100a59a78);
    InitClassId(&g_unk_717274e4b8.field_0x18, "TRIGGERS");
    InitClassId(&g_unk_717274e4b8.field_0x20, "ATTACHED_TRIGGERS");
    InitClassId(&g_unk_717274e4b8.field_0x28, "ATTACHED_TRIGGERS_CHECKPINT");
    {
        CStrId n("CTriggerComponent::SActivationCondition", true);
        fn_7100a5a24c(&g_unk_717274d718, n, nullptr, nullptr, fn_7100a59af0, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717274d718, g_unk_7101a19000);
    fn_710009ec00(g_unk_717274d850, &g_unk_717274d718);
    fn_710009ec00(g_unk_717274d851, fn_7100a5a3c0());
    fn_710009ec00(g_unk_717274d852, fn_7100a5a5c4());
    fn_710009ec00(g_unk_717274d853, fn_7100a5a6a0());
    fn_710009ec00(g_unk_717274d854, fn_7100a5a77c());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CTriggerComponent::SActivationCondition>>>::TKeyElementIterator::value_type", true);
        fn_7100a5abdc(&g_unk_717274d858, n, g_unk_7101d5e2e0, nullptr, fn_7100a5a858, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717274d858, g_unk_7101a19000);
    fn_710009ec00(g_unk_717274d990, &g_unk_717274d858);
    fn_710009ec00(g_unk_717274d991, fn_7100a5ad60());
    {
        CStrId n("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CTriggerComponent::SActivationCondition>>>::TKeyElementConstIterator::value_type", true);
        fn_7100a5b1c0(&g_unk_717274d998, n, g_unk_7101d5e2e0, nullptr, fn_7100a5ae3c, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_717274d998, g_unk_7101a19000);
    fn_710009ec00(g_unk_717274dad0, &g_unk_717274d998);
    fn_710009ec00(g_unk_717274dad1, fn_7100a5b344());
    {
        CStrId n("base::global::CRntVector<std::unique_ptr<CTriggerComponent::SActivationCondition>>", true);
        fn_7100a5b420(&g_unk_717274dad8, n, nullptr, nullptr);
    }
    __cxa_atexit(fn_710009891c, &g_unk_717274dad8, g_unk_7101a19000);
    fn_710009ec00(g_unk_717274dc48, &g_unk_717274dad8);
    InitConversion(&g_unk_717274dc50, fn_7100a5a3c0, fn_7100a5a5c4, fn_7100a62ed0);
    InitEnum(&g_unk_717274dc70, "CTriggerComponent::EEvent", fn_7100a5b92c, fn_7100a5b600, g_unk_717274dd88, &g_unk_717274dd90, fn_7100a62edc, &g_unk_717274ddb0, fn_7100a62ee8);
    fn_7100a5bab0();
    fn_710009ec00(g_unk_717274ddd0, g_unk_717274e4b8.field_0x48);
    fn_7100a5d274();
    fn_710009ec00(g_unk_717274ddd1, g_unk_717274e4b8.field_0x180);
    InitTunable(&g_unk_717274ddd8, fn_7100136ecc, g_unk_7101a8f830, fn_7100075104);
}

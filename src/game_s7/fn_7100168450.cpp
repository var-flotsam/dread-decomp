// Static initialiser of the unit of msapi::api::shdr::EShaderType,
// msapi::api::rs::ERenderStateValue, msapi::api::idx::EPrimitiveType and others: the header ids,
// then the unit's own statics and registrations (helpers in UnkStaticInitS7.h). Layouts are
// placeholders.
#include "UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_7100168450 {
    HeaderIds ids;
};
extern UnkStatics_7100168450 g_unk_716e34c1b0 __attribute__((visibility("hidden")));
extern UnkConversion g_unk_716e34b090;
extern UnkConversion g_unk_716e34b0b0;
void fn_7100167cf8() __attribute__((visibility("hidden")));
void fn_7100167d04() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_716e34af70;
extern char g_unk_716e34b088[];
void fn_7100163228(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710016307c();
extern UnkEnumType g_unk_716e34b0d0;
void fn_7100163d34(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_71001633a8();
extern char g_unk_716e34b1e8[];
extern UnkEnumType g_unk_716e34b1f0;
void fn_7100164180(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_7100163eb4();
extern char g_unk_716e34b308[];
extern UnkEnumType g_unk_716e34b310;
void fn_710016450c(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_7100164300();
extern char g_unk_716e34b428[];
extern UnkEnumType g_unk_716e34b430;
void fn_7100164d18(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_710016468c();
extern char g_unk_716e34b548[];
extern UnkEnumType g_unk_716e34b550;
void fn_71001650a4(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_7100164e98();
extern char g_unk_716e34b668[];
extern UnkEnumType g_unk_716e34b670;
void fn_7100165430(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_7100165224();
extern char g_unk_716e34b788[];
extern UnkEnumType g_unk_716e34b790;
void fn_710016587c(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_71001655b0();
extern char g_unk_716e34b8a8[];
extern UnkEnumType g_unk_716e34b8b0;
void fn_7100165cc8(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_71001659fc();
extern char g_unk_716e34b9c8[];
extern UnkEnumType g_unk_716e34b9d0;
void fn_71001662f4(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_7100165e48();
extern char g_unk_716e34bae8[];
extern UnkEnumType g_unk_716e34baf0;
void fn_7100166680(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_7100166474();
extern char g_unk_716e34bc08[];
extern UnkEnumType g_unk_716e34bc10;
void fn_7100166bec(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_7100166800();
extern char g_unk_716e34bd28[];
extern UnkEnumType g_unk_716e34bd30;
void fn_7100167158(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_7100166d6c();
extern char g_unk_716e34be48[];
extern UnkEnumType g_unk_716e34be50;
void fn_71001674e4(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_71001672d8();
extern char g_unk_716e34bf68[];
extern UnkEnumType g_unk_716e34bf70;
void fn_7100167870(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_7100167664();
extern char g_unk_716e34c088[];
extern UnkEnumType g_unk_716e34c090;
void fn_7100167b3c(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_71001679f0();
extern char g_unk_716e34c1a8[];

void fn_7100168450() {
    InitHeaderIds(&g_unk_716e34c1b0.ids);
    InitEnum(&g_unk_716e34af70, "msapi::api::shdr::EShaderType", fn_7100163228, fn_710016307c, g_unk_716e34b088, &g_unk_716e34b090, fn_7100167cf8, &g_unk_716e34b0b0, fn_7100167d04);
    {
        CStrId n("msapi::api::rs::ERenderStateValue", true);
        fn_7100163d34(&g_unk_716e34b0d0, n, fn_71001633a8, nullptr);
    }
    __cxa_atexit(fn_7100099468, &g_unk_716e34b0d0, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34b1e8, &g_unk_716e34b0d0);
    {
        CStrId n("msapi::api::idx::EPrimitiveType", true);
        fn_7100164180(&g_unk_716e34b1f0, n, fn_7100163eb4, nullptr);
    }
    __cxa_atexit(fn_7100099468, &g_unk_716e34b1f0, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34b308, &g_unk_716e34b1f0);
    {
        CStrId n("msapi::api::idx::EDataType", true);
        fn_710016450c(&g_unk_716e34b310, n, fn_7100164300, nullptr);
    }
    __cxa_atexit(fn_7100099468, &g_unk_716e34b310, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34b428, &g_unk_716e34b310);
    {
        CStrId n("msapi::api::tex::EComponents", true);
        fn_7100164d18(&g_unk_716e34b430, n, fn_710016468c, nullptr);
    }
    __cxa_atexit(fn_7100099468, &g_unk_716e34b430, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34b548, &g_unk_716e34b430);
    {
        CStrId n("msapi::api::tex::EArrangement", true);
        fn_71001650a4(&g_unk_716e34b550, n, fn_7100164e98, nullptr);
    }
    __cxa_atexit(fn_7100099468, &g_unk_716e34b550, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34b668, &g_unk_716e34b550);
    {
        CStrId n("msapi::api::tex::ETiling", true);
        fn_7100165430(&g_unk_716e34b670, n, fn_7100165224, nullptr);
    }
    __cxa_atexit(fn_7100099468, &g_unk_716e34b670, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34b788, &g_unk_716e34b670);
    {
        CStrId n("msapi::api::tex::EFiltering", true);
        fn_710016587c(&g_unk_716e34b790, n, fn_71001655b0, nullptr);
    }
    __cxa_atexit(fn_7100099468, &g_unk_716e34b790, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34b8a8, &g_unk_716e34b790);
    {
        CStrId n("msapi::api::types::blendOp::EBlendOp", true);
        fn_7100165cc8(&g_unk_716e34b8b0, n, fn_71001659fc, nullptr);
    }
    __cxa_atexit(fn_7100099468, &g_unk_716e34b8b0, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34b9c8, &g_unk_716e34b8b0);
    {
        CStrId n("msapi::api::types::blendFactor::EBlendFactor", true);
        fn_71001662f4(&g_unk_716e34b9d0, n, fn_7100165e48, nullptr);
    }
    __cxa_atexit(fn_7100099468, &g_unk_716e34b9d0, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34bae8, &g_unk_716e34b9d0);
    {
        CStrId n("msapi::api::types::cullMode::ECullMode", true);
        fn_7100166680(&g_unk_716e34baf0, n, fn_7100166474, nullptr);
    }
    __cxa_atexit(fn_7100099468, &g_unk_716e34baf0, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34bc08, &g_unk_716e34baf0);
    {
        CStrId n("msapi::api::types::cmpMode::ECmpMode", true);
        fn_7100166bec(&g_unk_716e34bc10, n, fn_7100166800, nullptr);
    }
    __cxa_atexit(fn_7100099468, &g_unk_716e34bc10, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34bd28, &g_unk_716e34bc10);
    {
        CStrId n("msapi::api::types::stencilOp::EStencilOp", true);
        fn_7100167158(&g_unk_716e34bd30, n, fn_7100166d6c, nullptr);
    }
    __cxa_atexit(fn_7100099468, &g_unk_716e34bd30, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34be48, &g_unk_716e34bd30);
    {
        CStrId n("msapi::api::types::textureAddress::ETextureAddress", true);
        fn_71001674e4(&g_unk_716e34be50, n, fn_71001672d8, nullptr);
    }
    __cxa_atexit(fn_7100099468, &g_unk_716e34be50, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34bf68, &g_unk_716e34be50);
    {
        CStrId n("msapi::api::types::textureFilter::ETextureFilter", true);
        fn_7100167870(&g_unk_716e34bf70, n, fn_7100167664, nullptr);
    }
    __cxa_atexit(fn_7100099468, &g_unk_716e34bf70, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34c088, &g_unk_716e34bf70);
    {
        CStrId n("msapi::api::types::fillMode::EFillMode", true);
        fn_7100167b3c(&g_unk_716e34c090, n, fn_71001679f0, nullptr);
    }
    __cxa_atexit(fn_7100099468, &g_unk_716e34c090, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34c1a8, &g_unk_716e34c090);
}

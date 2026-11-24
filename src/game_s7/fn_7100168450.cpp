// Static initialiser of the unit of msapi::api::shdr::EShaderType,
// msapi::api::rs::ERenderStateValue, msapi::api::idx::EPrimitiveType and others: the header ids,
// then the unit's own statics and registrations (helpers in UnkStaticInitS7.h). Layouts are
// placeholders.
#include "UnkStaticInitS7.h"
#include "base/reflection/TypeObject.h"

using namespace unk_static_init;

struct UnkStatics_7100168450 {
    HeaderIds ids;
};
extern UnkStatics_7100168450 g_unk_716e34c1b0 __attribute__((visibility("hidden")));
extern UnkConversion g_unk_716e34b090;
extern UnkConversion g_unk_716e34b0b0;
void fn_7100167cf8() __attribute__((visibility("hidden")));
void fn_7100167d04() __attribute__((visibility("hidden")));
namespace msapi::api::shdr { enum class EShaderType : u32; }
template <> UnkEnumType base::reflection::TypeObject<msapi::api::shdr::EShaderType, UnkEnumType>::object asm("g_unk_716e34af70");
extern char g_unk_716e34b088[];
void fn_7100163228(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710016307c();
namespace msapi::api::rs { enum class ERenderStateValue : u32; }
template <> UnkEnumType base::reflection::TypeObject<msapi::api::rs::ERenderStateValue, UnkEnumType>::object asm("g_unk_716e34b0d0");
void fn_7100163d34(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_71001633a8();
extern char g_unk_716e34b1e8[];
namespace msapi::api::idx { enum class EPrimitiveType : u32; }
template <> UnkEnumType base::reflection::TypeObject<msapi::api::idx::EPrimitiveType, UnkEnumType>::object asm("g_unk_716e34b1f0");
void fn_7100164180(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_7100163eb4();
extern char g_unk_716e34b308[];
namespace msapi::api::idx { enum class EDataType : u32; }
template <> UnkEnumType base::reflection::TypeObject<msapi::api::idx::EDataType, UnkEnumType>::object asm("g_unk_716e34b310");
void fn_710016450c(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_7100164300();
extern char g_unk_716e34b428[];
namespace msapi::api::tex { enum class EComponents : u32; }
template <> UnkEnumType base::reflection::TypeObject<msapi::api::tex::EComponents, UnkEnumType>::object asm("g_unk_716e34b430");
void fn_7100164d18(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_710016468c();
extern char g_unk_716e34b548[];
namespace msapi::api::tex { enum class EArrangement : u32; }
template <> UnkEnumType base::reflection::TypeObject<msapi::api::tex::EArrangement, UnkEnumType>::object asm("g_unk_716e34b550");
void fn_71001650a4(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_7100164e98();
extern char g_unk_716e34b668[];
namespace msapi::api::tex { enum class ETiling : u32; }
template <> UnkEnumType base::reflection::TypeObject<msapi::api::tex::ETiling, UnkEnumType>::object asm("g_unk_716e34b670");
void fn_7100165430(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_7100165224();
extern char g_unk_716e34b788[];
namespace msapi::api::tex { enum class EFiltering : u32; }
template <> UnkEnumType base::reflection::TypeObject<msapi::api::tex::EFiltering, UnkEnumType>::object asm("g_unk_716e34b790");
void fn_710016587c(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_71001655b0();
extern char g_unk_716e34b8a8[];
namespace msapi::api::types::blendOp { enum class EBlendOp : u32; }
template <> UnkEnumType base::reflection::TypeObject<msapi::api::types::blendOp::EBlendOp, UnkEnumType>::object asm("g_unk_716e34b8b0");
void fn_7100165cc8(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_71001659fc();
extern char g_unk_716e34b9c8[];
namespace msapi::api::types::blendFactor { enum class EBlendFactor : u32; }
template <> UnkEnumType base::reflection::TypeObject<msapi::api::types::blendFactor::EBlendFactor, UnkEnumType>::object asm("g_unk_716e34b9d0");
void fn_71001662f4(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_7100165e48();
extern char g_unk_716e34bae8[];
namespace msapi::api::types::cullMode { enum class ECullMode : u32; }
template <> UnkEnumType base::reflection::TypeObject<msapi::api::types::cullMode::ECullMode, UnkEnumType>::object asm("g_unk_716e34baf0");
void fn_7100166680(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_7100166474();
extern char g_unk_716e34bc08[];
namespace msapi::api::types::cmpMode { enum class ECmpMode : u32; }
template <> UnkEnumType base::reflection::TypeObject<msapi::api::types::cmpMode::ECmpMode, UnkEnumType>::object asm("g_unk_716e34bc10");
void fn_7100166bec(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_7100166800();
extern char g_unk_716e34bd28[];
namespace msapi::api::types::stencilOp { enum class EStencilOp : u32; }
template <> UnkEnumType base::reflection::TypeObject<msapi::api::types::stencilOp::EStencilOp, UnkEnumType>::object asm("g_unk_716e34bd30");
void fn_7100167158(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_7100166d6c();
extern char g_unk_716e34be48[];
namespace msapi::api::types::textureAddress { enum class ETextureAddress : u32; }
template <> UnkEnumType base::reflection::TypeObject<msapi::api::types::textureAddress::ETextureAddress, UnkEnumType>::object asm("g_unk_716e34be50");
void fn_71001674e4(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_71001672d8();
extern char g_unk_716e34bf68[];
namespace msapi::api::types::textureFilter { enum class ETextureFilter : u32; }
template <> UnkEnumType base::reflection::TypeObject<msapi::api::types::textureFilter::ETextureFilter, UnkEnumType>::object asm("g_unk_716e34bf70");
void fn_7100167870(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_7100167664();
extern char g_unk_716e34c088[];
namespace msapi::api::types::fillMode { enum class EFillMode : u32; }
template <> UnkEnumType base::reflection::TypeObject<msapi::api::types::fillMode::EFillMode, UnkEnumType>::object asm("g_unk_716e34c090");
void fn_7100167b3c(UnkEnumType* type, const CStrId& name, void (*)(), void*);
void fn_71001679f0();
extern char g_unk_716e34c1a8[];

void InitMsapiApiEnumTypeStatics() {
    InitHeaderIds(&g_unk_716e34c1b0.ids);
    InitEnum(&base::reflection::TypeObject<msapi::api::shdr::EShaderType, UnkEnumType>::object, "msapi::api::shdr::EShaderType", fn_7100163228, fn_710016307c, g_unk_716e34b088, &g_unk_716e34b090, fn_7100167cf8, &g_unk_716e34b0b0, fn_7100167d04);
    {
        CStrId n("msapi::api::rs::ERenderStateValue", true);
        fn_7100163d34(&base::reflection::TypeObject<msapi::api::rs::ERenderStateValue, UnkEnumType>::object, n, fn_71001633a8, nullptr);
    }
    __cxa_atexit(fn_7100099468, &base::reflection::TypeObject<msapi::api::rs::ERenderStateValue, UnkEnumType>::object, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34b1e8, &base::reflection::TypeObject<msapi::api::rs::ERenderStateValue, UnkEnumType>::object);
    {
        CStrId n("msapi::api::idx::EPrimitiveType", true);
        fn_7100164180(&base::reflection::TypeObject<msapi::api::idx::EPrimitiveType, UnkEnumType>::object, n, fn_7100163eb4, nullptr);
    }
    __cxa_atexit(fn_7100099468, &base::reflection::TypeObject<msapi::api::idx::EPrimitiveType, UnkEnumType>::object, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34b308, &base::reflection::TypeObject<msapi::api::idx::EPrimitiveType, UnkEnumType>::object);
    {
        CStrId n("msapi::api::idx::EDataType", true);
        fn_710016450c(&base::reflection::TypeObject<msapi::api::idx::EDataType, UnkEnumType>::object, n, fn_7100164300, nullptr);
    }
    __cxa_atexit(fn_7100099468, &base::reflection::TypeObject<msapi::api::idx::EDataType, UnkEnumType>::object, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34b428, &base::reflection::TypeObject<msapi::api::idx::EDataType, UnkEnumType>::object);
    {
        CStrId n("msapi::api::tex::EComponents", true);
        fn_7100164d18(&base::reflection::TypeObject<msapi::api::tex::EComponents, UnkEnumType>::object, n, fn_710016468c, nullptr);
    }
    __cxa_atexit(fn_7100099468, &base::reflection::TypeObject<msapi::api::tex::EComponents, UnkEnumType>::object, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34b548, &base::reflection::TypeObject<msapi::api::tex::EComponents, UnkEnumType>::object);
    {
        CStrId n("msapi::api::tex::EArrangement", true);
        fn_71001650a4(&base::reflection::TypeObject<msapi::api::tex::EArrangement, UnkEnumType>::object, n, fn_7100164e98, nullptr);
    }
    __cxa_atexit(fn_7100099468, &base::reflection::TypeObject<msapi::api::tex::EArrangement, UnkEnumType>::object, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34b668, &base::reflection::TypeObject<msapi::api::tex::EArrangement, UnkEnumType>::object);
    {
        CStrId n("msapi::api::tex::ETiling", true);
        fn_7100165430(&base::reflection::TypeObject<msapi::api::tex::ETiling, UnkEnumType>::object, n, fn_7100165224, nullptr);
    }
    __cxa_atexit(fn_7100099468, &base::reflection::TypeObject<msapi::api::tex::ETiling, UnkEnumType>::object, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34b788, &base::reflection::TypeObject<msapi::api::tex::ETiling, UnkEnumType>::object);
    {
        CStrId n("msapi::api::tex::EFiltering", true);
        fn_710016587c(&base::reflection::TypeObject<msapi::api::tex::EFiltering, UnkEnumType>::object, n, fn_71001655b0, nullptr);
    }
    __cxa_atexit(fn_7100099468, &base::reflection::TypeObject<msapi::api::tex::EFiltering, UnkEnumType>::object, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34b8a8, &base::reflection::TypeObject<msapi::api::tex::EFiltering, UnkEnumType>::object);
    {
        CStrId n("msapi::api::types::blendOp::EBlendOp", true);
        fn_7100165cc8(&base::reflection::TypeObject<msapi::api::types::blendOp::EBlendOp, UnkEnumType>::object, n, fn_71001659fc, nullptr);
    }
    __cxa_atexit(fn_7100099468, &base::reflection::TypeObject<msapi::api::types::blendOp::EBlendOp, UnkEnumType>::object, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34b9c8, &base::reflection::TypeObject<msapi::api::types::blendOp::EBlendOp, UnkEnumType>::object);
    {
        CStrId n("msapi::api::types::blendFactor::EBlendFactor", true);
        fn_71001662f4(&base::reflection::TypeObject<msapi::api::types::blendFactor::EBlendFactor, UnkEnumType>::object, n, fn_7100165e48, nullptr);
    }
    __cxa_atexit(fn_7100099468, &base::reflection::TypeObject<msapi::api::types::blendFactor::EBlendFactor, UnkEnumType>::object, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34bae8, &base::reflection::TypeObject<msapi::api::types::blendFactor::EBlendFactor, UnkEnumType>::object);
    {
        CStrId n("msapi::api::types::cullMode::ECullMode", true);
        fn_7100166680(&base::reflection::TypeObject<msapi::api::types::cullMode::ECullMode, UnkEnumType>::object, n, fn_7100166474, nullptr);
    }
    __cxa_atexit(fn_7100099468, &base::reflection::TypeObject<msapi::api::types::cullMode::ECullMode, UnkEnumType>::object, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34bc08, &base::reflection::TypeObject<msapi::api::types::cullMode::ECullMode, UnkEnumType>::object);
    {
        CStrId n("msapi::api::types::cmpMode::ECmpMode", true);
        fn_7100166bec(&base::reflection::TypeObject<msapi::api::types::cmpMode::ECmpMode, UnkEnumType>::object, n, fn_7100166800, nullptr);
    }
    __cxa_atexit(fn_7100099468, &base::reflection::TypeObject<msapi::api::types::cmpMode::ECmpMode, UnkEnumType>::object, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34bd28, &base::reflection::TypeObject<msapi::api::types::cmpMode::ECmpMode, UnkEnumType>::object);
    {
        CStrId n("msapi::api::types::stencilOp::EStencilOp", true);
        fn_7100167158(&base::reflection::TypeObject<msapi::api::types::stencilOp::EStencilOp, UnkEnumType>::object, n, fn_7100166d6c, nullptr);
    }
    __cxa_atexit(fn_7100099468, &base::reflection::TypeObject<msapi::api::types::stencilOp::EStencilOp, UnkEnumType>::object, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34be48, &base::reflection::TypeObject<msapi::api::types::stencilOp::EStencilOp, UnkEnumType>::object);
    {
        CStrId n("msapi::api::types::textureAddress::ETextureAddress", true);
        fn_71001674e4(&base::reflection::TypeObject<msapi::api::types::textureAddress::ETextureAddress, UnkEnumType>::object, n, fn_71001672d8, nullptr);
    }
    __cxa_atexit(fn_7100099468, &base::reflection::TypeObject<msapi::api::types::textureAddress::ETextureAddress, UnkEnumType>::object, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34bf68, &base::reflection::TypeObject<msapi::api::types::textureAddress::ETextureAddress, UnkEnumType>::object);
    {
        CStrId n("msapi::api::types::textureFilter::ETextureFilter", true);
        fn_7100167870(&base::reflection::TypeObject<msapi::api::types::textureFilter::ETextureFilter, UnkEnumType>::object, n, fn_7100167664, nullptr);
    }
    __cxa_atexit(fn_7100099468, &base::reflection::TypeObject<msapi::api::types::textureFilter::ETextureFilter, UnkEnumType>::object, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34c088, &base::reflection::TypeObject<msapi::api::types::textureFilter::ETextureFilter, UnkEnumType>::object);
    {
        CStrId n("msapi::api::types::fillMode::EFillMode", true);
        fn_7100167b3c(&base::reflection::TypeObject<msapi::api::types::fillMode::EFillMode, UnkEnumType>::object, n, fn_71001679f0, nullptr);
    }
    __cxa_atexit(fn_7100099468, &base::reflection::TypeObject<msapi::api::types::fillMode::EFillMode, UnkEnumType>::object, g_unk_7101a19000);
    fn_710009ec00(g_unk_716e34c1a8, &base::reflection::TypeObject<msapi::api::types::fillMode::EFillMode, UnkEnumType>::object);
}

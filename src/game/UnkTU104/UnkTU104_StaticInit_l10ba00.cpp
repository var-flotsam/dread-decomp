// Unit 104 (tools/tu_map.py): the static initialiser of the sound system's reflected enums
// (base::snd::ESndType, EAttenuationCurve, EReverbIntensity, ELowPassFilter, EPositionalType,
// ESndPlayerId, ESndLogicId), each with its converters to and from int, and the low-pass cutoff table.
// Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/TypeObject.h"
#include "base/reflection/UnkReflConverter.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "game/UnkTUInit.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::TypeObject;

namespace base::snd { enum class ESndType : u32; }
template <> CType TypeObject<base::snd::ESndType>::object asm("g_unk_7108b6c9c8");
void fn_7100109230(CType* self, const CStrId& name, void (*init)(void*), void* extra);  // refl_InitType<base::snd::ESndType>
void fn_7100108ea4(void* e);
extern char g_unk_7108b6cae0[];
extern UnkConverter g_unk_7108b6cae8;
extern UnkConverter g_unk_7108b6cb08;
void fn_710010b628() __attribute__((visibility("hidden")));
void fn_710010b634() __attribute__((visibility("hidden")));

namespace base::snd { enum class EAttenuationCurve : u32; }
template <> CType TypeObject<base::snd::EAttenuationCurve>::object asm("g_unk_7108b6cb28");
void fn_71001094fc(CType* self, const CStrId& name, void (*init)(void*), void* extra);  // refl_InitType<base::snd::EAttenuationCurve>
void fn_71001093b0(void* e);
extern char g_unk_7108b6cc40[];
extern UnkConverter g_unk_7108b6cc48;
extern UnkConverter g_unk_7108b6cc68;
void fn_710010b640() __attribute__((visibility("hidden")));
void fn_710010b64c() __attribute__((visibility("hidden")));

namespace base::snd { enum class EReverbIntensity : u32; }
template <> CType TypeObject<base::snd::EReverbIntensity>::object asm("g_unk_7108b6cc88");
void fn_71001098e8(CType* self, const CStrId& name, void (*init)(void*), void* extra);  // refl_InitType<base::snd::EReverbIntensity>
void fn_710010967c(void* e);
extern char g_unk_7108b6cda0[];
extern UnkConverter g_unk_7108b6cda8;
extern UnkConverter g_unk_7108b6cdc8;
void fn_710010b658() __attribute__((visibility("hidden")));
void fn_710010b664() __attribute__((visibility("hidden")));

namespace base::snd { enum class ELowPassFilter : u32; }
template <> CType TypeObject<base::snd::ELowPassFilter>::object asm("g_unk_7108b6cde8");
void fn_710010a454(CType* self, const CStrId& name, void (*init)(void*), void* extra);  // refl_InitType<base::snd::ELowPassFilter>
void fn_7100109a68(void* e);
extern char g_unk_7108b6cf00[];
extern UnkConverter g_unk_7108b6cf08;
extern UnkConverter g_unk_7108b6cf28;
void fn_710010b670() __attribute__((visibility("hidden")));
void fn_710010b67c() __attribute__((visibility("hidden")));

namespace base::snd { enum class EPositionalType : u32; }
template <> CType TypeObject<base::snd::EPositionalType>::object asm("g_unk_7108b6cf48");
void fn_710010a720(CType* self, const CStrId& name, void (*init)(void*), void* extra);  // refl_InitType<base::snd::EPositionalType>
void fn_710010a5d4(void* e);
extern char g_unk_7108b6d060[];
extern UnkConverter g_unk_7108b6d068;
extern UnkConverter g_unk_7108b6d088;
void fn_710010b688() __attribute__((visibility("hidden")));
void fn_710010b694() __attribute__((visibility("hidden")));

namespace base::snd { enum class ESndPlayerId : u32; }
template <> CType TypeObject<base::snd::ESndPlayerId>::object asm("g_unk_7108b6d110");
void fn_710010ae7c(CType* self, const CStrId& name, void (*init)(void*), void* extra);  // refl_InitType<base::snd::ESndPlayerId>
void fn_710010aaf0(void* e);
extern char g_unk_7108b6d228[];
extern UnkConverter g_unk_7108b6d230;
extern UnkConverter g_unk_7108b6d250;
void fn_710010b6a0() __attribute__((visibility("hidden")));
void fn_710010b6ac() __attribute__((visibility("hidden")));

namespace base::snd { enum class ESndLogicId : u32; }
template <> CType TypeObject<base::snd::ESndLogicId>::object asm("g_unk_7108b6d270");
void fn_710010b4a8(CType* self, const CStrId& name, void (*init)(void*), void* extra);  // refl_InitType<base::snd::ESndLogicId>
void fn_710010affc(void* e);
extern char g_unk_7108b6d388[];
extern UnkConverter g_unk_7108b6d390;
extern UnkConverter g_unk_7108b6d3b0;
void fn_710010b6b8() __attribute__((visibility("hidden")));
void fn_710010b6c4() __attribute__((visibility("hidden")));

// The low-pass filter cutoffs in hertz, one per ELowPassFilter value (none first).
extern u32 g_unk_7108b6d0a8[25];

void InitSndEnumTypesAndLowPassCutoffs() {
    {
        CStrId name("base::snd::ESndType", true);
        fn_7100109230(&TypeObject<base::snd::ESndType>::object, name, fn_7100108ea4, nullptr);
    }
    fn_7100000250(CEnumType_Destroy, &TypeObject<base::snd::ESndType>::object, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6cae0, &TypeObject<base::snd::ESndType>::object);
    InitEnumConverters(reinterpret_cast<UnkReflType*>(&TypeObject<base::snd::ESndType>::object), g_unk_7108b6cae8, fn_710010b628, g_unk_7108b6cb08, fn_710010b634);

    {
        CStrId name("base::snd::EAttenuationCurve", true);
        fn_71001094fc(&TypeObject<base::snd::EAttenuationCurve>::object, name, fn_71001093b0, nullptr);
    }
    fn_7100000250(CEnumType_Destroy, &TypeObject<base::snd::EAttenuationCurve>::object, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6cc40, &TypeObject<base::snd::EAttenuationCurve>::object);
    InitEnumConverters(reinterpret_cast<UnkReflType*>(&TypeObject<base::snd::EAttenuationCurve>::object), g_unk_7108b6cc48, fn_710010b640, g_unk_7108b6cc68, fn_710010b64c);

    {
        CStrId name("base::snd::EReverbIntensity", true);
        fn_71001098e8(&TypeObject<base::snd::EReverbIntensity>::object, name, fn_710010967c, nullptr);
    }
    fn_7100000250(CEnumType_Destroy, &TypeObject<base::snd::EReverbIntensity>::object, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6cda0, &TypeObject<base::snd::EReverbIntensity>::object);
    InitEnumConverters(reinterpret_cast<UnkReflType*>(&TypeObject<base::snd::EReverbIntensity>::object), g_unk_7108b6cda8, fn_710010b658, g_unk_7108b6cdc8, fn_710010b664);

    {
        CStrId name("base::snd::ELowPassFilter", true);
        fn_710010a454(&TypeObject<base::snd::ELowPassFilter>::object, name, fn_7100109a68, nullptr);
    }
    fn_7100000250(CEnumType_Destroy, &TypeObject<base::snd::ELowPassFilter>::object, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6cf00, &TypeObject<base::snd::ELowPassFilter>::object);
    InitEnumConverters(reinterpret_cast<UnkReflType*>(&TypeObject<base::snd::ELowPassFilter>::object), g_unk_7108b6cf08, fn_710010b670, g_unk_7108b6cf28, fn_710010b67c);

    {
        CStrId name("base::snd::EPositionalType", true);
        fn_710010a720(&TypeObject<base::snd::EPositionalType>::object, name, fn_710010a5d4, nullptr);
    }
    fn_7100000250(CEnumType_Destroy, &TypeObject<base::snd::EPositionalType>::object, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6d060, &TypeObject<base::snd::EPositionalType>::object);
    InitEnumConverters(reinterpret_cast<UnkReflType*>(&TypeObject<base::snd::EPositionalType>::object), g_unk_7108b6d068, fn_710010b688, g_unk_7108b6d088, fn_710010b694);

    u32* cutoffs = g_unk_7108b6d0a8;
    cutoffs[0] = ~0u;
    cutoffs[1] = 80;
    cutoffs[2] = 100;
    cutoffs[3] = 128;
    cutoffs[4] = 160;
    cutoffs[5] = 200;
    cutoffs[6] = 256;
    cutoffs[7] = 320;
    cutoffs[8] = 400;
    cutoffs[9] = 500;
    cutoffs[10] = 640;
    cutoffs[11] = 800;
    cutoffs[12] = 1000;
    cutoffs[13] = 1280;
    cutoffs[14] = 1600;
    cutoffs[15] = 2000;
    cutoffs[16] = 2560;
    cutoffs[17] = 3200;
    cutoffs[18] = 4000;
    cutoffs[19] = 5120;
    cutoffs[20] = 6400;
    cutoffs[21] = 8000;
    cutoffs[22] = 10240;
    cutoffs[23] = 12800;
    cutoffs[24] = 16000;

    {
        CStrId name("base::snd::ESndPlayerId", true);
        fn_710010ae7c(&TypeObject<base::snd::ESndPlayerId>::object, name, fn_710010aaf0, nullptr);
    }
    fn_7100000250(CEnumType_Destroy, &TypeObject<base::snd::ESndPlayerId>::object, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6d228, &TypeObject<base::snd::ESndPlayerId>::object);
    InitEnumConverters(reinterpret_cast<UnkReflType*>(&TypeObject<base::snd::ESndPlayerId>::object), g_unk_7108b6d230, fn_710010b6a0, g_unk_7108b6d250, fn_710010b6ac);

    {
        CStrId name("base::snd::ESndLogicId", true);
        fn_710010b4a8(&TypeObject<base::snd::ESndLogicId>::object, name, fn_710010affc, nullptr);
    }
    fn_7100000250(CEnumType_Destroy, &TypeObject<base::snd::ESndLogicId>::object, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6d388, &TypeObject<base::snd::ESndLogicId>::object);
    InitEnumConverters(reinterpret_cast<UnkReflType*>(&TypeObject<base::snd::ESndLogicId>::object), g_unk_7108b6d390, fn_710010b6b8, g_unk_7108b6d3b0, fn_710010b6c4);
}

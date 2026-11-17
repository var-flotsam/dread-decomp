// Static initialiser of the unit of the CRinkaUnitComponent::ERinkaType, CRinkaUnitComponent::ECentralUnitType enums.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/TypeObject.h"

using namespace unk_static_init;

struct UnkStatics_7100661340 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkFilePathStrIdBase field_0x30;
    u8 pad_0x38[0x90];
    u8 field_0xc8[8];
};
extern UnkStatics_7100661340 g_unk_71726fd0f0 __attribute__((visibility("hidden")));
extern char g_unk_71726fce28[];
namespace CRinkaUnitComponent { enum class ERinkaType : u32; }
template <> UnkEnumType base::reflection::TypeObject<CRinkaUnitComponent::ERinkaType, UnkEnumType>::object asm("g_unk_71726fce30");
void fn_710065ece4(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710065eb38();
extern char g_unk_71726fcf48[];
extern UnkConversion g_unk_71726fcf50;
extern UnkConversion g_unk_71726fcf70;
void fn_7100660e00() __attribute__((visibility("hidden")));
void fn_7100660e0c() __attribute__((visibility("hidden")));
namespace CRinkaUnitComponent { enum class ECentralUnitType : u32; }
template <> UnkEnumType base::reflection::TypeObject<CRinkaUnitComponent::ECentralUnitType, UnkEnumType>::object asm("g_unk_71726fcf90");
void fn_710065f0d0(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710065ee64();
extern char g_unk_71726fd0a8[];
extern UnkConversion g_unk_71726fd0b0;
extern UnkConversion g_unk_71726fd0d0;
void fn_7100660e18() __attribute__((visibility("hidden")));
void fn_7100660e24() __attribute__((visibility("hidden")));

void fn_7100661340() {
    InitHeaderIds(&g_unk_71726fd0f0.ids);
    fn_710065e7c4();
    fn_710009ec00(g_unk_71726fce28, g_unk_71726fd0f0.field_0xc8);
    InitEnum(&base::reflection::TypeObject<CRinkaUnitComponent::ERinkaType, UnkEnumType>::object, "CRinkaUnitComponent::ERinkaType", fn_710065ece4, fn_710065eb38,
             g_unk_71726fcf48, &g_unk_71726fcf50, fn_7100660e00, &g_unk_71726fcf70, fn_7100660e0c);
    InitEnum(&base::reflection::TypeObject<CRinkaUnitComponent::ECentralUnitType, UnkEnumType>::object, "CRinkaUnitComponent::ECentralUnitType", fn_710065f0d0, fn_710065ee64,
             g_unk_71726fd0a8, &g_unk_71726fd0b0, fn_7100660e18, &g_unk_71726fd0d0, fn_7100660e24);
    InitId(&g_unk_71726fd0f0.field_0x20, "rinkainitloop");
    InitId(&g_unk_71726fd0f0.field_0x28, "rinkaspawningloop");
    InitPathId(&g_unk_71726fd0f0.field_0x30, "actors/props/rinkaunit/fx/rinka_init.bcptl");
}

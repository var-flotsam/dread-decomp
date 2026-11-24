// Static initialiser of the unit of the behaviortree::SStatus::Enum, behaviortree::SCompareOperator::Enum.
#include "../../../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/TypeObject.h"

using namespace unk_static_init;

struct UnkStatics_71003ab270 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_71003ab270 g_unk_71726bc3f8 __attribute__((visibility("hidden")));
namespace behaviortree::SStatus { enum class Enum : u32; }
template <> UnkEnumType base::reflection::TypeObject<behaviortree::SStatus::Enum, UnkEnumType>::object asm("g_unk_71726bc130");
void fn_71003a9a9c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71003a9710();
extern char g_unk_71726bc248[];
extern UnkConversion g_unk_71726bc250;
extern UnkConversion g_unk_71726bc270;
void fn_71003aaf38() __attribute__((visibility("hidden")));
void fn_71003aaf44() __attribute__((visibility("hidden")));
namespace behaviortree::SCompareOperator { enum class Enum : u32; }
template <> UnkEnumType base::reflection::TypeObject<behaviortree::SCompareOperator::Enum, UnkEnumType>::object asm("g_unk_71726bc290");
void fn_71003a9ee8(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71003a9c1c();
extern char g_unk_71726bc3a8[];
extern UnkConversion g_unk_71726bc3b0;
extern UnkConversion g_unk_71726bc3d0;
void fn_71003aaf50() __attribute__((visibility("hidden")));
void fn_71003aaf5c() __attribute__((visibility("hidden")));
extern char g_unk_71726bc3f0[];

void CBehaviorTree_InitStatics() {
    InitHeaderIds(&g_unk_71726bc3f8.ids);
    InitEnum(&base::reflection::TypeObject<behaviortree::SStatus::Enum, UnkEnumType>::object, "behaviortree::SStatus::Enum", fn_71003a9a9c, fn_71003a9710,
             g_unk_71726bc248, &g_unk_71726bc250, fn_71003aaf38, &g_unk_71726bc270, fn_71003aaf44);
    InitEnum(&base::reflection::TypeObject<behaviortree::SCompareOperator::Enum, UnkEnumType>::object, "behaviortree::SCompareOperator::Enum", fn_71003a9ee8, fn_71003a9c1c,
             g_unk_71726bc3a8, &g_unk_71726bc3b0, fn_71003aaf50, &g_unk_71726bc3d0, fn_71003aaf5c);
    fn_71003aa0c0();
    fn_710009ec00(g_unk_71726bc3f0, g_unk_71726bc3f8.field_0x20);
}

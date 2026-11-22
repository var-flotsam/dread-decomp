// CHeatRoomComponent unit (tools/tu_map.py unit 1538): its static initialiser. The unit header, the
// "HEAT_ROOMS" and "HeatRoomEnabled" properties, the heat room type enum with its converters, the two
// config classes, the component's class and its id. Names and layouts are placeholders.
#include "base/UnkStrIdRegistry.h"
#include "base/reflection/CCollectionType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CEmmyProtoAIComponent.h"
#include "game/CHeatRoomComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/UnkTUInit.h"
#include "base/reflection/TypeObject.h"

// The unit's data (its statics, merged into one block).
struct UnkHeatRoomTU {
    UnkTUHeader header;
    u64 field_0x18;
    CStrId id;  // 0x20
    u64 field_0x28;
    f32 field_0x30[2];
    bool field_0x38;
    u8 pad_0x39[0xb0 - 0x39];
    UnkReflType component_type;  // 0xb0
};
static_assert(__builtin_offsetof(UnkHeatRoomTU, component_type) == 0xb0, "layout");

extern UnkHeatRoomTU g_unk_7172746770 __attribute__((visibility("hidden")));
extern UnkPropertyEx<> g_unk_71727461c8;
enum class EThermalHeatRoomType : u32;
template <> UnkReflType base::reflection::TypeObject<EThermalHeatRoomType, UnkReflType>::object asm("g_unk_7172746390");
void fn_7100a24c3c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_7100a24af0();
extern char g_unk_71727464a8;
extern UnkConverter g_unk_71727464b0;
extern UnkConverter g_unk_71727464d0;
void fn_7100a280ec() __attribute__((visibility("hidden")));
void fn_7100a280f8() __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727464f0;
void fn_7100a25258(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100a24dbc();
extern char g_unk_7172746628;
extern UnkReflType g_unk_7172746630;
void fn_7100a2586c(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100a253dc();
extern char g_unk_7172746768;
extern char g_unk_7172746769;

void fn_7100a28310() {
    UnkHeatRoomTU& tu = g_unk_7172746770;
    InitHeader(tu.header);
    tu.field_0x30[0] = -1.0f;
    tu.field_0x30[1] = -1.0f;
    tu.field_0x38 = true;
    InitPropertyExVtableFirst(&g_unk_71727461c8, g_unk_7101a45fe8, g_unk_71726db788, g_unk_7101a45f90, fn_71002ef5bc,
                              "HEAT_ROOMS", static_cast<void*>(&g_unk_7101d5c998), fn_71004fd2cc, fn_71004fa3b8);
    InitPropertyExVtableFirst(&g_unk_7172746208, g_unk_7101a889c8, g_unk_71726db788, g_unk_7101a8d830, fn_7100ae9cc8,
                              "HeatRoomEnabled", refl_TypeOf<fn_710000d228>(), fn_71004fd2cc, fn_7100a24a50);
    InitEnum(&base::reflection::TypeObject<EThermalHeatRoomType, UnkReflType>::object, "EThermalHeatRoomType", fn_7100a24c3c, fn_7100a24af0);
    fn_710009ec00(&g_unk_71727464a8, &base::reflection::TypeObject<EThermalHeatRoomType, UnkReflType>::object);
    InitEnumConverters(&base::reflection::TypeObject<EThermalHeatRoomType, UnkReflType>::object, g_unk_71727464b0, fn_7100a280ec, g_unk_71727464d0, fn_7100a280f8);
    InitClass(&g_unk_71727464f0, "CHeatRoomConfig", fn_7100a25258, fn_7100a24dbc);
    fn_710009ec00(&g_unk_7172746628, &g_unk_71727464f0);
    InitClass(&g_unk_7172746630, "CHeatRoomCoolConfig", fn_7100a2586c, fn_7100a253dc);
    fn_710009ec00(&g_unk_7172746768, &g_unk_7172746630);
    fn_7100a259f4();
    fn_710009ec00(&g_unk_7172746769, &tu.component_type);
    new (&tu.id) CStrId(fn_7100de8f78(CStrId("CHeatRoomComponent", true)));
    fn_7100000250(CStrId_Destroy, &tu.id, &g_unk_7101a19000);
}

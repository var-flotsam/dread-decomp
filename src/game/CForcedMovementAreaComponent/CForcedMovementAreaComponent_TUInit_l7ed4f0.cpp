// CForcedMovementAreaComponent's static initialiser (unit 1256): the unit header, the area's event
// names, the component's id, the EForcedDirection enum, the component's registration and three
// message names. Names and layouts are placeholders.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/TypeObject.h"

using namespace unk_static_init;

struct UnkStatics_71007ed4f0 {
    HeaderIds ids;
    UnkStrIdBase turnoff;     // 0x18
    UnkStrIdBase turnon;      // 0x20
    UnkStrIdBase running;     // 0x28
    UnkStrIdBase name;        // 0x30
    UnkStrIdBase field_0x38;  // 0x38, the class name again
    CStrId id;                // 0x40
    u8 pad_0x48[8];
    UnkStrIdBase forcemovement;       // 0x50
    UnkStrIdBase door;                // 0x58
    UnkStrIdBase blockforcemovement;  // 0x60
    u8 field_0x68[8];
};
extern UnkStatics_71007ed4f0 g_unk_717271fd98 __attribute__((visibility("hidden")));
namespace CForcedMovementAreaComponent { enum class EForcedDirection : u32; }
template <> UnkEnumType base::reflection::TypeObject<CForcedMovementAreaComponent::EForcedDirection, UnkEnumType>::object asm("g_unk_717271fc20");
void fn_71007e8e2c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71007e8c80();
extern char g_unk_717271fd38[];
extern UnkConversion g_unk_717271fd40;
extern UnkConversion g_unk_717271fd60;
void fn_71007ece64() __attribute__((visibility("hidden")));
void fn_71007ece70() __attribute__((visibility("hidden")));
extern char g_unk_717271fd80[];

// The component's name as an id, in an inline function: its guard and object are reached through
// the GOT.
inline const CStrId& ForcedMovementAreaId() {
    static CStrId s_id("FORCED_MOVEMENT_AREA", true);
    return s_id;
}

void fn_71007ed4f0() {
    UnkStatics_71007ed4f0& tu = g_unk_717271fd98;
    InitHeaderIds(&tu.ids);
    InitId(&tu.turnoff, "turnoff");
    InitId(&tu.turnon, "turnon");
    InitId(&tu.running, "running");
    InitId(&tu.name, "ForcedMovementAreaComponent");
    InitId(&tu.field_0x38, "ForcedMovementAreaComponent");
    new (&tu.id) CStrId(fn_7100de8f78(ForcedMovementAreaId()));
    fn_7100000250(CStrId_Destroy, &tu.id, g_unk_7101a19000);
    InitEnum(&base::reflection::TypeObject<CForcedMovementAreaComponent::EForcedDirection, UnkEnumType>::object, "CForcedMovementAreaComponent::EForcedDirection", fn_71007e8e2c, fn_71007e8c80,
             g_unk_717271fd38, &g_unk_717271fd40, fn_71007ece64, &g_unk_717271fd60, fn_71007ece70);
    fn_71007e8fb0();
    fn_710009ec00(g_unk_717271fd80, tu.field_0x68);
    InitId(&tu.forcemovement, "forcemovement");
    InitId(&tu.door, "door");
    InitId(&tu.blockforcemovement, "blockforcemovement");
}

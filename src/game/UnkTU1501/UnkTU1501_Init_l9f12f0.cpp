// UnkTU1501 (CSamusMovement's unit, tools/tu_map.py unit 1501): the unit's static initialiser: its
// string ids, the run sound preset's id and path, the CTunableSamusMovement and CSamusMovement
// registrations, the tunable and the CSamusMovement::EUpdateCollisionsPhaseOrder enum. Names and
// layouts are placeholders.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAbilitySonar.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU25.h"
#include "game/UnkTUProperty.h"
#include "base/reflection/TypeObject.h"

using namespace unk_static_init;

struct UnkIdPathPair_7100333544;

struct UnkStatics_71009f12f0 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    UnkStrIdBase field_0x40;
    UnkStrIdBase field_0x48;
    UnkStrIdBase field_0x50;
    UnkStrIdBase field_0x58;
    u8 pad_0x60[0x30];
    f32 field_0x90;
    f32 field_0x94;
    bool field_0x98;
    u8 pad_0x99[7];
    u8 field_0xa0[0x10];  // a sound preset's id and path (fn_7100333544 destroys it)
    u8 field_0xb0[8];
    u8 pad_0xb8[0x1e8 - 0xb8];
    u8 field_0x1e8[8];
};
static_assert(__builtin_offsetof(UnkStatics_71009f12f0, field_0x1e8) == 0x1e8, "layout");
extern UnkStatics_71009f12f0 g_unk_7172740fc8 __attribute__((visibility("hidden")));

extern char g_unk_7172740dd8[];
extern char g_unk_7172740e61[];
namespace CSamusMovement { enum class EUpdateCollisionsPhaseOrder : u32; }
template <> UnkEnumType base::reflection::TypeObject<CSamusMovement::EUpdateCollisionsPhaseOrder, UnkEnumType>::object asm("g_unk_7172740e68");
void fn_71009df32c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71009df1e0();
extern char g_unk_7172740f80[];
extern UnkConversion g_unk_7172740f88;
extern UnkConversion g_unk_7172740fa8;
void fn_71009f0c1c() __attribute__((visibility("hidden")));
void fn_71009f0c28() __attribute__((visibility("hidden")));
extern void* g_unk_7101a8a468[];

void fn_71009f12f0() {
    InitHeaderIds(&g_unk_7172740fc8.ids);
    g_unk_7172740fc8.field_0x90 = -1.0f;
    g_unk_7172740fc8.field_0x94 = -1.0f;
    g_unk_7172740fc8.field_0x98 = true;
    InitId(&g_unk_7172740fc8.field_0x18, "CharacterFloorSlide");
    InitId(&g_unk_7172740fc8.field_0x20, "fFallEndTransition");
    InitId(&g_unk_7172740fc8.field_0x28, "BossMelee");
    InitId(&g_unk_7172740fc8.field_0x30, "SamusMovementInput");
    InitId(&g_unk_7172740fc8.field_0x38, "PostAnimation");
    InitId(&g_unk_7172740fc8.field_0x40, "LeftLeg");
    InitId(&g_unk_7172740fc8.field_0x48, "RightLeg");
    {
        base::UnkNamedEntry path;
        fn_710007e704(&path, "audiopreset:system/snd/presets/samus/samus_run_r");
        fn_710001f074(reinterpret_cast<UnkPathIds*>(g_unk_7172740fc8.field_0xa0),
                      reinterpret_cast<const UnkInlineString*>(&path));
        fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(&path));
    }
    fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_7100333544), g_unk_7172740fc8.field_0xa0, g_unk_7101a19000);
    InitId(&g_unk_7172740fc8.field_0x50, "Auto");
    InitId(&g_unk_7172740fc8.field_0x58, "CCharClassSamusActivateLegsNodesOverrideTrack");
    fn_71009dc3c0();
    fn_710009ec00(g_unk_7172740dd8, g_unk_7172740fc8.field_0xb0);
    InitTunable(reinterpret_cast<UnkTunable*>(&g_unk_7172740de0), fn_7100136ecc, g_unk_7101a8a468, fn_7100075104);
    fn_71009dda1c();
    fn_710009ec00(g_unk_7172740e61, g_unk_7172740fc8.field_0x1e8);
    InitEnum(&base::reflection::TypeObject<CSamusMovement::EUpdateCollisionsPhaseOrder, UnkEnumType>::object, "CSamusMovement::EUpdateCollisionsPhaseOrder", fn_71009df32c, fn_71009df1e0,
             g_unk_7172740f80, &g_unk_7172740f88, fn_71009f0c1c, &g_unk_7172740fa8, fn_71009f0c28);
}

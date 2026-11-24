// Static initialiser of the unit of GUI::CGlobalMapComposition::SState: the header ids, the unit's
// own statics (a string parameter, the transport labels, the fade times) and its registrations
// (helpers in UnkStaticInitS7.h). Names and layouts are placeholders.
#include "../../../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/GUI/CGlobalMapComposition.h"
#include "game/UnkBufferStringPool.h"
#include "base/reflection/TypeObject.h"

using namespace unk_static_init;

// The fade times: built by fn_71001d81bc, then given the unit's values.
struct UnkMapFadeTimes {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    u8 pad_0x14[4];
    f32 field_0x18;
};

struct __attribute__((packed, aligned(4))) UnkStatics_7100b2bc80 {
    HeaderIds4 ids;
    UnkStrIdBase field_0x14;
    UnkStrIdBase field_0x1c;
    UnkStrIdBase field_0x24;
    UnkStrIdBase field_0x2c;
    UnkStrIdBase field_0x34;
    UnkStrIdBase field_0x3c;
    UnkStrIdBase field_0x44;
    UnkStrIdBase field_0x4c;
    UnkStrIdBase field_0x54;
    u8 pad_0x5c[0xf0];
    f32 field_0x14c;
    f32 field_0x150;
    bool field_0x154;
    u8 pad_0x155[0x3];
    UnkMapFadeTimes field_0x158;
    u8 field_0x174[8];
};
static_assert(__builtin_offsetof(UnkStatics_7100b2bc80, field_0x14c) == 0x14c, "layout");
static_assert(__builtin_offsetof(UnkStatics_7100b2bc80, field_0x174) == 0x174, "layout");
extern UnkStatics_7100b2bc80 g_unk_7172762d04 __attribute__((visibility("hidden")));

extern UnkBufferParam g_unk_7172762b18;
void fn_71001d81bc(UnkMapFadeTimes* self);
extern char g_unk_7172762b98[];
namespace GUI::CGlobalMapComposition { enum class SState : u32; }
template <> UnkEnumType base::reflection::TypeObject<GUI::CGlobalMapComposition::SState, UnkEnumType>::object asm("g_unk_7172762ba0");
void fn_7100b2390c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100b235e0();
extern char g_unk_7172762cb8[];
extern UnkConversion g_unk_7172762cc0;
extern UnkConversion g_unk_7172762ce0;
void fn_7100b2b1c8() __attribute__((visibility("hidden")));
void fn_7100b2b1d4() __attribute__((visibility("hidden")));

void fn_7100b2bc80() {
    InitHeaderIds(&g_unk_7172762d04.ids);
    g_unk_7172762d04.field_0x14c = -1.0f;
    g_unk_7172762d04.field_0x150 = -1.0f;
    g_unk_7172762d04.field_0x154 = true;

    UnkBufferParam* param = &g_unk_7172762b18;
    param->field_0x0 = 0.0f;
    param->field_0x4 = -4.0f;
    param->field_0x8 = 3.0f;
    auto* str = reinterpret_cast<base::global::CRntString*>(param->field_0x10);
    CRntString_InitWithAllocator(str, reinterpret_cast<base::IAllocator*>(&param->field_0x40));
    param->field_0x40 = g_unk_7101a1b5a0 + 2;
    param->field_0x68 = 0;
    param->field_0x70 = 0;
    CRntString_AssignCString(str, g_unk_710162e2ed);
    param->field_0x78 = 1;
    fn_7100000250(fn_7100b228e0, param, g_unk_7101a19000);

    InitId(&g_unk_7172762d04.field_0x14, "#GUI_TRANSPORT_AQUA");
    InitId(&g_unk_7172762d04.field_0x1c, "#GUI_TRANSPORT_CAVE");
    InitId(&g_unk_7172762d04.field_0x24, "#GUI_TRANSPORT_FOREST");
    InitId(&g_unk_7172762d04.field_0x2c, "#GUI_TRANSPORT_LAB");
    InitId(&g_unk_7172762d04.field_0x34, "#GUI_TRANSPORT_MAGMA");
    InitId(&g_unk_7172762d04.field_0x3c, "#GUI_TRANSPORT_QUARANTINE");
    InitId(&g_unk_7172762d04.field_0x44, "#GUI_TRANSPORT_SANC");
    InitId(&g_unk_7172762d04.field_0x4c, "#GUI_TRANSPORT_SHIPYARD");
    InitId(&g_unk_7172762d04.field_0x54, "#GUI_TRANSPORT_SKYBASE");

    UnkMapFadeTimes* fade = &g_unk_7172762d04.field_0x158;
    fn_71001d81bc(fade);
    fade->field_0x0 = 0.3f;
    fade->field_0x4 = 0.02f;
    fade->field_0x8 = 1.0f;
    fade->field_0xc = 0.02f;
    fade->field_0x10 = 0.7f;
    fade->field_0x18 = 0.08f;

    fn_7100b23388();
    fn_710009ec00(g_unk_7172762b98, g_unk_7172762d04.field_0x174);
    InitEnum(&base::reflection::TypeObject<GUI::CGlobalMapComposition::SState, UnkEnumType>::object, "GUI::CGlobalMapComposition::SState", fn_7100b2390c, fn_7100b235e0,
             g_unk_7172762cb8, &g_unk_7172762cc0, fn_7100b2b1c8, &g_unk_7172762ce0, fn_7100b2b1d4);
}

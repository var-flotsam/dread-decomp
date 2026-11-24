// Static initialiser of the unit of OnOpened, OnClosed, OnSelected and others: the header ids,
// then the unit's own statics and registrations (helpers in UnkStaticInitS7.h). Layouts are
// placeholders.
#include "../../../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/sound/SGUISoundEventsSelector.h"

using namespace unk_static_init;

struct UnkStatics_7100fcbf50 {
    HeaderIds ids;
    u8 pad_0x18[0x30];
    f32 field_0x48;
    f32 field_0x4c;
    bool field_0x50;
    u8 pad_0x51[0xc7];
    u8 field_0x118[8];
};
extern UnkStatics_7100fcbf50 g_unk_71729f02e8 __attribute__((visibility("hidden")));
extern char g_unk_71729f0174[];
struct UnkStateKey;
void SGUISoundEventsSelector_RegisterEventId(UnkEnumType* type, const CStrId& name);
void fn_7100fc91a8(void* type);  // the type's destructor, as registered
extern UnkEnumType g_unk_71729f0190;
extern UnkEnumType g_unk_71729f0198;
extern UnkEnumType g_unk_71729f01b8;
extern UnkEnumType g_unk_71729f01c0;
extern UnkEnumType g_unk_71729f01c8;
extern UnkEnumType g_unk_71729f01d8;
extern UnkEnumType g_unk_71729f01e0;
extern UnkEnumType g_unk_71729f01e8;
extern UnkEnumType g_unk_71729f01f0;
extern UnkEnumType g_unk_71729f01f8;
extern UnkEnumType g_unk_71729f0200;
extern UnkEnumType g_unk_71729f0208;
extern UnkEnumType g_unk_71729f0210;
extern UnkEnumType g_unk_71729f0218;
extern UnkEnumType g_unk_71729f0220;
extern UnkEnumType g_unk_71729f0230;
extern UnkEnumType g_unk_71729f0238;
extern UnkEnumType g_unk_71729f0240;
extern UnkEnumType g_unk_71729f0248;
extern UnkEnumType g_unk_71729f0250;
extern UnkEnumType g_unk_71729f0258;
extern UnkEnumType g_unk_71729f0260;
extern UnkEnumType g_unk_71729f0268;
extern UnkEnumType g_unk_71729f0270;
extern UnkEnumType g_unk_71729f0280;
extern UnkEnumType g_unk_71729f0288;
extern UnkEnumType g_unk_71729f0290;
extern UnkEnumType g_unk_71729f0298;
extern UnkEnumType g_unk_71729f02a0;
extern UnkEnumType g_unk_71729f02a8;

void SGUISoundEventsSelector_InitStatics() {
    InitHeaderIds(&g_unk_71729f02e8.ids);
    g_unk_71729f02e8.field_0x48 = -1.0f;
    g_unk_71729f02e8.field_0x4c = -1.0f;
    g_unk_71729f02e8.field_0x50 = true;
    fn_7100fc87f0();
    fn_710009ec00(g_unk_71729f0174, g_unk_71729f02e8.field_0x118);
    {
        CStrId n("OnOpened", true);
        SGUISoundEventsSelector_RegisterEventId(&reinterpret_cast<UnkEnumType&>(g_unk_71729f0178), n);
    }
    __cxa_atexit(fn_7100fc91a8, &reinterpret_cast<UnkEnumType&>(g_unk_71729f0178), g_unk_7101a19000);
    {
        CStrId n("OnClosed", true);
        SGUISoundEventsSelector_RegisterEventId(&reinterpret_cast<UnkEnumType&>(g_unk_71729f0180), n);
    }
    __cxa_atexit(fn_7100fc91a8, &reinterpret_cast<UnkEnumType&>(g_unk_71729f0180), g_unk_7101a19000);
    {
        CStrId n("OnSelected", true);
        SGUISoundEventsSelector_RegisterEventId(&reinterpret_cast<UnkEnumType&>(g_unk_71729f0188), n);
    }
    __cxa_atexit(fn_7100fc91a8, &reinterpret_cast<UnkEnumType&>(g_unk_71729f0188), g_unk_7101a19000);
    {
        CStrId n("OnExpand", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0190, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0190, g_unk_7101a19000);
    {
        CStrId n("OnRestore", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0198, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0198, g_unk_7101a19000);
    {
        CStrId n("OnActionDenied", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f01a0, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01a0, g_unk_7101a19000);
    {
        CStrId n("OnActivated", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f01a8, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01a8, g_unk_7101a19000);
    {
        CStrId n("OnCancel", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f01b0, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01b0, g_unk_7101a19000);
    {
        CStrId n("OnToggleOn", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f01b8, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01b8, g_unk_7101a19000);
    {
        CStrId n("OnToggleOff", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f01c0, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01c0, g_unk_7101a19000);
    {
        CStrId n("OnNext", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f01c8, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01c8, g_unk_7101a19000);
    {
        CStrId n("OnStartGame", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f01d0, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01d0, g_unk_7101a19000);
    {
        CStrId n("OnLoadGame", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f01d8, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01d8, g_unk_7101a19000);
    {
        CStrId n("OnCopyGame", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f01e0, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01e0, g_unk_7101a19000);
    {
        CStrId n("OnDeleteGame", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f01e8, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01e8, g_unk_7101a19000);
    {
        CStrId n("OnEmptySlotActive", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f01f0, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01f0, g_unk_7101a19000);
    {
        CStrId n("OnDataSlotActive", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f01f8, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01f8, g_unk_7101a19000);
    {
        CStrId n("OnZoom", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0200, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0200, g_unk_7101a19000);
    {
        CStrId n("OnMove", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0208, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0208, g_unk_7101a19000);
    {
        CStrId n("OnRecenter", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0210, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0210, g_unk_7101a19000);
    {
        CStrId n("OnFastMovement", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0218, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0218, g_unk_7101a19000);
    {
        CStrId n("OnItemSelected", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0220, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0220, g_unk_7101a19000);
    {
        CStrId n("OnItemRevealed", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0228, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0228, g_unk_7101a19000);
    {
        CStrId n("OnMapDownload", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0230, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0230, g_unk_7101a19000);
    {
        CStrId n("OnEmmyZonePresented", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0238, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0238, g_unk_7101a19000);
    {
        CStrId n("OnTransportInit", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0240, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0240, g_unk_7101a19000);
    {
        CStrId n("OnTransportDest", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0248, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0248, g_unk_7101a19000);
    {
        CStrId n("OnTeleportTravelArrow", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0250, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0250, g_unk_7101a19000);
    {
        CStrId n("OnMarkerPlaced", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0258, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0258, g_unk_7101a19000);
    {
        CStrId n("OnMarkerRemoved", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0260, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0260, g_unk_7101a19000);
    {
        CStrId n("OnAllMarkersRemoved", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0268, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0268, g_unk_7101a19000);
    {
        CStrId n("OnFailure", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0270, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0270, g_unk_7101a19000);
    {
        CStrId n("OnSuccess", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0278, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0278, g_unk_7101a19000);
    {
        CStrId n("OnAeionRecharge", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0280, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0280, g_unk_7101a19000);
    {
        CStrId n("OnAeionFill", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0288, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0288, g_unk_7101a19000);
    {
        CStrId n("OnHardModeUnlocked", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0290, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0290, g_unk_7101a19000);
    {
        CStrId n("OnScroll", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f0298, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0298, g_unk_7101a19000);
    {
        CStrId n("OnRumbleChanged", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f02a0, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f02a0, g_unk_7101a19000);
    {
        CStrId n("OnTankObtained", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f02a8, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f02a8, g_unk_7101a19000);
    {
        CStrId n("OnEnergyRefill", true);
        SGUISoundEventsSelector_RegisterEventId(&g_unk_71729f02b0, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f02b0, g_unk_7101a19000);
}

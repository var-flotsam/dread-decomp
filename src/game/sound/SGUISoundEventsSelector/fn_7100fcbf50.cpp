// Static initialiser of the unit of OnOpened, OnClosed, OnSelected and others: the header ids,
// then the unit's own statics and registrations (helpers in UnkStaticInitS7.h). Layouts are
// placeholders.
#include "../../../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

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
extern UnkEnumType g_unk_71729f0178;
void fn_7100fc9084(UnkEnumType* type, const CStrId& name);
void fn_7100fc91a8(void* type);  // the type's destructor, as registered
extern UnkEnumType g_unk_71729f0180;
extern UnkEnumType g_unk_71729f0188;
extern UnkEnumType g_unk_71729f0190;
extern UnkEnumType g_unk_71729f0198;
extern UnkEnumType g_unk_71729f01a0;
extern UnkEnumType g_unk_71729f01a8;
extern UnkEnumType g_unk_71729f01b0;
extern UnkEnumType g_unk_71729f01b8;
extern UnkEnumType g_unk_71729f01c0;
extern UnkEnumType g_unk_71729f01c8;
extern UnkEnumType g_unk_71729f01d0;
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
extern UnkEnumType g_unk_71729f0228;
extern UnkEnumType g_unk_71729f0230;
extern UnkEnumType g_unk_71729f0238;
extern UnkEnumType g_unk_71729f0240;
extern UnkEnumType g_unk_71729f0248;
extern UnkEnumType g_unk_71729f0250;
extern UnkEnumType g_unk_71729f0258;
extern UnkEnumType g_unk_71729f0260;
extern UnkEnumType g_unk_71729f0268;
extern UnkEnumType g_unk_71729f0270;
extern UnkEnumType g_unk_71729f0278;
extern UnkEnumType g_unk_71729f0280;
extern UnkEnumType g_unk_71729f0288;
extern UnkEnumType g_unk_71729f0290;
extern UnkEnumType g_unk_71729f0298;
extern UnkEnumType g_unk_71729f02a0;
extern UnkEnumType g_unk_71729f02a8;
extern UnkEnumType g_unk_71729f02b0;

void fn_7100fcbf50() {
    InitHeaderIds(&g_unk_71729f02e8.ids);
    g_unk_71729f02e8.field_0x48 = -1.0f;
    g_unk_71729f02e8.field_0x4c = -1.0f;
    g_unk_71729f02e8.field_0x50 = true;
    fn_7100fc87f0();
    fn_710009ec00(g_unk_71729f0174, g_unk_71729f02e8.field_0x118);
    {
        CStrId n("OnOpened", true);
        fn_7100fc9084(&g_unk_71729f0178, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0178, g_unk_7101a19000);
    {
        CStrId n("OnClosed", true);
        fn_7100fc9084(&g_unk_71729f0180, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0180, g_unk_7101a19000);
    {
        CStrId n("OnSelected", true);
        fn_7100fc9084(&g_unk_71729f0188, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0188, g_unk_7101a19000);
    {
        CStrId n("OnExpand", true);
        fn_7100fc9084(&g_unk_71729f0190, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0190, g_unk_7101a19000);
    {
        CStrId n("OnRestore", true);
        fn_7100fc9084(&g_unk_71729f0198, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0198, g_unk_7101a19000);
    {
        CStrId n("OnActionDenied", true);
        fn_7100fc9084(&g_unk_71729f01a0, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01a0, g_unk_7101a19000);
    {
        CStrId n("OnActivated", true);
        fn_7100fc9084(&g_unk_71729f01a8, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01a8, g_unk_7101a19000);
    {
        CStrId n("OnCancel", true);
        fn_7100fc9084(&g_unk_71729f01b0, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01b0, g_unk_7101a19000);
    {
        CStrId n("OnToggleOn", true);
        fn_7100fc9084(&g_unk_71729f01b8, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01b8, g_unk_7101a19000);
    {
        CStrId n("OnToggleOff", true);
        fn_7100fc9084(&g_unk_71729f01c0, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01c0, g_unk_7101a19000);
    {
        CStrId n("OnNext", true);
        fn_7100fc9084(&g_unk_71729f01c8, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01c8, g_unk_7101a19000);
    {
        CStrId n("OnStartGame", true);
        fn_7100fc9084(&g_unk_71729f01d0, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01d0, g_unk_7101a19000);
    {
        CStrId n("OnLoadGame", true);
        fn_7100fc9084(&g_unk_71729f01d8, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01d8, g_unk_7101a19000);
    {
        CStrId n("OnCopyGame", true);
        fn_7100fc9084(&g_unk_71729f01e0, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01e0, g_unk_7101a19000);
    {
        CStrId n("OnDeleteGame", true);
        fn_7100fc9084(&g_unk_71729f01e8, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01e8, g_unk_7101a19000);
    {
        CStrId n("OnEmptySlotActive", true);
        fn_7100fc9084(&g_unk_71729f01f0, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01f0, g_unk_7101a19000);
    {
        CStrId n("OnDataSlotActive", true);
        fn_7100fc9084(&g_unk_71729f01f8, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f01f8, g_unk_7101a19000);
    {
        CStrId n("OnZoom", true);
        fn_7100fc9084(&g_unk_71729f0200, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0200, g_unk_7101a19000);
    {
        CStrId n("OnMove", true);
        fn_7100fc9084(&g_unk_71729f0208, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0208, g_unk_7101a19000);
    {
        CStrId n("OnRecenter", true);
        fn_7100fc9084(&g_unk_71729f0210, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0210, g_unk_7101a19000);
    {
        CStrId n("OnFastMovement", true);
        fn_7100fc9084(&g_unk_71729f0218, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0218, g_unk_7101a19000);
    {
        CStrId n("OnItemSelected", true);
        fn_7100fc9084(&g_unk_71729f0220, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0220, g_unk_7101a19000);
    {
        CStrId n("OnItemRevealed", true);
        fn_7100fc9084(&g_unk_71729f0228, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0228, g_unk_7101a19000);
    {
        CStrId n("OnMapDownload", true);
        fn_7100fc9084(&g_unk_71729f0230, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0230, g_unk_7101a19000);
    {
        CStrId n("OnEmmyZonePresented", true);
        fn_7100fc9084(&g_unk_71729f0238, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0238, g_unk_7101a19000);
    {
        CStrId n("OnTransportInit", true);
        fn_7100fc9084(&g_unk_71729f0240, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0240, g_unk_7101a19000);
    {
        CStrId n("OnTransportDest", true);
        fn_7100fc9084(&g_unk_71729f0248, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0248, g_unk_7101a19000);
    {
        CStrId n("OnTeleportTravelArrow", true);
        fn_7100fc9084(&g_unk_71729f0250, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0250, g_unk_7101a19000);
    {
        CStrId n("OnMarkerPlaced", true);
        fn_7100fc9084(&g_unk_71729f0258, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0258, g_unk_7101a19000);
    {
        CStrId n("OnMarkerRemoved", true);
        fn_7100fc9084(&g_unk_71729f0260, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0260, g_unk_7101a19000);
    {
        CStrId n("OnAllMarkersRemoved", true);
        fn_7100fc9084(&g_unk_71729f0268, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0268, g_unk_7101a19000);
    {
        CStrId n("OnFailure", true);
        fn_7100fc9084(&g_unk_71729f0270, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0270, g_unk_7101a19000);
    {
        CStrId n("OnSuccess", true);
        fn_7100fc9084(&g_unk_71729f0278, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0278, g_unk_7101a19000);
    {
        CStrId n("OnAeionRecharge", true);
        fn_7100fc9084(&g_unk_71729f0280, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0280, g_unk_7101a19000);
    {
        CStrId n("OnAeionFill", true);
        fn_7100fc9084(&g_unk_71729f0288, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0288, g_unk_7101a19000);
    {
        CStrId n("OnHardModeUnlocked", true);
        fn_7100fc9084(&g_unk_71729f0290, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0290, g_unk_7101a19000);
    {
        CStrId n("OnScroll", true);
        fn_7100fc9084(&g_unk_71729f0298, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f0298, g_unk_7101a19000);
    {
        CStrId n("OnRumbleChanged", true);
        fn_7100fc9084(&g_unk_71729f02a0, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f02a0, g_unk_7101a19000);
    {
        CStrId n("OnTankObtained", true);
        fn_7100fc9084(&g_unk_71729f02a8, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f02a8, g_unk_7101a19000);
    {
        CStrId n("OnEnergyRefill", true);
        fn_7100fc9084(&g_unk_71729f02b0, n);
    }
    __cxa_atexit(fn_7100fc91a8, &g_unk_71729f02b0, g_unk_7101a19000);
}

// Copies of fn_7100572360 (src/game_s2/fn_7100572360.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_71008c18b0, fn_7100217bb0.
// Static initialiser of the unit of the EEmmyAlertLevel, EGrabEmmySamusQTEMode.
#include "../game_s7/UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_7100572360 {
    HeaderIds ids;
};
extern UnkStatics_7100572360 g_unk_71727306e0 __attribute__((visibility("hidden")));
extern UnkStatics_7100572360 g_unk_716e599400 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_7172730420;
extern UnkEnumType g_unk_716e599140;
void fn_7100571cc4(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71008c04fc();
void fn_710021701c();
extern char g_unk_7172730538[];
extern char g_unk_716e599258[];
extern UnkConversion g_unk_7172730540;
extern UnkConversion g_unk_716e599260;
extern UnkConversion g_unk_7172730560;
extern UnkConversion g_unk_716e599280;
void fn_71008c1798() __attribute__((visibility("hidden")));
void fn_7100217a94() __attribute__((visibility("hidden")));
void fn_71008c17a4() __attribute__((visibility("hidden")));
void fn_7100217aa0() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_7172730580;
extern UnkEnumType g_unk_716e5992a0;
void fn_7100571f90(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71008c0948();
void fn_7100217648();
extern char g_unk_7172730698[];
extern char g_unk_716e5993b8[];
extern UnkConversion g_unk_71727306a0;
extern UnkConversion g_unk_716e5993c0;
extern UnkConversion g_unk_71727306c0;
extern UnkConversion g_unk_716e5993e0;
void fn_71008c17b0() __attribute__((visibility("hidden")));
void fn_7100217aac() __attribute__((visibility("hidden")));
void fn_71008c17bc() __attribute__((visibility("hidden")));
void fn_7100217ab8() __attribute__((visibility("hidden")));

void fn_71008c07c8(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71008c0af4(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71002174c8(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100217914(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71008c18b0() {
    InitHeaderIds(&g_unk_71727306e0.ids);
    InitEnum(&g_unk_7172730420, "EDamageType", fn_71008c07c8, fn_71008c04fc,
             g_unk_7172730538, &g_unk_7172730540, fn_71008c1798, &g_unk_7172730560, fn_71008c17a4);
    InitEnum(&g_unk_7172730580, "EForcedDamageMode", fn_71008c0af4, fn_71008c0948,
             g_unk_7172730698, &g_unk_71727306a0, fn_71008c17b0, &g_unk_71727306c0, fn_71008c17bc);
}

void fn_7100217bb0() {
    InitHeaderIds(&g_unk_716e599400.ids);
    InitEnum(&g_unk_716e599140, "engine::utils::EMaterialConstantColor", fn_71002174c8, fn_710021701c,
             g_unk_716e599258, &g_unk_716e599260, fn_7100217a94, &g_unk_716e599280, fn_7100217aa0);
    InitEnum(&g_unk_716e5992a0, "engine::utils::ELightPreset", fn_7100217914, fn_7100217648,
             g_unk_716e5993b8, &g_unk_716e5993c0, fn_7100217aac, &g_unk_716e5993e0, fn_7100217ab8);
}


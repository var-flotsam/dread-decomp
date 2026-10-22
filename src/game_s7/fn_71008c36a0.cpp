// Copies of fn_7100359b10 (src/game_s2/fn_7100359b10.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_71008c36a0, fn_710021c330, fn_71009547c0, fn_7100982ae0.
// Static initialiser of the unit of the EAimDirection.
#include "../game_s7/UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_7100359b10 {
    HeaderIds ids;
};
extern UnkStatics_7100359b10 g_unk_717273afa8 __attribute__((visibility("hidden")));
extern UnkStatics_7100359b10 g_unk_717273a268 __attribute__((visibility("hidden")));
extern UnkStatics_7100359b10 g_unk_71727309b8 __attribute__((visibility("hidden")));
extern UnkStatics_7100359b10 g_unk_716e599aa8 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_717273ae48;
extern UnkEnumType g_unk_717273a108;
extern UnkEnumType g_unk_7172730858;
extern UnkEnumType g_unk_716e599948;
void fn_71003598f8(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71009826c4();
void fn_710095410c();
void fn_71008c31c0();
void fn_710021be4c();
extern char g_unk_717273af60[];
extern char g_unk_717273a220[];
extern char g_unk_7172730970[];
extern char g_unk_716e599a60[];
extern UnkConversion g_unk_717273af68;
extern UnkConversion g_unk_717273a228;
extern UnkConversion g_unk_7172730978;
extern UnkConversion g_unk_716e599a68;
extern UnkConversion g_unk_717273af88;
extern UnkConversion g_unk_717273a248;
extern UnkConversion g_unk_7172730998;
extern UnkConversion g_unk_716e599a88;
void fn_7100982a50() __attribute__((visibility("hidden")));
void fn_71009546e4() __attribute__((visibility("hidden")));
void fn_71008c360c() __attribute__((visibility("hidden")));
void fn_710021c298() __attribute__((visibility("hidden")));
void fn_7100982a5c() __attribute__((visibility("hidden")));
void fn_71009546f0() __attribute__((visibility("hidden")));
void fn_71008c3618() __attribute__((visibility("hidden")));
void fn_710021c2a4() __attribute__((visibility("hidden")));

void fn_71008c348c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710021c118(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100954438(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71009828d0(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71008c36a0() {
    InitHeaderIds(&g_unk_71727309b8.ids);
    InitEnum(&g_unk_7172730858, "EDamageStrength", fn_71008c348c, fn_71008c31c0,
             g_unk_7172730970, &g_unk_7172730978, fn_71008c360c, &g_unk_7172730998, fn_71008c3618);
}

void fn_710021c330() {
    InitHeaderIds(&g_unk_716e599aa8.ids);
    InitEnum(&g_unk_716e599948, "media::EMovieStatus", fn_710021c118, fn_710021be4c,
             g_unk_716e599a60, &g_unk_716e599a68, fn_710021c298, &g_unk_716e599a88, fn_710021c2a4);
}

void fn_71009547c0() {
    InitHeaderIds(&g_unk_717273a268.ids);
    InitEnum(&g_unk_717273a108, "EAIAnimationStates", fn_7100954438, fn_710095410c,
             g_unk_717273a220, &g_unk_717273a228, fn_71009546e4, &g_unk_717273a248, fn_71009546f0);
}

void fn_7100982ae0() {
    InitHeaderIds(&g_unk_717273afa8.ids);
    InitEnum(&g_unk_717273ae48, "ESamusUsingGrapplePullPointStatePullStage", fn_71009828d0, fn_71009826c4,
             g_unk_717273af60, &g_unk_717273af68, fn_7100982a50, &g_unk_717273af88, fn_7100982a5c);
}


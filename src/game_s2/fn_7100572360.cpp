// Static initialiser of the unit of the EEmmyAlertLevel, EGrabEmmySamusQTEMode.
#include "../game_s7/UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_7100572360 {
    HeaderIds ids;
};
extern UnkStatics_7100572360 g_unk_71726e3390 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726e30d0;
void fn_7100571cc4(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100571b18();
extern char g_unk_71726e31e8[];
extern UnkConversion g_unk_71726e31f0;
extern UnkConversion g_unk_71726e3210;
void fn_710057223c() __attribute__((visibility("hidden")));
void fn_7100572248() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726e3230;
void fn_7100571f90(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100571e44();
extern char g_unk_71726e3348[];
extern UnkConversion g_unk_71726e3350;
extern UnkConversion g_unk_71726e3370;
void fn_7100572254() __attribute__((visibility("hidden")));
void fn_7100572260() __attribute__((visibility("hidden")));

void fn_7100572360() {
    InitHeaderIds(&g_unk_71726e3390.ids);
    InitEnum(&g_unk_71726e30d0, "EEmmyAlertLevel", fn_7100571cc4, fn_7100571b18,
             g_unk_71726e31e8, &g_unk_71726e31f0, fn_710057223c, &g_unk_71726e3210, fn_7100572248);
    InitEnum(&g_unk_71726e3230, "EGrabEmmySamusQTEMode", fn_7100571f90, fn_7100571e44,
             g_unk_71726e3348, &g_unk_71726e3350, fn_7100572254, &g_unk_71726e3370, fn_7100572260);
}

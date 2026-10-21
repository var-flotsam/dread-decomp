// Static initialiser of the unit of the EFollowPathRotationMode, EFollowInPathOrientation enums.
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_71006f7e70 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_71006f7e70 g_unk_717270cbc8 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_717270c900;
void fn_71006f5aac(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71006f57e0();
extern char g_unk_717270ca18[];
extern UnkConversion g_unk_717270ca20;
extern UnkConversion g_unk_717270ca40;
void fn_71006f7934() __attribute__((visibility("hidden")));
void fn_71006f7940() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_717270ca60;
void fn_71006f5e38(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71006f5c2c();
extern char g_unk_717270cb78[];
extern UnkConversion g_unk_717270cb80;
extern UnkConversion g_unk_717270cba0;
void fn_71006f794c() __attribute__((visibility("hidden")));
void fn_71006f7958() __attribute__((visibility("hidden")));
void* fn_71006f5fbc();  // CCharClassAINavigationComponent::refl_GetClass
extern char g_unk_717270cbc0[];

void fn_71006f7e70() {
    InitHeaderIds(&g_unk_717270cbc8.ids);
    InitEnum(&g_unk_717270c900, "EFollowPathRotationMode", fn_71006f5aac, fn_71006f57e0,
             g_unk_717270ca18, &g_unk_717270ca20, fn_71006f7934, &g_unk_717270ca40, fn_71006f7940);
    InitEnum(&g_unk_717270ca60, "EFollowInPathOrientation", fn_71006f5e38, fn_71006f5c2c,
             g_unk_717270cb78, &g_unk_717270cb80, fn_71006f794c, &g_unk_717270cba0, fn_71006f7958);
    fn_71006f5fbc();
    fn_710009ec00(g_unk_717270cbc0, g_unk_717270cbc8.field_0x20);
}

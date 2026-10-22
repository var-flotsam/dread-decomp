// Static initialiser of SmallExplosion's unit (with the CEscapeSequenceExplosionComponent::EExplosionType enum).
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_71007d6070 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    u8 field_0x38[8];
};
extern UnkStatics_71007d6070 g_unk_717271e2b0 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_717271e138;
void fn_71007d573c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71007d5590();
extern char g_unk_717271e250[];
extern UnkConversion g_unk_717271e258;
extern UnkConversion g_unk_717271e278;
void fn_71007d5f58() __attribute__((visibility("hidden")));
void fn_71007d5f64() __attribute__((visibility("hidden")));
void* fn_71007d58c0();  // CEscapeSequenceExplosionComponent::refl_GetClass
extern char g_unk_717271e298[];

void fn_71007d6070() {
    InitHeaderIds(&g_unk_717271e2b0.ids);
    InitEnum(&g_unk_717271e138, "CEscapeSequenceExplosionComponent::EExplosionType", fn_71007d573c, fn_71007d5590,
             g_unk_717271e250, &g_unk_717271e258, fn_71007d5f58, &g_unk_717271e278, fn_71007d5f64);
    fn_71007d58c0();
    fn_710009ec00(g_unk_717271e298, g_unk_717271e2b0.field_0x38);
    InitClassId(&g_unk_717271e2b0.field_0x20, "SmallExplosion");
    InitClassId(&g_unk_717271e2b0.field_0x28, "MidExplosion");
    InitClassId(&g_unk_717271e2b0.field_0x30, "BigExplosion");
}

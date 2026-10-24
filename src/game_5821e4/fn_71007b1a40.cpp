// Static initialiser of a reflected component's unit.
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_71007b1a40 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_71007b1a40 g_unk_717271a0b0 __attribute__((visibility("hidden")));
extern UnkStrIdBase g_unk_717271a038;
extern UnkProperty g_unk_717271a040;
extern UnkProperty* g_unk_71726db788;
extern void* g_unk_7101a70508[];
void fn_71007b16c0(UnkProperty* p);
void fn_71007afa60(void* p);
void* fn_71007afadc();  // CDemolitionBlockComponent::refl_GetClass
extern char g_unk_717271a068[];

void fn_71007b1a40() {
    InitHeaderIds(&g_unk_717271a0b0.ids);
    InitId(&g_unk_717271a038, "DEMOLITIONBLOCK");
    InitProperty(&g_unk_717271a040, g_unk_7101a70508, g_unk_71726db788, "Used", TypeOf<fn_710000d228>(),
                 fn_71007b16c0, fn_71007afa60);
    fn_71007afadc();
    fn_710009ec00(g_unk_717271a068, g_unk_717271a0b0.field_0x20);
}

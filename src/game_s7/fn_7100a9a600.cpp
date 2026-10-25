// Copies of fn_7100434c90 (src/game_s2/fn_7100434c90.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_7100a9a600.
// Static initialiser of the unit of CTrainUsableComponent and ETrainDirection.
#include "../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

// The original's statics with the class type 0x10 bytes nearer.
struct UnkStatics_7100a9a600 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    u8 pad_0x28[0x8];
    u8 field_0x30[8];
};
extern UnkStatics_7100a9a600 g_unk_7172752d18 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_7172752bb0;
void fn_710042fd24(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100a98a00();
extern char g_unk_7172752cc8[];
extern UnkConversion g_unk_7172752cd0;
extern UnkConversion g_unk_7172752cf0;
void fn_7100a9a480() __attribute__((visibility("hidden")));
void fn_7100a9a48c() __attribute__((visibility("hidden")));
extern char g_unk_7172752d10[];

void fn_7100a98b4c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_7100a9a600() {
    InitHeaderIds(&g_unk_7172752d18.ids);
    InitId(&g_unk_7172752d18.field_0x18, "CTrainUsableComponent");
    InitId(&g_unk_7172752d18.field_0x20, "initopen");
    InitEnum(&g_unk_7172752bb0, "ETrainDirection", fn_7100a98b4c, fn_7100a98a00,
             g_unk_7172752cc8, &g_unk_7172752cd0, fn_7100a9a480, &g_unk_7172752cf0, fn_7100a9a48c);
    fn_7100a98cd0();
    fn_710009ec00(g_unk_7172752d10, g_unk_7172752d18.field_0x30);
}


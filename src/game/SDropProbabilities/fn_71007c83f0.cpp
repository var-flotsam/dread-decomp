// Static initialiser of the unit of the SDropProbabilities::SDir, SDropProbabilities::SType enums.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71007c83f0 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    u8 field_0x20[8];
};
extern UnkStatics_71007c83f0 g_unk_717271bc38 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_717271b970;
void fn_71007c62d4(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71007c5e88();
extern char g_unk_717271ba88[];
extern UnkConversion g_unk_717271ba90;
extern UnkConversion g_unk_717271bab0;
void fn_71007c7e70() __attribute__((visibility("hidden")));
void fn_71007c7e7c() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_717271bad0;
void fn_71007c6600(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71007c6454();
extern char g_unk_717271bbe8[];
extern UnkConversion g_unk_717271bbf0;
extern UnkConversion g_unk_717271bc10;
void fn_71007c7e88() __attribute__((visibility("hidden")));
void fn_71007c7e94() __attribute__((visibility("hidden")));
extern char g_unk_717271bc30[];

void fn_71007c83f0() {
    InitHeaderIds(&g_unk_717271bc38.ids);
    InitEnum(&g_unk_717271b970, "SDropProbabilities::SDir", fn_71007c62d4, fn_71007c5e88,
             g_unk_717271ba88, &g_unk_717271ba90, fn_71007c7e70, &g_unk_717271bab0, fn_71007c7e7c);
    InitEnum(&g_unk_717271bad0, "SDropProbabilities::SType", fn_71007c6600, fn_71007c6454,
             g_unk_717271bbe8, &g_unk_717271bbf0, fn_71007c7e88, &g_unk_717271bc10, fn_71007c7e94);
    fn_71007c6784();
    fn_710009ec00(g_unk_717271bc30, g_unk_717271bc38.field_0x20);
}

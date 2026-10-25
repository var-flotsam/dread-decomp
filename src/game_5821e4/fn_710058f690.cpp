// Static initialiser of the unit of the CGoliathAttack::EGoliathAttackState enum.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_710058f690 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    u8 field_0x38[8];
};
extern UnkStatics_710058f690 g_unk_71726e62c0 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_71726e6158;
void fn_710058df44(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710058dd38();
extern char g_unk_71726e6270[];
extern UnkConversion g_unk_71726e6278;
extern UnkConversion g_unk_71726e6298;
void fn_710058f314() __attribute__((visibility("hidden")));
void fn_710058f320() __attribute__((visibility("hidden")));
extern char g_unk_71726e62b8[];

void fn_710058f690() {
    InitHeaderIds(&g_unk_71726e62c0.ids);
    InitEnum(&g_unk_71726e6158, "CGoliathAttack::EGoliathAttackState", fn_710058df44, fn_710058dd38,
             g_unk_71726e6270, &g_unk_71726e6278, fn_710058f314, &g_unk_71726e6298, fn_710058f320);
    fn_710058e0c8();
    fn_710009ec00(g_unk_71726e62b8, g_unk_71726e62c0.field_0x38);
    InitId(&g_unk_71726e62c0.field_0x20, "attack_init");
    InitId(&g_unk_71726e62c0.field_0x28, "attack_init_nearwall");
    InitId(&g_unk_71726e62c0.field_0x30, "floorshockwave");
}

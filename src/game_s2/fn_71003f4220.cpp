// Static initialiser of the unit of the CBigkranXSpitAttack::EState.
#include "../game_s7/UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_71003f4220 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    u8 field_0x30[8];
};
extern UnkStatics_71003f4220 g_unk_71726c2868 __attribute__((visibility("hidden")));
void* fn_71003f0df4();  // CBigkranXSpitAttack::refl_GetClass
extern char g_unk_71726c26f0[];
extern UnkEnumType g_unk_71726c26f8;
void fn_71003f127c(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71003f1070();
extern char g_unk_71726c2810[];
extern UnkConversion g_unk_71726c2818;
extern UnkConversion g_unk_71726c2838;
void fn_71003f3ae8() __attribute__((visibility("hidden")));
void fn_71003f3af4() __attribute__((visibility("hidden")));
struct UnkWord12 {
    u64 field_0x0;
    u32 field_0x8;
};
extern const UnkWord12 g_unk_710164779c;
extern UnkWord12 g_unk_71726c2858;

void fn_71003f4220() {
    InitHeaderIds(&g_unk_71726c2868.ids);
    fn_71003f0df4();
    fn_710009ec00(g_unk_71726c26f0, g_unk_71726c2868.field_0x30);
    InitEnum(&g_unk_71726c26f8, "CBigkranXSpitAttack::EState", fn_71003f127c, fn_71003f1070,
             g_unk_71726c2810, &g_unk_71726c2818, fn_71003f3ae8, &g_unk_71726c2838, fn_71003f3af4);
    InitId(&g_unk_71726c2868.field_0x20, "attack_magmarain_attack-init");
    InitId(&g_unk_71726c2868.field_0x28, "attack_magmarain_attack-init_fromrelax");
    u64 a = g_unk_710164779c.field_0x0;
    u32 b = g_unk_710164779c.field_0x8;
    g_unk_71726c2858.field_0x0 = a;
    g_unk_71726c2858.field_0x8 = b;
}

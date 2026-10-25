// Static initialiser of the unit of the CSluggerSpitAttack::EState enum.
#include "UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_71006c4230 {
    HeaderIds ids;
    u8 pad_0x18[0x8];
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    UnkStrIdBase field_0x40;
    UnkStrIdBase field_0x48;
    UnkStrIdBase field_0x50;
    u8 field_0x58[8];
};
extern UnkStatics_71006c4230 g_unk_7172707ba0 __attribute__((visibility("hidden")));
extern char g_unk_7172707a28[];
extern UnkEnumType g_unk_7172707a30;
void fn_71006c06bc(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_71006c0450();
extern char g_unk_7172707b48[];
extern UnkConversion g_unk_7172707b50;
extern UnkConversion g_unk_7172707b70;
void fn_71006c3b84() __attribute__((visibility("hidden")));
void fn_71006c3b90() __attribute__((visibility("hidden")));
struct UnkVec3 {
    f32 x;
    f32 y;
    f32 z;
};
extern const UnkVec3 g_unk_710164779c;
extern UnkVec3 g_unk_7172707b90;

void fn_71006c4230() {
    InitHeaderIds(&g_unk_7172707ba0.ids);
    fn_71006c01d4();
    fn_710009ec00(g_unk_7172707a28, g_unk_7172707ba0.field_0x58);
    InitEnum(&g_unk_7172707a30, "CSluggerSpitAttack::EState", fn_71006c06bc, fn_71006c0450,
             g_unk_7172707b48, &g_unk_7172707b50, fn_71006c3b84, &g_unk_7172707b70, fn_71006c3b90);
    InitId(&g_unk_7172707ba0.field_0x20, "fAngle");
    InitId(&g_unk_7172707ba0.field_0x28, "relax");
    InitId(&g_unk_7172707ba0.field_0x30, "attack_acidball_fromwalk_init");
    InitId(&g_unk_7172707ba0.field_0x38, "attack_acidball_init");
    InitId(&g_unk_7172707ba0.field_0x40, "attack_acidball_fromwalk_turn_init");
    InitId(&g_unk_7172707ba0.field_0x48, "attack_acidball_turn_init");
    InitId(&g_unk_7172707ba0.field_0x50, "attack_acidball");
    g_unk_7172707b90 = g_unk_710164779c;
}

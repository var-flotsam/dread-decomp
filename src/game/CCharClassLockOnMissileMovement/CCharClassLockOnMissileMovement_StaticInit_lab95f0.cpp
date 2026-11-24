// Static initialiser of CCharClassLockOnMissileMovement's unit: the header ids, the three classes'
// registrations, the movement's tunable, the CLockOnMissileMovement::SSubState enum and the trail
// material's path. Names and layouts are placeholders.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkStatics_7100ab95f0 {
    HeaderIds ids;
    u8 pad_0x18[0x18];
    UnkFilePathStrIdBase trail_material;  // 0x30
    u8 pad_0x38[0x10];
    u8 field_0x48[8];
    u8 pad_0x50[0x130];
    u8 field_0x180[8];
    u8 pad_0x188[0x130];
    u8 field_0x2b8[8];
};
static_assert(__builtin_offsetof(UnkStatics_7100ab95f0, field_0x2b8) == 0x2b8, "layout");
extern UnkStatics_7100ab95f0 g_unk_7172754a38 __attribute__((visibility("hidden")));

extern char g_unk_7172754860[];
extern char g_unk_7172754861[];
extern char g_unk_71727548d1[];

// CLockOnMissileMovement's tunable: three words cleared after the base constructor.
struct UnkLockOnMissileTunable : UnkTunable {
    u8 pad_0x8[0x1c];
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
};
static_assert(__builtin_offsetof(UnkLockOnMissileTunable, field_0x2c) == 0x2c, "layout");
extern UnkTunable g_unk_7172754868;
extern void* g_unk_7101a95e50[];

extern UnkEnumType g_unk_71727548d8;
void fn_7100ab5fdc(void* e);
void fn_7100ab6188(UnkEnumType* type, const CStrId& name, void (*factory)(void*), void* arg);
extern char g_unk_71727549f0[];
extern UnkConversion g_unk_71727549f8;
extern UnkConversion g_unk_7172754a18;
void fn_7100ab8dfc() __attribute__((visibility("hidden")));
void fn_7100ab8e08() __attribute__((visibility("hidden")));

void CCharClassLockOnMissileMovement_InitStatics() {
    InitHeaderIds(&g_unk_7172754a38.ids);
    fn_7100ab4bc0();
    fn_710009ec00(g_unk_7172754860, g_unk_7172754a38.field_0x48);
    fn_7100ab4e40();
    fn_710009ec00(g_unk_7172754861, g_unk_7172754a38.field_0x180);
    {
        UnkLockOnMissileTunable* tunable = static_cast<UnkLockOnMissileTunable*>(&g_unk_7172754868);
        fn_7100136ecc(tunable);
        tunable->vtable = g_unk_7101a95e50 + 2;
        tunable->field_0x24 = 0;
        tunable->field_0x28 = 0;
        tunable->field_0x2c = 0;
        fn_7100000250(fn_7100075104, tunable, g_unk_7101a19000);
    }
    fn_7100ab5d80();
    fn_710009ec00(g_unk_71727548d1, g_unk_7172754a38.field_0x2b8);
    InitEnum(&g_unk_71727548d8, "CLockOnMissileMovement::SSubState", fn_7100ab6188, fn_7100ab5fdc,
             g_unk_71727549f0, &g_unk_71727549f8, fn_7100ab8dfc, &g_unk_7172754a18, fn_7100ab8e08);
    InitPathId(&g_unk_7172754a38.trail_material,
               "actors/weapons/lockonmissile/fx/imats/lockon_missile_trail_material.bsmat");
}

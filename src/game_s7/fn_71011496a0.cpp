// Static initialiser of a reflected unit: the header ids, then the unit's own statics and
// registrations (helpers in UnkStaticInitS7.h). Layouts are placeholders.
#include "UnkStaticInitS7.h"

using namespace unk_static_init;

struct UnkStatics_71011496a0 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    UnkStrIdBase field_0x40;
    UnkStrIdBase field_0x48;
    UnkStrIdBase field_0x50;
    UnkStrIdBase field_0x58;
    UnkStrIdBase field_0x60;
    UnkStrIdBase field_0x68;
    UnkStrIdBase field_0x70;
    UnkStrIdBase field_0x78;
    UnkStrIdBase field_0x80;
    UnkStrIdBase field_0x88;
    UnkStrIdBase field_0x90;
    UnkStrIdBase field_0x98;
    UnkStrIdBase field_0xa0;
    UnkStrIdBase field_0xa8;
    UnkStrIdBase field_0xb0;
    UnkStrIdBase field_0xb8;
    UnkStrIdBase field_0xc0;
    UnkStrIdBase field_0xc8;
    UnkStrIdBase field_0xd0;
    UnkStrIdBase field_0xd8;
    UnkStrIdBase field_0xe0;
    UnkStrIdBase field_0xe8;
    UnkStrIdBase field_0xf0;
    UnkStrIdBase field_0xf8;
    UnkStrIdBase field_0x100;
    UnkStrIdBase field_0x108;
    UnkStrIdBase field_0x110;
    UnkStrIdBase field_0x118;
    UnkStrIdBase field_0x120;
    UnkStrIdBase field_0x128;
    UnkStrIdBase field_0x130;
    UnkStrIdBase field_0x138;
    UnkStrIdBase field_0x140;
    UnkStrIdBase field_0x148;
    UnkStrIdBase field_0x150;
    UnkStrIdBase field_0x158;
    UnkStrIdBase field_0x160;
    UnkStrIdBase field_0x168;
    UnkStrIdBase field_0x170;
    u8 pad_0x178[0x10];
    u8 field_0x188[8];
    u8 pad_0x190[0x130];
    u8 field_0x2c0[8];
};
extern UnkStatics_71011496a0 g_unk_7172a09298 __attribute__((visibility("hidden")));
inline const CStrId& StaticName_71727b85b0() {
    static CStrId s("INVENTORY", true);
    return s;
}
void* fn_710113f97c();
extern char g_unk_7172a09260[];
extern UnkTunable g_unk_7172a09268;
void fn_7100136ecc(UnkTunable* self);  // the base constructor
extern void* g_unk_7101cb3540[];
void fn_7100075104(void* self);  // its destructor, as registered
void* fn_71011408c4();
extern char g_unk_7172a09291[];

void fn_71011496a0() {
    InitHeaderIds(&g_unk_7172a09298.ids);
    InitClassIdFrom(&g_unk_7172a09298.field_0x18, StaticName_71727b85b0());
    InitId(&g_unk_7172a09298.field_0x20, "ITEM_VARIA_SUIT");
    InitId(&g_unk_7172a09298.field_0x28, "ITEM_GRAVITY_SUIT");
    InitId(&g_unk_7172a09298.field_0x30, "ITEM_HYPER_SUIT");
    InitId(&g_unk_7172a09298.field_0x38, "ITEM_WEAPON_CHARGE_BEAM");
    InitId(&g_unk_7172a09298.field_0x40, "ITEM_WEAPON_WIDE_BEAM");
    InitId(&g_unk_7172a09298.field_0x48, "ITEM_WEAPON_BOMB");
    InitId(&g_unk_7172a09298.field_0x50, "ITEM_WEAPON_DIFFUSION_BEAM");
    InitId(&g_unk_7172a09298.field_0x58, "ITEM_WEAPON_GRAPPLE_BEAM");
    InitId(&g_unk_7172a09298.field_0x60, "ITEM_WEAPON_WAVE_BEAM");
    InitId(&g_unk_7172a09298.field_0x68, "ITEM_WEAPON_PLASMA_BEAM");
    InitId(&g_unk_7172a09298.field_0x70, "ITEM_WEAPON_HYPER_BEAM");
    InitId(&g_unk_7172a09298.field_0x78, "ITEM_WEAPON_SUPER_MISSILE");
    InitId(&g_unk_7172a09298.field_0x80, "ITEM_WEAPON_ICE_MISSILE");
    InitId(&g_unk_7172a09298.field_0x88, "ITEM_WEAPON_LINE_BOMB");
    InitId(&g_unk_7172a09298.field_0x90, "ITEM_WEAPON_POWER_BOMB");
    InitId(&g_unk_7172a09298.field_0x98, "ITEM_DOUBLE_JUMP");
    InitId(&g_unk_7172a09298.field_0xa0, "ITEM_MORPH_BALL");
    InitId(&g_unk_7172a09298.field_0xa8, "ITEM_SCREW_ATTACK");
    InitId(&g_unk_7172a09298.field_0xb0, "ITEM_SPACE_JUMP");
    InitId(&g_unk_7172a09298.field_0xb8, "ITEM_MAGNET_GLOVE");
    InitId(&g_unk_7172a09298.field_0xc0, "ITEM_OPTIC_CAMOUFLAGE");
    InitId(&g_unk_7172a09298.field_0xc8, "ITEM_SPEED_BOOSTER");
    InitId(&g_unk_7172a09298.field_0xd0, "ITEM_GHOST_AURA");
    InitId(&g_unk_7172a09298.field_0xd8, "ITEM_METROIDNIZATION");
    InitId(&g_unk_7172a09298.field_0xe0, "ITEM_SONAR");
    InitId(&g_unk_7172a09298.field_0xe8, "ITEM_MULTILOCKON");
    InitId(&g_unk_7172a09298.field_0xf0, "ObtainedItemTypeID");
    InitId(&g_unk_7172a09298.field_0xf8, "MaxItemsOfThisType");
    InitId(&g_unk_7172a09298.field_0x100, "ObtainedItemID");
    InitId(&g_unk_7172a09298.field_0x108, "ObtainedAbilityID");
    InitId(&g_unk_7172a09298.field_0x110, "ITEM_WEAPON_MISSILE_MAX");
    InitId(&g_unk_7172a09298.field_0x118, "ITEM_WEAPON_POWER_BOMB_MAX");
    InitId(&g_unk_7172a09298.field_0x120, "ITEM_MAX_LIFE");
    InitId(&g_unk_7172a09298.field_0x128, "ITEM_LIFE_SHARDS");
    InitId(&g_unk_7172a09298.field_0x130, "item");
    InitId(&g_unk_7172a09298.field_0x138, "Event log when item is obtained");
    InitId(&g_unk_7172a09298.field_0x140, "ability");
    InitId(&g_unk_7172a09298.field_0x148, "Event log when ability is obtained");
    InitId(&g_unk_7172a09298.field_0x150, "item_complete");
    InitId(&g_unk_7172a09298.field_0x158, "Event log when all items are obtained");
    InitId(&g_unk_7172a09298.field_0x160, "Energy");
    InitId(&g_unk_7172a09298.field_0x168, "Missile");
    InitId(&g_unk_7172a09298.field_0x170, "PowerBombs");
    fn_710113f97c();
    fn_710009ec00(g_unk_7172a09260, g_unk_7172a09298.field_0x188);
    InitTunable(&g_unk_7172a09268, fn_7100136ecc, g_unk_7101cb3540, fn_7100075104);
    fn_71011408c4();
    fn_710009ec00(g_unk_7172a09291, g_unk_7172a09298.field_0x2c0);
}

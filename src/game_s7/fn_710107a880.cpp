// lua_bind::GainAllItemsAndPowerUps (name from the binding's registration string): refills two
// values on the player, then gives every item and power-up to the player's inventory.
// Layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"

using base::global::CStrId;

struct UnkIdSource {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual const CStrId& vf_0x20();
    virtual const CStrId& vf_0x28();
};

struct UnkQualifiedId {
    CStrId id;
    UnkIdSource* source;
};
void fn_7100ae9a10(UnkQualifiedId* self, UnkIdSource* source, const CStrId& prefix);

// An item definition: its qualified id is built on first use.
struct UnkItemDef : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedId qid;
};
extern const CStrId g_unk_7101d470e0;  // the qualified ids' prefix

// Compared as integers, as in fn_7100ae9a10.
static inline const UnkQualifiedId& GetQualifiedId(UnkItemDef& def) {
    if ((u64)def.qid.id.data == (u64)g_unk_7101d47108 + 0x10)
        fn_7100ae9a10(&def.qid, &def, g_unk_7101d470e0);
    return def.qid;
}

extern UnkItemDef g_unk_717275a058;
extern UnkItemDef g_unk_717275a098;
extern UnkItemDef g_unk_717275a118;
extern UnkItemDef g_unk_717275a158;
extern UnkItemDef g_unk_717275a198;
extern UnkItemDef g_unk_717275a1d8;
extern UnkItemDef g_unk_717275a218;
extern UnkItemDef g_unk_717275a258;
extern UnkItemDef g_unk_717275a298;
extern UnkItemDef g_unk_717275a418;
extern UnkItemDef g_unk_717275a458;
extern UnkItemDef g_unk_717275a498;
extern UnkItemDef g_unk_717275a4d8;
extern UnkItemDef g_unk_717275a518;
extern UnkItemDef g_unk_717275a558;
extern UnkItemDef g_unk_717275a598;
extern UnkItemDef g_unk_717275a5d8;
extern UnkItemDef g_unk_717275a618;
extern UnkItemDef g_unk_717275a658;
extern UnkItemDef g_unk_717275a698;
extern UnkItemDef g_unk_717275a6d8;
extern UnkItemDef g_unk_717275a758;
extern UnkItemDef g_unk_717275a798;
extern UnkItemDef g_unk_717275a7d8;
extern UnkItemDef g_unk_717275a858;
extern UnkItemDef g_unk_717275a898;
extern UnkItemDef g_unk_717275a8d8;
extern UnkItemDef g_unk_717275a918;
extern UnkItemDef g_unk_717275a958;
extern UnkItemDef g_unk_717275a998;
extern UnkItemDef g_unk_717275a9d8;
extern UnkItemDef g_unk_717275aa18;
extern UnkItemDef g_unk_717275aa58;
extern UnkItemDef g_unk_717275aa98;
extern UnkItemDef g_unk_717275aad8;
extern UnkItemDef g_unk_717275ab58;
extern UnkItemDef g_unk_717275ab98;

struct UnkInventory;
void fn_7101142a28(UnkInventory* inventory, const UnkQualifiedId& item, bool a, bool b, bool c, f32 amount,
                   f32 extra);

struct UnkLifeComponent;
void fn_7100a1f998(UnkLifeComponent* life, f32 max);
void fn_7100a1fa98(UnkLifeComponent* life, bool a, bool b, f32 value);
UnkLifeComponent* fn_7100303054();

struct UnkAmmo {
    void** vtable;
    void vf_0x168(f32 value, bool flag) {
        reinterpret_cast<void (*)(UnkAmmo*, f32, bool)>(vtable[0x168 / 8])(this, value, flag);
    }
    void vf_0x170(f32 value) { reinterpret_cast<void (*)(UnkAmmo*, f32)>(vtable[0x170 / 8])(this, value); }
};

struct UnkPlayer {
    u8 unk_0x0[0x1b8];
    UnkAmmo* ammo;
    u8 unk_0x1c0[0xb0];
    UnkInventory* inventory;
};
UnkPlayer* fn_7100dec788(UnkGameManager* manager);
void fn_7100dffd48(UnkGameManager* manager);

int fn_710107a880(void* /*lua_State*/) {
    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    if (!player)
        return 0;
    if (UnkLifeComponent* life = fn_7100303054()) {
        fn_7100a1f998(life, 2200.0f);
        fn_7100a1fa98(life, false, true, 2200.0f);
    }
    if (UnkAmmo* ammo = player->ammo) {
        ammo->vf_0x170(1099.0f);
        ammo->vf_0x168(1099.0f, true);
    }
    UnkInventory* inventory = player->inventory;
    if (!inventory)
        return 0;
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a058), true, true, true, 799.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a118), true, true, true, 799.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a098), true, true, true, 0.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a158), true, true, true, 1000.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a198), true, true, true, 1000.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a1d8), true, true, true, 150.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a218), true, true, true, 150.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a258), true, true, true, 10.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a298), true, true, true, 10.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a418), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a458), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a498), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a4d8), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a518), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a558), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a598), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a5d8), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a618), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a658), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a698), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a6d8), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a758), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a798), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a7d8), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275ab98), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a858), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a898), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a8d8), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a918), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a958), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a998), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275a9d8), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275aa18), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275aa58), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275aa98), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275aad8), true, true, true, 1.0f, 0.0f);
    fn_7101142a28(inventory, GetQualifiedId(g_unk_717275ab58), true, true, true, 1.0f, 0.0f);
    fn_7100dffd48(g_unk_7101d47310);
    return 0;
}

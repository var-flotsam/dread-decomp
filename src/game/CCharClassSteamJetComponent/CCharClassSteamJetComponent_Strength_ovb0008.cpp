// CCharClassSteamJetComponent's unit (tools/tu_map.py, unit 1350): the steam jet's constructor
// (fn_710089dfe4: strength 500, list at +0xc10, the "Steam Jet" name at +0xb78), whether it hurts
// the player (fn_710089f7a8: while active and the description says so, only without the varia or
// gravity suit), and the two places that hand the jet's sound target (+0x2b8 of the owner) a volume
// by strength band on every channel (fn_710071a398): setting the strength (fn_710089f238) and the
// restart (fn_710089eed0). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CAudioComponent.h"
#include "game/CCharClassBaseDamageTriggerComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkBaseDamageTrigger3;
void fn_7100a45a30(UnkBaseDamageTrigger3* self);
struct UnkLiquidPool_b44a2;
struct UnkPlayer;
struct UnkInventory;
bool fn_71011428bc(UnkInventory* inventory, const UnkQualifiedId& item);

extern char g_unk_7101a7ccb0[];  // vtable

// An allocator-backed list (as src/game/CAIComponent/UnkAIDeviceList.cpp): the base vtable is
// stored first, then the final one.
struct UnkJetList_ovb0008 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;
    bool field_0x1c;
    u32 field_0x20;
    base::UnkAllocator* field_0x28;
    u32 field_0x30;
};

struct UnkSteamJetOwner_ovb0008 {
    u8 pad_0x0[0x2b8];
    void* sound;  // 0x2b8
};

struct UnkSteamJetDesc_ovb0008 {
    u8 pad_0x0[0x98];
    bool hurts;  // 0x98
};

struct UnkSteamJet_ovb0008 {
    void* vtable;
    u8 pad_0x8[0x28];
    UnkSteamJetOwner_ovb0008* owner;  // 0x30
    u8 pad_0x38[0xab1];
    bool field_0xae9;
    u8 pad_0xaea[0x8e];
    UnkStrId name;  // 0xb78
    u8 pad_0xb80[0x40];
    u32 field_0xbc0;
    u8 pad_0xbc4[0x20];
    f32 strength;  // 0xbe4
    f32 field_0xbe8;
    f32 field_0xbec;
    f32 field_0xbf0;
    u32 field_0xbf4;
    u32 field_0xbf8;
    u8 active;  // 0xbfc
    bool field_0xbfd;
    bool field_0xbfe;
    u8 pad_0xbff;
    f32 field_0xc00;
    f32 field_0xc04;
    bool field_0xc08;
    u8 pad_0xc09[7];
    UnkJetList_ovb0008 list;  // 0xc10
    u64 field_0xc48;
    void* field_0xc50;
    void* field_0xc58;
    void* field_0xc60;
    void* field_0xc68;
    f32 field_0xc70;
    f32 field_0xc74;
    UnkSteamJetDesc_ovb0008* desc;  // 0xc78
    u64 field_0xc80;
    f32 field_0xc88;
    u16 field_0xc8c;
    u8 pad_0xc8e[2];
    u64 field_0xc90;
    bool field_0xc98;
};
static_assert(__builtin_offsetof(UnkSteamJet_ovb0008, field_0xc98) == 0xc98, "UnkSteamJet_ovb0008 layout");

void fn_710089f390(UnkSteamJet_ovb0008* self);
void fn_710089ebd0(UnkSteamJet_ovb0008* self);
void fn_710089ed88(UnkSteamJet_ovb0008* self, bool on);

void fn_710089dfe4(UnkSteamJet_ovb0008* self) {
    fn_7100a45a30(reinterpret_cast<UnkBaseDamageTrigger3*>(self));
    self->vtable = g_unk_7101a7ccb0 + 0x10;
    self->strength = 500.0f;
    self->field_0xbe8 = 100.0f;
    self->field_0xbec = 1.0f;
    self->field_0xbf0 = 1.0f;
    self->field_0xbf4 = 0;
    self->active = 0;
    self->field_0xbfd = false;
    self->field_0xbfe = false;
    self->field_0xc00 = 0.0f;
    self->field_0xc04 = 1.0f;
    self->field_0xc08 = false;
    UnkJetList_ovb0008& list = self->list;
    list.vtable = g_unk_7101a20998 + 2;
    list.field_0x8 = 0;
    list.field_0x10 = 0;
    list.field_0x18 = 10;
    list.field_0x1c = true;
    list.field_0x20 = 0xf;
    list.field_0x28 = fn_7100083990();
    list.field_0x30 = 0;
    list.vtable = g_unk_7101a20950 + 2;
    self->field_0xc48 = 0;
    self->field_0xc50 = nullptr;
    self->field_0xc58 = nullptr;
    self->field_0xc60 = nullptr;
    self->field_0xc68 = nullptr;
    self->field_0xc70 = 1.0f;
    self->field_0xc74 = 1.0f;
    self->name = UnkStrId("Steam Jet", true);
    self->field_0xc80 = 0;
    self->field_0xc88 = 2.0f;
    self->field_0xc8c = 0;
    self->field_0xc90 = 0;
    self->field_0xc98 = false;
    self->field_0xae9 = true;
}

struct UnkSuitPlayer_ovb0008 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;  // 0x270
};

// An item's qualified id, built on first use.
static inline const UnkQualifiedId& ItemId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

bool fn_710089f7a8(UnkSteamJet_ovb0008* self) {
    u8 active = self->active;
    bool result = active;
    if (self->desc && self->desc->hurts) {
        auto* player = reinterpret_cast<UnkSuitPlayer_ovb0008*>(fn_7100dec788(g_unk_7101d47310));
        if (player && player->inventory) {
            UnkInventory* inventory = player->inventory;
            bool suited = fn_71011428bc(inventory, ItemId(g_unk_717275a498)) ||  // ITEM_VARIA_SUIT
                          fn_71011428bc(inventory, ItemId(g_unk_717275a458));    // ITEM_GRAVITY_SUIT
            result = (!suited & active) != 0;
        }
    }
    return result;
}

// A channel mask with no channels set (its constructor is not a constant expression).
struct UnkChannelMask_ovb0008 {
    u32 bits;
    UnkChannelMask_ovb0008() { bits = 0; }
};

inline const UnkChannelMask_ovb0008& NoChannels_ovb0008() {
    static UnkChannelMask_ovb0008 s_none;
    return s_none;
}

// Every channel (the low 11 bits).
inline u32 AllChannels_ovb0008() {
    static const u32 s_all = ~NoChannels_ovb0008().bits & 0x7ff;
    return s_all;
}

void fn_710089f238(UnkSteamJet_ovb0008* self, f32 strength) {
    self->strength = strength;
    fn_710089f390(self);
    if (fn_7100deabe4(g_unk_7101d47310))
        fn_710089ebd0(self);
    if (!self->owner || !self->owner->sound)
        return;
    void* target = self->owner->sound;
    f32 value = self->strength;
    f32 volume;
    if (value <= 200.0f)
        volume = 0.5f;
    else
        volume = value > 200.0f && value < 400.0f ? 0.7f : 1.0f;
    alignas(8) u32 mask = AllChannels_ovb0008();
    fn_710071a398(target, &mask, volume, 0.0f);
}

void fn_710089eed0(UnkSteamJet_ovb0008* self) {
    fn_7100a45cac(reinterpret_cast<UnkLiquidPool_b44a2*>(self));
    self->field_0xbf8 = 0;
    self->field_0xbc0 = self->field_0xbf4;
    fn_710089ed88(self, false);
    self->field_0xbfd = false;
    self->field_0xbfe = false;
    self->field_0xc00 = 0.0f;
    if (!self->owner || !self->owner->sound)
        return;
    void* target = self->owner->sound;
    f32 value = self->strength;
    f32 volume;
    if (value <= 200.0f)
        volume = 0.4f;
    else
        volume = value > 200.0f && value < 400.0f ? 0.5f : 0.7f;
    alignas(8) u32 mask = AllChannels_ovb0008();
    fn_710071a398(target, &mask, volume, 0.0f);
}

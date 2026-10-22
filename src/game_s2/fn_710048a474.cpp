// Per-frame update of a boss fight timer: advances the timer at 0x138 by the frame time, lets the
// Chozo Warrior X AI component fn_710048fc80 run when the owner's attack is a let-boss-die track
// and its object at +0x240 answers true to vtable slot 0x2d8, then, for the player's ultimate
// grab energy recharge attack, retunes the player's object at +0x1c0: when its two floats at
// 0x50 leave a positive amount to cover, the vtable slot 0x168 is called with the rate.
#include "types.h"

struct UnkObject {
    void** vtable;
    const void* vf_0x0() { return reinterpret_cast<const void* (*)(UnkObject*)>(vtable[0])(this); }
    bool vf_0x2d8() { return reinterpret_cast<bool (*)(UnkObject*)>(vtable[0x5b])(this); }
};

struct UnkPlayerObject {
    void** vtable;
    u8 pad_0x8[0x48];
    f32 field_0x50;
    f32 field_0x54;
    void vf_0x168(u32 mode, f32 rate) {
        reinterpret_cast<void (*)(UnkPlayerObject*, u32, f32)>(vtable[0x2d])(this, mode, rate);
    }
};

struct UnkAttack;

struct UnkFound {
    u8 pad_0x0[0x14];
    f32 field_0x14;
};

struct UnkQuery {
    void* class_ptr;
    UnkFound* found;
    f32 field_0x10;
};

struct UnkPlayer {
    u8 pad_0x0[0x1c0];
    UnkPlayerObject* field_0x1c0;
    u8 pad_0x1c8[0x58];
    UnkAttack* field_0x220;
};

struct UnkOwner {
    u8 pad_0x0[0x220];
    UnkAttack* field_0x220;
    u8 pad_0x228[0x18];
    UnkObject* field_0x240;
};

struct UnkSelf {
    u8 pad_0x0[8];
    UnkOwner* field_0x8;
    u8 pad_0x10[0x128];
    f32 field_0x138;
};

struct UnkManager;
extern UnkManager* g_unk_7101d47310;
void* fn_7100c3aaa8();  // CCharClassLetBossDieTrack::refl_GetClass
void* fn_710048db44();  // CChozoWarriorXAIComponent::refl_GetClass
void* fn_7100c63ac8();  // CCharClassUltimateGrabEnergyRecharge::refl_GetClass
bool fn_710117b348(UnkAttack* object, void* class_ptr);
bool fn_710009d50c(const void* from, void* to);
void fn_710048fc80(UnkObject* component);
UnkPlayer* fn_7100dec788(UnkManager* manager);
void fn_710117b08c(UnkAttack* attack, UnkQuery* query);
f32 fn_7101177ea0(UnkAttack* attack, u64 hash);

void fn_710048a474(UnkSelf* self, f32 delta) {
    f32 previous = self->field_0x138;
    f32 timer = previous + delta;
    UnkOwner* owner = self->field_0x8;
    self->field_0x138 = timer;
    if (fn_710117b348(owner->field_0x220, fn_7100c3aaa8())) {
        if (self->field_0x8->field_0x240->vf_0x2d8()) {
            UnkObject* object = self->field_0x8->field_0x240;
            UnkObject* component = nullptr;
            if (object && fn_710009d50c(object->vf_0x0(), fn_710048db44()))
                component = object;
            fn_710048fc80(component);
        }
    }
    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    if (!player)
        return;
    UnkAttack* attack = player->field_0x220;
    if (!attack)
        return;
    if (!fn_710117b348(attack, fn_7100c63ac8()))
        return;
    UnkQuery query = {fn_7100c63ac8(), nullptr, -1.0f};
    fn_710117b08c(attack, &query);
    if (!query.found)
        return;
    f32 limit = query.found->field_0x14;
    f32 elapsed = fn_7101177ea0(attack, 0xf4061ff25a2163d6ull);
    player = fn_7100dec788(g_unk_7101d47310);
    if (!player)
        return;
    UnkPlayerObject* object = player->field_0x1c0;
    if (!object)
        return;
    f32 scaled = 0.7f * object->field_0x50;
    f32 current = object->field_0x54;
    f32 remaining = scaled - current;
    if (remaining > 0.0f) {
        f32 span = limit - elapsed;
        f32 rate = scaled;
        if (span > 0.0f)
            rate = current + remaining / span;
        object->vf_0x168(1, rate);
    }
}

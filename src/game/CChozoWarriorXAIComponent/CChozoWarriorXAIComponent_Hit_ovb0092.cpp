// CChozoWarriorXAIComponent's unit (tools/tu_map.py, unit 721): two uses of the lazily created manager
// g_unk_71726af468. A hit (fn_710048f2f4): unless the manager says otherwise (fn_7100da9254), passes it a
// value chosen by the hit's flags (+0xb4) through fn_7100da929c, as CChozoCommanderAIComponent's
// fn_71004530fc does. After the base step (fn_71004990ac), while the manager's value (fn_7100da9260) is
// set (fn_710048f3c4): unless the player (+0x278 of the owner) is in an ultimate grab attack, runs
// fn_710048ecbc, the player's slot 0x178 and fn_7101134420 with the entry at +0x14dd0; then
// fn_7100da928c. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAIAttackComponent.h"
#include "game/UnkManager_71726af468.h"
#include "types.h"

using base::reflection::UnkReflObject;

bool fn_7100da9254(UnkManager_71726af468* self);
void fn_7100da929c(UnkManager_71726af468* self, f32 value);
f32 fn_7100da9260(UnkManager_71726af468* self);
void fn_7100da928c(UnkManager_71726af468*);
struct UnkWarriorXAI_ovb0092;
void fn_71004990ac(UnkWarriorXAI_ovb0092* self);
struct UnkSelf;
void fn_710048ecbc(UnkSelf* self);
struct UnkCommanderPlayer_b67a2;

static inline UnkManager_71726af468* Manager_ovb0092() {
    UnkManager_71726af468* m = g_unk_71726af468;
    if (!m) {
        m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return m;
}

struct UnkWarriorXHit_ovb0092 {
    u8 pad_0x0[0xb4];
    u16 flags;  // 0xb4
};

struct UnkWarriorXPlayer_ovb0092 {
    virtual void vf_0x0();
    void vf_0x178() {
        reinterpret_cast<void (*)(UnkWarriorXPlayer_ovb0092*)>((*reinterpret_cast<void***>(this))[0x178 / 8])(this);
    }
    u8 pad_0x8[0x198];
    UnkReflObject* attack;  // 0x1a0
};

struct UnkWarriorXOwner_ovb0092 {
    u8 pad_0x0[0x278];
    UnkWarriorXPlayer_ovb0092* player;  // 0x278
};

struct UnkWarriorXAI_ovb0092 {
    u8 pad_0x0[0x30];
    UnkWarriorXOwner_ovb0092* owner;  // 0x30
    u8 pad_0x38[0x14d98];
    u8 entry[8];                      // 0x14dd0
};
static_assert(__builtin_offsetof(UnkWarriorXAI_ovb0092, entry) == 0x14dd0, "layout");

void fn_710048f3c4(UnkWarriorXAI_ovb0092* self) {
    fn_71004990ac(self);
    if (fn_7100da9260(Manager_ovb0092()) != 0.0f) {
        UnkWarriorXPlayer_ovb0092* player = self->owner->player;
        bool grabbing = false;
        if (UnkReflObject* attack = player->attack)
            grabbing = fn_710009d50c(attack->vf_0x0(), fn_7100486188());
        if (player && !grabbing) {
            fn_710048ecbc(reinterpret_cast<UnkSelf*>(self));
            self->owner->player->vf_0x178();
            fn_7101134420(reinterpret_cast<UnkCommanderPlayer_b67a2*>(self->owner->player), self->entry, 0, 0,
                          nullptr, -1.0f);
        }
        fn_7100da928c(Manager_ovb0092());
    }
}

void fn_710048f2f4(UnkWarriorXAI_ovb0092* self, const UnkWarriorXHit_ovb0092* hit) {
    if (fn_7100da9254(Manager_ovb0092()))
        return;
    f32 value = (hit->flags & 0x504) ? 0.26f : 0.1f;
    fn_7100da929c(Manager_ovb0092(), value);
}

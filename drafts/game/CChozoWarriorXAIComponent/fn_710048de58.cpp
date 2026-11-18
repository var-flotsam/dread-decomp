// CChozoWarriorXAIComponent unit: the destructor. Releases the two counted references (+0x154f0, +0x154e8),
// destroys the ultimate grab attack (+0x14dd0, fn_710048a60c) and the five other attacks (each with its
// listener, last first), then runs the base AI component's destructor (fn_710049880c). Names and layouts are
// placeholders.
#include "../../../src/game_5821e4/CAttackDtor.h"
#include "types.h"

extern void* g_unk_7101a3b830[];  // the component's vtable
extern void* g_unk_7101a3b338[];  // the attack vtable at +0x14d08
extern void* g_unk_7101a3b040[];  // the attack vtable at +0x14ca0
extern void* g_unk_7101a3b240[];  // the attack vtable at +0x14c38
extern void* g_unk_7101a3b140[];  // the attack vtable at +0x14b78

struct UnkUltimateGrab_48a60c;
void fn_710048a60c(UnkUltimateGrab_48a60c* self);

struct UnkChozoWarriorXAI_l48de58;
void fn_710049880c(UnkChozoWarriorXAI_l48de58* self);

// An attack whose listener sits at `ListenerOffset`; its destructor resets the vtable, destroys the listener
// and then CAttack's part (fn_71003606c0).
template <u32 ListenerOffset>
struct UnkChozoXAttack_l48de58 {
    void** vtable;
    u8 pad_0x8[ListenerOffset - 0x8];
    UnkAttackListenerRaw listener;

    void Destroy(void** vt) {
        vtable = vt + 2;
        listener.Destroy();
        fn_71003606c0(this);
    }
};

struct UnkChozoXPlainAttack_l48de58 {
    void** vtable;
    u8 pad_0x8[0x48];
};

struct UnkChozoWarriorXAI_l48de58 {
    void** vtable;
    u8 pad_0x8[0x14b78 - 0x8];
    UnkChozoXAttack_l48de58<0x50> attack_e;  // 0x14b78
    UnkChozoXPlainAttack_l48de58 attack_d;   // 0x14be8
    UnkChozoXAttack_l48de58<0x48> attack_c;  // 0x14c38
    UnkChozoXAttack_l48de58<0x48> attack_b;  // 0x14ca0
    UnkChozoXAttack_l48de58<0xa8> attack_a;  // 0x14d08
    u8 grab[0x718];                          // 0x14dd0
    UnkAttackRefPtrRaw ref_a;                // 0x154e8
    UnkAttackRefPtrRaw ref_b;                // 0x154f0
};
static_assert(__builtin_offsetof(UnkChozoWarriorXAI_l48de58, attack_a) == 0x14d08, "layout");
static_assert(__builtin_offsetof(UnkChozoWarriorXAI_l48de58, ref_b) == 0x154f0, "layout");

void fn_710048de58(UnkChozoWarriorXAI_l48de58* self) {
    self->vtable = g_unk_7101a3b830 + 2;
    self->ref_b.Release();
    self->ref_a.Release();
    fn_710048a60c(reinterpret_cast<UnkUltimateGrab_48a60c*>(self->grab));
    self->attack_a.Destroy(g_unk_7101a3b338);
    self->attack_b.Destroy(g_unk_7101a3b040);
    self->attack_c.Destroy(g_unk_7101a3b240);
    fn_71003606c0(&self->attack_d);
    self->attack_e.Destroy(g_unk_7101a3b140);
    fn_710049880c(self);
}

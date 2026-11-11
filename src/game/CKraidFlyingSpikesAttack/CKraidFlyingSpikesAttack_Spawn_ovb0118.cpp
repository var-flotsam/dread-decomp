// CKraidFlyingSpikesAttack's unit (tools/tu_map.py). fn_71005f513c: whether the attack can start:
// the base test (fn_71005f2014) holds, none of the three blocking tests does and, when the owner's
// AI (+0x240) is a CKraidAIComponent, it is not busy, passes fn_71005fdb70 and fails fn_71005ff8c0,
// and its attacker list (+0x1170 count, +0x1178 head) holds one of its own two attacks before this
// one. fn_71005f5250: loads the next of the three spike actor definitions (by the count at +0x38)
// through the actordef factory and hands it to fn_71005f535c. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CKraidAttack.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkActorDefFactory;
struct UnkPowerUpLifeEntry_b52;
struct UnkPowerUpLifeType_b52;
UnkPowerUpLifeEntry_b52* fn_710119438c(UnkActorDefFactory* factory, const char* path, CStrId* id, const UnkPowerUpLifeType_b52& type,
                                       void* arg);

// The three spike actor definitions.
extern CStrId g_unk_71726f2e90 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726f2e98 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726f2ea0 __attribute__((visibility("hidden")));

struct UnkKraidName_ovb0118 {
    u8 unk_0x0[0x10];
    char text[0x34];
    u32 use_count;  // 0x44
};

struct alignas(8) UnkKraidDefType_ovb0118 {
    u32 id;
};

// Entries of the Kraid AI component's attacker list.
struct UnkKraidAttackNode_ovb0118 {
    u8 unk_0x0[0x8];
    void* attack;  // 0x8
    u8 unk_0x10[0x8];
    UnkKraidAttackNode_ovb0118* next;  // 0x18
};

struct UnkKraidAI_ovb0118 : UnkReflObject {
    u8 unk_0x8[0x1168];
    s32 attack_count;  // 0x1170
    u8 unk_0x1174[0x4];
    UnkKraidAttackNode_ovb0118* attacks;  // 0x1178
    u8 unk_0x1180[0x13840];
    bool busy;  // 0x149c0
    u8 unk_0x149c1[0x23f];
    u8 other_attacks[2][0x68];  // 0x14c00, 0x14c68
};

struct UnkKraidOwner_ovb0118 {
    u8 unk_0x0[0x240];
    UnkKraidAI_ovb0118* ai;  // 0x240
};

struct UnkKraidSpikesAttack_ovb0118 {
    u8 unk_0x0[0x8];
    UnkKraidOwner_ovb0118* owner;  // 0x8
    u8 unk_0x10[0x28];
    s32 spawned;  // 0x38: how many spikes were spawned
};

void fn_71005f535c(UnkKraidSpikesAttack_ovb0118* self, UnkReflObject* spike);

// Raises the id's use count and has the actordef factory load the definition it names. A macro
// rather than an inline function: the three loads must stay separate until after inlining.
#define LOAD_SPIKE(ID, TYPE)                                                                      \
    do {                                                                                          \
        UnkActorDefFactory* factory = g_unk_71726a0220;                                           \
        u32 type_id = g_unk_7172a0ba48;                                                           \
        UnkKraidName_ovb0118* name = reinterpret_cast<UnkKraidName_ovb0118*>((ID).data);          \
        ++name->use_count;                                                                        \
        (TYPE).id = type_id;                                                                      \
        spike = reinterpret_cast<UnkReflObject*>(fn_710119438c(                                   \
            factory, name->text, &(ID), reinterpret_cast<const UnkPowerUpLifeType_b52&>(TYPE),    \
            nullptr));                                                                            \
    } while (0)

void fn_71005f5250(UnkKraidSpikesAttack_ovb0118* self) {
    UnkReflObject* spike;
    switch (self->spawned) {
    // The type ids are declared inside the switch: a case label jumping past them leaves them
    // without lifetime markers, so each keeps its own stack slot.
        UnkKraidDefType_ovb0118 type1, type0, type2;
    case 0:
        LOAD_SPIKE(g_unk_71726f2e90, type0);
        break;
    case 1:
        LOAD_SPIKE(g_unk_71726f2e98, type1);
        break;
    case 2:
        LOAD_SPIKE(g_unk_71726f2ea0, type2);
        break;
    default:
        return;
    }
    fn_71005f535c(self, spike);
}

struct UnkSelf_kt41;
bool fn_71005ff8c0(UnkReflObject* ai);
bool fn_71005fdb70(UnkKraidAI_ovb0118* ai);

bool fn_71005f513c(UnkKraidSpikesAttack_ovb0118* self, void* a, void* b) {
    UnkSelf_kt41* base = reinterpret_cast<UnkSelf_kt41*>(self);
    if (!fn_71005f2014(base, a, b) || fn_71005f2024(base) || fn_71005f2084(base) || fn_71005f20e4(base))
        return false;
    UnkKraidAI_ovb0118* ai = self->owner->ai;
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_71005fac74()))
        return true;
    if (ai->busy || fn_71005ff8c0(ai) || !fn_71005fdb70(ai) || ai->attack_count == 0)
        return false;
    // The two attacks the component owns itself.
    void* second = ai->other_attacks[1];
    void* first = ai->other_attacks[0];
    for (UnkKraidAttackNode_ovb0118* node = ai->attacks; node; node = node->next) {
        if (node->attack == first || node->attack == second)
            return true;
        if (node->attack == self)
            return false;
    }
    return false;
}

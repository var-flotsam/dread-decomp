// CScorpiusAIComponent unit: the component's destructor: its list links, callback lists, node lists and
// attacks, last first, then CBossAIComponent's destructor (fn_71003ac388). Names and layouts are
// placeholders.
#include "../../../src/game_5821e4/CAttackDtor.h"
#include "base/UnkCloneHelpers.h"
#include "game/CBossAIComponent.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkTU341.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

extern void* g_unk_7101a60f58[];  // CScorpiusAIComponent's vtable
extern void* g_unk_7101a60590[];  // the listening attack's vtable
extern void* g_unk_7101a60790[];  // CScorpiusMovingPoisonousGasAttack's vtable
extern void* g_unk_7101a60890[];  // CScorpiusPoisonousGasAttack's vtable
extern char g_unk_7101a617a0[];
extern char g_unk_7101a617c0[];

struct UnkScorpiusCallbacks_l691600;
template <>
struct base::UnkCallbackTraits<UnkScorpiusCallbacks_l691600> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkScorpiusCallbackList_l691600 = base::UnkCallbackListShort<UnkScorpiusCallbacks_l691600>;

struct UnkScorpiusNodes_l691600;
template <>
struct base::UnkPoolListTraits<UnkScorpiusNodes_l691600> {
    static char* vtable() { return g_unk_7101a617a0; }
    static char* base_vtable() { return g_unk_7101a617c0; }
};
using UnkScorpiusNodeList_l691600 = base::UnkPoolList<UnkScorpiusNodes_l691600, 0x8, 0x10>;

// A link into a list (SNavMeshPath): unlinked and cleared on destruction.
struct UnkListLink_l691600 {
    UnkLinkList* list;
    void* field_0x8;
    void* field_0x10;

    ~UnkListLink_l691600() {
        if (list) {
            fn_71002aeb9c(list, &list);
            list = nullptr;
        }
        field_0x10 = nullptr;
        field_0x8 = nullptr;
    }
};

// CAttack's part, destroyed out of line.
struct UnkAttack_l691600 {
    void** vtable;
    u8 pad_0x8[0x30];

    ~UnkAttack_l691600() { fn_71003606c0(this); }
};

struct UnkAttackWide_l691600 : UnkAttack_l691600 {
    u8 pad_0x38[0x10];
};

struct UnkAttackLarge_l691600 : UnkAttack_l691600 {
    u8 pad_0x38[0x20];
};

// An attack with a listener at +0x38.
struct UnkListeningAttack_l691600 : UnkAttack_l691600 {
    UnkAttackListenerRaw listener;  // 0x38

    ~UnkListeningAttack_l691600() {
        vtable = g_unk_7101a60590 + 2;
        listener.Destroy();
    }
};

struct UnkPoisonousGasAttack_l691600 : UnkAttack_l691600 {
    UnkAttackOwner* owner;  // 0x38
    u8 pad_0x40[0x28];

    ~UnkPoisonousGasAttack_l691600() {
        vtable = g_unk_7101a60890 + 2;
        if (UnkAttackOwner* p = owner) {
            if (--p->ref_count == 0)
                p->Delete();
        }
    }
};

struct UnkMovingPoisonousGasAttack_l691600 : UnkPoisonousGasAttack_l691600 {
    u8 pad_0x68[0x10];
    UnkAttackListenerRaw listener;  // 0x78

    ~UnkMovingPoisonousGasAttack_l691600() {
        vtable = g_unk_7101a60790 + 2;
        listener.Destroy();
    }
};

// The path-following part at 0x14cd8.
struct UnkScorpiusPath_l691600 {
    UnkListLink_l691600 link_a;                          // 0x14cd8
    u8 pad_0x18[0x20];
    UnkScorpiusNodeList_l691600 nodes_b;                 // 0x14d10
    u8 pad_0xa8[0x10];
    UnkScorpiusCallbackList_l691600 callbacks_a;         // 0x14d90
    UnkScorpiusCallbackList_l691600 callbacks_b;         // 0x14dd0
    UnkScorpiusCallbackList_l691600 callbacks_c;         // 0x14e10
    UnkListLink_l691600 links[4];                        // 0x14e50
};

struct UnkScorpiusAI_l691600 {
    void** vtable;
    u8 pad_0x8[0x149a8 - 0x8];
    UnkAttack_l691600 attack_a;                          // 0x149a8
    UnkAttackWide_l691600 attack_b;                      // 0x149e0
    UnkAttack_l691600 attack_c;                          // 0x14a28
    UnkListeningAttack_l691600 attack_d;                 // 0x14a60
    UnkAttack_l691600 attack_e;                          // 0x14ab8
    UnkPoisonousGasAttack_l691600 gas_attack;            // 0x14af0
    UnkMovingPoisonousGasAttack_l691600 moving_gas_attack;  // 0x14b58
    UnkAttackLarge_l691600 attack_f;                     // 0x14bf0
    UnkScorpiusNodeList_l691600 nodes_a;                 // 0x14c48
    u8 pad_0x14cb8[0x20];
    UnkScorpiusPath_l691600 path;                        // 0x14cd8
};
static_assert(__builtin_offsetof(UnkScorpiusAI_l691600, attack_f) == 0x14bf0, "layout");
static_assert(__builtin_offsetof(UnkScorpiusAI_l691600, path) == 0x14cd8, "layout");

static_assert(sizeof(UnkScorpiusPath_l691600) == 0x1d8, "layout");

void fn_7100691600(UnkScorpiusAI_l691600* self) {
    self->vtable = g_unk_7101a60f58 + 2;
    for (int i = 4; i-- > 0;)
        self->path.links[i].~UnkListLink_l691600();
    self->path.callbacks_c.~UnkScorpiusCallbackList_l691600();
    self->path.callbacks_b.~UnkScorpiusCallbackList_l691600();
    self->path.callbacks_a.~UnkScorpiusCallbackList_l691600();
    self->path.nodes_b.~UnkScorpiusNodeList_l691600();
    self->path.link_a.~UnkListLink_l691600();
    self->nodes_a.~UnkScorpiusNodeList_l691600();
    self->attack_f.~UnkAttackLarge_l691600();
    self->moving_gas_attack.~UnkMovingPoisonousGasAttack_l691600();
    self->gas_attack.~UnkPoisonousGasAttack_l691600();
    self->attack_e.~UnkAttack_l691600();
    self->attack_d.~UnkListeningAttack_l691600();
    self->attack_c.~UnkAttack_l691600();
    self->attack_b.~UnkAttackWide_l691600();
    self->attack_a.~UnkAttack_l691600();
    fn_71003ac388(self);
}

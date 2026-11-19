// CChozoWarriorAIComponent's unit (tools/tu_map.py unit 733): the destructor. The listener at
// +0x14b50, the three attacks at +0x14ae0, +0x14a78 and +0x14a30, the counted reference at +0x14a20
// and the callback list at +0x149d8, then the boss AI's destructor. Names and layouts are placeholders.
#include "../../game_5821e4/CAttackDtor.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "game/CAttack.h"
#include "game/CBossAIComponent.h"
#include "game/CChozoWarriorAIComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

extern char g_unk_7101a3c9b0[];   // the listening attack's vtable

struct UnkChozoWarriorCallbacks_l49880c;
template <>
struct base::UnkCallbackTraits<UnkChozoWarriorCallbacks_l49880c> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkChozoWarriorCallbackList_l49880c = base::UnkCallbackListShort<UnkChozoWarriorCallbacks_l49880c>;

// An attack with no members of its own to release.
struct UnkChozoWarriorAttack_l49880c {
    void** vtable;
    u8 pad_0x8[0x40];

    void Destroy() { fn_71003606c0(this); }
};

// An attack that also listens to its owner.
struct UnkChozoWarriorListeningAttack_l49880c {
    void** vtable;
    u8 pad_0x8[0x40];
    UnkAttackListenerRaw listener;  // 0x48

    void Destroy() {
        vtable = reinterpret_cast<void**>(g_unk_7101a3c9b0 + 0x10);
        listener.Destroy();
        fn_71003606c0(this);
    }
};

struct UnkChozoWarriorAI_l49880c {
    void** vtable;
    u8 pad_0x8[0x149d0];
    UnkChozoWarriorCallbackList_l49880c callbacks;   // 0x149d8
    u8 pad_0x14a18[0x8];
    base::UnkRefPtrMember ref;                       // 0x14a20
    u8 pad_0x14a28[0x8];
    UnkChozoWarriorAttack_l49880c attack_a;          // 0x14a30
    UnkChozoWarriorListeningAttack_l49880c attack_b; // 0x14a78
    UnkChozoWarriorAttack_l49880c attack_c;          // 0x14ae0
    u8 pad_0x14b28[0x28];
    UnkAttackListenerRaw listener;                   // 0x14b50
};
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l49880c, attack_c) == 0x14ae0, "layout");
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l49880c, listener) == 0x14b50, "layout");

void fn_710049880c(UnkChozoWarriorAI_l49880c* self) {
    self->vtable = g_unk_7101a3cbc8 + 2;
    self->listener.Destroy();
    self->attack_c.Destroy();
    self->attack_b.Destroy();
    self->attack_a.Destroy();
    using base::UnkRefPtrMember;
    self->ref.~UnkRefPtrMember();
    self->callbacks.~UnkChozoWarriorCallbackList_l49880c();
    fn_71003ac388(reinterpret_cast<UnkBossAI_l3ac388*>(self));
}

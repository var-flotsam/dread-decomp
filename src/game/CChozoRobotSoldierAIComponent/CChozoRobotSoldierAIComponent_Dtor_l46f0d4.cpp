// CChozoRobotSoldierAIComponent's unit (tools/tu_map.py unit 694): the destructor. The array at +0x27768,
// the three holders at +0x27628..+0x276f8, the path at +0x14b90, the hit and entry arrays, the callback
// list at +0x14ad8, four attacks, then the boss AI component's destructor. Names and layouts are placeholders.
#include "game/CChozoRobotSoldierCannonShotAttack.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "game/CAttack.h"
#include "game/CBossAIComponent.h"
#include "game/UnkNavMeshHit.h"
#include "game/UnkTU341.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

extern void* g_unk_7101a38c68[];  // CChozoRobotSoldierAIComponent's vtable

struct UnkHolderOwner_b47;
void fn_71002c5394(UnkHolderOwner_b47* self);

struct UnkChozoRobotSoldierCallbacks_l46f0d4;
template <>
struct base::UnkCallbackTraits<UnkChozoRobotSoldierCallbacks_l46f0d4> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkChozoRobotSoldierCallbackList_l46f0d4 =
    base::UnkCallbackListShort<UnkChozoRobotSoldierCallbacks_l46f0d4>;

// A 0x68-byte holder destroyed out of line (fn_71002c5394).
struct UnkChozoRobotSoldierHolder_l46f0d4 {
    u8 pad_0x0[0x68];
    ~UnkChozoRobotSoldierHolder_l46f0d4() {
        fn_71002c5394(reinterpret_cast<UnkHolderOwner_b47*>(this));
    }
};

// The path object destroyed by fn_71002ad0dc.
struct UnkChozoRobotSoldierPath_l46f0d4 {
    u8 pad_0x0[0x8];
    ~UnkChozoRobotSoldierPath_l46f0d4() {
        fn_71002ad0dc(reinterpret_cast<UnkObject_71002ad0dc*>(this));
    }
};

// An array element (0x40 bytes) destroyed through its vtable.
struct UnkChozoRobotSoldierEntry_l46f0d4 {
    virtual ~UnkChozoRobotSoldierEntry_l46f0d4();
    u8 pad_0x8[0x38];
};

// An attack: its destructor is fn_71003606c0.
struct UnkChozoRobotSoldierAttackBase_l46f0d4 {
    void** vtable;
    ~UnkChozoRobotSoldierAttackBase_l46f0d4() { fn_71003606c0(this); }
};

template <u32 Size>
struct UnkChozoRobotSoldierAttack_l46f0d4 : UnkChozoRobotSoldierAttackBase_l46f0d4 {
    u8 pad_0x8[Size - 0x8];
};

// The cannon shot attack, with its counted reference at +0x60.
struct UnkChozoRobotSoldierShot_l46f0d4 : UnkChozoRobotSoldierAttackBase_l46f0d4 {
    u8 pad_0x8[0x58];
    base::UnkRefPtrMember ref;  // 0x60
    u8 pad_0x68[0x8];
    ~UnkChozoRobotSoldierShot_l46f0d4() { vtable = &g_unk_7101a383a0[2]; }
};

struct UnkChozoRobotSoldierAIComponent_l46f0d4 {
    void** vtable;
    u8 pad_0x8[0x149a8 - 0x8];
    UnkChozoRobotSoldierShot_l46f0d4 shot;                     // 0x149a8
    UnkChozoRobotSoldierAttack_l46f0d4<0x48> attack_a;         // 0x14a18
    UnkChozoRobotSoldierAttack_l46f0d4<0x30> attack_b;         // 0x14a60
    UnkChozoRobotSoldierAttack_l46f0d4<0x48> attack_c;         // 0x14a90
    UnkChozoRobotSoldierCallbackList_l46f0d4 callbacks;        // 0x14ad8
    u8 pad_0x14b18[0x10];
    base::global::CRntVector<UnkChozoRobotSoldierEntry_l46f0d4> entries;  // 0x14b28
    base::global::CRntVector<UnkNavMeshHit> hits;              // 0x14b50
    u8 pad_0x14b78[0x18];
    UnkChozoRobotSoldierPath_l46f0d4 path;                     // 0x14b90
    u8 pad_0x14b98[0x27628 - 0x14b98];
    UnkChozoRobotSoldierHolder_l46f0d4 holder_a;               // 0x27628
    UnkChozoRobotSoldierHolder_l46f0d4 holder_b;               // 0x27690
    UnkChozoRobotSoldierHolder_l46f0d4 holder_c;               // 0x276f8
    u8 pad_0x27760[0x8];
    base::global::CRntVector<u8> bytes;                        // 0x27768
};
static_assert(__builtin_offsetof(UnkChozoRobotSoldierAIComponent_l46f0d4, callbacks) == 0x14ad8, "layout");
static_assert(__builtin_offsetof(UnkChozoRobotSoldierAIComponent_l46f0d4, entries) == 0x14b28, "layout");
static_assert(__builtin_offsetof(UnkChozoRobotSoldierAIComponent_l46f0d4, path) == 0x14b90, "layout");
static_assert(__builtin_offsetof(UnkChozoRobotSoldierAIComponent_l46f0d4, holder_a) == 0x27628, "layout");
static_assert(__builtin_offsetof(UnkChozoRobotSoldierAIComponent_l46f0d4, bytes) == 0x27768, "layout");

void fn_710046f0d4(UnkChozoRobotSoldierAIComponent_l46f0d4* self) {
    self->vtable = &g_unk_7101a38c68[2];
    self->bytes.Free();
    self->holder_c.~UnkChozoRobotSoldierHolder_l46f0d4();
    self->holder_b.~UnkChozoRobotSoldierHolder_l46f0d4();
    self->holder_a.~UnkChozoRobotSoldierHolder_l46f0d4();
    self->path.~UnkChozoRobotSoldierPath_l46f0d4();
    self->hits.Free();
    self->entries.Free();
    self->callbacks.~UnkChozoRobotSoldierCallbackList_l46f0d4();
    self->attack_c.~UnkChozoRobotSoldierAttack_l46f0d4();
    self->attack_b.~UnkChozoRobotSoldierAttack_l46f0d4();
    self->attack_a.~UnkChozoRobotSoldierAttack_l46f0d4();
    self->shot.~UnkChozoRobotSoldierShot_l46f0d4();
    fn_71003ac388(reinterpret_cast<UnkBossAI_l3ac388*>(self));
}

// CEmmyAttackComponent's unit (tools/tu_map.py unit 812): an actor touches the E.M.M.I.'s attack. Anything
// the game manager doesn't treat as the player goes to the base handler (fn_7101134f04) with state 2. For
// the player, unless it is out of reach (grabbed, dead, in a cutscene or already the sub-area manager's
// target), the E.M.M.I. is set to chase it and the attack the global name picks is started when its check
// passes. Names and layouts are placeholders.
#include "base/UnkVtableList.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAIAttackComponent.h"
#include "game/CAttack.h"
#include "game/CGrabComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU2656.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU394.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkCommanderPlayer_b67a2;
struct UnkEmmyActor_b55;
struct UnkEmmyRelaxAI_b59;
struct UnkScorpiusAI_b69a4;

extern CStrId g_unk_71726de3f0;  // the name of the attack started on contact

struct UnkEmmyAttack_l56a8d0;
struct UnkHitInfo_l56a8d0;
s32 fn_7101134f04(UnkEmmyAttack_l56a8d0* self, UnkActor* actor, UnkHitInfo_l56a8d0* info, void* a, void* b);
bool fn_7101167340(void* life);
// Its second parameter is a state (2 here); CEmmySearchingAction_Resume_b63a3.cpp declares it bool.
void fn_7100547428(UnkEmmyActor_b55* self, s32 state, UnkActor* target, bool b);
bool fn_710054782c(UnkEmmyRelaxAI_b59* self);

// A list of nodes (link at +8) freed through the allocator.
struct UnkQueryNodeList_l56a8d0 : base::UnkVtableList<void> {
    ~UnkQueryNodeList_l56a8d0() {
        vtable = g_unk_7101a21540 + 0x10;
        void* n = head;
        while (n) {
            void* next = *reinterpret_cast<void**>(static_cast<char*>(n) + 8);
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

// A counted reference released on destruction.
struct UnkQueryRef_l56a8d0 {
    UnkRefObject* p;
    ~UnkQueryRef_l56a8d0() {
        if (p) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

// The query an attack check takes: built by fn_71002c6f80 from the actor and the parameters.
struct UnkAttackQuery_l56a8d0 {
    UnkQueryRef_l56a8d0 ref;
    u8 pad_0x8[0x20];
    UnkQueryNodeList_l56a8d0 nodes;  // 0x28

    UnkAttackQuery_l56a8d0(UnkActor* actor, void* params) {
        fn_71002c6f80(reinterpret_cast<UnkAttackQuery_ovb0115*>(this), actor, params);
    }
};

struct UnkAttack_l56a8d0 {
    void** vtable;

    bool Check(UnkAttackQuery_l56a8d0* query, u32 mode) {
        return reinterpret_cast<bool (*)(UnkAttack_l56a8d0*, UnkAttackQuery_l56a8d0*, u32)>(
            vtable[0x58 / 8])(this, query, mode);
    }
};

struct UnkEmmyMode_l56a8d0 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
};

struct UnkEmmyModes_l56a8d0 {
    u8 pad_0x0[0x40];
    UnkEmmyMode_l56a8d0* current;  // 0x40
};

struct UnkEmmyState_l56a8d0 {
    u8 pad_0x0[0x10];
    UnkEmmyModes_l56a8d0* modes;  // 0x10
};

struct UnkEmmyAI_l56a8d0 {
    virtual const base::reflection::CType* GetClass();
    u8 pad_0x8[0x1160];
    u8 attack_params[0x283f0];      // 0x1168
    UnkEmmyState_l56a8d0* state;    // 0x29558
};
static_assert(__builtin_offsetof(UnkEmmyAI_l56a8d0, state) == 0x29558, "layout");

struct UnkScene_l56a8d0 {
    u8 pad_0x0[0x70];
    s32 mode;  // 0x70
};

struct UnkEmmyOwner_l56a8d0 {
    u8 pad_0x0[0x1b8];
    UnkScene_l56a8d0* scene;  // 0x1b8
    u8 pad_0x1c0[0x80];
    UnkEmmyAI_l56a8d0* ai;  // 0x240
    u8 pad_0x248[0x18];
    void* grab;  // 0x260
};

struct UnkEmmyAttack_l56a8d0 {
    u8 pad_0x0[0x30];
    UnkEmmyOwner_l56a8d0* owner;  // 0x30
};

struct UnkHitInfo_l56a8d0 {
    u8 pad_0x0[0x1c];
    s32 state;  // 0x1c
};

struct UnkPlayerState_l56a8d0 {
    u8 pad_0x0[0x7b9];
    bool dead;  // 0x7b9
};

struct UnkTarget_l56a8d0 {
    u8 pad_0x0[0x1c0];
    void* life;  // 0x1c0
    u8 pad_0x1c8[0x38];
    UnkPlayerState_l56a8d0* player;  // 0x200
};

struct UnkSubAreaManager_l56a8d0 {
    u8 pad_0x0[0x148];
    UnkTarget_l56a8d0* target;  // 0x148
};

// The sub-area manager, created on first use.
static inline UnkSubAreaManager_l56a8d0* SubAreaManager() {
    UnkManager* manager = g_unk_7101d47360;
    if (!manager) {
        manager = static_cast<UnkManager*>(fn_71000839a0(0xae8, 0x3f));
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = manager;
    }
    return reinterpret_cast<UnkSubAreaManager_l56a8d0*>(manager);
}

// Whether the object is of the class `Class` returns (or derives from it).
template <const base::reflection::CClass* (*Class)()>
static inline bool IsA(UnkEmmyAI_l56a8d0* obj) {
    return fn_710009d50c(obj->GetClass(), reinterpret_cast<const base::reflection::CType*>(Class()));
}

// An id's string, counting the read (as CStrId::Text does).
static inline const CRntString& StringOf(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

s32 fn_710056a8d0(UnkEmmyAttack_l56a8d0* self, UnkTarget_l56a8d0* target, UnkHitInfo_l56a8d0* info, void* a,
                  void* b) {
    if (fn_710113d83c(self->owner->grab))
        return 6;
    if (!fn_7100df5c38(g_unk_7101d47310, target)) {
        info->state = 2;
        return fn_7101134f04(self, reinterpret_cast<UnkActor*>(target), info, a, b);
    }
    if (target->player && target->player->dead)
        return 6;
    if (target->life && fn_7101167340(target->life))
        return 6;
    UnkEmmyOwner_l56a8d0* owner = self->owner;
    if (owner->scene && owner->scene->mode == 2)
        return 6;
    UnkEmmyAI_l56a8d0* ai = owner->ai;
    // The original asks for the class and ignores the answer (what is left of a checked cast).
    IsA<fn_710053b934>(ai);
    UnkEmmyMode_l56a8d0* mode = ai->state->modes->current;
    if (!mode || (mode->kind | 4) != 6)
        return 6;
    if (fn_710054782c(reinterpret_cast<UnkEmmyRelaxAI_b59*>(ai)))
        return 6;
    if (fn_7100eef674(SubAreaManager()) && fn_7101053a98(g_unk_7101d47310->field_0x1040))
        return 6;
    if (SubAreaManager()->target == target)
        return 6;
    fn_7100547428(reinterpret_cast<UnkEmmyActor_b55*>(ai), 2, reinterpret_cast<UnkActor*>(target), false);
    auto* attack = static_cast<UnkAttack_l56a8d0*>(
        fn_7101109df4(reinterpret_cast<UnkScorpiusAI_b69a4*>(ai), const_cast<CRntString*>(&StringOf(g_unk_71726de3f0))));
    bool ok;
    {
        UnkAttackQuery_l56a8d0 query(reinterpret_cast<UnkActor*>(self->owner), ai->attack_params);
        ok = attack->Check(&query, 0);
    }
    if (ok)
        fn_7101134420(reinterpret_cast<UnkCommanderPlayer_b67a2*>(self), attack, true, 0, nullptr, -1.0f);
    return 6;
}

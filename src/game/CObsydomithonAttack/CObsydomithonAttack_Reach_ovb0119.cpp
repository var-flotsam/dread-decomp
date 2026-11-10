// CObsydomithonAttack's unit (tools/tu_map.py unit 945): fn_71006298d4, whether the target is in
// reach. With an AI component (+0x240 of the owner) and an actor to act against
// (fn_7100d7c990), the attack check (slot 0x58) runs on a query built from the AI's attack
// parameters (+0x1168, fn_71002c6f80, as CScourgeTongueSlashAttack_Tongue_ovb0115.cpp builds it)
// with both distances set to the x distance to the target, and the AI's mode (+0x1140). Names and
// layouts are placeholders.
#include <math.h>

#include "base/UnkVector.h"
#include "game/UnkActor.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"

using base::global::CStrId;

extern char g_unk_7101a21540[];  // the node list's vtable

// A list of nodes (link at +8) freed through the allocator.
struct UnkAttackNodeList_b0119 {
    void* vtable;
    u32 count;
    void* head;
    void* tail;
    base::UnkAllocator* allocator;

    ~UnkAttackNodeList_b0119() {
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

// The query an attack check takes: built by fn_71002c6f80 from the actor and the parameters.
struct UnkAttackQuery_ovb0115;
void fn_71002c6f80(UnkAttackQuery_ovb0115* self, UnkActor* actor, void* params);

// A counted reference released on destruction.
struct UnkAttackRef_b0119 {
    UnkRefObject* p;
    ~UnkAttackRef_b0119() {
        if (p) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkAttackQuery_b0119 {
    UnkAttackRef_b0119 ref;
    f32 distance[2];  // 0x8
    u8 pad_0x10[0x9];
    bool field_0x19;
    u8 pad_0x1a[0xe];
    UnkAttackNodeList_b0119 nodes;  // 0x28
    u8 pad_0x50[0x8];

    UnkAttackQuery_b0119(UnkActor* actor, void* params) {
        fn_71002c6f80(reinterpret_cast<UnkAttackQuery_ovb0115*>(this), actor, params);
    }
};

struct UnkObsydomithonAI_b0119 {
    u8 pad_0x0[0x1140];
    u32 field_0x1140;
    u8 pad_0x1144[0x24];
    u8 field_0x1168[8];  // the attack parameters
};

struct UnkObsydomithonCheck_b0119 {
    void** vtable;
    UnkActor* owner;  // 0x8

    bool vf_0x58(UnkAttackQuery_b0119* query, u32 mode) {
        return reinterpret_cast<bool (*)(UnkObsydomithonCheck_b0119*, UnkAttackQuery_b0119*, u32)>(
            vtable[0x58 / 8])(this, query, mode);
    }
};

static inline UnkObsydomithonAI_b0119* Ai(UnkActor* actor) {
    return static_cast<UnkObsydomithonAI_b0119*>(actor->field_0x240);
}

bool fn_71006298d4(UnkObsydomithonCheck_b0119* self) {
    if (!Ai(self->owner) || !fn_7100d7c990(self->owner))
        return false;
    UnkAttackQuery_b0119 query(self->owner, Ai(self->owner)->field_0x1168);
    query.field_0x19 = false;
    f32 x = self->owner->field_0x44;
    f32 d = fabsf(x - fn_7100d7c990(self->owner)->field_0x44);
    query.distance[0] = d;
    query.distance[1] = d;
    return self->vf_0x58(&query, Ai(self->owner)->field_0x1140);
}

// CObsydomithonAttack's unit (tools/tu_map.py unit 945): fn_710062a120 handles an animation event
// with a string id (the event's +0x8, its +0x10, its id at +0x38). Kind 1 with the id
// g_unk_71726f8e10 in stage 5 stops the attack (fn_710062a268). Kind 3 moves stage 3 on to 4 and
// stage 1 on to 2 on their own event hashes, and to 6 while the owner's character class (+0x220)
// plays a CCharClassAttackFailTrack (fn_710117b348). Always returns true. Names and layouts are
// placeholders.
extern CStrId g_unk_71726f8e10 __attribute__((visibility("hidden")));

void fn_710062a268(void* attack, bool value);

struct UnkObsydomithonOwner_b0119 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* anim;  // 0x220
};

struct UnkObsydomithonAttack_b0119 {
    void* vtable;
    UnkObsydomithonOwner_b0119* owner;  // 0x8
    u8 pad_0x10[0x40];
    s32 stage;  // 0x50
    s32 next;   // 0x54
};

struct UnkObsydomithonEventInner_b0119 {
    u8 pad_0x0[0x38];
    CStrId id;
};

struct UnkObsydomithonEventData_b0119 {
    u8 pad_0x0[0x10];
    UnkObsydomithonEventInner_b0119* field_0x10;
};

struct UnkObsydomithonEvent_b0119 {
    u32 kind;
    u8 pad_0x4[4];
    UnkObsydomithonEventData_b0119* field_0x8;
};

static inline u64 IdHash(const CStrId& id) {
    return *reinterpret_cast<u64*>(reinterpret_cast<char*>(id.data) + 0x30);
}

bool fn_710062a120(UnkObsydomithonAttack_b0119* self, UnkObsydomithonEvent_b0119* event) {
    UnkAnimationComponent* anim = self->owner ? self->owner->anim : nullptr;
    const CStrId& id = event->field_0x8 && event->field_0x8->field_0x10 ? event->field_0x8->field_0x10->id
                                                                         : g_unk_7101d470e0;
    if (!anim || id.data == &g_unk_7101d47108->empty)
        return true;
    switch (event->kind) {
    case 3:
        switch (self->stage) {
        case 1:
            if (IdHash(id) == 0x7640910a55220b1b)
                self->next = 2;
            break;
        case 3:
            if (IdHash(id) == 0x8d045f916fcc75ad)
                self->next = 4;
            break;
        }
        if (fn_710117b348(anim, fn_7100c0d1d8()))
            self->next = 6;
        break;
    case 1:
        if (self->stage == 5 && id.data == g_unk_71726f8e10.data)
            fn_710062a268(self, false);
        break;
    }
    return true;
}

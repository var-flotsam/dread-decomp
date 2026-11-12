// CBigFistAttack's unit (tools/tu_map.py): the attack's start: after the base (fn_71003608ac) sets +0x30 to
// 1, gives the owner's (+0x8) AI (+0x240) value 1 (fn_710111094c), hooks the callback fn_71003e181c (link at
// +0x38, handle at +0x50) up to the object the owner tracks (+0x220), as
// src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp; when the AI is a CBigFistAIComponent
// whose flag at +0x14a28 is set, clears it and has the tracked object run 0xf4061ff25a2163d6 for 30
// (fn_7101178010). +0x58 records whether that happened. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/UnkTU2702.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkAttackBase;
void fn_71003608ac(UnkAttackBase* self);
void fn_71003e181c();

struct UnkBigFistTrackLink_A2_b72 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkBigFistStart_A2_b72 {
    u8 pad_0x0[0x8];
    char* owner;
    u8 pad_0x10[0x20];
    u32 field_0x30;
    u8 pad_0x34[0x4];
    UnkBigFistTrackLink_A2_b72 link;  // 0x38
    UnkWeakHandle* handle;            // 0x50
    bool field_0x58;
};

static inline UnkReflObject* Ai_A2_b72(char* owner) { return *reinterpret_cast<UnkReflObject**>(owner + 0x240); }

void fn_71003e0aac(UnkBigFistStart_A2_b72* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    self->field_0x30 = 1;
    fn_710111094c(Ai_A2_b72(self->owner), 1);
    UnkBigFistTrackLink_A2_b72* link = &self->link;
    link->context = self;
    link->callback = fn_71003e181c;
    UnkWeakTarget* track = *reinterpret_cast<UnkWeakTarget**>(self->owner + 0x220);
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    if (track) {
        asm("" : "+r"(self));
        fn_7100316018(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
    bool fired = false;
    UnkReflObject* ai = Ai_A2_b72(self->owner);
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_71003e70dc())) {
        bool& pending = *reinterpret_cast<bool*>(reinterpret_cast<char*>(ai) + 0x14a28);
        if (pending) {
            fn_7101178010(*reinterpret_cast<void**>(self->owner + 0x220), 0xf4061ff25a2163d6ull, false, 30.0f);
            pending = false;
            fired = true;
        }
    }
    self->field_0x58 = fired;
}

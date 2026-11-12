// CCooldownXBossLavaDropsAttack's unit (tools/tu_map.py): after fn_71004c196c, unlinks the link at +0x328 from the
// target of the weak handle at +0x340 and drops the handle; asks the state machine at +0x68 for state 1 (the
// request of src/game/UnkStateMachineSet_b60.cpp, copied here); drops the render item at +0x348; sets the fixed id
// g_unk_7101649b00 on the owner's (+0x8) timeline (+0x2e8) to the all-flags mask; switches the object at +0x320
// off. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CCooldownXBossAttack.h"
#include "game/UnkActor.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTimelineCalls.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkLavaDropsKey_b62a2;
extern UnkLavaDropsKey_b62a2 g_unk_7101649b00;  // reached through the GOT

struct UnkSmState_b62a2 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_b62a2* __attribute__((aligned(4))) UnkSmStatePtr_b62a2;

struct UnkSmMachine_b62a2 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_b62a2* previous, UnkSmState_b62a2* next);
    virtual void vf_0x28(UnkSmState_b62a2* current, UnkSmState_b62a2* next);

    u64 field_0x8;
    UnkSmState_b62a2* current;
    UnkSmState_b62a2* previous;
    u32 count;
    UnkSmStatePtr_b62a2 states[16];
    u32 pending;
    bool busy;
};

__attribute__((always_inline)) static inline bool FindState_b62a2(UnkSmMachine_b62a2* self, u32 id,
                                                                 UnkSmState_b62a2*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_b62a2);
    asm("" : "+r"(n));
    for (UnkSmStatePtr_b62a2* p = self->states; n != 0; ++p, n -= sizeof(UnkSmStatePtr_b62a2)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

template <u32 None>
__attribute__((always_inline)) static inline void SwitchState_b62a2(UnkSmMachine_b62a2* self, u32 id,
                                                                   UnkSmState_b62a2* current) {
    if (!current || current->id != id) {
        UnkSmState_b62a2* found;
        UnkSmState_b62a2* next;
        u32 enter;
        if (FindState_b62a2(self, id, found)) {
            next = found;
            enter = true;
        } else {
            if (id != 0)
                goto done;
            enter = false;
            next = nullptr;
        }
        if (current) {
            current->vf_0x18(id);
            self->vf_0x28(self->current, next);
        }
        self->previous = self->current;
        self->current = found;
        if (enter) {
            typedef void (*EnteredFn)(UnkSmState_b62a2*, s32);
            s32 previous_id = self->previous ? self->previous->id : None;
            EnteredFn entered = (*reinterpret_cast<EnteredFn**>(found))[2];
            entered(next, previous_id);
            self->vf_0x20(self->previous, next);
        }
    }
done:
    self->pending = None;
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void RequestState_b62a2(UnkSmMachine_b62a2* self, u32 id) {
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_b62a2<None>(self, id, self->current);
}

struct UnkLavaDropsItem_b62a2 {
    u8 pad_0x0[0x8];
    u8 item[0x8];
};

struct UnkLavaDrops_b62a2 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x68 - 0x10];
    UnkSmMachine_b62a2 machine;
    u8 pad_0x118[0x320 - 0x118];
    UnkSwitchObject* field_0x320;
    u8 link[0x18];  // 0x328
    UnkWeakHandle* field_0x340;
    UnkLavaDropsItem_b62a2* field_0x348;
};
static_assert(__builtin_offsetof(UnkLavaDrops_b62a2, machine.pending) == 0x10c, "layout");

// The attack base's step every CCooldownXBoss attack calls first (one opaque type for all callers).
struct UnkCooldownXBossAttack_b62a2;

void fn_71004c7864(UnkLavaDrops_b62a2* self) {
    fn_71004c196c(reinterpret_cast<UnkCooldownXBossAttack_b62a2*>(self));
    if (UnkWeakHandle* handle = self->field_0x340) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->link);
        ResetRef(&self->field_0x340);
    }
    RequestState_b62a2<4, 5>(&self->machine, 1);
    if (UnkLavaDropsItem_b62a2* item = self->field_0x348) {
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(item->item), 0);
        self->field_0x348 = nullptr;
    }
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_7101649b00), &mask.id, false, -1.0f);
    if (self->field_0x320)
        fn_7100146e60(self->field_0x320, false);
}

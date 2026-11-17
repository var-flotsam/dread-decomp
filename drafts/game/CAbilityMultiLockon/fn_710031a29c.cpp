// CAbilityMultiLockon's unit (tools/tu_map.py, unit 431): ends the ability. Resets the owner's key
// (fn_7100d7c828), passes each aim pair's object on (fn_710031a460, told whether the end was forced); unless
// forced, releases the target of each of the five aim slots, drops the locked handles and takes every
// lock-on mark off the render manager, then runs CAbility's end (fn_71002f9460). Names and layouts are
// placeholders.
#include "game/CAbility.h"
#include "game/UnkActor.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkMultiLockon_ovb0092;
struct UnkStruct_71002334fc;
void fn_710031965c(UnkMultiLockon_ovb0092* self, UnkStruct_71002334fc* target);

// The key the owner's table is reset with, reached through the GOT.
extern u64 g_unk_7101648dc8;

// An aim pair (0x30 bytes); the object at +0x18 is what fn_710031a460 takes.
struct UnkAimPair_l31a29c {
    u8 pad_0x0[0x18];
    void* field_0x18;
    u8 pad_0x20[0x10];
};
static_assert(sizeof(UnkAimPair_l31a29c) == 0x30, "UnkAimPair_l31a29c layout");

// A mark on screen: its fade time and the mark object (drawn through its item at +8).
struct UnkLockonEntry_l31a29c {
    f32 time;
    u8* mark;
};

// A counted handle in the locked-target list.
struct UnkLockedRef_l31a29c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    u32 ref_count;
};

struct UnkMultiLockon_l31a29c {
    u8 pad_0x0[0x3a0];
    UnkWeakHandle* slots[5];  // 0x3a0
    UnkLockedRef_l31a29c** locked;  // 0x3c8
    u32 locked_count;         // 0x3d0
    u8 pad_0x3d4[0x22c];
    UnkActor* owner;  // 0x600
    u8 pad_0x608[0x4];
    u32 pair_count;                  // 0x60c
    UnkAimPair_l31a29c pairs[5];     // 0x610
    UnkLockonEntry_l31a29c* marks;   // 0x700
    u32 mark_count;                  // 0x708
};
static_assert(__builtin_offsetof(UnkMultiLockon_l31a29c, owner) == 0x600, "UnkMultiLockon_l31a29c layout");
static_assert(__builtin_offsetof(UnkMultiLockon_l31a29c, marks) == 0x700, "UnkMultiLockon_l31a29c layout");

void fn_710031a460(UnkMultiLockon_l31a29c* self, void* object, bool flag);

void fn_710031a29c(UnkMultiLockon_l31a29c* self, bool forced) {
    if (self->owner)
        fn_7100d7c828(self->owner, &g_unk_7101648dc8, false);
    for (UnkAimPair_l31a29c *pair = self->pairs, *end = pair + self->pair_count; pair != end; ++pair)
        fn_710031a460(self, pair->field_0x18, !forced);
    if (!forced) {
        for (u32 i = 0; i < 5; ++i) {
            UnkWeakHandle* slot = self->slots[i];
            if (slot && slot->target)
                fn_710031965c(reinterpret_cast<UnkMultiLockon_ovb0092*>(self),
                              reinterpret_cast<UnkStruct_71002334fc*>(slot->target));
        }
        for (u64 i = 0; i < self->locked_count; ++i) {
            if (UnkLockedRef_l31a29c* handle = self->locked[i]) {
                u32* count = &handle->ref_count;
                if (--*count == 0)
                    handle->vf_0x10();
            }
        }
        self->locked_count = 0;
        UnkLockonEntry_l31a29c* e = self->marks;
        if (self->mark_count) {
            for (UnkLockonEntry_l31a29c* end = e + self->mark_count; e != end; ++e) {
                if (e->mark)
                    fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(e->mark + 8), 0);
            }
            self->mark_count = 0;
        }
    }
    fn_71002f9460(reinterpret_cast<UnkSelf*>(self), false);
}

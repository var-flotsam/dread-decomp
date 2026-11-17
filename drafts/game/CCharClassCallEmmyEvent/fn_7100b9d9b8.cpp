// CCharClassCallEmmyEvent unit (tools/tu_map.py): the event calls the E.M.M.I. When exactly one is
// registered (g_unk_71726af0a8), the actor stands on its shape (fn_7100b9db84) and it is not in state 2,
// it is sent after the actor (fn_7100547428) unless it already chases that actor and the event's delay
// has not passed since its last call. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "game/CAbilityEnergyWave.h"
#include "game/UnkActor.h"
#include "game/UnkTU0.h"
#include "types.h"

struct UnkTransform_cce;
struct UnkScenario_cce;
bool fn_7100b9db84(void*, const UnkTransform_cce* transform, UnkScenario_cce* scenario);

struct UnkEmmyActor_b55 {
    virtual void vf_0();
};

void fn_7100547428(UnkEmmyActor_b55* self, bool a, UnkActor* target, bool b);

struct UnkEmmyEntry_b55 : base::UnkRefCounted {
    u8 pad_0xc[4];
    UnkEmmyActor_b55* actor;  // 0x10
};

struct __attribute__((packed, aligned(4))) UnkEmmyEntryPtr_b55 {
    base::UnkRefHandle h;
};

template <u32 N>
struct UnkEmmyEntryList_b55 {
    u32 count;
    UnkEmmyEntryPtr_b55 items[N];

    UnkEmmyEntryList_b55(const UnkEmmyEntryList_b55& other) {
        for (u64 i = 0; i < other.count; ++i) {
            items[i].h.ptr = nullptr;
            items[i].h.Set(other.items[i].h.ptr);
        }
        count = other.count;
    }
    UnkEmmyEntryPtr_b55* end() {
        return reinterpret_cast<UnkEmmyEntryPtr_b55*>(reinterpret_cast<u64>(items) + u64(count) * sizeof(UnkEmmyEntryPtr_b55));
    }
    ~UnkEmmyEntryList_b55() {
        for (u64 i = 0; i < count; ++i) {
            base::UnkRefCounted* p = items[i].h.ptr;
            if (p && --p->ref_count == 0)
                delete p;
        }
    }
};

using UnkEmmyList_b55 = UnkEmmyEntryList_b55<2>;
struct UnkEmmyRegistry_b55 {
    u8 pad_0x0[8];
    UnkEmmyList_b55 list;  // 0x8
};

// What an E.M.M.I. is after: a refcounted entry naming the actor (+0x10).
struct UnkEmmyTarget_lb9d9b8 : base::UnkRefCounted {
    u8 pad_0xc[4];
    UnkActor* actor;  // 0x10
};

// The E.M.M.I. as this event reads it.
struct UnkEmmyAI_lb9d9b8 {
    u8 pad_0x0[0x14b40];
    UnkEmmyTarget_lb9d9b8* target;  // 0x14b40
    u32 state;                      // 0x14b48
    f32 called_at;                  // 0x14b4c
};

struct UnkFrameTime_lb9d9b8 {
    u8 pad_0x0[0x2c];
    f32 time;  // 0x2c
};

struct UnkCallEmmyEvent_lb9d9b8 {
    u8 pad_0x0[0xc];
    f32 delay;  // 0xc
};

static inline UnkEmmyActor_b55* Emmy(const UnkEmmyEntryPtr_b55& entry) {
    return entry.h.ptr ? static_cast<UnkEmmyEntry_b55*>(entry.h.ptr)->actor : nullptr;
}

// How many E.M.M.I. the list holds.
static inline u64 Size(UnkEmmyList_b55& list) {
    return list.end() - list.items;
}

void fn_7100b9d9b8(UnkCallEmmyEvent_lb9d9b8* self, UnkActor** actor) {
    UnkEmmyList_b55 list(g_unk_71726af0a8->list);
    // Only when there is exactly one E.M.M.I.
    if (Size(list) != 1)
        return;
    if (!fn_7100b9db84(self, reinterpret_cast<const UnkTransform_cce*>(*actor),
                       reinterpret_cast<UnkScenario_cce*>(Emmy(list.items[0]))))
        return;
    UnkEmmyActor_b55* emmy = Emmy(list.items[0]);
    UnkEmmyAI_lb9d9b8* ai = reinterpret_cast<UnkEmmyAI_lb9d9b8*>(emmy);
    if (ai->state == 2)
        return;
    f32 elapsed = reinterpret_cast<UnkFrameTime_lb9d9b8*>(g_unk_7101d47328)->time - ai->called_at;
    if (elapsed >= self->delay || (ai->target ? ai->target->actor : nullptr) != fn_7100d7c990(*actor))
        fn_7100547428(emmy, true, *actor, false);
}

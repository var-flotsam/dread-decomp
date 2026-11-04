// CEmmyStateOverrideLogicAction unit (tools/tu_map.py, the name from reflection): the action's set-up. After
// the base set-up (fn_7100a55ac8) it copies the global list of E.M.M.I. entries (g_unk_71726af0a8: a count
// and 4-byte-aligned ref-counted pointers) and, for each entry's actor (+0x10): in mode 0 (+0x10 of the
// action) resets its override (fn_7100541dc8(1), clears the +0x29f25 flag, fn_7100545138, fn_7100541de4,
// fn_7100544a34 with g_unk_71726ded60); in mode 1, when the actor's animation component plays a
// fn_7100c0f988 track, the flag is clear and vtable slot 0x7d8 says no, calls fn_7100541dc8(0) and sets
// the flag. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/UnkRefHandle.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkTutoExitAction_b47;
void fn_7100a55ac8(UnkTutoExitAction_b47* self);

struct UnkEmmyActor_b55 {
    virtual void vf_0();
};

void fn_7100541dc8(UnkEmmyActor_b55* self, bool on);
void fn_7100545138(UnkEmmyActor_b55* self);
void fn_7100541de4(UnkEmmyActor_b55* self);
void fn_7100544a34(UnkEmmyActor_b55* self, char* data, bool on);
extern char g_unk_71726ded60[];

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
extern UnkEmmyRegistry_b55* g_unk_71726af0a8;

struct UnkEmmyStateOverrideAction_b55 {
    u8 pad_0x0[0x10];
    u32 mode;  // 0x10
};

static inline UnkAnimationComponent* Anim(UnkEmmyActor_b55* a) {
    char* c = *reinterpret_cast<char**>(reinterpret_cast<char*>(a) + 0x30);
    return *reinterpret_cast<UnkAnimationComponent**>(c + 0x220);
}

void fn_7100a4d4e8(UnkEmmyStateOverrideAction_b55* self) {
    fn_7100a55ac8(reinterpret_cast<UnkTutoExitAction_b47*>(self));
    UnkEmmyList_b55 list(g_unk_71726af0a8->list);
    for (UnkEmmyEntryPtr_b55* it = list.items; it != list.end(); ++it) {
        UnkEmmyActor_b55* actor = it->h.ptr ? static_cast<UnkEmmyEntry_b55*>(it->h.ptr)->actor : nullptr;
        switch (self->mode) {
        case 0:
            fn_7100541dc8(actor, true);
            *reinterpret_cast<u8*>(reinterpret_cast<char*>(actor) + 0x29f25) = 0;
            fn_7100545138(actor);
            fn_7100541de4(actor);
            fn_7100544a34(actor, g_unk_71726ded60, true);
            break;
        case 1: {
            u8& flag = *reinterpret_cast<u8*>(reinterpret_cast<char*>(actor) + 0x29f25);
            if (fn_710117b348(Anim(actor), fn_7100c0f988()) && !flag) {
                auto vf = (*reinterpret_cast<bool (***)(UnkEmmyActor_b55*)>(actor))[0x7d8 / 8];
                if (!vf(actor)) {
                    fn_7100541dc8(actor, false);
                    flag = 1;
                }
            }
            break;
        }
        }
    }
}

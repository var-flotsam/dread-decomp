// Sets up an actor's components (two of them through a mask of all flags), then hands a list of
// nineteen flag values to fn_71008724f8 under a fixed key. Layouts are placeholders.
#include "UnkFlagListS7.h"
#include "game/UnkActor.h"

using unk_flag_list::UnkFlagList;
using unk_flag_list::UnkFlagListKey;

// A small flag value whose constructors are not constexpr, so its statics are guarded (as in
// src/game_lo/fn_71003609b8.cpp).
struct UnkFlags_7100600c98 {
    u32 value;
    UnkFlags_7100600c98() { value = 0; }
};

struct UnkMask_7100600c98 {
    u32 value;
    explicit UnkMask_7100600c98(const UnkFlags_7100600c98& f) { value = (u8)~f.value; }
};

inline const UnkFlags_7100600c98& UnkNone_7100600c98() {
    static UnkFlags_7100600c98 s_value;
    return s_value;
}

inline const UnkMask_7100600c98& UnkAll_7100600c98() {
    static UnkMask_7100600c98 s_value(UnkNone_7100600c98());
    return s_value;
}

struct UnkS16_7100600c98;
struct UnkS12_7100600c98;
extern UnkS16_7100600c98 g_unk_7101cd0898_target;  // reached through the GOT
extern UnkS12_7100600c98 g_unk_7101cd08a0_target;
struct UnkKey_7100600c98;
extern UnkKey_7100600c98 g_unk_7101ce6100_target;

bool fn_7101176690(void* obj, u64 hash, u64 key, const UnkFlags_7100600c98& flags, const UnkS16_7100600c98& a,
                   const UnkS12_7100600c98& b, const UnkMask_7100600c98& mask);
void fn_7100a2cb7c(void* obj, UnkKey_7100600c98* key, UnkMask_7100600c98* mask, f32 value, u32 arg);

struct UnkStruct_7100600c98 {
    u8 pad_0x0[0x30];
    UnkActor* actor;
};

extern UnkFlagListKey g_unk_71726f37a0 __attribute__((visibility("hidden")));

// NON_MATCHING(minor): the first call's flags share the list's stack slot (the original gives
// them their own, after the mask).
void fn_7100600c98(UnkStruct_7100600c98* self) {
    if (void* p = self->actor->field_0x220) {
        UnkFlags_7100600c98 none;
        UnkMask_7100600c98 all = UnkAll_7100600c98();
        fn_7101176690(p, 0x039363ae10389fa6ull, 0xf4061ff25a2163d6ull, none, g_unk_7101cd0898_target,
                      g_unk_7101cd08a0_target, all);
    }
    if (void* q = self->actor->field_0x2e8) {
        UnkMask_7100600c98 all = UnkAll_7100600c98();
        fn_7100a2cb7c(q, &g_unk_7101ce6100_target, &all, -1.0f, 0);
    }
    if (void* target = fn_7100d7c990(self->actor)->field_0x1d8) {
        UnkFlagList list;
        list.PushBack(0x100);
        list.PushBack(0x40);
        list.PushBack(0x8000);
        list.PushBack(0x20);
        list.PushBack(0x10000);
        list.PushBack(0x20000);
        list.PushBack(0x80);
        list.PushBack(0x1);
        list.PushBack(0x2);
        list.PushBack(0x100000);
        list.PushBack(0x2000);
        list.PushBack(0x1000);
        list.PushBack(0x200);
        list.PushBack(0x400);
        list.PushBack(0x200000);
        list.PushBack(0x800);
        list.PushBack(0x10);
        list.PushBack(0x40000);
        list.PushBack(0x80000);
        fn_71008724f8(target, &g_unk_71726f37a0, &list, 1, 1, 0, 1);
    }
}

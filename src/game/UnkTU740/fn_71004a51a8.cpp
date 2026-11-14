// Unit 740 (hint CCooldownXBossLavaCarpetDef): a coin flip stored on the attack the owner's
// CCooldownXBossAIComponent names with g_unk_71726d33d8, and the destructor's array clean-up.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"
#include "game/UnkTU2702.h"

extern "C" int rand();

struct UnkScorpiusAI_b69a4;

extern char g_unk_71726d33d8[] __attribute__((visibility("hidden")));
extern char g_unk_71726d33c0[] __attribute__((visibility("hidden")));

struct UnkCooldownOwner_b0225 {
    u8 pad_0x0[0x240];
    base::reflection::UnkReflObject* ai;  // 0x240
};

struct UnkCooldownAttack_b0225 {
    u8 pad_0x0[0x98];
    u32 flag;  // 0x98
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void* fn_71004a51a8(void* unused, UnkCooldownOwner_b0225* owner) {
    s32 r = rand();
    base::reflection::UnkReflObject* obj = owner->ai;
    UnkScorpiusAI_b69a4* ai = nullptr;
    if (obj && fn_710009d50c(obj->vf_0x0(), fn_71004cf318()))
        ai = reinterpret_cast<UnkScorpiusAI_b69a4*>(obj);
    UnkCooldownAttack_b0225* attack = static_cast<UnkCooldownAttack_b0225*>(fn_7101109df4(ai, g_unk_71726d33d8));
    if (attack)
        attack->flag = r & 1;
    // keeps the random value's low bit tested again for the result
    asm("" : "+r"(r));
    return (r & 1) ? &g_unk_71726d33c0[8] : &g_unk_71726d33c0[0];
}

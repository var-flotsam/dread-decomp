// Unit 1442 of tools/tu_map.py (no name hint; see UnkTU1442_Steps_ovb0243.cpp): the step after the base's: the
// turn track check, the owner's navigation told it is blocked (a kind-5 parameter block) and the effect
// sink and movement told of a new kind (a kind-2 block and the zero vector). Names and layouts are placeholders.
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CEnemyMovement.h"
#include "game/CMovementComponent.h"
#include "game/UnkAIComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1440.h"
#include "game/UnkTU1442.h"
#include "game/UnkTU1444.h"
#include "game_s2/UnkParams.h"
#include "types.h"

struct UnkEffectSink;
struct UnkUnit1444_ovb0177;
struct UnkMovement_71010d08d4;
struct UnkMovementVec_71010d08d4;
struct UnkUnit1442Update;

struct UnkUnit1444_Event;
void fn_710096b720(UnkUnit1442Update* self);

struct UnkAI_b5a4 {
    void** vtable;
    u8 pad_0x8[0x68];
    u32 count;  // 0x70
    u8 pad_0x74[0x20];
    u8 field_0x94;

    bool vf_0x6f8() { return reinterpret_cast<bool (*)(UnkAI_b5a4*)>(vtable[0x6f8 / 8])(this); }
};

struct UnkOwner_b5a4 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkAI_b5a4* ai;  // 0x240
};

struct UnkTarget_b5a4 {
    u8 pad_0x0[0xf9];
    u8 field_0xf9;
    u8 pad_0xfa[0x16a6];
    u8 field_0x17a0[1];
};

struct UnkUnit1442Update {
    u8 pad_0x0[0x30];
    UnkOwner_b5a4* owner;  // 0x30
    UnkTarget_b5a4* target;  // 0x38
    u8 pad_0x40[0xfd];
    u8 field_0x13d;
};

void fn_710096b5a4(UnkUnit1442Update* self) {
    fn_7100969148(reinterpret_cast<UnkUnit1444_ovb0177*>(self));
    if (UnkAnimationComponent* animation = self->owner->animation) {
        if (!fn_710117b348(animation, fn_7100d2b3a8()))
            self->field_0x13d = 1;
    }
    fn_710096b720(self);
    fn_710096b8fc(reinterpret_cast<char*>(self));
    UnkTarget_b5a4* target = self->target;
    u8 checked = target->field_0xf9;
    UnkAI_b5a4* ai = self->owner->ai;
    if (!checked) {
        u32 count = ai->count;
        bool open = ai->field_0x94 == 0;
        if (count != ((count != 0) & open))
            return;
    }
    // keeps the null test of the AI from being folded into the unchecked reads above
    asm("" : "+r"(ai));
    if (ai) {
        if (ai->vf_0x6f8())
            return;
        target = self->target;
    }
    if (fn_71010cc3a4(target) == 4)
        fn_7100974798(reinterpret_cast<UnkUnit1444_Event*>(target->field_0x17a0), 1);
    if (ai) {
        if (UnkNavigation* navigation = fn_71010fccf8(reinterpret_cast<UnkAIComponent*>(ai))) {
            UnkParams params;
            params.field_0x1c = 0;
            __builtin_memset(params.field_0x20, 0, 16);
            params.field_0x30 = 0;
            params.vtable = g_unk_7101a25b10 + 2;
            __builtin_memset(params.field_0x8, 0, 13);
            params.field_0x39 = 0;
            params.field_0x3b = 0;
            params.field_0x3d = 0;
            params.field_0x34 = 5;
            fn_71006f4b0c(navigation, &params, 1);
        }
    }
    {
        struct Event {
            void** vtable;
            u8 pad_0x8[4];
            u8 field_0xc;
            u8 pad_0xd[7];
            u8 field_0x14;
            u8 pad_0x15[3];
            u8 field_0x18[13];
            u8 pad_0x25[3];
            u8 field_0x28[13];
            u8 pad_0x35[3];
            u32 kind;  // 0x38
        } ev;
        ev.field_0xc = 0;
        ev.field_0x14 = 0;
        __builtin_memset(ev.field_0x18, 0, 13);
        __builtin_memset(ev.field_0x28, 0, 13);
        ev.vtable = g_unk_7101a2e1c0 + 2;
        ev.kind = 2;
        fn_71010c7e6c(reinterpret_cast<UnkEffectSink*>(self->target), &ev, 1);
    }
    fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(self->target),
                  reinterpret_cast<const UnkMovementVec_71010d08d4*>(&g_unk_710164779c), true);
}

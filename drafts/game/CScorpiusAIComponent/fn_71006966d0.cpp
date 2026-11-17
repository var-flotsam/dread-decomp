// CScorpiusAIComponent's unit: picks the next attack (1 to 3) from the ones the current combo allows,
// at random where it allows several. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::reflection::UnkReflObject;

extern "C" int rand();

// One allowed attack, in a short chain.
struct UnkScorpiusAttackNode {
    s32 attack;
    u8 pad_0x4[0x4];
    UnkScorpiusAttackNode* next;  // 0x8
};

struct UnkScorpiusAttackChoice {
    u8 pad_0x0[0x8];
    s32 field_0x8;
    u8 pad_0xc[0x4];
    UnkScorpiusAttackNode* allowed;  // 0x10
};

struct UnkScorpiusBehavior {
    u8 pad_0x0[0x1a0];
    UnkReflObject* current;  // 0x1a0, the action running
};

struct UnkScorpiusOwner {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x50];
    UnkScorpiusBehavior* behavior;  // 0x278
};

struct UnkScorpiusAIComponent {
    u8 pad_0x0[0x30];
    UnkScorpiusOwner* owner;  // 0x30
};

static inline f32 RandomRange(f32 min, f32 max) {
    return static_cast<f32>((rand() & 0x7fff) * (1.0 / 32767.0)) * (max - min) + min;
}

// The chain holds at most three attacks.
static inline bool Allows(const UnkScorpiusAttackNode* node, s32 attack) {
    for (s32 i = 0; i < 3 && node; ++i, node = node->next) {
        if (node->attack == attack)
            return true;
    }
    return false;
}

s32 fn_71006966d0(UnkScorpiusAIComponent* self, UnkScorpiusAttackChoice* choice) {
    UnkScorpiusBehavior* behavior = self->owner->behavior;
    if (behavior) {
        UnkReflObject* current = behavior->current;
        if (current && fn_710009d50c(current->vf_0x0(), fn_710068aaf4()))
            return 3;
    }
    UnkAnimationComponent* animation = self->owner->animation;
    bool avoiding = false;
    if (animation)
        avoiding = fn_710117b348(animation, fn_7100c4ad78());

    const UnkScorpiusAttackNode* allowed = choice->allowed;
    if (allowed) {
        bool first = Allows(allowed, 1);
        bool second = Allows(allowed, 2);
        bool third = Allows(allowed, 3);
        if (first) {
            if (second) {
                if (!third)
                    return 3;
                if (choice->field_0x8 == 0)
                    return 1;
                switch (allowed->attack) {
                case 3: {
                    bool coin = RandomRange(0.0f, 1.0f) < 0.5f;
                    return avoiding || coin ? 1 : 2;
                }
                case 2:
                    return RandomRange(0.0f, 1.0f) < 0.5f ? 1 : 3;
                case 1:
                    break;
                default:
                    return 1;
                }
            } else if (third) {
                return avoiding ? 1 : 2;
            }
            bool coin = RandomRange(0.0f, 1.0f) < 0.5f;
            return coin && !avoiding ? 2 : 3;
        }
        if (second) {
            if (third)
                return 1;
            return RandomRange(0.0f, 1.0f) < 0.5f ? 1 : 3;
        }
    }
    bool coin = RandomRange(0.0f, 1.0f) < 0.5f;
    return avoiding || coin ? 1 : 2;
}

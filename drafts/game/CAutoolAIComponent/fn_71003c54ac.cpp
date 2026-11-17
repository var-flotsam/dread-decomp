// CAutoolAIComponent's unit (tools/tu_map.py unit 570): sets the three thrusters (right, left, bottom)
// from the owner's motion and the AI's speed, then lets each thruster track of the animation set its
// own. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CFXComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU134.h"
#include "types.h"

using base::global::CStrId;

struct UnkCollisionContainer;
struct UnkAutoolAI_ovb0137;
void fn_7100863730(void* collision, const CStrId* name, const u32* flags);
void fn_71008635a4(UnkCollisionContainer* self, const CStrId* id, const u32* extra);
void fn_71003c4fb0(UnkAutoolAI_ovb0137* self, u32 which, u32 mode);

// The colliders of the three thrusters (names not recovered).
extern CStrId g_unk_71726be9b8 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726be9c0 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726be9c8 __attribute__((visibility("hidden")));

struct UnkAutoolMotion_l3c54ac {
    u8 pad_0x0[0x40];
    f32 x;  // 0x40
    f32 y;  // 0x44
};

struct UnkAutoolOwner_l3c54ac {
    u8 pad_0x0[0x218];
    UnkAutoolMotion_l3c54ac* motion;  // 0x218
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x58];
    UnkWorld* effects;  // 0x280
    u8 pad_0x288[0x18];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkAutoolAI_l3c54ac {
    u8 pad_0x0[0x30];
    UnkAutoolOwner_l3c54ac* owner;  // 0x30
    u8 pad_0x38[0x1477c - 0x38];
    f32 speed;  // 0x1477c
};

// A thruster track's record: the mode it asks for at +0x18.
struct UnkAutoolThrusterTrack_l3c54ac {
    u8 pad_0x0[0x18];
    u32 mode;  // 0x18
};

// The same as fn_71003c4fb0, which this unit inlines where the thruster and the mode are constants:
// thruster `which` (0 right, 1 left, 2 bottom) gets its collider stopped in mode 0 and started
// otherwise, and its two effects: both off in mode 0, the second on in mode 1, the first on in mode 2.
inline void AutoolSetThruster_l3c54ac(UnkAutoolAI_l3c54ac* self, u32 which, u32 mode) {
    if (!self->owner->collision)
        return;
    const CStrId* name = nullptr;
    u64 b = ~0ull;
    u64 a = b;
    switch (which) {
    case 0:
        name = &g_unk_71726be9b8;
        a = 0xa08f6cbc1d98f726;
        b = 0xa8b007d46b53867d;
        break;
    case 1:
        name = &g_unk_71726be9c0;
        a = 0x9fd0648f1eae4a1c;
        b = 0x97ef0fe768653b47;
        break;
    case 2:
        name = &g_unk_71726be9c8;
        a = 0xc924614080498486;
        b = 0xd45cceb79172305e;
        break;
    }
    u32 flags = 0x100;
    if (mode)
        fn_71008635a4(self->owner->collision, name, &flags);
    else
        fn_7100863730(self->owner->collision, name, &flags);
    UnkPart* first = fn_7101152b90(self->owner->effects, a);
    UnkPart* second = fn_7101152b90(self->owner->effects, b);
    fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(first), mode == 2);
    fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(second), mode == 1);
}

static inline u32 ThrusterMode(UnkTrackRecord* record) {
    return reinterpret_cast<UnkAutoolThrusterTrack_l3c54ac*>(record)->mode;
}

void fn_71003c54ac(UnkAutoolAI_l3c54ac* self) {
    UnkAutoolMotion_l3c54ac* motion = self->owner->motion;
    UnkAnimationComponent* animation = self->owner->animation;
    if (!motion || !animation)
        return;

    f32 x = motion->x;
    f32 y = motion->y;
    if (__builtin_fabsf(x) < __builtin_fabsf(y)) {
        // Moving vertically: the side thrusters off, the bottom one by direction and speed.
        bool rising = y >= 0.0f;
        bool fast = self->speed >= 0.2f;
        AutoolSetThruster_l3c54ac(self, 0, 0);
        AutoolSetThruster_l3c54ac(self, 1, 0);
        if (rising) {
            if (fast)
                AutoolSetThruster_l3c54ac(self, 2, 2);
            else
                AutoolSetThruster_l3c54ac(self, 2, 1);
        } else {
            if (!fast)
                AutoolSetThruster_l3c54ac(self, 2, 2);
            else
                AutoolSetThruster_l3c54ac(self, 2, 1);
        }
    } else {
        f32 speed = self->speed;
        if (speed >= 0.9f) {
            AutoolSetThruster_l3c54ac(self, 0, 2);
            AutoolSetThruster_l3c54ac(self, 1, 2);
        } else if (speed >= 0.06f) {
            AutoolSetThruster_l3c54ac(self, 0, 1);
            AutoolSetThruster_l3c54ac(self, 1, 1);
        } else {
            AutoolSetThruster_l3c54ac(self, 0, 0);
            AutoolSetThruster_l3c54ac(self, 1, 0);
        }
        AutoolSetThruster_l3c54ac(self, 2, 1);
    }

    UnkAutoolAI_ovb0137* ai = reinterpret_cast<UnkAutoolAI_ovb0137*>(self);
    UnkTrackQuery right = {fn_7100c0e328(), nullptr, -1.0f};
    fn_710117b08c(animation, &right);
    if (right.result)
        fn_71003c4fb0(ai, 0, ThrusterMode(right.result));
    UnkTrackQuery left = {fn_7100c0dd08(), nullptr, -1.0f};
    fn_710117b08c(animation, &left);
    if (left.result)
        fn_71003c4fb0(ai, 1, ThrusterMode(left.result));
    UnkTrackQuery bottom = {fn_7100c0d6e8(), nullptr, -1.0f};
    fn_710117b08c(animation, &bottom);
    if (bottom.result)
        fn_71003c4fb0(ai, 2, ThrusterMode(bottom.result));
}

// CChozoRobotSoldierAIComponent's unit (tools/tu_map.py unit 694): after the base step (fn_71003ad414), on
// entering one of the component's states, clears and rolls its flags (+0x14b7c); then stores how far
// away the target is in g_unk_7101a38b20, snapped to 550 or 1150. Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "types.h"

extern "C" int rand();

struct UnkRobotSoldierChances_l4730dc {
    u8 pad_0x0[0x48];
    f32 field_0x48;  // a chance, 0 to 1
    f32 field_0x4c;  // a chance, 0 to 1
};
extern UnkRobotSoldierChances_l4730dc g_unk_71726cc6d8;  // reached through the GOT
extern f32 g_unk_7101a38b20;                             // reached through the GOT

struct UnkKraidUpdateAI_b62a4;
void fn_71003ad414(UnkKraidUpdateAI_b62a4* self);

// A node of the state machine: its state at +0x8 and its child node at +0x18.
struct UnkRobotSoldierStateNode_l4730dc {
    u8 pad_0x0[0x8];
    void* state;
    u8 pad_0x10[0x8];
    UnkRobotSoldierStateNode_l4730dc* child;
};

struct UnkRobotSoldierAI_l4730dc {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x1170 - 0x38];
    u32 field_0x1170;
    UnkRobotSoldierStateNode_l4730dc* field_0x1178;
    u8 pad_0x1180[0x149a8 - 0x1180];
    u8 field_0x149a8[0x14a18 - 0x149a8];  // a state
    u8 field_0x14a18[0x14a90 - 0x14a18];  // a state
    u8 field_0x14a90[0x14b1c - 0x14a90];  // another state
    u8 pad_0x14b1c[0x14b7c - 0x14b1c];
    u32 field_0x14b7c;
    u32 field_0x14b80;
    u32 field_0x14b84;
};
static_assert(__builtin_offsetof(UnkRobotSoldierAI_l4730dc, field_0x14b84) == 0x14b84, "layout");

static inline f32 Rand01_l4730dc() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

static inline f32 RandomRange_l4730dc(f32 low, f32 high) {
    return low + Rand01_l4730dc() * (high - low);
}

void fn_71004730dc(UnkRobotSoldierAI_l4730dc* self, void* state) {
    fn_71003ad414(reinterpret_cast<UnkKraidUpdateAI_b62a4*>(self));
    void* idle = self->field_0x14a18;
    void* idle2 = self->field_0x14a90;
    if (state == idle || state == idle2) {
        self->field_0x14b7c = 0;
        void* other = self->field_0x149a8;
        UnkRobotSoldierStateNode_l4730dc* node;
        self->field_0x14b84 =
            (self->field_0x1170 != 0 && (node = self->field_0x1178) && node->state != other && node->child &&
             node->child->state != other && node->child->child && node->child->child->state != other) ||
            !(Rand01_l4730dc() > g_unk_71726cc6d8.field_0x48);
    } else if (state == self->field_0x149a8) {
        self->field_0x14b80 = 0;
        UnkRobotSoldierStateNode_l4730dc* node;
        if (self->field_0x1170 != 0 && (node = self->field_0x1178) && node->state != idle &&
            node->state != idle2 && node->child && node->child->state != idle && node->child->state != idle2)
            self->field_0x14b84 = 0;
        else
            self->field_0x14b84 = !(Rand01_l4730dc() <= g_unk_71726cc6d8.field_0x4c);
    }

    f32 x = self->owner->field_0x44;
    f32 distance = __builtin_fabsf(x - fn_7100d7c990(self->owner)->field_0x44);
    g_unk_7101a38b20 = distance;
    f32 fromNear = __builtin_fabsf(550.0f - distance);
    f32 fromFar = __builtin_fabsf(1150.0f - distance);
    if (fromNear >= 300.0f && fromFar >= 300.0f)
        g_unk_7101a38b20 = RandomRange_l4730dc(0.0f, 1.0f) > 0.5f ? 550.0f : 1150.0f;
    else if (fromNear > fromFar && fromNear >= 300.0f)
        g_unk_7101a38b20 = 550.0f;
    else if (fromFar > fromNear && fromFar >= 300.0f)
        g_unk_7101a38b20 = 1150.0f;
}

// CChozoRobotSoldierAIComponent's unit (tools/tu_map.py unit 694): three small checks of the component.
// fn_7100472fa4: whether the timer at +0x14b1c is running (at least 0 and under the tuning's limit at
// +0x2c of g_unk_71726cc6d8). fn_7100474ae0: whether the owner's (+0x30) target (fn_7100d7c990) has a
// +0x208 object whose count (fn_71009e8590) is above 1. fn_7100473378: after the base step
// (fn_7101106af4), when the flag at +0x48 is set and +0x14b84 is still 0, sets +0x14b84 to 1 with the
// tuning's chance (+0x50), unless the state machine (+0x1170 count, +0x1178 node) is in a state other
// than the two at +0x14a18 and +0x14a90, both itself and in its child (+0x18). Names and layouts are
// placeholders.
#include "game/UnkActor.h"
#include "types.h"

extern "C" int rand();

struct UnkRobotSoldierTuning_ovb0002 {
    u8 pad_0x0[0x2c];
    f32 field_0x2c;  // the timer's limit
    u8 pad_0x30[0x20];
    f32 field_0x50;  // a chance, 0 to 1
};
extern UnkRobotSoldierTuning_ovb0002 g_unk_71726cc6d8;  // reached through the GOT

// A node of the state machine: its state at +0x8 and its child node at +0x18.
struct UnkRobotSoldierStateNode_ovb0002 {
    u8 pad_0x0[0x8];
    void* state;
    u8 pad_0x10[0x8];
    UnkRobotSoldierStateNode_ovb0002* child;
};

struct UnkRobotSoldierAI_ovb0002 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x10];
    bool field_0x48;
    u8 pad_0x49[0x1170 - 0x49];
    u32 field_0x1170;
    UnkRobotSoldierStateNode_ovb0002* field_0x1178;
    u8 pad_0x1180[0x14a18 - 0x1180];
    u8 field_0x14a18[0x14a90 - 0x14a18];  // a state
    u8 field_0x14a90[0x14b1c - 0x14a90];  // another state
    f32 field_0x14b1c;
    u8 pad_0x14b20[0x14b84 - 0x14b20];
    u32 field_0x14b84;
};
static_assert(__builtin_offsetof(UnkRobotSoldierAI_ovb0002, field_0x14b84) == 0x14b84,
              "UnkRobotSoldierAI_ovb0002 layout");

bool fn_7100472fa4(UnkRobotSoldierAI_ovb0002* self) {
    f32 t = self->field_0x14b1c;
    return (t >= 0.0f) & (t < g_unk_71726cc6d8.field_0x2c);
}

// As src/game/CChozoWarriorAIComponent/fn_710049cbd8.cpp declares it.
struct UnkFlagged;
s32 fn_71009e8590(UnkFlagged* object);

bool fn_7100474ae0(UnkRobotSoldierAI_ovb0002* self) {
    if (UnkActor* target = fn_7100d7c990(self->owner)) {
        if (target->field_0x208 && fn_71009e8590(static_cast<UnkFlagged*>(target->field_0x208)) > 1)
            return true;
    }
    return false;
}

void fn_7101106af4(UnkRobotSoldierAI_ovb0002* self);

void fn_7100473378(UnkRobotSoldierAI_ovb0002* self) {
    fn_7101106af4(self);
    if (!self->field_0x48 || self->field_0x14b84 != 0)
        return;
    if (self->field_0x1170 != 0) {
        if (UnkRobotSoldierStateNode_ovb0002* node = self->field_0x1178) {
            void* idle = self->field_0x14a18;
            void* idle2 = self->field_0x14a90;
            if (node->state != idle && node->state != idle2) {
                UnkRobotSoldierStateNode_ovb0002* child = node->child;
                if (child && child->state != idle && child->state != idle2)
                    return;
            }
        }
    }
    f32 r = (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
    if (r <= g_unk_71726cc6d8.field_0x50)
        self->field_0x14b84 = 1;
}

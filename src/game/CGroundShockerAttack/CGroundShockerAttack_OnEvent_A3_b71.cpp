// CGroundShockerAttack's unit (tools/tu_map.py unit 856): its handling of an animation event (message type
// 3). In phase 2 the event with name hash 0x71b966d985d3d7c4 copies the tunable's value at +0x2c (when the
// attack has a target at +0x28) and moves the step to 3; in phase 4 the event with hash 0xf05d756d06130ba3
// copies the value at +0x30, tells the owner's CGroundShockerAIComponent (+0x240, fn_71005a50c8) and moves the
// step to 5. Always handled. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::reflection::UnkTunable;

// As src/reflection/CCharClassGroundShockerAttack.cpp declares it.
extern UnkTunable g_unk_71726e7be0;

struct UnkGroundShockerAI_A3_b71;
void fn_71005a50c8(UnkGroundShockerAI_A3_b71* self);

struct UnkGroundShockerEvName_A3_b71 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkGroundShockerEv_A3_b71 {
    u8 pad_0x0[0x38];
    UnkGroundShockerEvName_A3_b71* name;
};

struct UnkGroundShockerEvHolder_A3_b71 {
    u8 pad_0x0[0x10];
    UnkGroundShockerEv_A3_b71* event;
};

struct UnkGroundShockerMsg_A3_b71 {
    u32 type;
    UnkGroundShockerEvHolder_A3_b71* holder;
};

struct UnkGroundShockerOwner_A3_b71 {
    u8 pad_0x0[0x240];
    base::reflection::UnkReflObject* ai;
};

struct UnkGroundShockerAttack_A3_b71 {
    u8 pad_0x0[0x8];
    UnkGroundShockerOwner_A3_b71* owner;
    u8 pad_0x10[0x18];
    void* target;
    u32 phase;
    u32 step;
    u32 field_0x38;
    u32 field_0x3c;
};

bool fn_71005a1010(UnkGroundShockerAttack_A3_b71* self, UnkGroundShockerMsg_A3_b71* msg) {
    if (msg->type == 3) {
        UnkGroundShockerEv_A3_b71* ev = msg->holder->event;
        char* tunable = reinterpret_cast<char*>(&g_unk_71726e7be0);
        if (self->phase == 2) {
            if (ev->name->hash == 0x71b966d985d3d7c4) {
                if (self->target)
                    self->field_0x38 = *reinterpret_cast<u32*>(tunable + 0x2c);
                self->step = 3;
            }
        } else if (self->phase == 4) {
            if (ev->name->hash == 0xf05d756d06130ba3) {
                if (self->target)
                    self->field_0x3c = *reinterpret_cast<u32*>(tunable + 0x30);
                if (base::reflection::UnkReflObject* ai = self->owner->ai) {
                    if (fn_710009d50c(ai->vf_0x0(), fn_71005a4004()))
                        fn_71005a50c8(reinterpret_cast<UnkGroundShockerAI_A3_b71*>(ai));
                }
                self->step = 5;
            }
        }
    }
    return true;
}

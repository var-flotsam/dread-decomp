// Unit 1376 (CDoorLifeComponent hint): counts down the timer at +0x63c; when it runs out and the slot
// colour level matches the target (+0x640) with the open animation playing, fades the door towards 0.3
// (0 when busy or closed, 1 in state 3); otherwise restarts the timer at 0.2. Names and layouts are placeholders.
#include <math.h>

#include "game/UnkTU1376.h"
#include "types.h"
#include "base/color/CColor4F.h"
#include "base/math/CVector4D.h"
#include "game/UnkAnimationComponent.h"

using base::color::CColor4F;
using base::math::CVector4D;

struct UnkDoorLifeA5_b61;
void fn_71001c287c(void* target, u32 a, u32 b, CVector4D* colour, u32 c);
void* fn_71008d89b0(void* owner);

struct UnkGameManagerIdKey_b0251 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkGameManagerIdTable {
    u8 pad_0x0[0xb10];
    UnkGameManagerIdKey_b0251* field_0xb10;
    u8 pad_0xb18[0x2578 - 0xb18];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

struct UnkDoorGfx_b0251 {
    u8 pad_0x0[0x138];
    void* slots;
};
struct UnkDoorHolder_b0251 {
    u8 pad_0x0[0x50];
    UnkDoorGfx_b0251* gfx;
};
struct UnkDoorOwner3_b0251 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
    UnkDoorHolder_b0251* holder;
};

struct UnkDoorState_b0251 {
    u8 pad_0x0[0x10];
    s32 id;
};
struct UnkDoorMachine_b0251 {
    virtual void vf_0x0();
    u64 field_0x8;
    UnkDoorState_b0251* current;
    u8 pad_0x18[0xb0 - 0x18];
};

struct UnkDoor_b0251 {
    u8 pad_0x0[0x30];
    UnkDoorOwner3_b0251* owner;
    u8 pad_0x38[0x41c - 0x38];
    u8 field_0x41c;
    u8 pad_0x41d[3];
    UnkDoorMachine_b0251 machine;
    u8 pad_0x4d0[0x638 - 0x4d0];
    f32 field_0x638;
    union {
        f32 field_0x63c;
        u32 bits_0x63c;
    };
    f32 field_0x640;
    u32 field_0x644;
    u32 field_0x648;
    u8 pad_0x64c[0xa10 - 0x64c];
    u32 field_0xa10;
};

void fn_71008d387c(UnkDoor_b0251* self, f32 dt) {
    if (!(self->field_0x63c > 0.0f))
        return;
    f32 t = self->field_0x63c - dt;
    self->field_0x63c = t;
    if (!(t < 0.0f))
        return;
    UnkDoorGfx_b0251* gfx = self->owner->holder->gfx;
    CColor4F colour(1.0f, 1.0f, 1.0f, 1.0f);
    f32 level;
    if (self->field_0xa10 != 0xffffffff) {
        fn_71001c287c(gfx->slots, self->field_0xa10, 0, reinterpret_cast<CVector4D*>(&colour), 1);
        level = colour.a;
    } else {
        level = 0.0f;
    }
    f32 d = level - self->field_0x640;
    if (!(d + 0.001f >= 0.0f))
        goto reset;
    if (!(d + -0.001f <= 0.0f))
        goto reset;
    if (!fn_710117ae38(self->owner->animation, GameManagerIdTable().field_0xb10->id, false))
        goto reset;
    {
        const u32 mask = 0x08000400;
        u32 a = self->field_0x644;
        if (!(a & mask)) {
            u32 b = self->field_0x648;
            if (!((b & mask) | (a & 1) | (b & 1))) {
                if (!fn_71008d89b0(self->owner)) {
                    self->field_0x63c = 0.0f;
                    return;
                }
                a = self->field_0x644;
            }
        }
        bool busy;
        if (a & mask)
            busy = true;
        else
            busy = (self->field_0x648 & mask) != 0;
        bool idle = self->field_0x41c == 0;
        f32 v = (busy || idle) ? 0.0f : 0.3f;
        if (self->machine.current && self->machine.current->id == 3)
            v = 1.0f;
        fn_71008d0c24(reinterpret_cast<UnkDoorLifeA5_b61*>(self), v, self->field_0x638);
        self->field_0x63c = 0.0f;
        return;
    }
reset:
    self->bits_0x63c = 0x3e4ccccd;
}

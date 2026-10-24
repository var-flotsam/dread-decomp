// Pushes the owner back out of a blocking shape: when the owner is not dead (+0x58 >= 0), its
// target body's object at +0x1f8 is not flagged and a shape query against the owner's entity at
// +0x228 hits, the sign of the owner's facing is compared with the sign of the overlap along x; if
// they agree and the owner is past the edge of the hit shape, the offset is added to both of two
// fields of the output.
#include "game/UnkActor.h"
#include "types.h"

struct UnkShape {
    void** vtable;
    u8 pad_0x8[0x38];
    f32 field_0x40;
    u8 pad_0x44[4];
    f32 field_0x48;
    u8 pad_0x4c[0x14];
    u8 field_0x60;
    u8 pad_0x61[7];
    void vf_0x98() { reinterpret_cast<void (*)(UnkShape*)>(vtable[19])(this); }
};

struct UnkTarget {
    u8 pad_0x0[0x1f8];
    UnkTarget* field_0x1f8;
    u8 pad_0x200[0x0];
};

struct UnkFlagged {
    u8 pad_0x0[0xf9];
    u8 field_0xf9;
};

struct UnkDirection {
    u8 pad_0x0[8];
    f32 field_0x8;
};

struct UnkEntity;

struct UnkSelf {
    u8 pad_0x0[8];
    UnkActor* field_0x8;
    u8 pad_0x10[0x48];
    f32 field_0x58;
};

struct UnkOutput {
    u8 pad_0x0[0x60];
    f32 field_0x60;
    u8 pad_0x64[8];
    f32 field_0x6c;
};

struct UnkMatrix {
    f32 m[16];
};

UnkShape* fn_7100976588(UnkFlagged* flagged);
UnkDirection* fn_710002614c(void* object);
extern char g_unk_7101649570[];
bool fn_71011735e8(UnkEntity* entity, const void* key, UnkMatrix* out, u32 flags);

void fn_710043c084(UnkSelf* self, UnkOutput* out) {
    UnkActor* target = fn_7100d7c990(self->field_0x8);
    if (!target)
        return;
    UnkFlagged* flagged = static_cast<UnkFlagged*>(target->field_0x1f8);
    if (!flagged)
        return;
    if (!(self->field_0x58 >= 0.0f))
        return;
    if (flagged->field_0xf9)
        return;
    UnkEntity* entity = static_cast<UnkEntity*>(self->field_0x8->field_0x228);
    UnkShape* shape = fn_7100976588(flagged);
    if (!entity || !shape)
        return;
    UnkMatrix m = {};
    m.m[15] = 1.0f;
    m.m[10] = 1.0f;
    m.m[5] = 1.0f;
    m.m[0] = 1.0f;
    if (!fn_71011735e8(entity, g_unk_7101649570, &m, 0))
        return;
    f32 x = fn_710002614c(self->field_0x8->field_0x38)->field_0x8;
    s32 facing = (x > 0.0f) - (x < 0.0f);
    f32 facing_f = (f32)facing;
    if (shape->field_0x60)
        shape->vf_0x98();
    f32 sel = facing < 0 ? shape->field_0x40 : shape->field_0x48;
    f32 base = out->field_0x6c;
    f32 edge = sel + base - m.m[3];
    f32 side = (f32)((edge > 0.0f) - (edge < 0.0f));
    if (facing_f != side)
        return;
    f32 reach;
    if (facing >= 0)
        reach = base + shape->field_0x40 - m.m[3];
    else
        reach = m.m[3] - (base + shape->field_0x48);
    f32 limit = self->field_0x58;
    if (reach < limit) {
        f32 push = (limit - reach) * facing_f;
        out->field_0x6c = base + push;
        out->field_0x60 = push + out->field_0x60;
    }
}

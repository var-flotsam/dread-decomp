#include "types.h"

struct UnkUnit211Vec2 {
    f32 x;
    f32 y;
};

struct UnkUnit211Target {
    u8 unk_0x0[0x18];
    UnkUnit211Vec2* values;
};

struct UnkUnit211Source {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2(u32 index);
};

struct UnkUnit211Op {
    u8 unk_0x0[0x8];
    UnkUnit211Target* target;
    UnkUnit211Source* source;
};

// Saves the target's value, lets the source recompute it in place (vf_2), then combines the two.
void fn_7100193e50(UnkUnit211Op* self, const u32* indices, u32 count) {
    for (; count != 0; --count) {
        u32 index = *indices++;
        UnkUnit211Target* target = self->target;
        UnkUnit211Source* source = self->source;
        UnkUnit211Vec2 saved = target->values[index];
        source->vf_2(index);
        UnkUnit211Vec2& out = self->target->values[index];
        UnkUnit211Vec2 r = {saved.x + out.x, saved.y + out.y};
        out = r;
    }
}

void fn_7100193fcc(UnkUnit211Op* self, const u32* indices, u32 count) {
    for (; count != 0; --count) {
        u32 index = *indices++;
        UnkUnit211Target* target = self->target;
        UnkUnit211Source* source = self->source;
        UnkUnit211Vec2 saved = target->values[index];
        source->vf_2(index);
        UnkUnit211Vec2& out = self->target->values[index];
        UnkUnit211Vec2 r = {saved.x * out.x, saved.y * out.y};
        out = r;
    }
}

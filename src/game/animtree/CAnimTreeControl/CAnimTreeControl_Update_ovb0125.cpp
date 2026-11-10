// animtree::CAnimTreeControl's unit (tools/tu_map.py unit 400): the update (fn_71002d61c0). While blending
// (+0x44) the time (+0x3c) advances up to the duration (+0x40) and the weight (time over duration, reversed
// with +0x38; 1 for a duration within 0.001 of zero) goes to the object vf_0x90 returns; then, while the
// target's +0x8 is positive, vf_0x80 runs when there is no delay (+0x48), else vf_0x88 with the delay
// counted down, and the tree is marked dirty. Names and layouts are placeholders.
#include "types.h"

struct UnkAnimTreeWeight_ovb0125 {
    u8 pad_0x0[0x8];
    f32 weight;
};

struct UnkAnimTreeTarget_ovb0125 {
    u8 pad_0x0[0x8];
    f32 field_0x8;
};

struct UnkAnimTreeDirty_ovb0125 {
    u8 pad_0x0[0x68];
    bool dirty;
};

struct UnkAnimTreeTree_ovb0125 {
    u8 pad_0x0[0x70];
    UnkAnimTreeDirty_ovb0125* field_0x70;
};

struct UnkAnimTreeControlBase_ovb0125 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88(f32 dt);
    virtual UnkAnimTreeWeight_ovb0125* vf_0x90();
};

struct UnkAnimTreeControl_ovb0125 : UnkAnimTreeControlBase_ovb0125 {
    u8 pad_0x8[0x10];
    UnkAnimTreeTree_ovb0125* tree;  // 0x18
    u8 pad_0x20[0x8];
    UnkAnimTreeTarget_ovb0125* target;  // 0x28
    u8 pad_0x30[0x8];
    u32 reverse;     // 0x38
    f32 time;        // 0x3c
    f32 duration;    // 0x40
    bool blending;   // 0x44
    u8 pad_0x45[0x3];
    f32 delay;       // 0x48

};

void fn_71002d61c0(UnkAnimTreeControl_ovb0125* self, f32 dt) {
    if (self->blending) {
        self->time += dt;
        if (self->time > self->duration) {
            self->time = self->duration;
            self->blending = false;
        }
        UnkAnimTreeWeight_ovb0125* out = self->vf_0x90();
        f32 d = self->duration;
        f32 w;
        if (!(d + 0.001f >= 0.0f && d - 0.001f <= 0.0f)) {
            w = self->time / d;
            if (self->reverse != 0)
                w = 1.0f - w;
        } else {
            w = 1.0f;
        }
        out->weight = w;
    }
    if (self->target->field_0x8 > 0.0f) {
        if (self->delay == 0.0f) {
            self->vf_0x80();
        } else {
            self->vf_0x88(dt);
            if (self->delay > 0.0f) {
                f32 v = self->delay - dt;
                self->delay = v < 0.0f ? 0.0f : v;
            }
        }
        self->tree->field_0x70->dirty = true;
    }
}

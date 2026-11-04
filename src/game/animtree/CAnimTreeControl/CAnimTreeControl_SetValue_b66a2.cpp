// animtree::CAnimTreeControl unit (tools/tu_map.py, the name from reflection): stores a non-negative value
// twice (+0x30 and +0x34), then switches the control on (vf_0x78) or off (vf_0x80) when its active state
// (the source's +0x8 positive and +0x4c clear) differs from `on`. Names and layouts are placeholders.
#include <math.h>

#include "types.h"

struct UnkAnimTreeControlSource_b66a2 {
    u8 pad_0x0[0x8];
    f32 value;
};

struct UnkAnimTreeControl_b66a2 {
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

    u8 pad_0x8[0x20];
    UnkAnimTreeControlSource_b66a2* source;
    f32 field_0x30;
    f32 field_0x34;
    u8 pad_0x38[0x14];
    bool paused;
};

void fn_71002d6074(UnkAnimTreeControl_b66a2* self, f32 value, bool on) {
    value = fmaxf(value, 0.0f);
    self->field_0x30 = self->field_0x34 = value;
    bool active = (self->source->value > 0.0f) & !self->paused;
    if (active != on) {
        if (on) {
            self->vf_0x78();
        } else {
            self->vf_0x80();
        }
    }
}

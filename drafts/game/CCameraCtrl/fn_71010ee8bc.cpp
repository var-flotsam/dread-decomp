// CCameraCtrl (unit 2693): applies a blend time to each object fn_71010e383c hands out for the
// controller's current source, stopping at the first that does not accept more. A negative time
// starts it with no blend. Names and layouts are placeholders.
#include "types.h"

struct UnkCamObject_l0ee8bc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40(void* owner);
    virtual void vf_0x48(f32 time, bool flag);
    virtual void vf_0x50();
    virtual void vf_0x58(f32 time);
    virtual bool vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0(f32 value);
};

struct UnkCamSource_l0ee8bc {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkCamCtrl_l0ee8bc {
    u8 pad_0x0[0x18];
    void* field_0x18;
    u8 pad_0x20[0x90];
    UnkCamSource_l0ee8bc* source;  // 0xb0
};

UnkCamObject_l0ee8bc* fn_71010e383c(void* source);

UnkCamObject_l0ee8bc* fn_71010ee8bc(UnkCamCtrl_l0ee8bc* self, f32 time) {
    f32 blend = !(time < 0.0f) ? time : 0.0f;
    UnkCamObject_l0ee8bc* object;
    while ((object = fn_71010e383c(self->source ? self->source->field_0x10 : nullptr)) != nullptr) {
        if (time < 0.0f) {
            object->vf_0x40(self->field_0x18);
            object->vf_0x48(blend, true);
            object->vf_0xd0(-1.0f);
        } else {
            object->vf_0x58(blend);
        }
        if (!object->vf_0x60())
            break;
    }
    return object;
}

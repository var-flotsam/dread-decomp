// CCameraComponent (unit 2691): walks the component's object list, calling each object's virtual
// 0x48 until one reports not-ready through virtual 0x60, and flags it. Names and layouts are placeholders.
#include "types.h"

struct UnkCamObject_l0e46cc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(f32 value, bool flag);
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual bool vf_0x60();
    u8 pad_0x8[0x90];
    bool field_0x98;
};

struct UnkCamNode_l0e46cc {
    UnkCamObject_l0e46cc* value;
    UnkCamNode_l0e46cc* next;
};

struct UnkCamComponent_l0e46cc {
    u8 pad_0x0[0x58];
    u32 count;
    UnkCamNode_l0e46cc* nodes;
};

void fn_71010e46cc(UnkCamComponent_l0e46cc* self, bool flag) {
    UnkCamNode_l0e46cc* first = self->nodes;
    if (!first)
        return;
    UnkCamObject_l0e46cc* object = first->value;
    if (!object)
        return;
    UnkCamObject_l0e46cc* previous;
    do {
        previous = object;
        object->vf_0x48(0.0f, flag);
        if (object->vf_0x60()) {
            if (self->count < 2)
                return;
            UnkCamNode_l0e46cc* node = self->nodes->next;
            for (; node; node = node->next) {
                if (!node->value->vf_0x60())
                    break;
            }
            if (!node)
                return;
            object = node->value;
            if (!object)
                return;
        }
    } while (object != previous);
    object->field_0x98 = !flag;
}

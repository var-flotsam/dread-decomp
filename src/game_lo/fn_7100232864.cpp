// Sets a boolean state flag on an object; when it changes, notifies the object through one of two
// virtual hooks (on/off) unless the 0x1b0 flag is set with no 0x1a8 pointer, then always reports the new value through another one.
#include "types.h"

struct UnkStruct_7100232864;

struct UnkVtbl_7100232864 {
    void* pad_0x0[34];
    void (*vf_34)(UnkStruct_7100232864*, bool);
    void* pad_0x118[4];
    void (*vf_39)(UnkStruct_7100232864*);
    void (*vf_40)(UnkStruct_7100232864*);
};

struct UnkStruct_7100232864 {
    UnkVtbl_7100232864* vtbl;
    u8 pad_0x8[0x1a0];
    void* field_0x1a8;
    bool field_0x1b0;
    u8 pad_0x1b1[3];
    bool field_0x1b4;
};

void fn_7100232864(UnkStruct_7100232864* self, bool value) {
    if (self->field_0x1b4 == value)
        return;
    self->field_0x1b4 = value;
    if (!self->field_0x1b0 || self->field_0x1a8 != nullptr) {
        if (value)
            self->vtbl->vf_39(self);
        else
            self->vtbl->vf_40(self);
    }
    self->vtbl->vf_34(self, self->field_0x1b4);
}

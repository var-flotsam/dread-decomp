// Sets an enabled flag and, when the combined "active" state (flag set and the linked
// object ready) changes, calls the matching activate/deactivate virtual.
#include "types.h"

struct UnkStruct_710024fa0c_Link {
    u8 pad_0x0[0x1a8];
    void* field_0x1a8;
    bool field_0x1b0;
    u8 pad_0x1b1[3];
    bool field_0x1b4;

    bool IsReady() const {
        if (!field_0x1b4)
            return false;
        if (!field_0x1b0)
            return true;
        return field_0x1a8 != nullptr;
    }
};

struct UnkStruct_710024fa0c {
    void (**vtable)(UnkStruct_710024fa0c*);
    u8 pad_0x8[0x10];
    UnkStruct_710024fa0c_Link* field_0x18;
    bool field_0x20;

    bool IsActive() const { return field_0x20 && field_0x18 && field_0x18->IsReady(); }
};

void fn_710024fa0c(UnkStruct_710024fa0c* self, bool enabled) {
    if (self->field_0x20 == enabled)
        return;
    bool was = self->IsActive();
    self->field_0x20 = enabled;
    bool now = self->IsActive();
    if (was != now) {
        if (!was)
            self->vtable[0x108 / 8](self);
        else
            self->vtable[0x110 / 8](self);
    }
}

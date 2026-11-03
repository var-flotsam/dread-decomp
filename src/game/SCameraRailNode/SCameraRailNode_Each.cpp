// A function in the SCameraRailNode unit: fn_7100c03fec calls slot 0x30 of every 0x58-byte element of the
// array at +0x8 (count at +0x10) with `arg`. Names and layouts are placeholders.
#include "types.h"

struct UnkCameraRailElement {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30(void* arg);
    u8 pad_0x8[0x50];
};
static_assert(sizeof(UnkCameraRailElement) == 0x58, "UnkCameraRailElement placeholder layout");

struct UnkCameraRailArray {
    u8 pad_0x0[0x8];
    UnkCameraRailElement* data;
    u32 count;
};

void fn_7100c03fec(UnkCameraRailArray* self, void* arg) {
    if (self->count == 0)
        return;
    UnkCameraRailElement* it = self->data;
    do {
        UnkCameraRailElement* e = it++;
        e->vf_0x30(arg);
    } while (it < self->data + self->count);
}

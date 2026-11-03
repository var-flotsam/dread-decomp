// From unit 300 of tools/tu_map.py (0x7100211a84-0x710021234c): reads two values from `source` (its
// virtuals at 0x38 and 0x40, each returned through a temporary) and hands each to the receiver's
// virtual at 0x60. Names and layouts are placeholders.
#include "types.h"

struct UnkUnit300Value {
    u64 lo;
    u64 hi;
    UnkUnit300Value();
    UnkUnit300Value(const UnkUnit300Value& other);
};

struct UnkUnit300Source {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual UnkUnit300Value vf_0x38();
    virtual UnkUnit300Value vf_0x40();
};

struct UnkUnit300Receiver {
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
    virtual void vf_0x60(const UnkUnit300Value& value);
};

void fn_7100211a90(UnkUnit300Receiver* self, UnkUnit300Source* source) {
    self->vf_0x60(source->vf_0x38());
    self->vf_0x60(source->vf_0x40());
}

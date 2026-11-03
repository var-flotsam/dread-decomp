// GUI::CAmiiboButton unit (unit 1649 of tools/tu_map.py): rounds a value to the nearest integer (halves away
// from zero) and passes it with a flag to the object's slot 0x78. Names and layouts are placeholders.
#include "types.h"

struct UnkAmiiboButtonTarget {
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
    virtual void vf_0x78(s32 value, bool flag);
};

void fn_7100af07f8(UnkAmiiboButtonTarget* self, bool flag, f32 value) {
    self->vf_0x78((s32)(value + (value > 0.0f ? 0.5f : -0.5f)), flag);
}

// Unit 135 of tools/tu_map.py: the constructor of an object tagged for debugging, as in
// src/base/UnkTaggedObject124.cpp: the flags word (0xff09f000), the live tag 0x600d600d, no counted
// resource, the sub-object at +0x18 (fn_71001bcbec, with no argument), the self pointer, then the
// flags' upper half set to 0x70c. Names and layouts are placeholders.
#include "types.h"

struct UnkTaggedObject135 {
    u16 flags_lo;
    u16 flags_hi;
    u32 tag;                    // 0x600d600d, 0xdeaddead once destroyed
    void* resource;             // 0x8
    UnkTaggedObject135* self;   // 0x10
    u8 sub[0x8];                // 0x18, size unknown
};

void fn_71001bcbec(void* sub, void* arg);  // the sub-object's constructor

void fn_71001478bc(UnkTaggedObject135* self) {
    self->flags_lo = 0xf000;
    self->flags_hi = 0xff09;
    self->tag = 0x600d600d;
    self->resource = nullptr;
    fn_71001bcbec(self->sub, nullptr);
    self->self = self;
    self->flags_hi = 0x70c;
}

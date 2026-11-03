// From unit 51 of tools/tu_map.py (base::input::CRumbleManager's unit): whether an address lies in
// the block that starts at +0x30 and is +0x10 bytes long (fn_7100075730). Names are placeholders.
#include "types.h"

struct UnkMemoryBlock_7100075730 {
    u8 unk_0x0[0x10];
    u64 size;
    u8 unk_0x18[0x18];
    const char* begin;
};

bool fn_7100075730(const UnkMemoryBlock_7100075730* self, const char* p) {
    return p >= self->begin && p < self->begin + self->size;
}

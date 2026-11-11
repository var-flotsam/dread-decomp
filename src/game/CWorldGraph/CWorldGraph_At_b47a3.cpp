// CWorldGraph unit (tools/tu_map.py): a bounds-checked lookup into the array held at +0x30 (count at +0x38)
// that returns nullptr when the index is out of range. Names and layouts are placeholders.
#include "game/CWorldGraph.h"
#include "types.h"

struct UnkWorldGraphArray;

struct UnkWorldGraphHolder {
    u8 unk_0x0[0x30];
    u8 array[8];
    u32 count;
};

__attribute__((disable_tail_calls)) void* fn_7100acca54(UnkWorldGraphHolder* self, u32 index) {
    if (index < self->count)
        return fn_7100acc418(reinterpret_cast<UnkWorldGraphArray*>(self->array), index);
    return nullptr;
}

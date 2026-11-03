// Functions in the CCharClassBreakableScenarioComponent unit (tools/tu_map.py; the
// CBreakableScenarioComponent side). Names and layouts are placeholders.
#include "types.h"

struct UnkBreakablePart {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();
    u8 pad_0x8[0x38];
};
static_assert(sizeof(UnkBreakablePart) == 0x40, "UnkBreakablePart placeholder layout");

struct UnkBreakableTarget {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkBreakableScenario {
    u8 pad_0x0[0x48];
    UnkBreakablePart* parts;  // 0x48
    u32 part_count;           // 0x50
};

UnkBreakableTarget** fn_71002a76a4(UnkBreakablePart* part);
void* fn_7100751520(void* key);
void fn_710076351c(void* obj);

// For each part whose virtual slot 4 says no, resolves its target's +0x10 (fn_7100751520) and passes
// what that finds to fn_710076351c. Always true.
bool fn_71007519d8(UnkBreakableScenario* self) {
    u32 count = self->part_count;
    if (count == 0)
        return true;
    UnkBreakablePart* it = self->parts;
    u64 size = (u64)count * sizeof(UnkBreakablePart);
    asm("" : "+r"(size));
    do {
        if (!it->vf_0x20()) {
            UnkBreakableTarget* target = *fn_71002a76a4(it);
            if (void* obj = fn_7100751520(target ? target->field_0x10 : nullptr))
                fn_710076351c(obj);
        }
        ++it;
        size -= sizeof(UnkBreakablePart);
    } while (size != 0);
    return true;
}

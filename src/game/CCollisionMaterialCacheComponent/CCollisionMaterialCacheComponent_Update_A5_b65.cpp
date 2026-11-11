// CCollisionMaterialCacheComponent's unit: reads the owner's current material (+0x1b8 of the object at
// +0x1f0, valid while bit 6 of +0x1b4 is set, else 0); when it changed, keeps the new and previous
// values and refreshes the owner's effect (fn_7100798eec, then fn_7101156808). Names are placeholders.
#include "game/CCollisionMaterialCacheComponent.h"
#include "types.h"

struct UnkStruct_710024fa0c;
void fn_7101156808(UnkStruct_710024fa0c* fx);

struct UnkMaterialSource_A5_b65 {
    u8 pad_0x0[0x1b4];
    u8 flags;
    u8 pad_0x1b5[3];
    u32 material;
};

struct UnkMaterialOwner_A5_b65 {
    u8 pad_0x0[0x1f0];
    UnkMaterialSource_A5_b65* source;
};

struct UnkMaterialCache_A5_b65 {
    u8 pad_0x0[0x30];
    UnkMaterialOwner_A5_b65* owner;
    u8 pad_0x38[8];
    u32 current;
    u32 previous;
};

void fn_7100798e98(UnkMaterialCache_A5_b65* self) {
    UnkMaterialOwner_A5_b65* owner = self->owner;
    UnkMaterialSource_A5_b65* source = owner->source;
    u32 material = (source->flags & 0x40) ? source->material : 0;
    u32 current = self->current;
    if (material == current)
        return;
    self->current = material;
    self->previous = current;
    fn_7101156808(fn_7100798eec(owner));
}

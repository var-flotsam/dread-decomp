// In the unit of COffset (CCharClassNailongAIComponent, CShootDCBones, CPattern): fn_7100615118 reads
// the float at +0x76c of `self` when `other`'s AI component (+0x240) is a CNailongAIComponent for
// which fn_7100622194 holds, else the float at +0x414. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::UnkReflObject;

bool fn_7100622194(UnkReflObject* ai);

struct UnkNailongOther {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;
};

struct UnkNailongOffsets {
    u8 pad_0x0[0x414];
    f32 field_0x414;
    u8 pad_0x418[0x76c - 0x418];
    f32 field_0x76c;
};

f32 fn_7100615118(UnkNailongOffsets* self, UnkNailongOther* other) {
    UnkReflObject* ai = other->ai;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_710061f464()) && fn_7100622194(ai))
        return self->field_0x76c;
    return self->field_0x414;
}

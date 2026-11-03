// Unit 241: evaluates entry `index`'s object at a time (the owner's rate at +0xc0 times `t`,
// rounded down to hundredths; fn_7100151758) and reads back its position (+0xac), direction
// (+0xd0) and two values (+0xdc, +0xe0). Names and layouts are placeholders inferred from the
// code.

#include "UnkClone.h"
#include "base/UnkVector.h"
#include "game/UnkXformNode.h"
#include "types.h"

#include <math.h>

struct UnkClone_fn_71001b459c;
using UnkElem_71001b46ec = UnkCloneElem<UnkClone_fn_71001b459c, 8>;
extern template UnkElem_71001b46ec& base::UnkVector<UnkElem_71001b46ec>::GrowAt(u32);

struct UnkSampled_71001b46ec {
    u8 pad_0x0[0xac];
    UnkVec3f position;
    u8 pad_0xb8[0x18];
    UnkVec3f direction;
    f32 field_0xdc;
    f32 field_0xe0;
};

struct UnkSampledHolder_71001b46ec {
    u8 pad_0x0[0x10];
    UnkSampled_71001b46ec* object;
};

void fn_7100151758(UnkSampled_71001b46ec* object, f32 time);

struct UnkSampler_71001b46ec {
    u8 pad_0x0[0x80];
    base::UnkVector<UnkElem_71001b46ec> entries;
    u8 pad_0xa8[0x10];
    u32 count;
    u32 field_0xbc;
    f32 rate;
};
static_assert(__builtin_offsetof(UnkSampler_71001b46ec, count) == 0xb8, "");

void fn_71001b46ec(UnkSampler_71001b46ec* self, u32 index, UnkVec3f* position, UnkVec3f* direction, f32* a,
                   f32* b, f32 t) {
    if (index < self->count) {
        UnkElem_71001b46ec& e = self->entries.GrowAt(index);
        UnkSampled_71001b46ec* object = (*reinterpret_cast<UnkSampledHolder_71001b46ec**>(&e))->object;
        fn_7100151758(object, floorf(self->rate * t * 100.0f) / 100.0f);
        *position = object->position;
        *direction = object->direction;
        *b = object->field_0xdc;
        *a = object->field_0xe0;
    }
}

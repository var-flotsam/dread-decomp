// CSwarmControllerComponent unit (tools/tu_map.py unit 1163): the element at an index of an array of
// 16-byte records, growing the array up to it with zeroed records: CRntVector::GrowAt written out.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

// Four words, all zero when constructed; copied word by word.
struct UnkSwarmRecord_l74b1ec {
    u32 words[4];

    UnkSwarmRecord_l74b1ec() : words() {}
    UnkSwarmRecord_l74b1ec(const UnkSwarmRecord_l74b1ec& o) {
        for (u32 i = 0; i < 4; ++i)
            words[i] = o.words[i];
    }
    ~UnkSwarmRecord_l74b1ec() {}
};

UnkSwarmRecord_l74b1ec* fn_710074b1ec(base::global::CRntVector<UnkSwarmRecord_l74b1ec>* self, u32 index) {
    if (self->field_0x14 && self->count <= index) {
        // First to the next multiple of the step, then to exactly index + 1 records.
        u32 cap = self->capacity;
        if (cap <= index && self->step != 0) {
            u32 grown = (index / self->step + 1) * self->step;
            if (cap < grown) {
                self->Reserve(grown);
                cap = grown;
            }
        }
        u32 n = index + 1;
        if (cap < n)
            self->Reserve(n);
        for (u32 i = self->count; i < n; ++i)
            new (&self->data[i]) UnkSwarmRecord_l74b1ec();
        self->count = n;
    }
    return &self->data[index];
}

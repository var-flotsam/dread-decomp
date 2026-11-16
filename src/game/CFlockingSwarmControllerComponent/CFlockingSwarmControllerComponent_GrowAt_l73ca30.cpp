// Unit 1158 (CFlockingSwarmControllerComponent): the element at an index of an array of 8-byte
// records, growing the array up to it with zeroed records: CRntVector::GrowAt written out. Names and
// layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

// Two words, both zero when constructed.
struct UnkPair_l73ca30 {
    u32 field_0x0 = 0;
    u32 field_0x4 = 0;

    ~UnkPair_l73ca30() {}
};

UnkPair_l73ca30* fn_710073ca30(base::global::CRntVector<UnkPair_l73ca30>* self, u32 index) {
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
            new (&self->data[i]) UnkPair_l73ca30();
        self->count = n;
    }
    return &self->data[index];
}

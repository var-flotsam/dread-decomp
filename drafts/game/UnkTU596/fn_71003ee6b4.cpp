// Unit 596: the element at an index of an array of 12-byte records, growing the array up to it with
// default records (zero, zero and -1): CRntVector::GrowAt written out. Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

// Two words that start at zero and a value that starts at -1.
struct UnkKeyValue_l3ee6b4 {
    f32 field_0x0 = 0.0f;
    f32 field_0x4 = 0.0f;
    f32 value = -1.0f;

    ~UnkKeyValue_l3ee6b4() {}
};

UnkKeyValue_l3ee6b4* fn_71003ee6b4(base::global::CRntVector<UnkKeyValue_l3ee6b4>* self, u32 index) {
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
            self->data[i] = UnkKeyValue_l3ee6b4();
        self->count = n;
    }
    return &self->data[index];
}

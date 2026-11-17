// Unit 430 (CAbilityGhostAura): the element at an index of an array of 12-byte records, growing the
// array up to it with zeroed records: CRntVector::GrowAt written out. Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

// A one-byte flag, copied and cleared as a whole.
struct UnkAuraFlag_l311fe4 {
    u8 value;
};

// A flag and two words; copied member by member.
struct UnkAuraRecord_l311fe4 {
    UnkAuraFlag_l311fe4 flag;
    u32 field_0x4;
    u32 field_0x8;

    UnkAuraRecord_l311fe4() : flag(), field_0x4(0), field_0x8(0) {}
    UnkAuraRecord_l311fe4(const UnkAuraRecord_l311fe4& o)
        : flag(o.flag), field_0x4(o.field_0x4), field_0x8(o.field_0x8) {}
    ~UnkAuraRecord_l311fe4() {}
};
static_assert(sizeof(UnkAuraRecord_l311fe4) == 0xc, "layout");

UnkAuraRecord_l311fe4* fn_7100311fe4(base::global::CRntVector<UnkAuraRecord_l311fe4>* self, u32 index) {
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
            new (&self->data[i]) UnkAuraRecord_l311fe4;
        self->count = n;
    }
    return &self->data[index];
}

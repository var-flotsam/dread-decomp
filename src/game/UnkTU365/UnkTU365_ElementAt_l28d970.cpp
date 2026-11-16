// Unit 365 (navmesh::CNavMeshGenerator): the element at an index of an array of 0x30-byte records,
// growing the array up to it with zeroed records: CRntVector::GrowAt written out. Names and layouts
// are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkNavVec_l28d970 {
    f32 x, y, z;
};

// Two points, each with two words; copied member by member.
struct UnkNavRecord_l28d970 {
    UnkNavVec_l28d970 point0 = {};
    u32 field_0xc = 0;
    u32 field_0x10 = 0;
    u8 pad_0x14[4];
    UnkNavVec_l28d970 point1 = {};
    u32 field_0x24 = 0;
    u32 field_0x28 = 0;
    u8 pad_0x2c[4];

    UnkNavRecord_l28d970() = default;
    UnkNavRecord_l28d970(const UnkNavRecord_l28d970& o)
        : point0(o.point0), field_0xc(o.field_0xc), field_0x10(o.field_0x10), point1(o.point1),
          field_0x24(o.field_0x24), field_0x28(o.field_0x28) {}
    ~UnkNavRecord_l28d970() {}
};
static_assert(sizeof(UnkNavRecord_l28d970) == 0x30, "layout");

UnkNavRecord_l28d970* fn_710028d970(base::global::CRntVector<UnkNavRecord_l28d970>* self, u32 index) {
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
            new (&self->data[i]) UnkNavRecord_l28d970;
        self->count = n;
    }
    return &self->data[index];
}

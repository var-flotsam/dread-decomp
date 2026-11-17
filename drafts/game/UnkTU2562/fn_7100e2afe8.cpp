// Dropped-item manager's unit (2562): builds a smooth path through at least three points. Each inner point
// gets an incoming and an outgoing control point along the tangent between its neighbours (scaled by
// `tension`), the ends one each, and the path's length is summed. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CRntVector.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CRntVector;

template <class T>
struct UnkCopiedVector;
struct UnkThreeWords;
UnkCopiedVector<UnkThreeWords>* fn_71000cbcf0(UnkCopiedVector<UnkThreeWords>* self,
                                              const UnkCopiedVector<UnkThreeWords>* other);

struct UnkSmoothPath_le2afe8 {
    f32 tension;
    f32 length;
    CRntVector<UnkVec3f> points;    // 0x8: the path's points with their control points between them
    CRntVector<UnkVec3f> source;    // 0x30: the points it was built from
    CRntVector<UnkVec3f> controls;  // 0x58
};
static_assert(__builtin_offsetof(UnkSmoothPath_le2afe8, controls) == 0x58, "layout");

static inline f32 Distance(const UnkVec3f& a, const UnkVec3f& b) {
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    f32 dz = a.z - b.z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

// Appends a copy of `value`, growing as CRntVector::PushBackCopy does (including its hang).
inline void Append(CRntVector<UnkVec3f>& array, const UnkVec3f& value) {
    u32 n = array.count;
    u32 capacity = array.capacity;
    u32 grown = capacity;
    if (capacity <= n && array.step != 0)
        grown = (n / array.step + 1) * array.step;
    if ((s32)n >= (s32)grown)
        for (;;) {
        }
    array.Reserve(grown);
    new (&array.data[array.count]) UnkVec3f(value);
    ++array.count;
}

bool fn_7100e2afe8(UnkSmoothPath_le2afe8* self, const CRntVector<UnkVec3f>* input, f32 tension) {
    if (input->count < 3)
        return false;

    self->points.Clear();
    self->points.Reserve(input->count * 3);
    self->source.Clear();
    self->controls.Clear();
    self->controls.Reserve(input->count * 3);
    fn_71000cbcf0(reinterpret_cast<UnkCopiedVector<UnkThreeWords>*>(&self->source),
                  reinterpret_cast<const UnkCopiedVector<UnkThreeWords>*>(input));
    self->tension = tension;
    self->length = 0.0f;

    const UnkVec3f& first = input->data[0];
    UnkVec3f prev = first;
    Append(self->points, first);

    {
        const UnkVec3f& a = input->data[1];
        const UnkVec3f& b = input->data[2];
        UnkVec3f start(a.x + (a.x - b.x) * tension, a.y + (a.y - b.y) * tension, a.z + (a.z - b.z) * tension);
        Append(self->controls, start);
        Append(self->points, start);
    }

    u32 n = input->count;
    for (u32 i = 1; i < n - 1; ++i) {
        const UnkVec3f& cur = input->data[i];
        self->length += Distance(cur, prev);
        prev = cur;

        const UnkVec3f& before = input->data[i - 1];
        const UnkVec3f& after = input->data[i + 1];
        UnkVec3f out((after.x - before.x) * tension * 0.5f + cur.x, (after.y - before.y) * tension * 0.5f + cur.y,
                     (after.z - before.z) * tension * 0.5f + cur.z);
        UnkVec3f in(cur.x + (cur.x - out.x), cur.y + (cur.y - out.y), cur.z + (cur.z - out.z));
        Append(self->controls, in);
        Append(self->controls, out);
        Append(self->points, in);
        Append(self->points, cur);
        Append(self->points, out);
    }

    const UnkVec3f& last = input->data[n - 1];
    const UnkVec3f& a = input->data[n - 2];
    const UnkVec3f& b = input->data[n - 3];
    UnkVec3f end(a.x + (a.x - b.x) * tension, a.y + (a.y - b.y) * tension, a.z + (a.z - b.z) * tension);
    Append(self->controls, end);
    Append(self->points, end);
    Append(self->points, last);
    self->length += Distance(last, a);
    return true;
}

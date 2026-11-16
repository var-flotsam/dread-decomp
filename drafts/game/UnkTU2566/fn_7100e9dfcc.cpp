// Unit 2566 (CMinimapManager::CTunableMinimapManager): builds a minimap view from two others: the
// entry for one string id, the bounds of the first clamped to the second, a colour from the tunable,
// and the centre moved to the middle of the two (bounds following) when it is off by more than a margin.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkTU2566.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

extern CStrId g_unk_71727c80a0;
extern const f32 g_unk_7101647774;

struct UnkMmEntry_le9dfcc {
    u8 bytes[0xc8];
};

// Entries with a string-id key array alongside.
struct UnkMmEntries_le9dfcc {
    UnkMmEntry_le9dfcc* values;
    CStrId* keys;
    u32 count;

    UnkMmEntry_le9dfcc* End() { return &values[count]; }
    UnkMmEntry_le9dfcc* Find(const CStrId& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i].data == key.data)
                return &values[i];
        return End();
    }
    UnkMmEntry_le9dfcc* FindOrNull(const CStrId& key) {
        UnkMmEntry_le9dfcc* v = Find(key);
        return v == End() ? nullptr : v;
    }
};

struct UnkMmData_le9dfcc {
    u8 pad_0x0[0xa8];
    UnkMmEntries_le9dfcc entries;  // 0xa8
};

struct UnkMmOwner_le9dfcc {
    u8 pad_0x0[0x60];
    UnkMmData_le9dfcc* data;  // 0x60
};

struct UnkMmView_le9dfcc {
    UnkMmEntry_le9dfcc* entry;
    f32 center_x;  // 0x8
    f32 center_y;
    CVector2D min;  // 0x10
    CVector2D max;  // 0x18
    CVector3D color;  // 0x20
};

// The colours of the minimap's tunable.
struct UnkMmTunable_le9dfcc {
    u8 pad_0x0[0x90];
    CVector3D color;        // 0x90
    CVector3D color_other;  // 0x9c
};

// Each component of `v`, at most (Lowest) or at least (Highest) the one of `limit`.
inline CVector2D Lowest(const CVector2D& v, const CVector2D& limit) {
    return {v.x < limit.x ? v.x : limit.x, v.y < limit.y ? v.y : limit.y};
}
inline CVector2D Highest(const CVector2D& v, const CVector2D& limit) {
    return {v.x > limit.x ? v.x : limit.x, v.y > limit.y ? v.y : limit.y};
}

// Whether `value` is more than `margin` away from `target`.
inline bool OffBy(f32 target, f32 value, f32 margin) { return target - margin > value || target + margin < value; }

void fn_7100e9dfcc(UnkMmOwner_le9dfcc* self, UnkMmView_le9dfcc* out, const UnkMmView_le9dfcc* a,
                   const UnkMmView_le9dfcc* b, bool other) {
    out->entry = self->data->entries.FindOrNull(g_unk_71727c80a0);

    out->min.x = a->min.x;
    out->min.y = a->min.y;
    out->max.x = a->max.x;
    out->max.y = a->max.y;
    out->min = Lowest(out->min, b->min);
    out->max = Highest(out->max, b->max);

    const UnkMmTunable_le9dfcc* tunable = reinterpret_cast<const UnkMmTunable_le9dfcc*>(&g_unk_71727ca410);
    const CVector3D& color = other ? tunable->color_other : tunable->color;
    out->color.x = color.x;
    out->color.y = color.y;
    out->color.z = color.z;

    f32 center_x = (a->center_x + b->center_x) * 0.5f;
    f32 center_y = (a->center_y + b->center_y) * 0.5f;
    f32 margin = g_unk_7101647774;
    if (OffBy(out->center_x, center_x, margin) || OffBy(out->center_y, center_y, margin)) {
        f32 dx = center_x - out->center_x;
        f32 dy = center_y - out->center_y;
        out->center_x = center_x;
        out->center_y = center_y;
        out->min.x += dx;
        out->min.y += dy;
        out->max.x += dx;
        out->max.y += dy;
    }
}

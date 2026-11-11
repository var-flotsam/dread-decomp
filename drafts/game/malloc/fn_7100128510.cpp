// Unit 110 (malloc hint): clips a 2D segment against an axis-aligned box and records the entry and exit
// points as contacts; optionally reports a segment that starts inside the box as one contact at its middle.
// Names and layouts are placeholders.

#include "base/math/UnkSegment2D.h"
#include "types.h"

struct UnkBox2D {
    f32 minX;
    f32 minY;
    f32 maxX;
    f32 maxY;
};

struct UnkSideInfo {
    u32 field_0x0;
    u32 field_0x4;
};

struct UnkBoxContact {
    UnkVec2Result pos;
    u32 field_0x8;
    UnkSideInfo side;
    u32 field_0x14;
    UnkVec2Result normal;
    u32 field_0x20;
    u32 field_0x24;
    u32 flags;
};

struct UnkBoxContactList {
    u32 count;
    UnkBoxContact items[1];
};
static_assert(__builtin_offsetof(UnkBoxContactList, items[0].flags) == 0x2c, "UnkBoxContactList layout");
static_assert(__builtin_offsetof(UnkBoxContactList, items[1]) == 0x30, "UnkBoxContactList layout");

extern const f32 g_unk_7101647774;

// The side of the box opposite to the given one.
u32 fn_710010d00c(u32 side);
// The pair of words describing a side of the box.
const UnkSideInfo* fn_710010cffc(u32 side);

static inline bool NearEps(f32 a, f32 b) {
    f32 lo = a - g_unk_7101647774;
    f32 hi = a + g_unk_7101647774;
    return hi >= b && lo <= b;
}

static inline void AddContact(UnkBoxContactList* list, const UnkVec2Result& pos, const UnkSideInfo* side,
                              const UnkVec2Result& normal) {
    UnkBoxContact* c = &list->items[list->count];
    c->pos = pos;
    c->field_0x8 = 0;
    c->side = *side;
    c->field_0x14 = 0;
    c->normal = normal;
    c->field_0x20 = 0;
    c->field_0x24 = 0;
    c->flags = 0x38;
    list->count++;
}

bool fn_7100128510(const UnkBox2D* box, const UnkSegment2D* seg, UnkBoxContactList* out, bool acceptInside) {
    if (NearEps(seg->start.x, seg->end.x) && NearEps(seg->start.y, seg->end.y)) {
        return false;
    }

    f32 dx = seg->end.x - seg->start.x;
    f32 dy = seg->end.y - seg->start.y;
    f32 invDx = 1.0f / dx;
    f32 invDy = 1.0f / dy;

    // Where along the segment each side's line is crossed: left, right, top, bottom.
    f32 t[4];
    t[0] = invDx * (box->minX - seg->start.x);
    t[1] = invDx * (box->maxX - seg->start.x);
    t[3] = invDy * (box->minY - seg->start.y);
    t[2] = invDy * (box->maxY - seg->start.y);

    u32 nearX = t[0] <= t[1] ? 0 : 1;
    u32 nearY = t[2] <= t[3] ? 2 : 3;
    u32 enterSide = t[nearX] >= t[nearY] ? nearX : nearY;
    u32 exitSide = fn_710010d00c(t[fn_710010d00c(nearX)] <= t[fn_710010d00c(nearY)] ? nearX : nearY);

    f32 tExit = t[exitSide];
    f32 tEnter = t[enterSide];

    if (tExit < 0.0f || tEnter > 1.0f || tEnter > tExit) {
        if (!acceptInside) {
            return false;
        }
        if (seg->start.x >= box->minX && seg->start.x <= box->maxX && seg->start.y >= box->minY &&
            seg->start.y <= box->maxY) {
            if (out->count == 0) {
                __builtin_memset(&out->items[0], 0, sizeof(UnkBoxContact));
                out->count = 1;
            }
            UnkBoxContact* c = &out->items[0];
            c->field_0x8 = 0;
            c->flags |= 8;
            c->pos = UnkVec2Result((seg->start.x + seg->end.x) * 0.5f, (seg->start.y + seg->end.y) * 0.5f);
            return true;
        }
        return false;
    }

    out->count = 0;
    UnkVec2Result normal = fn_7100125e78(seg);

    if (tEnter >= 0.0f) {
        AddContact(out, UnkVec2Result(dx * tEnter + seg->start.x, dy * tEnter + seg->start.y),
                   fn_710010cffc(enterSide), normal);
    }
    if (tExit >= 0.0f && tExit <= 1.0f) {
        AddContact(out, UnkVec2Result(dx * tExit + seg->start.x, dy * tExit + seg->start.y),
                   fn_710010cffc(exitSide), normal);
    }
    return out->count != 0;
}

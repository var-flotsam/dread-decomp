// CMinimapManager unit (UnkTU2566): fills an icon's draw record from the icon (breakable-tile icons are
// skipped): its entry, rectangle and colour, the rectangle scaled about its centre and, for icons that
// float, the record moved above the icon along the definition's direction. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkMinimapIconKinds.h"
#include "game/UnkTU2566.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;

extern CStrId g_unk_71727c80a0;
extern const f32 g_unk_7101647774;  // the tolerance positions are compared with

struct UnkIconRect_le9dc0c {
    f32 left, top, right, bottom;

    UnkIconRect_le9dc0c& operator=(const UnkIconRect_le9dc0c& other) {
        left = other.left;
        top = other.top;
        right = other.right;
        bottom = other.bottom;
        return *this;
    }

    // Scales the rectangle about its centre.
    void Scale(f32 scale) {
        f32 cx = (left + right) * 0.5f;
        f32 cy = (top + bottom) * 0.5f;
        left = cx + scale * (left - cx);
        top = cy + scale * (top - cy);
        right = cx + scale * (right - cx);
        bottom = cy + scale * (bottom - cy);
    }
};

struct UnkIconColor_le9dc0c {
    f32 r, g, b;

    UnkIconColor_le9dc0c& operator=(const UnkIconColor_le9dc0c& other) {
        r = other.r;
        g = other.g;
        b = other.b;
        return *this;
    }
};

struct UnkMinimapTunable_le9dc0c : UnkTunable {
    u8 pad_0x8[0x88];
    UnkIconColor_le9dc0c color;              // 0x90
    UnkIconColor_le9dc0c highlighted_color;  // 0x9c
};
static_assert(__builtin_offsetof(UnkMinimapTunable_le9dc0c, highlighted_color) == 0x9c, "UnkMinimapTunable_le9dc0c layout");

struct UnkIconDef_le9dc0c {
    CStrId kind;  // 0x0
    u8 pad_0x8[0xa0];
    CVector2D direction;  // 0xa8
    bool scaled_by_layers;  // 0xb0
};
static_assert(__builtin_offsetof(UnkIconDef_le9dc0c, scaled_by_layers) == 0xb0, "UnkIconDef_le9dc0c layout");

struct UnkIconLayer_le9dc0c {
    u8 pad_0x0[0x50];
    f32 scale_a;  // 0x50
    f32 scale_b;  // 0x54
    u8 pad_0x58[0x80];
};
static_assert(sizeof(UnkIconLayer_le9dc0c) == 0xd8, "UnkIconLayer_le9dc0c layout");

struct UnkIcon_le9dc0c {
    UnkIconDef_le9dc0c* def;  // 0x0
    CVector2D position;  // 0x8
    UnkIconRect_le9dc0c rect;  // 0x10
    u8 pad_0x20[0x10];
    f32 scale;  // 0x30
    u8 pad_0x34;
    bool flipped;  // 0x35
    u8 pad_0x36[0x2b6];
    u32 layer_count;  // 0x2ec
    UnkIconLayer_le9dc0c layers[1];  // 0x2f0
};
static_assert(__builtin_offsetof(UnkIcon_le9dc0c, layer_count) == 0x2ec, "UnkIcon_le9dc0c layout");

struct UnkIconEntry_le9dc0c {
    u8 pad_0x0[0xc8];
};

struct UnkIconEntryMap_le9dc0c {
    u8 pad_0x0[0xa8];
    UnkIconEntry_le9dc0c* values;  // 0xa8
    CStrId* keys;  // 0xb0
    u32 count;  // 0xb8

    // The value under `key`, or &values[count].
    UnkIconEntry_le9dc0c* Find(const CStrId& key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return &values[count];
    }
};

struct UnkMinimapOwner_le9dc0c {
    u8 pad_0x0[0x60];
    UnkIconEntryMap_le9dc0c* entries;  // 0x60
};

struct UnkIconDraw_le9dc0c {
    UnkIconEntry_le9dc0c* entry;  // 0x0
    CVector2D position;  // 0x8
    UnkIconRect_le9dc0c rect;  // 0x10
    UnkIconColor_le9dc0c color;  // 0x20
    u8 pad_0x2c[0xc];
    UnkIconDef_le9dc0c* def;  // 0x38
};

static inline bool IsTileKind(const CStrId& kind) {
    return kind == g_unk_71727c80b0 || kind == g_unk_71727c80b8 || kind == g_unk_71727c80c0 ||
           kind == g_unk_71727c80d0 || kind == g_unk_71727c80d8 || kind == g_unk_71727c80e0 ||
           kind == g_unk_71727c80e8;
}

// Moves the record to `to`, rectangle and all, unless it is already there (within the tolerance).
static inline void MoveTo(UnkIconDraw_le9dc0c* draw, const CVector2D& to) {
    f32 eps = g_unk_7101647774;
    CVector2D& at = draw->position;
    if (!(at.x - eps > to.x || eps + at.x < to.x || at.y - eps > to.y || eps + at.y < to.y))
        return;
    f32 dx = to.x - at.x;
    f32 dy = to.y - at.y;
    at = to;
    draw->rect.left = dx + draw->rect.left;
    draw->rect.top = dy + draw->rect.top;
    draw->rect.right = dx + draw->rect.right;
    draw->rect.bottom = dy + draw->rect.bottom;
}

void fn_7100e9dc0c(UnkMinimapOwner_le9dc0c* self, UnkIconDraw_le9dc0c* draw, UnkIcon_le9dc0c* icon,
                   bool highlighted) {
    if (IsTileKind(icon->def->kind))
        return;

    UnkIconEntryMap_le9dc0c* map = self->entries;
    UnkIconEntry_le9dc0c* entry = map->Find(g_unk_71727c80a0);
    draw->entry = entry == map->values + map->count ? nullptr : entry;
    draw->def = icon->def;
    draw->rect = icon->rect;
    const UnkMinimapTunable_le9dc0c& tunable = static_cast<const UnkMinimapTunable_le9dc0c&>(g_unk_71727ca410);
    draw->color = highlighted ? tunable.highlighted_color : tunable.color;

    f32 scale = icon->scale;
    if (icon->def->scaled_by_layers) {
        for (const UnkIconLayer_le9dc0c *it = icon->layers, *end = icon->layers + icon->layer_count; it != end; ++it)
            scale *= it->scale_a > it->scale_b ? it->scale_a : it->scale_b;
        if (!(scale > 1.0f))
            return;
        draw->rect.Scale(scale);
        // Grown icons are raised by the height they gained.
        f32 lift = (scale - 1.0f) * ((icon->rect.bottom - icon->rect.top) * 0.5f);
        MoveTo(draw, CVector2D{draw->position.x, draw->position.y - lift});
        return;
    }

    if (scale > 1.0f)
        draw->rect.Scale(scale);
    // Floating icons sit above (or, flipped, below) the icon, along the definition's direction.
    f32 base = icon->flipped ? -250.0f : 250.0f;
    CVector2D dir = icon->def->direction;
    if (icon->flipped)
        dir.y = -dir.y;
    f32 offset_x = 0.0f + dir.x * 250.0f;
    f32 offset_y = dir.y * 250.0f + (base + -(draw->rect.bottom - draw->rect.top) * 0.5f);
    MoveTo(draw, CVector2D{offset_x + icon->position.x, icon->position.y + offset_y});
}

// CKraidAIComponent: the box the camera keeps Kraid in. With no phase set at +0x1140, or when the
// object at +0x158 of the area data is there, it is the area's two corners widened by fixed
// margins; otherwise the phase's corners, the top pushed up to keep the target (with the height
// of its movement, fn_7100977bec) in view. Names and layouts are placeholders.
#include "game/CCharacterMovement.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkKraidVec2 {
    f32 x;
    f32 y;

    UnkKraidVec2() = default;
    UnkKraidVec2(const UnkKraidVec2& other) : x(other.x), y(other.y) {}
    UnkKraidVec2& operator=(const UnkKraidVec2& other) {
        x = other.x;
        y = other.y;
        return *this;
    }
};

struct UnkKraidVec3 {
    UnkKraidVec2 xy;
    f32 z;
};

struct UnkKraidBox {
    UnkKraidVec3 min;
    UnkKraidVec3 max;
};

struct UnkKraidArea {
    UnkKraidVec2 corner0;  // 0x0
    UnkKraidVec2 corner1;  // 0x8
    u8 pad_0x10[0x40];
    UnkKraidVec2 phase0;  // 0x50
    UnkKraidVec2 phase1;  // 0x58
    u8 pad_0x60[0xf8];
    void* field_0x158;
};

struct UnkKraidAIComponent {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x1108];
    u32 phase;  // 0x1140
};

struct UnkFlagged;

static inline f32 Max(f32 a, f32 b) { return a > b ? a : b; }

static inline UnkKraidVec2 Add(const UnkKraidVec2& v, f32 dx, f32 dy) {
    UnkKraidVec2 r;
    r.x = v.x + dx;
    r.y = v.y + dy;
    return r;
}

// The point at z = 0.
static inline void Set(UnkKraidVec3* out, UnkKraidVec2 v) {
    out->xy = v;
    out->z = 0.0f;
}

void fn_7100600734(UnkKraidAIComponent* self, UnkKraidBox* box) {
    UnkKraidArea* area = reinterpret_cast<UnkKraidArea*>(reinterpret_cast<char*>(self) + 0x14ce0);
    if (self->phase == 0 || area->field_0x158) {
        Set(&box->min, Add(area->corner0, 0.0f, -100.0f));
        Set(&box->max, Add(area->corner1, 400.0f, 200.0f));
        return;
    }
    Set(&box->min, area->phase0);
    Set(&box->max, Add(area->phase1, 0.0f, -1000.0f));
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target)
        return;
    f32 y = target->field_0x54;
    f32 low = y - fn_7100977bec(static_cast<UnkFlagged*>(target->field_0x1f8)) + -400.0f;
    box->min.xy.y = Max(box->min.xy.y, low);
    y = target->field_0x54;
    f32 high = y + (200.0f - fn_7100977bec(static_cast<UnkFlagged*>(target->field_0x1f8))) + 600.0f;
    box->max.xy.y = Max(box->max.xy.y, high);
}

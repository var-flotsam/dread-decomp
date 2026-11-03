// GUI::CDisplayObject: the inverse of fn_71011cf12c (UnkLocalPoint.cpp): a point taken out of the
// object's local space, after stale derived state is brought up to date (fn_71011cee40): moved by the
// object's position (+0x40), multiplied by its scale (+0x54, +0x58), then turned by its angle (+0x48),
// a result the original drops. Names and layouts are placeholders.
#include "base/math/UnkSegment2D.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

void fn_71011cee40(UnkGuiDisplayObject* object);

// The placement fields read here.
struct UnkGuiPlacement_b44a3 {
    u8 pad_0x0[0x40];
    f32 x;      // 0x40
    f32 y;      // 0x44
    f32 angle;  // 0x48
    u8 pad_0x4c[8];
    f32 scale_x;  // 0x54
    f32 scale_y;  // 0x58
};

UnkVec2Result fn_71011cf0a8(UnkGuiDisplayObject* self, const UnkVec2Result& point) {
    UnkVec2Result r(point);
    if (self->field_0x183)
        fn_71011cee40(self);
    UnkGuiPlacement_b44a3* p = reinterpret_cast<UnkGuiPlacement_b44a3*>(self);
    f32 x = p->x + r.x;
    f32 y = p->y + r.y;
    r.x = x * p->scale_x;
    r.y = y * p->scale_y;
    // The turned point is dropped: the result is the unturned one.
    fn_710007a2c0(r, p->angle);
    return r;
}

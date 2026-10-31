// GUI::CDisplayObject: a point taken into the object's local space, after stale derived state is
// brought up to date (fn_71011cee40): divided by the object's scale (+0x54, +0x58), and in
// fn_71011cf12c first moved by minus its position (+0x40) then turned by minus its
// angle (+0x48), a result the original drops. Names and layouts are placeholders.
#include "base/math/UnkSegment2D.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

void fn_71011cee40(UnkGuiDisplayObject* object);

// The placement fields read here.
struct UnkGuiPlacement {
    u8 pad_0x0[0x40];
    f32 x;      // 0x40
    f32 y;      // 0x44
    f32 angle;  // 0x48
    u8 pad_0x4c[8];
    f32 scale_x;  // 0x54
    f32 scale_y;  // 0x58
};

static inline UnkGuiPlacement* Placement(UnkGuiDisplayObject* self) {
    return reinterpret_cast<UnkGuiPlacement*>(self);
}

UnkVec2Result fn_71011cf1b4(UnkGuiDisplayObject* self, const UnkVec2Result& point) {
    UnkVec2Result r(point);
    if (self->field_0x183)
        fn_71011cee40(self);
    r.x = r.x / Placement(self)->scale_x;
    r.y = r.y / Placement(self)->scale_y;
    return r;
}

UnkVec2Result fn_71011cf12c(UnkGuiDisplayObject* self, const UnkVec2Result& point) {
    UnkVec2Result r(point);
    if (self->field_0x183)
        fn_71011cee40(self);
    UnkGuiPlacement* p = Placement(self);
    f32 x = r.x - p->x;
    f32 y = r.y - p->y;
    r.x = x / p->scale_x;
    r.y = y / p->scale_y;
    // The turned point is dropped: the result is the unturned one.
    fn_710007a2c0(r, -p->angle);
    return r;
}
